#include "premiseparameters.h"

PremiseParameters::PremiseParameters(double fisher_p_value, double clause_max_overlap, double min_overlap): 
                                     FISHER_P_VALUE(fisher_p_value), 
                                     CLAUSE_MAX_OVERLAP(clause_max_overlap),
                                     PAIRS_MIN_OVERLAP(min_overlap),
                                     RULE_MERGE_MIN_OVERLAP(min_overlap)
                                     {}

