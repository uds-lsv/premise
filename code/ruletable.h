#ifndef RULETABLE_H
#define RULETABLE_H

// #include <optional>
#include <queue>

#include "densebitvec.h"
#include "database.h"
#include "rule.h"
#include "embeddings.h"
#include "candidatequeue.h"
#include "premiseparameters.h"

class RuleTable
{

    public:

        // Minimum and maximum index of how many neighbours to be considered of singleton id1 respectively id2 to fulfill the minimum overlap criteria.
        struct stPairInfo
        {
            uint32_t id1;
            uint32_t k_min1;
            uint32_t id2;
            uint32_t k_min2;
        };

        RuleTable(Database& db, const EmbeddingNeighborLookup& embed, PremiseParameters& parameters);

        // the singleton rules describing the features
        const std::vector<StRule>& getStFeatures() const
        {
            return stFeatures;
        }

        // the singleton rules describing the features after initialization
        // before any other rules were added
        // useful for getting the tids for the estimated gains
        const std::vector<StRule>& getStFeaturesBaseline() const
        {
            return stFeaturesBaseline;
        }

        // returns the some of all the singleton usages of the features
        // i.e. how many feature-items appear over all transactions
        // needed to normalize probability in MDL later
        const uint32_t getStFeatureUsg() const
        {
            return stFeatureUsg;
        }

        const std::vector<StRule>& getStLabels() const
        {
            return stLabels;
        }

        // returns the some of all the singleton usages of the labels
        // i.e. how many label-items appear over all transactions
        // needed to normalize probability in MDL later
        const uint32_t getStLabelUsg() const
        {
            return stLabelUsg;
        }

        // the non-singelton rules
        const std::vector<std::shared_ptr<BiasedRule> >& getRules() const
        {
            return rules;
        }

        Database& getDB() const
        {
            return db;
        }

        // returns the current code length
        double getCL() const
        {
            assert(cl != -1); // code length not initialized
            return cl;
        }

        // generates the best candidate
        Candidate generateCandidate();
        // adds the given candidate as a rule (or two rules if it is a biased rule)
        void addCandidateAsRule(Candidate& cand);

        // the label in which the rule would shift the label-bias
        // return label indices (i.e. L1=0 and L2=1)
        uint32_t directionOfBias(std::shared_ptr<Tail> tail);


    protected:
        Database& db;
        const EmbeddingNeighborLookup& embed;
        const PremiseParameters parameters;
        std::vector<StRule> stFeatures;
        std::vector<StRule> stFeaturesBaseline;
        std::vector<StRule> stLabels;
        std::vector<std::shared_ptr<BiasedRule> > rules;
        std::vector<stPairInfo> pairsToExplore;
        // vector of largest neighbourhood index k for which the overlap between the neighbourhood still satisfies the threshold
        // ordered by item id, e.g. maxK[5] == 3 means that a clause of item 5 with more than the 3 closest neighbours has more overlap than CONST::CLAUSE_MAX_OVERLAP
        std::vector<uint32_t> maxK;
        uint32_t stFeatureUsg = 0;
        uint32_t stLabelUsg = 0;
        double cl = -1; //code length

        // generate all feature singletons
        void generateStFeatures();
        // generate all label (singeltons)
        void generateLabels();
        // go through database transactions and convert them to singletons
        // can be used either on the features (for stRules) or the labels (for stLabels)
        void dbToSingletons(std::vector<StRule>& singletonContainer, Database::DataDescriptor& dataDescriptor,
                            std::vector<std::shared_ptr<BitCollection> >& transactions);
        // compute code length
        double computeCL();

        // creates a Tail object from TailClauses incl. the calculation of the tids
        std::shared_ptr<Tail> createTailFromClauses(std::shared_ptr<TailClauses > tailClauses);

        std::shared_ptr<Tail> createTailFromSingleton(std::shared_ptr<BitCollection> sts);

        // creates a new Tail object from an OldTail and an appended new clause
        std::shared_ptr<Tail> createTailThroughExtension(const Tail& oldTail, std::shared_ptr<BitCollection> newClause);

        // creates a new Tail by merging two existing tails
        std::shared_ptr<Tail> createTailThroughMerge(const Tail& tail1, const Tail& tail2);

        std::shared_ptr<Tail> createTailThroughOverlapMerge(const Tail& tail1, const Tail& tail2);
        std::pair<bool, std::shared_ptr<Tail> > createTailThroughDisjunctionMerge(const Tail& tail1, const Tail& tail2);

        // for the generateCandidatesPairBiasedRules(), we can pre-compute once for all pairs
        // if it makes sense to look at that pair or if we can skip it because it has too low
        // of a max-overlap
        void checkMinOverlapPairs();

        // when not performing a phase2 search (i.e. neighborhood search is in phase1),
        // we can already precompute till which neighbor we have to check for each feature
        // until the overlap of the neighbors (within a clause) is higher than 
        // CONST::CLAUSE_MAX_OVERLAP (and can therefore be ignored). Also see variable maxK
        // which is filled in this method
        void checkMaxClauseOverlap();

        // Checks if the similarity of a neighbor is below the .75 quantile of
        // neighborhood similarities. The aim is to remove neighbors that are
        // not similar.
        // Updates variable maxK
        void checkMinEmbeddingSimilarity();

        // check if rule would be significant according to Fisher's Exact Test
        double checkTailFishersTest(std::shared_ptr<Tail>);

        // generates all canidates of the biased rule type
        // labels (heads) are singletons, tails are up to two singletons.
        // stores them in the given candidates priority queue
        // assumes that the transactions of the labels are all disjunct!
        void generateCandidatesSTBiasedRules(OnlyBestQueue<Candidate, CandidateComparator>& candidates);

        void generateCandidatesPairBiasedRules(OnlyBestQueue<Candidate, CandidateComparator>& candidates);
        void exploreNeighborhoodForCandidatesPairBiased(OnlyBestQueue<Candidate, CandidateComparator> &candidates,
                                                           uint32_t featureIdx1, uint32_t featureIdx2, uint32_t label1Idx, uint32_t label2Idx,
                                                           StRule &label1, StRule &label2,
                                                           uint32_t k_min1,
                                                           uint32_t k_min2);

        // Merges existing Biased rules with a singleton
        // stores them in the given candidates priority queue
        // assumes that the transactions of the labels are all disjunct!
        void generateCandidatesComplexSTBiasedRules(OnlyBestQueue<Candidate, CandidateComparator>& candidates);

        // Merges existing Biased rules with another existing Biased rule
        // stores them in the given candidates priority queue
        // assumes that the transactions of the labels are all disjunct!
        void generateCandidatesComplexComplexBiasedRules(OnlyBestQueue<Candidate, CandidateComparator>& candidates);

        void addBiasedRule(Candidate& cand);

        void removeGenRule(std::shared_ptr<BiasedRule> r);

        // updates the cover for all rules that have not yet been finalized
        void updateCoverAllRules();

        // updates the cover for a rule
        // no longer sending singletons where this is already covered by rules
        void updateCoverBiased(std::shared_ptr<BiasedRule> rule);

        // reset the singleton tids to baseline
        void resetSingletonTids();
};

std::ostream& operator<<(std::ostream& os, const RuleTable& rt);


#endif