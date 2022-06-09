#include <fstream>
#include <cmath>
#include "main_routines.h"
#include "rule.h"
#include "const.h"

#include <chrono>

void main_routine(Database &db, const EmbeddingNeighborLookup &embed, std::string resultFilePath, PremiseParameters& parameters)
{
    outputAllConstValues(parameters);

    RuleTable rt(db, embed, parameters);

    auto startTime = std::chrono::high_resolution_clock::now();

    search_phase(rt);

    std::chrono::duration<double> elapsedTime = std::chrono::high_resolution_clock::now() - startTime;
    std::cout << "Elapsed time: " << elapsedTime.count() / 60 << " minutes." << std::endl;

    std::ofstream resultFile(resultFilePath);
    outputAllRules(rt, resultFile);
}

void search_phase(RuleTable &rt)
{
    std::cout << "** Search Phase **" << std::endl;

    double oldCL = rt.getCL();
    bool addedNewCandidate;
    do
    {
        addedNewCandidate = false;

        Candidate cand = rt.generateCandidate();
        if (cand.gain < 0)
        {
            addedNewCandidate = true;
            rt.addCandidateAsRule(cand);
            double gainDiff = cand.gain - (rt.getCL() - oldCL);
            std::cout << "Difference to estimated gain: (" << cand.gain << " - " << (rt.getCL() - oldCL) << ") = " << gainDiff << "\n";
            assert(std::abs(gainDiff) < 0.001);
            std::cout << rt << "\n";
            oldCL = rt.getCL();
        }
    } while (addedNewCandidate);
}


void outputAllRules(RuleTable &rt, std::ostream &os)
{
    for (auto &rule : rt.getRules())
    {
        os << *rule->tail;
        os << ";" << rule->tail->getTidsL1()->getPopCount() << ":" << rule->tail->getTidsL2()->getPopCount();
        os << ";" << rt.directionOfBias(rule->tail);
        os << ";" << rule->fisherValue;
        os << ";" << rule->gain;
        os << ";" << rule->generationType;
        os << "\n";
    }
}

void outputAllConstValues(PremiseParameters& parameters, std::ostream& os)
{
    os << "* Const values *\n";
    os << "Epsilon for double: " << CONST::EPSDOUBLE << std::endl;
    os << "Filter min neighbor similarity: " << CONST::FILTER_MIN_NEIGHBOR_SIMILARITY << std::endl;
    os << "Fisher p value: " << parameters.FISHER_P_VALUE << std::endl;
    os << "Neighbor monotone minimum assumption: " << CONST::NEIGHBOR_MONOTONE_MINIMUM_ASSUMPTION << std::endl;
    os << "Pairs min overlap: " << parameters.PAIRS_MIN_OVERLAP << std::endl;
    os << "Perform generate candidates pair: " << CONST::PERFORM_GENERATE_CANDIDATES_PAIR << std::endl;
    os << "Rule merge min overlap: " << parameters.RULE_MERGE_MIN_OVERLAP << std::endl;
    os << "Clause max overlap: " << parameters.CLAUSE_MAX_OVERLAP << std::endl;
    os << "Sparse threshold: " << CONST::SPARSETHRESH << std::endl;
}