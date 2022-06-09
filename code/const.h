#ifndef CONST_H
#define CONST_H

#include <stdlib.h>

namespace CONST
{
    // Threshold for number of ones in data for the usage of sparse bitvecs
    // If the proportion of ones is less than threshold, use sparse DS
    constexpr double SPARSETHRESH = 0.01;

    // Epsilon for double comparison
    constexpr double EPSDOUBLE = 10e-5;

    // whether to run generateCandidatesPairBiasedRules()
    constexpr bool PERFORM_GENERATE_CANDIDATES_PAIR = true;

    constexpr bool FILTER_MIN_NEIGHBOR_SIMILARITY = true;

    // whether to assume that adding more neighbors results in
    // a monotone increase of gain until a maximum is reached
    // allows to stop the search earlier.
    constexpr bool NEIGHBOR_MONOTONE_MINIMUM_ASSUMPTION = true;

    #define CORENUM 1
}

#endif // CONST_H
