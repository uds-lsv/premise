#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include "main_routines.h"
#include "database.h"
#include "embeddings.h"
#include "premiseparameters.h"

#include "mdlalgo.h"


int main(int argc, char *argv[])
{
    if(argc == 7 || argc == 10)
    {
        std::string pathFeatures = argv[1];
        std::string pathLabels = argv[2];
        std::string pathResult = argv[3];
        std::string pathEmbeddings = argv[4];
        uint32_t embeddingSize = std::stoi(argv[5]);
        uint32_t neighborMaxDistance = std::stoi(argv[6]);
        std::string embeddingCache = "";
        
        
        double fisher_p_value;
        double clause_max_overlap;
        double min_overlap;
        if (argc == 7)
        {
            fisher_p_value = 0.01;
            clause_max_overlap = 0.05;
            min_overlap = 0.3;
        }
        else
        {
            fisher_p_value = std::stod(argv[7]);
            clause_max_overlap = std::stod(argv[8]);
            min_overlap = std::stod(argv[9]);
        }
        PremiseParameters parameters = PremiseParameters(fisher_p_value, clause_max_overlap, min_overlap);

        EmbeddingNeighborLookup embeddingLookup(pathEmbeddings, embeddingSize, neighborMaxDistance, embeddingCache);
        Database db (pathFeatures, pathLabels);
        main_routine(db, embeddingLookup, pathResult, parameters);
    }
    else
    {
        std::cout << "Either 6 arguments are required:" << std::endl << "path_to_features.dat" << std::endl;
        std::cout << "path_to_labels.dat" << std::endl << "path_to_store_results.dat" << std::endl << "path_to_embeddings.dat" << std::endl;
        std::cout << "embedding size (uint)" << std::endl << "max_neighbor_distance (uint)" << std::endl << std::endl;
        std::cout << "Or 9 arguments are required, the 7 from above plus:" << std::endl;
        std::cout << "fisher p-value (float, 0-1)" << std::endl << "clause_max_overlap (float, 0-1)" << std::endl << "min_overlap (float, 0-1)" << std::endl;
        std::cout << "If you do not want to use embeddings, set max_neighbor_distance and embedding_size to 0 and point the path_to_embeddings to an empty file." << std::endl;
        throw std::runtime_error("Incorrect number of arguments!");
    }

    return 0;
}