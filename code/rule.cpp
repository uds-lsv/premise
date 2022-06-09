#include <iostream>
#include <memory>
#include <algorithm>
#include <assert.h>

#include "rule.h"
#include "bitcollection.h"

//#define RULETABLE_LOG_TRANS

bool Tail::argIsSubsetOfAClause(const BitCollection& arg) const 
{
    for(const auto& clause: *tailClauses)
    {
        if (clause->argIsSubset(arg))
        {
            return true;
        }
    }
    return false;
}

void Tail::calculateItemsFromClauses() 
{
    std::vector<BitCollection *> clausePointers;
    for(const auto& clause: *tailClauses)
    {
        clausePointers.push_back(clause.get());
    }
    // compute union of all clauses to get all items used
    // TODO: setunionMany is actually like a static method that does not change the 
    // tailClauses->at(0) object. Could be changed in bitcollection.h to reflect that.
    items = std::shared_ptr<BitCollection>(tailClauses->at(0)->setunionMany(clausePointers.cbegin(), clausePointers.cend()));
}

std::ostream &operator<<(std::ostream &os, const StRule &stRule)
{
    os << "ST[empty->" << stRule.tailIndex << " (" << stRule.tids->getPopCount() << " transactions)";
    #ifdef RULETABLE_LOG_TRANS
        os << "; Transactions: ";
        stRule.tids->print(os);
    #endif
    os << "]";
    return os;
}

std::ostream &operator<<(std::ostream &os, const Candidate &cand)
{
    os << "Candidate[";
    cand.head1->print(os);
    if (cand.head2 != nullptr)
    {
        os << "/";
        cand.head2->print(os);
    }
    os << "->" << *cand.tail;
    os << "; generation: " << cand.generationType << "; gain:" << cand.gain << "; fisher: " << cand.fisherValue;
    os << "; " << cand.tail->getTidsL1()->getPopCount() << "/" << cand.tail->getTidsL2()->getPopCount() << " transactions]";
    return os;
}

std::ostream& operator<<(std::ostream& os, const Tail& tail)
{
    for(uint32_t i = 0; i < tail.getClauses().size(); ++i)
    {
        std::unique_ptr<const std::vector<uint32_t> > ones (tail.getClauses().at(i)->getOnes());
        for(uint32_t j = 0; j < ones->size(); ++j)
        {
            os << ones->at(j);
            if (j != ones->size() -1) { // if not last element
                os << "-or-";
            }
        }
        if (i != tail.getClauses().size() -1) // if not last clause
        {
            os << " and ";
        }
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const BiasedRule &rule)
{
    os << "BiasedRule[ L1/L2 ->";
    os << *rule.tail;
    os << "; generation: " << rule.generationType << "; gain:" << rule.gain << "; fisher: " << rule.fisherValue;
    os << " (" << rule.tail->getTidsL1()->getPopCount() << "/" << rule.tail->getTidsL2()->getPopCount() << " transactions)";
    #ifdef RULETABLE_LOG_TRANS
        os << "  tids:  ";
        rule.tail->getTidsL1()->print(os);
        os << " / ";
        rule.tail->getTidsL2()->print(os);
    #endif
    os << "]";
    return os;
}

std::ostream& operator<<(std::ostream& os, const CandidateGenerationType type)
{
    switch(type)
    {
        case CandidateGenerationType::SINGLETON : return os << "Singleton";
        case CandidateGenerationType::PAIR : return os << "Pair";
        case CandidateGenerationType::COMP_SINGLETON_MERGE : return os << "Comp+Singleton";
        case CandidateGenerationType::COMP_COMP_MERGE : return os << "Comp+Comp";
        case CandidateGenerationType::PHASE2_OR_MERGE : return os << "Phase2-OR";
        case CandidateGenerationType::PHASE2_AND_MERGE : return os << "Phase2-AND";
        case CandidateGenerationType::NONE: return os << "None";
    }
    return os << "Unknown Generation Type";
}