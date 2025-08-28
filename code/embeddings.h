/*
Code to load embeddings and look-up nearest neighbouring embeddings.
*/

#ifndef EMBEDDINGS_H
#define EMBEDDINGS_H

#include <string>
#include <vector>
#include <memory>

#include "bitcollection.h"
#include <cstdint>


typedef std::vector<double> Embedding;

class EmbeddingNeighborLookup
{
    public:
        // Args:
        //     embeddingFilepath: path to a file where each line represents a word embedding vector
        //                        the values of the vector are space separated
        //                        the i-th line corresponds to the vector of the i-th token/item i
        //     embeddingDim: dimensionality of the embeddings
        //     maxNeighbors: how many neighbors to store. Nearest neighbors can only be retrieved till this value
        //     embeddingCacheFile: a filepath to save a cached version of the embedding neighborhood or if the file already exists
        //                         to load from it. Use empty string "" if no cache should be used.
        EmbeddingNeighborLookup(std::string embeddingFilepath, uint32_t embeddingDim, uint32_t maxNeighbors, std::string embeddingCacheFilepath);

        // Get the index of the item that is a neighbor of the item with the given index
        // with the given distance.
        // i.e. distance=0 is the item itself (index), distance=1 is the nearest neighbor,
        // distance=2 is the second nearest neighbor, ...
        // distance must be <= maxNeighbors
        uint32_t getNearestNeighbor(uint32_t index, uint32_t distance=0) const;

        // Get the similarity of the item that is a neighbor of the item with the given index
        // with the given distance.
        // i.e. distance=0 is the item itself (index), distance=1 is the nearest neighbor,
        // distance=2 is the second nearest neighbor, ...
        // distance must be <= maxNeighbors
        double getNeighborSimilarity(uint32_t index, uint32_t distance) const;

        // returns true if the neighborhoods of index1 and index2 overlap
        // or if one index is in the neighborhood of the other.
        // not a fancy implementation, but neighborhoods are probably small
        // so maybe the most efficient solution in the end?
        bool doNeighborsOverlap(uint32_t index1, uint32_t index2) const;

        inline uint32_t getMaxNeighbors() const
        {
            return maxNeighbors;
        }

        // returns the number of embeddings loaded and for which
        // the neighborhood can be retrieved
        uint32_t getNumEmbeddings() const
        {
            return nearestNeighbors.size();
        }

        double getSimilarityThirdQuartile(uint32_t distance) const
        {
            return neighborhoodSimilarityThirdQuartiles[distance-1];
        }

    private:
        struct Neighbor
        {
            uint32_t index;
            double similarity;
        };

        static bool compareNeighbors(const Neighbor& a, const Neighbor& b)
        {
            return a.similarity > b.similarity; // we are using similarity, so higher should be first
        }

        // Saves the calculated neighborhood to a file for caching/loading
        // it more quickly on next run.
        void saveNeighborhoodToCache(std::string cacheFilePath);

        // Loads the neighborhood information from the cached file
        void loadNeighborhoodFromCache(std::string cacheFilePath);

        uint32_t maxNeighbors;
        std::unique_ptr<std::vector<Embedding> > loadEmbeddings(std::string embeddingFilepath, uint32_t embeddingDim);
        void calculateNeighborhood(std::vector<Embedding>& embeddings, double similarityFunc(Embedding& a, Embedding& b));
        // Neighbors could be filtered if they are not similar enough. Calculate the Quartiles of the 
        // for all neighborhood distances
        void calculateNeighborhoodThirdQuartiles();
        void addNeighbor(std::vector<Neighbor>& neighborhood, uint32_t neighborIndex, double neighborSimilarity);
        std::vector<std::vector<Neighbor> > nearestNeighbors;
        std::vector<double> neighborhoodSimilarityThirdQuartiles;
};


namespace EmbeddingMath
{
    double cosineSimilarity(Embedding& a, Embedding& b);
    // compute the median (can also specify if the list of values is already sorted)
    double median(std::vector<double>::iterator begin, std::vector<double>::iterator end, bool isSorted);
    double thirdQuartile(std::vector<double> values);
}

#endif