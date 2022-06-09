#include "embeddings.h"

#include <math.h>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <experimental/filesystem>


EmbeddingNeighborLookup::EmbeddingNeighborLookup(std::string embeddingFilepath, uint32_t embeddingDim, uint32_t maxNumNeighbors, 
                                                 std::string embeddingCacheFilepath): maxNeighbors(maxNumNeighbors)
{
    if(maxNumNeighbors == 0)
    {
        std::cout << "Since maxNumNeighbors == 0, no neighborhoods will be loaded and cache is ignored. ";
        std::cout << "NeighborLookup will just retrieve the item itself as neighbor=0." << std::endl;
        return;
    }

    if(embeddingCacheFilepath.size() == 0 || !std::experimental::filesystem::exists(embeddingCacheFilepath))
    {
        std::cout << "No cached file for the neighborhood exists, therefore, creating neighorhoods." << std::endl;
        std::unique_ptr<std::vector<Embedding> > embeddings = loadEmbeddings(embeddingFilepath, embeddingDim);
        calculateNeighborhood(*embeddings, EmbeddingMath::cosineSimilarity);
        saveNeighborhoodToCache(embeddingCacheFilepath);
    } 
    else
    {
        std::cout << "Using cached neighborhood " << embeddingCacheFilepath << "." << std::endl;
        loadNeighborhoodFromCache(embeddingCacheFilepath);
    } 
    std::cout << "Found " << nearestNeighbors.size() << " embeddings." << std::endl;
    calculateNeighborhoodThirdQuartiles();  
}

std::unique_ptr<std::vector<Embedding> > EmbeddingNeighborLookup::loadEmbeddings(std::string embeddingFilepath, uint32_t embeddingDim)
{
    std::unique_ptr<std::vector<Embedding> > embeddings(new std::vector<Embedding>());

    std::ifstream ifs (embeddingFilepath);
    std::string line;
    if (ifs.is_open())
    {
        // each line is a vector
        while (std::getline(ifs,line))
        {
            // parse space separated line of embedding vector values
            std::stringstream lineStream(line);
            std::string value;
            Embedding embedding;
            while (std::getline(lineStream, value, ' ')) {
                embedding.push_back(stod(value));
            }
            if (embedding.size() != embeddingDim)
            {
                throw std::runtime_error("Embedding has not the expected size.");
            }
            embeddings->push_back(embedding);
        }
    }
    if(embeddings->size() == 0)
    {
        throw std::runtime_error("Could not load embeddings from " + embeddingFilepath + ".");
    }
    return embeddings;
}

void EmbeddingNeighborLookup::calculateNeighborhood(std::vector<Embedding>& embeddings, double similarityFunc(Embedding& a, Embedding& b))
{
    std::cout << "Calculating embedding neighborhoods. This might take a bit." << std::endl;


    nearestNeighbors = std::vector<std::vector<Neighbor> >(embeddings.size());
    for (uint32_t i = 0; i < embeddings.size(); ++i)
    {
        for (uint32_t j = i+1; j < embeddings.size(); ++j)
        {
            double similarity = similarityFunc(embeddings[i], embeddings[j]);
            // similarity is symetric
            addNeighbor(nearestNeighbors[i], j, similarity);
            addNeighbor(nearestNeighbors[j], i, similarity);
        }
    }
}

void EmbeddingNeighborLookup::addNeighbor(std::vector<Neighbor>& neighborhood, uint32_t neighborIndex, double neighborSimilarity)
{
    if (neighborhood.size() < maxNeighbors || neighborhood.back().similarity < neighborSimilarity)
    {
        neighborhood.push_back(Neighbor{neighborIndex, neighborSimilarity});
        std::sort(neighborhood.begin(), neighborhood.end(), EmbeddingNeighborLookup::compareNeighbors);
        while(neighborhood.size() > maxNeighbors)
        {
            neighborhood.pop_back();
        }
    }
}

uint32_t EmbeddingNeighborLookup::getNearestNeighbor(uint32_t index, uint32_t distance) const
{
    assert(distance <= maxNeighbors);
    if (distance == 0)
        return index;
    return nearestNeighbors[index][distance-1].index;
}

double EmbeddingNeighborLookup::getNeighborSimilarity(uint32_t index, uint32_t distance) const
{
    assert(distance <= maxNeighbors);
    if (distance == 0)
        return 1;
    return nearestNeighbors[index][distance-1].similarity;
}

bool EmbeddingNeighborLookup::doNeighborsOverlap(uint32_t index1, uint32_t index2) const
{
    for(uint32_t i = 0; i < maxNeighbors; ++i)
    {
        if(nearestNeighbors[index1][i].index == index2)
        {
            return true;
        }
        for(uint32_t j = 0; j < maxNeighbors; ++j)
        {
            if(nearestNeighbors[index2][j].index == index1 || nearestNeighbors[index1][i].index == nearestNeighbors[index2][j].index)
            {
                return true;
            }
        }
    }
    return false;
}

void EmbeddingNeighborLookup::calculateNeighborhoodThirdQuartiles()
{
    for(uint32_t neighborDistance = 1; neighborDistance <= maxNeighbors; ++neighborDistance)
    {
        std::vector<double> similarities;
        for(uint32_t i = 0; i < nearestNeighbors.size(); ++i)
        {
            similarities.push_back(nearestNeighbors[i][neighborDistance-1].similarity);
        }
        double thirdQuartile = EmbeddingMath::thirdQuartile(similarities);
        neighborhoodSimilarityThirdQuartiles.push_back(thirdQuartile);
    }
}

void EmbeddingNeighborLookup::saveNeighborhoodToCache(std::string cacheFilePath) 
{
    if(cacheFilePath.size() == 0)
    {
        std::cout << "Cache filepath is empty, therefore, no cache is saved." << std::endl;
        return;
    }

    std::cout << "Saving a cached version of the neighborhood to " << cacheFilePath << std::endl;
    std::ofstream outfile;
    outfile.open(cacheFilePath);
    for(auto& row: nearestNeighbors)
    {
        for(auto& neighbor: row)
        {
            outfile << neighbor.index << ";" << neighbor.similarity << "\t";
        }
        outfile << std::endl;
    }
    outfile.close();
    std::cout << "Cache saved to file." << std::endl;
}

void EmbeddingNeighborLookup::loadNeighborhoodFromCache(std::string cacheFilePath)
{
    assert(nearestNeighbors.size() == 0);

    std::ifstream ifs (cacheFilePath);
    std::string line;
    if (ifs.is_open())
    {
        // each line is a list of neighbors
        while (std::getline(ifs,line))
        {
            nearestNeighbors.push_back(std::vector<Neighbor>());
            // parse tab separated line of neighbors
            std::stringstream lineStream(line);
            std::string neighborString;
            while (std::getline(lineStream, neighborString, '\t')) {
                // each neighbor is stored as index;distance
                uint32_t delimiterPos = neighborString.find(";");
                uint32_t index = std::stoi(neighborString.substr(0, delimiterPos));
                double similarity = std::stod(neighborString.substr(delimiterPos+1));
                nearestNeighbors.back().push_back(Neighbor{index, similarity});
            }
            if (nearestNeighbors.back().size() != maxNeighbors)
            {
                throw std::runtime_error("In the cache, a row of neighbors does not contain the expected number of neighbors. Maybe the cache does not match the setting?");
            }
        }
    }
}


double EmbeddingMath::cosineSimilarity(Embedding& a, Embedding& b)
{
    assert(a.size() == b.size());
    double dot = 0;
    double denom_a = 0;
    double denom_b = 0;
     for(uint32_t i = 0; i < a.size(); ++i) {
        dot += a[i] * b[i] ;
        denom_a += a[i] * a[i] ;
        denom_b += b[i] * b[i] ;
    }
    return dot / (std::sqrt(denom_a) * std::sqrt(denom_b)) ;
}

double EmbeddingMath::median(std::vector<double>::iterator begin, std::vector<double>::iterator end, bool isSorted)
{
    if(!isSorted)
    {
        std::sort(begin, end);
    }
    uint32_t numValues = end - begin;
    if (numValues % 2 == 0)
    {
        return ( ( *(begin + (numValues/2)) + *(begin + (numValues/2)-1)) ) / 2;
    } else
    {
        return  *(begin + (numValues/2)) ;
    }
}


double EmbeddingMath::thirdQuartile(std::vector<double> values)
{
    std::sort(values.begin(), values.end());

    uint32_t numValues = values.size();
    if (numValues % 2 == 0)
    {
        return EmbeddingMath::median(values.begin()+numValues/2, values.end(), true);
    } else {
        // Method 2 variant from https://en.wikipedia.org/wiki/Quartile (including the median)
        return EmbeddingMath::median( (values.begin()+numValues/2), values.end(), true);
    }
}
