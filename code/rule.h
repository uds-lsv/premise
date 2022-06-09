#ifndef RULE_H
#define RULE_H

#include <vector>

#include "bitcollection.h"

// a list of and-clauses (e.g. s1 and (s2 or s3))
// each entry in the vector represents a clause that is and'ed
// each item in the clause/BitCollection is or'ed
typedef std::vector<std::shared_ptr<BitCollection> > TailClauses;

class Tail
{
    public:
        Tail(std::shared_ptr<TailClauses> theTailClauses,
             std::shared_ptr<BitCollection> theTidsL1, std::shared_ptr<BitCollection> theTidsL2):
                 tailClauses(theTailClauses), tidsL1(theTidsL1), tidsL2(theTidsL2) {
                     calculateItemsFromClauses(); // item vector is cached
                     assert(tidsL1->getPopCount() > 0 || tidsL2->getPopCount() > 0); // making sure the tid lists are initalized
                 }

        Tail(const Tail&) = delete; // There should be no reason at the moment to copy a Tail. Could indicate a bug.

        double getDataCost();

        const std::shared_ptr<BitCollection> getTidsL1() const
        {
            return tidsL1;
        }

        const std::shared_ptr<BitCollection> getTidsL2() const
        {
            return tidsL2;
        }

        // using the vector iterators to allow iteration over tail
        using const_iterator = TailClauses::const_iterator;
        const_iterator begin() const
        {
            return tailClauses->cbegin();
        }

        const_iterator end() const
        {
            return tailClauses->cend();
        }

        uint32_t size() const
        {
            return tailClauses->size();
        }

        const TailClauses& getClauses() const
        {
            return *tailClauses;
        }

        // Returns all items in the tail
        std::shared_ptr<BitCollection> getItems() const
        {
            return items;
        }

        // Returns true if the given BitCollection is contained in at least one clause
        bool argIsSubsetOfAClause(const BitCollection& args) const;

    private:
        // a list of and-clauses (e.g. s1 and (s2 or s3))
        // each entry in the vector represents a clause that is and'ed
        // each item in the clause/BitCollection is or'ed
        std::shared_ptr<TailClauses> tailClauses;
        // the items contained in the tail (could be computed from the tailClauses)
        // but is cached as we need it quite often
        std::shared_ptr<BitCollection> items;
        // tids where rule holds (head + tail) for each label
        std::shared_ptr<BitCollection> tidsL1;
        std::shared_ptr<BitCollection> tidsL2;

        void calculateItemsFromClauses();
};

// Singleton Rule: empty head -> tail
struct StRule
{
    uint32_t tailIndex;
    std::shared_ptr<BitCollection> tail; // this is not a Tail object; could tecnically be one; but this also works
    std::shared_ptr<BitCollection> tids;
    // indicates if this singleton can't generate gain with some form of biased rule during the singleton candidate generation
    bool noGain;
};

// how the candidate was generated
enum class CandidateGenerationType
{
    SINGLETON, PAIR, COMP_SINGLETON_MERGE, COMP_COMP_MERGE, PHASE2_OR_MERGE, PHASE2_AND_MERGE, NONE
};

// (non-singleton) Rule:
struct BiasedRule
{
    std::shared_ptr<Tail> tail;

    // store how the candidate for this rule was generated
    CandidateGenerationType generationType;
    // the estimated gain obtained from its candidate
    double gain;
    // the Fisher's test value from its candidate
    double fisherValue;
};

// rule candidate
struct Candidate
{
    std::shared_ptr<BitCollection> head1;
    std::shared_ptr<BitCollection> head2;
    std::shared_ptr<Tail> tail;

    // Generating rules, is NULL if generated from singletons
    std::shared_ptr<BiasedRule> genRule1;
    std::shared_ptr<BiasedRule> genRule2;

    // store how the candidate was generated
    // for better inspecting and understanding what the algorithm does
    CandidateGenerationType generationType;
    // the estimated gain obtained of this candidate
    double gain;
    // the Fisher's test value of this candidate
    double fisherValue;
};

// Compares two candidates based on their gain
struct CandidateComparator
{
    bool operator()(const Candidate &lCand, const Candidate &rCand) const
    {
        return lCand.gain > rCand.gain;
    }
};

std::ostream& operator<<(std::ostream& os, const StRule& stRule);
std::ostream& operator<<(std::ostream& os, const Tail& tail);
std::ostream& operator<<(std::ostream& os, const BiasedRule& rule);
std::ostream& operator<<(std::ostream& os, const Candidate& cand);
std::ostream& operator<<(std::ostream& os, const CandidateGenerationType type);

#endif