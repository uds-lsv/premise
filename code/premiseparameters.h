#ifndef PREMISEPARAMETER_H
#define PREMISEPARAMETER_H


class PremiseParameters {
    
    public:
        PremiseParameters(double fisher_p_value, double clause_max_overlap, double min_overlap);

        const double FISHER_P_VALUE;

        // Maximum allowed overlap of neighbors
        const double CLAUSE_MAX_OVERLAP;

        // minimum overlap required by pairs
        const double PAIRS_MIN_OVERLAP;

        // minimum overlap required to allow merge
        const double RULE_MERGE_MIN_OVERLAP;
};


#endif