#include <vector>
#include <cassert>

#include "ruletable.h"
#include "bitfactory.h"
#include "mdlalgo.h"
#include "const.h"

RuleTable::RuleTable(Database &input_db, const EmbeddingNeighborLookup& input_embed, PremiseParameters& premise_parameters):
        db(input_db),
        embed(input_embed),
        parameters(premise_parameters),
        maxK(input_db.getDescriptorFeatures().numCols, embed.getMaxNeighbors())
{
    if(db.getDescriptorFeatures().numCols != embed.getNumEmbeddings() && embed.getMaxNeighbors() > 0)
    {
        throw std::runtime_error("The number of embeddings must be equal to the number of items/unique tokens in the data. Or the neighborhood size must be 0 (no neighbors used).");
    }
    generateStFeatures();
    generateLabels();
    computeCL();

    if(CONST::PERFORM_GENERATE_CANDIDATES_PAIR)
    {
        checkMinOverlapPairs();
    }

    checkMaxClauseOverlap();

    if(CONST::FILTER_MIN_NEIGHBOR_SIMILARITY)
    {
        checkMinEmbeddingSimilarity();
    }
}

void RuleTable::generateStFeatures()
{
    // create a singleton rule for each feature
    dbToSingletons(stFeatures, db.getDescriptorFeatures(), db.getFeatures());

    // deep copy stFeatures to stFeaturesBaseline
    stFeaturesBaseline.reserve(stFeatures.size());
    for (uint32_t stIdx = 0; stIdx < stFeatures.size(); ++stIdx)
    {
        stFeaturesBaseline.push_back({stFeatures[stIdx].tailIndex,
                                      stFeatures[stIdx].tail,
                                      std::shared_ptr<BitCollection>(stFeatures[stIdx].tids->copy()),
                                      false});
    }

    stFeatureUsg = 0;
    for (StRule stRule : stFeatures)
    {
        stFeatureUsg += stRule.tids->getPopCount();
    }
    // TODO: Should be equivalent, therefore, do we need to do the summing up?
    assert(stFeatureUsg == db.getDescriptorFeatures().numOnes);
}

void RuleTable::generateLabels()
{
    dbToSingletons(stLabels, db.getDescriptorLabels(), db.getLabels());

    stLabelUsg = 0;
    for (StRule &stLabel : stLabels)
    {
        stLabelUsg += stLabel.tids->getPopCount();
    }

    // TODO: Should be equivalent, therefore, do we need to do the summing up?
    assert(stLabelUsg == db.getDescriptorLabels().numOnes);
}

void RuleTable::dbToSingletons(std::vector<StRule> &singletonContainer, Database::DataDescriptor &dataDescriptor,
                               std::vector<std::shared_ptr<BitCollection>> &transactions)
{
    // collect from the list of transactions a matrix of ones where the rows are
    // the item indices and the columns the transaction indices
    // this is afterwards used to set the tids for the singletons.
    // TODO: Is this more efficient than creating empty tids and then going
    // through the transactions and setting the corresponding bits? (not allowed for sparse-vector)
    std::vector<std::vector<uint32_t>> ones(dataDescriptor.numCols);
    uint32_t tid = 0;
    for (auto &transaction : transactions)
    {
        std::unique_ptr<const std::vector<uint32_t> > cover(transaction->getOnes());
        for (uint32_t coverIdx : *cover)
        {
            ones[coverIdx].push_back(tid);
        }
        ++tid;
    }

    // create a singleton rule for each item (feature or label)
    auto tails = BitFactory::getSingletons(dataDescriptor.dataType, dataDescriptor.numCols);
    for (uint32_t stIdx = 0; stIdx < dataDescriptor.numCols; ++stIdx)
    {
        singletonContainer.push_back({stIdx, tails[stIdx],
                                      BitFactory::genBitrep(dataDescriptor.dataType, ones[stIdx], dataDescriptor.numRows),
                                      false});
    }
}

void RuleTable::checkMinOverlapPairs()
{
    // check for all pairs if max overlap is given (i.e. if it makes sense to explore)
    std::cout << "Checking which pairs have enough overlap." << std::endl;
    uint32_t numFeatures = db.getDescriptorFeatures().numCols;
    uint32_t countThresholdPairs = 0;
    pairsToExplore.reserve(numFeatures);
    for(uint32_t i=0; i < numFeatures; ++i)
    {
        // Minimum neighbor distance needed to pass MIN_OVERLAP 
        uint32_t k_mini = embed.getMaxNeighbors() + 1;
        // Current count of how many transactions overlap in clause around i
        std::unique_ptr<BitCollection> iTids (new dbitvec(this->getDB().getDescriptorFeatures().numRows));
        for(uint32_t j=0; j < numFeatures; ++j)
        {
            if(i == j) // do not make to 'and' items with the same item
            {
                continue;
            }

            std::unique_ptr<BitCollection> jTids (new dbitvec(this->getDB().getDescriptorFeatures().numRows));
            uint32_t k_minj = embed.getMaxNeighbors() + 1;

            for (uint32_t ki = 0; ki <= embed.getMaxNeighbors(); ++ki)
            {
                iTids->thisUnion(*this->getStFeaturesBaseline()[embed.getNearestNeighbor(i, ki)].tids);
                for (uint32_t kj = 0; kj <= embed.getMaxNeighbors(); ++kj)
                {
                    // See if this neighbour has any overlap with the current i-clause at all
                    const uint32_t kId = embed.getNearestNeighbor(j, kj);
                    if (this->stFeaturesBaseline[kId].tids->intersectionCount(*iTids) == 0)
                    {
                        break; // TODO if we want to allow skipping items, we might have to change this
                    }
                    // If so, check if the new clause has enough overlap
                    jTids->thisUnion(*this->getStFeaturesBaseline()[kId].tids);
                    double overlap = iTids->intersectionCount(*jTids);
                    double overlapPercentage = std::min(overlap/iTids->getPopCount(), overlap/jTids->getPopCount());
                    if (overlapPercentage > parameters.PAIRS_MIN_OVERLAP)
                    {
                        k_mini = std::min(k_mini, ki);
                        k_minj = std::min(k_minj, kj);
                    }
                }
            } 
            if (k_mini <= embed.getMaxNeighbors() && k_minj <= embed.getMaxNeighbors()) 
            {
                ++countThresholdPairs;
                pairsToExplore.push_back({i, k_mini, j, k_minj});
            }
        }
    }
    std::cout << "For the pair search using only " << countThresholdPairs << " of the " << numFeatures*numFeatures << " possible pairs ";
    std::cout << "(" << ((double) countThresholdPairs / (numFeatures*numFeatures)) << ") due to the threshold of " << parameters.PAIRS_MIN_OVERLAP << std::endl;
}

void RuleTable::checkMaxClauseOverlap()
{
    //go once over each item singleton and test maxoverlap for neighbourhood
    uint32_t numFeatures = db.getDescriptorFeatures().numCols;

    std::cout << "Checking for each feature which neighbors are disjunct enough (Clause max overlap)." << std::endl;
    for(uint32_t i = 0; i < numFeatures; ++i)
    {
        std::unique_ptr<BitCollection> tids (this->stFeaturesBaseline[i].tids->copy());
        std::unique_ptr<BitCollection> overlapTids (new dbitvec(this->getDB().getDescriptorFeatures().numRows));
        for (uint32_t k = 1; k <= embed.getMaxNeighbors(); ++k)
        {
            const uint32_t kId = embed.getNearestNeighbor(i, k);
            std::unique_ptr<BitCollection> inter (tids->setinter(*this->stFeaturesBaseline[kId].tids));
            overlapTids->thisUnion(*inter);
            tids->thisUnion(*this->stFeaturesBaseline[kId].tids);
            
            if (overlapTids->getPopCount()/(float)tids->getPopCount() > parameters.CLAUSE_MAX_OVERLAP)
            {
                maxK[i] = k-1;
                break;
            } else 
            {
                //std::cout << "For this one it actually does work" << std::endl;
            }
        }
    }
}

void RuleTable::checkMinEmbeddingSimilarity()
{
    uint32_t numFeatures = db.getDescriptorFeatures().numCols;
    std::cout << "Checking for min embedding similarity." << std::endl;
    for(uint32_t i = 0; i < numFeatures; ++i)
    {
        for (uint32_t k = 1; k <= embed.getMaxNeighbors(); ++k)
        {
            if(embed.getNeighborSimilarity(i,k) < embed.getSimilarityThirdQuartile(k))
            {
                maxK[i] = std::min(maxK[i],k-1);
                break;
            }
        }
    }
}

double RuleTable::computeCL()
{
    cl = MDLAlgo::computeCLData(*this) + MDLAlgo::computeCLRuleTable(*this);
    return cl;
}

Candidate RuleTable::generateCandidate()
{
    std::cout << "Generating a candidate\n";
    OnlyBestQueue<Candidate, CandidateComparator> candidates;
    generateCandidatesSTBiasedRules(candidates);

    if(CONST::PERFORM_GENERATE_CANDIDATES_PAIR)
    {
        generateCandidatesPairBiasedRules(candidates);
    }
    generateCandidatesComplexSTBiasedRules(candidates);
    generateCandidatesComplexComplexBiasedRules(candidates);
    if (!candidates.isEmpty())
    {
        return candidates.top();
    }
    else // if no candidate achieves a gain
    {
        return {NULL, NULL, NULL, NULL, NULL, CandidateGenerationType::NONE, 1};
    }
}

void RuleTable::addCandidateAsRule(Candidate &cand)
{
    std::cout << "Adding " << cand << "\n";
    addBiasedRule(cand);
    removeGenRule(cand.genRule1);
    removeGenRule(cand.genRule2);
    updateCoverAllRules();
    computeCL();
}

void RuleTable::addBiasedRule(Candidate &cand)
{
    // current implementation of biased rule assumes that there are exactly 2 labels
    assert(this->getDB().getDescriptorLabels().numCols == 2);

    std::shared_ptr<BiasedRule> rule(new BiasedRule{cand.tail, cand.generationType, cand.gain, cand.fisherValue});
    rules.push_back(rule);
}

void RuleTable::removeGenRule(std::shared_ptr<BiasedRule> r)
{
    if (r == nullptr)
        return;

    for (auto rIt = this->rules.begin(); rIt != this->rules.end(); ++rIt)
    {
        if (r == *rIt) // TODO: Identity not value equality, should be fine but maybe one should think about it again
        {
            this->rules.erase(rIt);
            return;
        }
    }
    // If rule is not existing in table something definetely went wrong
    std::cerr << "No rule remove - something went wrong.\n\n";
    assert(false);
}

void RuleTable::updateCoverAllRules()
{
    // several rules might have been added in the last step
    this->resetSingletonTids();
    for (auto &rule : rules)
    {
        updateCoverBiased(rule);
    } // TODO: Do we need to update the cover for all rules? A lot will not have changed
}

void RuleTable::updateCoverBiased(std::shared_ptr<BiasedRule> newRule)
{
    // for each clause in the new rule
    for (std::shared_ptr<BitCollection> clause : *newRule->tail)
    {
        std::unique_ptr<const std::vector<uint32_t> > cOnes (clause->getOnes());
        for(uint32_t featureIdx: *cOnes)
        {
            StRule &stFeatureRule = stFeatures[featureIdx];
            // do not send the singleton anymore for the tids
            // covered by the new rule
            stFeatureRule.tids->thisAndNot(*(newRule->tail->getTidsL1()));
            stFeatureRule.tids->thisAndNot(*(newRule->tail->getTidsL2()));
        }
    }
}

void RuleTable::resetSingletonTids()
{
    for (size_t stIdx = 0; stIdx < this->stFeatures.size(); ++stIdx)
    {
        this->stFeatures[stIdx].tids->copyContent(this->stFeaturesBaseline[stIdx].tids.get());
    }
}

std::shared_ptr<Tail> RuleTable::createTailFromClauses(std::shared_ptr<TailClauses> tailClauses)
{
    assert(stLabels.size() == 2); // only implemented for label1 and label2
    auto tidsL1 = MDLAlgo::computeTidsBiased(0, tailClauses, *this);
    auto tidsL2 = MDLAlgo::computeTidsBiased(1, tailClauses, *this);
    std::shared_ptr<Tail> newTailPtr (new Tail(tailClauses, tidsL1, tidsL2));
    return newTailPtr;
}

std::shared_ptr<Tail> RuleTable::createTailThroughExtension(const Tail& oldTail, std::shared_ptr<BitCollection> newClause)
{
    std::shared_ptr<TailClauses> newClauses(new TailClauses(oldTail.getClauses()));
    newClauses->push_back(newClause);
    return createTailFromClauses(newClauses);
}

std::shared_ptr<Tail> RuleTable::createTailThroughMerge(const Tail& tail1, const Tail& tail2)
{
    std::shared_ptr<TailClauses> newClauses(new TailClauses(tail1.getClauses()));
    newClauses->reserve(newClauses->size() + tail2.getClauses().size());
    newClauses->insert(newClauses->end(),tail2.getClauses().begin(),tail2.getClauses().end());
    return createTailFromClauses(newClauses);
}

std::shared_ptr<Tail> RuleTable::createTailFromSingleton(std::shared_ptr<BitCollection> sts)
{
    std::shared_ptr<TailClauses> clauses(new TailClauses{sts});
    return createTailFromClauses(clauses);
}

uint32_t RuleTable::directionOfBias(std::shared_ptr<Tail> tail)
{
    uint32_t ruleHoldsLabel1 = tail->getTidsL1()->getPopCount();
    uint32_t notRuleHoldsLabel1 = stLabels[0].tids->getPopCount() - ruleHoldsLabel1;
    uint32_t ruleHoldsLabel2 = tail->getTidsL2()->getPopCount();
    uint32_t notRuleHoldsLabel2 = stLabels[1].tids->getPopCount() - ruleHoldsLabel2;

    if( (ruleHoldsLabel1/(double)notRuleHoldsLabel1) > (ruleHoldsLabel2/(double)notRuleHoldsLabel2))
    {
        return 0;
    }
    return 1;
}

double RuleTable::checkTailFishersTest(std::shared_ptr<Tail> tail)
{
    uint32_t directionOfBias = this->directionOfBias(tail);
    assert(directionOfBias == 0 || directionOfBias == 1);

    uint32_t ruleHoldsLabel1 = tail->getTidsL1()->getPopCount();
    uint32_t notRuleHoldsLabel1 = stLabels[0].tids->getPopCount() - ruleHoldsLabel1;
    uint32_t ruleHoldsLabel2 = tail->getTidsL2()->getPopCount();
    uint32_t notRuleHoldsLabel2 = stLabels[1].tids->getPopCount() - ruleHoldsLabel2;

    // a one sided (left-sided) test is performed, so depending whether the biased rule tends
    // towards L1 or L2, we have to switch the values
    if(directionOfBias == 0)
    {
        return MDLMath::fishersExactTest(notRuleHoldsLabel1, notRuleHoldsLabel2, ruleHoldsLabel1, ruleHoldsLabel2);
    } else {
        return MDLMath::fishersExactTest(notRuleHoldsLabel2, notRuleHoldsLabel1, ruleHoldsLabel2, ruleHoldsLabel1);
    }
}


void RuleTable::generateCandidatesSTBiasedRules(OnlyBestQueue<Candidate, CandidateComparator> &candidates)
{
    assert(stLabels.size() == 2); // only implemented for two fixed labels

    uint32_t numTransactions = db.getDescriptorFeatures().numRows;
    uint32_t label1Idx = 0;
    uint32_t label2Idx = 1;

    StRule label1 = stLabels[label1Idx];
    StRule label2 = stLabels[label2Idx];

    // assumed that intersection is 0 by MDLAlgo below
    assert(label1.tids->intersectionCount(*label2.tids) == 0);

    // for each feature
    for (uint32_t featureIdx = 0; featureIdx < stFeatures.size(); ++featureIdx)
    {
        StRule &feature = stFeatures[featureIdx];
        uint32_t usgFeature = feature.tids->getPopCount();

        if (usgFeature == 0)
            continue;

        if (!feature.noGain)
        {
            bool yieldsGain = false;

            std::shared_ptr<BitCollection> clause(new dbitvec(stFeatures.size()));
            double bestGain = -CONST::EPSDOUBLE; // minimum gain needed

            for (uint32_t k = 0; k <= this->maxK[featureIdx]; ++k)
            {
                uint32_t neigh = embed.getNearestNeighbor(featureIdx, k);
                clause->setBit(neigh, true);

                double newGain = MDLAlgo::gainBiasedStRule(numTransactions,
                                                           clause,
                                                           label1Idx,
                                                           label2Idx,
                                                           *this);

                if (newGain > bestGain) // Monotonicity assumpution
                {
                    break;
                }
                if (newGain < -CONST::EPSDOUBLE)
                {
                    yieldsGain = true;

                    // only need to create Candidate if better then best current candidate
                    if(! (candidates.isEmpty() || newGain < candidates.top().gain) ) 
                    {
                        continue;
                    }

                    auto tail = createTailFromSingleton(std::shared_ptr<BitCollection>(clause->copy()));
                    double fisherValue = checkTailFishersTest(tail);
                    if (fisherValue > parameters.FISHER_P_VALUE)
                    {
                        continue;
                    }

                    Candidate cand = {
                            label1.tail,
                            label2.tail,
                            tail,
                            nullptr,
                            nullptr,
                            CandidateGenerationType::SINGLETON,
                            newGain,
                            fisherValue};
                    candidates.push(cand);
                }
            }

            if (!yieldsGain)
            {
                feature.noGain = true;
            }
        }
    }
}

void RuleTable::generateCandidatesPairBiasedRules(OnlyBestQueue<Candidate, CandidateComparator> &candidates)
{
    assert(stLabels.size() == 2); // only implemented for two fixed labels

    uint32_t label1Idx = 0;
    uint32_t label2Idx = 1;

    StRule label1 = stLabels[label1Idx];
    StRule label2 = stLabels[label2Idx];

    // assumed that intersection is 0 by MDLAlgo below
    assert(label1.tids->intersectionCount(*label2.tids) == 0);

    // for all pairs of features that fulfill min overlap criterion
    for (const stPairInfo stPair : this->pairsToExplore)
    {
        StRule &feature1 = stFeatures[stPair.id1];
        if (feature1.tids->getPopCount() == 0)
            continue;

        StRule &feature2 = stFeatures[stPair.id2];

        if (feature2.tids->getPopCount() == 0)
            continue;

        exploreNeighborhoodForCandidatesPairBiased(candidates, stPair.id1, stPair.id2, label1Idx, label2Idx, label1, label2, stPair.k_min1, stPair.k_min2);
        // if the two features have in their neighbors overlapping items, then exploreNeighborhood is not symetric
        // in its search. Therefore, also perform in opposite order.
        // TODO: Couldn't this be remove as we now do all pairs symetrically in the first search part
        if(embed.doNeighborsOverlap(stPair.id1, stPair.id2))
        {
            exploreNeighborhoodForCandidatesPairBiased(candidates, stPair.id2, stPair.id1, label1Idx, label2Idx, label1, label2, stPair.k_min2, stPair.k_min1);
        }
    }
}

void RuleTable::exploreNeighborhoodForCandidatesPairBiased(OnlyBestQueue<Candidate, CandidateComparator> &candidates,
                                                           uint32_t featureIdx1, uint32_t featureIdx2, uint32_t label1Idx, uint32_t label2Idx,
                                                           StRule &label1, StRule &label2,
                                                           uint32_t k_min1,
                                                           uint32_t k_min2)
{
    uint32_t numTransactions = db.getDescriptorFeatures().numRows;

    // generate clause for feature 1 neighbourhood
    std::shared_ptr<BitCollection> clause1(new dbitvec(stFeatures.size()));

    // go through feature 1 neighbourhood
    // add at least min neighbors
    for (uint32_t k1 = 0; k1 < k_min1; ++k1)
    {
        uint32_t neigh1 = embed.getNearestNeighbor(featureIdx1, k1);
        clause1->setBit(neigh1, true);
    }
    // now check the rest of the neighborhood
    for (uint32_t k1 = k_min1; k1 <= this->maxK[featureIdx1]; ++k1)
    {
        uint32_t neigh1 = embed.getNearestNeighbor(featureIdx1, k1);

        //TODO: If neighbor never occurs in the data, should not encode?
        // But have to indicate that neighbor has to be skipped

        // add neighbour to clause
        clause1->setBit(neigh1, true);

        // generate clause for feature 2 neighbourhood
        std::shared_ptr<BitCollection> clause2(new dbitvec(stFeatures.size()));
        for (uint32_t k2 = 0; k2 < k_min2; ++k2)
        {
            uint32_t neigh2 = embed.getNearestNeighbor(featureIdx2, k2);
            if ((*clause1)[neigh2])
            {
                continue;
            }
            clause2->setBit(neigh2, true);
        }
        for (uint32_t k2 = k_min2; k2 <= this->maxK[featureIdx2]; ++k2)
        {
            uint32_t neigh2 = embed.getNearestNeighbor(featureIdx2, k2);

            //TODO: If neighbor never occurs in the data, should not encode?
            // But have to indicate that neighbor has to be skipped

            // if this item already appears in clause1, we do not want it in clause2
            // skip item then.
            // If clause1 and clause2 share a neighbor, this results in this neighbor
            // only being possible in clause1. Therefore, breaks symetry. Calling function
            // should take care of that.
            if ((*clause1)[neigh2])
            {
                continue;
            }

            clause2->setBit(neigh2, true);

            double gain = MDLAlgo::gainEstBiasedStRulePair(numTransactions,
                                                    clause1,
                                                    clause2,
                                                    label1Idx,
                                                    label2Idx,
                                                    *this);


            // TODO: We can run again into symetry issues here. Therefore,
            // deactivated for now
            // under the assumption that there is a simple monoton
            // change in gain till a maximum is reached
            // stop if this maximum is reached for this neighborhood
            /*if(CONST::NEIGHBOR_MONOTONE_MINIMUM_ASSUMPTION)
            {
                if (gain < bestGainClause2)
                {
                    bestGainClause2 = gain;
                }
                else
                {
                    break;
                }
            }*/          

            if (gain > -CONST::EPSDOUBLE)
            {
                continue;
            }

            // only need to add if better then current best
            if (! (candidates.isEmpty() || gain < candidates.top().gain)) 
            {
                continue;
            }

            std::shared_ptr<Tail> tail = createTailFromClauses(std::shared_ptr<TailClauses>(new TailClauses{std::shared_ptr<BitCollection>(clause1->copy()),
                                                                                       std::shared_ptr<BitCollection>(clause2->copy())}));  

            double fisherValue = checkTailFishersTest(tail);
            if(fisherValue > parameters.FISHER_P_VALUE)
            {
                continue;
            }

            Candidate cand = {
                    label1.tail,
                    label2.tail,
                    tail,
                    nullptr,
                    nullptr,
                    CandidateGenerationType::PAIR,
                    gain,
                    fisherValue};
            candidates.push(cand);
        }
    }
}

void RuleTable::generateCandidatesComplexSTBiasedRules(OnlyBestQueue<Candidate, CandidateComparator> &candidates)
{
    uint32_t numTransactions = db.getDescriptorFeatures().numRows;
    assert(stLabels.size() == 2);

    // Merge each rule with other thing
    for (size_t rIdx = 0; rIdx < this->rules.size(); ++rIdx)
    {
        std::shared_ptr<BiasedRule> r = this->rules[rIdx];
        // Merge r with a singleton or singleton clause
        for (size_t stIdx = 0; stIdx < this->stFeatures.size(); ++stIdx)
        {

            const StRule &st = this->stFeatures[stIdx];

            // If there is nothing to be covered or is already part of rule, skip it
            // To be aware: This enforces that each item is only possible in maximum one of the clauses
            if (st.tids->getPopCount() == 0 || r->tail->argIsSubsetOfAClause(*st.tail))
            {
                continue;
            }


            double gain = 0;

            std::shared_ptr<BitCollection> stClause (new dbitvec(stFeatures.size()));
            for (uint32_t k = 0; k <= this->maxK[stIdx]; ++k)
            {
                uint32_t neigh = embed.getNearestNeighbor(stIdx, k);
                stClause->setBit(neigh, true);

                // if current neighbour does not have enough overlap with rule
                // TODO: super-biased rules (only in L1 or L2) result in division by zero
                if ( (r->tail->getTidsL1()->getPopCount() != 0 &&                 
                    r->tail->getTidsL1()->intersectionCount(*this->stFeaturesBaseline[neigh].tids)/(float)r->tail->getTidsL1()->getPopCount() <= parameters.RULE_MERGE_MIN_OVERLAP )
                     ||
                    ( r->tail->getTidsL2()->getPopCount() != 0 &&
                r->tail->getTidsL2()->intersectionCount(*this->stFeaturesBaseline[neigh].tids)/(float)r->tail->getTidsL2()->getPopCount() <= parameters.RULE_MERGE_MIN_OVERLAP ) )
                {
                    break;
                }

                double newGain = MDLAlgo::gainEstBiasedComplexStRule(numTransactions,
                                                                    r,
                                                                    stClause,
                                                                    0,
                                                                    1,
                                                                    *this);
                if (newGain > gain)
                {
                    continue;
                }
                gain = newGain;
                if (! (candidates.isEmpty() || gain < candidates.top().gain)) // only need to add candidate if better then current best
                {
                    continue;
                }

                auto tail = createTailThroughExtension(*r->tail, std::shared_ptr<BitCollection> (stClause->copy()));
                double fisherValue = checkTailFishersTest(tail);
                if(fisherValue > parameters.FISHER_P_VALUE)
                {
                    continue;
                }
                
                Candidate cand = {
                        getStLabels()[0].tail,
                        getStLabels()[1].tail,
                        tail,
                        r,
                        NULL,
                        CandidateGenerationType::COMP_SINGLETON_MERGE,
                        gain,
                        fisherValue};
                candidates.push(cand);
            }
        }
    }
}

void RuleTable::generateCandidatesComplexComplexBiasedRules(OnlyBestQueue<Candidate, CandidateComparator> &candidates)
{
    uint32_t numTransactions = db.getDescriptorFeatures().numRows;
    assert(stLabels.size() == 2);

    // Merge each rule with other thing
    for (size_t r1Idx = 0; r1Idx < this->rules.size(); ++r1Idx)
    {
        std::shared_ptr<BiasedRule> r1 = this->rules[r1Idx];
        //Merge rule with other rule
        for (size_t r2Idx = r1Idx + 1; r2Idx < this->rules.size(); ++r2Idx)
        {
            std::shared_ptr<BiasedRule> r2 = this->rules[r2Idx];


            // We currently do not allow overlap for merging rules
            if (r1->tail->getItems()->hasOverlap(*r2->tail->getItems()))
            {
                continue;
            }

            // if rules do not overlap enough
            // TODO: super-biased rules (only in L1 or L2) result in division by zero
            if ( 
                (std::min(r1->tail->getTidsL1()->getPopCount(), r2->tail->getTidsL1()->getPopCount()) != 0 &&
                 (double)r1->tail->getTidsL1()->intersectionCount(*r2->tail->getTidsL1())/std::min(r1->tail->getTidsL1()->getPopCount(), r2->tail->getTidsL1()->getPopCount()) <= parameters.RULE_MERGE_MIN_OVERLAP )
              ||
              ( std::min(r1->tail->getTidsL2()->getPopCount(), r2->tail->getTidsL2()->getPopCount()) != 0 &&
            (double)r1->tail->getTidsL2()->intersectionCount(*r2->tail->getTidsL2())/std::min(r1->tail->getTidsL2()->getPopCount(), r2->tail->getTidsL2()->getPopCount()) <= parameters.RULE_MERGE_MIN_OVERLAP) )
            {
                continue;
            }

            double gain = MDLAlgo::gainEstBiasedComplexComplexRule(numTransactions,
                                                                    r1,
                                                                    r2,
                                                                    0,
                                                                    1,
                                                                    *this);
            if (gain > -CONST::EPSDOUBLE)
            {
                continue;
            }

            if (! (candidates.isEmpty() || gain < candidates.top().gain))
            {
                continue;
            }
            
            auto tail = createTailThroughMerge(*r1->tail, *r2->tail);

            double fisherValue = checkTailFishersTest(tail);
            if(fisherValue > parameters.FISHER_P_VALUE)
            {
                continue;
            }

            Candidate cand = {
                    stLabels[0].tail,
                    stLabels[1].tail,
                    tail,
                    r1,
                    r2,
                    CandidateGenerationType::COMP_COMP_MERGE,
                    gain,
                    fisherValue};
            candidates.push(cand);
        }
    }
}

std::ostream &operator<<(std::ostream &os, const RuleTable &rt)
{
    os << "RuleTable:\n";
    /*os << "  Labels:\n";
    for (const StRule& label : rt.getStLabels())
    {
        os << "  - " << label << "\n";
    }
    os << "  StFeatures:\n";
    for (const StRule& stFeature : rt.getStFeatures())
    {
        os << "  - " << stFeature << "\n";
    }
    os << "  Baseline:\n";
    for (const StRule& stFeature : rt.getStFeaturesBaseline())
    {
        os << "  - " << stFeature << "\n";
    }*/
    os << "  Rules (label->feature):\n";
    for (const auto &rule : rt.getRules())
    {
        os << "  - " << *rule << "\n";
    }
    os << "  Code length: " << rt.getCL();
    return os;
}
