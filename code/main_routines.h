#ifndef MAIN_ROUTINES_H
#define MAIN_ROUTINES_H

#include <ostream>
#include <string>
#include "database.h"
#include "ruletable.h"
#include "embeddings.h"
#include "premiseparameters.h"

void main_routine(Database& db, const EmbeddingNeighborLookup& embed, const std::string resultFilePath, PremiseParameters& parameters);

void search_phase(RuleTable& rt);

// outputs all interesting rules (i.e. no singletons)
// found for the given RuleTable
void outputAllRules(RuleTable& rt, std::ostream& os=std::cout);

// Outputs the values of the const settings
void outputAllConstValues(PremiseParameters& parameters, std::ostream& os=std::cout);


#endif