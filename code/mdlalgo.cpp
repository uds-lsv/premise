#include <math.h>

#include "mdlalgo.h"
#include "database.h"
#include "rule.h"
#include "codestats.h"

double MDLAlgo::computeCLData(RuleTable &rt)
{
    // Note: the number of samples is not required to be encoded as it does not change for different models
    double cl = 0;
    Database::DataDescriptor descriptorFeatures = rt.getDB().getDescriptorFeatures();

    // send singleton usages
    for (const StRule &st : rt.getStFeatures())
    {
        uint32_t usg = st.tids->getPopCount();
        if (usg == 0 || usg == descriptorFeatures.numRows)
            continue;
        cl += MDLMath::logBinom(descriptorFeatures.numRows, usg); // Transmit all occurences of singleton
    }

    // We do not need to encode the labels as they are always the same

    // TODO: Buffer these computations, i.e. save the data cost for each rule somewhere. Otherwise we do all the intersections for each clause in every round
    for (auto& rule : rt.getRules())
    {
        assert(rt.getStLabels().size() == 2); // Only implemented for rules with two labels
        // send (L1 over clauses) and (L2 over clauses)
        cl += MDLMath::logBinom(rt.getStLabels()[0].tids->getPopCount(), rule->tail->getTidsL1()->getPopCount());
        cl += MDLMath::logBinom(rt.getStLabels()[1].tids->getPopCount(), rule->tail->getTidsL2()->getPopCount());

        // send the specific items given the clauses, i.e. simplified
        // (L1 and clauses over item) for all items
        // also transmits the model costs for that part
        cl += MDLAlgo::computeCostSubmitItemGivenClause(rt, rule->tail->getClauses(), rule->tail->getTidsL1(), 0);
        cl += MDLAlgo::computeCostSubmitItemGivenClause(rt, rule->tail->getClauses(), rule->tail->getTidsL2(), 1);
    }

    return cl;
}

double MDLAlgo::computeCostSubmitItemGivenClause(const RuleTable &rt,
                                                const TailClauses &clauses,
                                                std::shared_ptr<BitCollection> tidsLabelAndClauses,
                                                uint32_t labelIndex)
{
    assert(rt.getStLabels().size() == 2);
    double cl = 0;
    for(auto &clause: clauses)
    {
        // if we do not have an "or" in a clause (i.e. the clause consists of only one item)
        // we can skip the computation below because all transactions of that clause contain this item
        // the receiver can detect that no "or" is contained because they now the clause length
        if (clause->getPopCount() == 1)
        {
            continue;
        }

        // An x-or of the clauses is assumed. Having specified the instances for one item in the clause
        // this reduces the number of possible instances for the other clauses
        uint32_t numPossibleTransactions = tidsLabelAndClauses->getPopCount();

        std::unique_ptr<const std::vector<uint32_t>> cOnes(clause->getOnes());
        for (uint32_t item : *cOnes)
        {
            std::unique_ptr<BitCollection> itemTids(rt.getStFeaturesBaseline()[item].tids->setinter(*tidsLabelAndClauses));
            uint32_t numTransactionsWithItem = itemTids->getPopCount();

            // breaks if xor assumption breaks
            assert(numPossibleTransactions >= numTransactionsWithItem);

            // cost for sending the specific item given the clauses and the label
            // (L and clauses over item)
            cl += MDLMath::logBinom(numPossibleTransactions, numTransactionsWithItem);

            // this is actually part of the model cost, but we have the number of transactions here
            cl += MDLMath::paramCompApprox(numPossibleTransactions);
            numPossibleTransactions -= numTransactionsWithItem;
        }
        assert(numPossibleTransactions == 0);
    }
    return cl;
}

double MDLAlgo::computeCLRuleTable(RuleTable &rt)
{
    double cl = 0;
    Database::DataDescriptor dataDescriptor = rt.getDB().getDescriptorFeatures();
    // only transmit regret of non zero usage singleton
    // the receiver can deduct from the empty head rules which singletons are excluded
    uint32_t singletonCount = 0;
    for (auto &stRule : rt.getStFeatures())
    {
        if (stRule.tids->getPopCount() > 0)
        {
            ++singletonCount;
        }
    }
    cl += singletonCount * MDLMath::paramCompApprox(dataDescriptor.numRows);
    // Case that only singletons are used
    if (rt.getRules().size() == 0)
    {
        return cl;
    }

    cl += MDLMath::univCodeIntegerL(rt.getRules().size()); // cost to send the number of rules

    for (auto& rule : rt.getRules())
    {
        // Assuming there are only two labels
        assert(rt.getDB().getDescriptorLabels().numCols == 2);
        // Costs of sending the rule
        cl += computeRuleTransCost(rule->tail->getClauses(), rt.getDB().getDescriptorFeatures().numCols);

        // Model costs for tid sending
        // Cost for binomials (L1 over clauses) and (L2 over clauses)
        cl += MDLMath::paramCompApprox(rt.getStLabels()[0].tids->getPopCount());
        cl += MDLMath::paramCompApprox(rt.getStLabels()[1].tids->getPopCount());

        for(auto& clause: rule->tail->getClauses())
        {
            int32_t litNum = clause->getPopCount();
            if(litNum > 1)
            {
                // This is sent within the data cost code as we can reduce the cost here
                // knowing the xor assumption
                //cl += litNum*MDLMath::paramCompApprox(rule->tail->getTidsL1()->getPopCount());
                //cl += litNum*MDLMath::paramCompApprox(rule->tail->getTidsL2()->getPopCount());
            }
        }
    }
    return cl;
}

std::shared_ptr<BitCollection> MDLAlgo::computeTidsBiased(uint32_t labelIdx, std::shared_ptr<TailClauses> tailClauses, RuleTable &rt)
{
    std::vector<BitCollection *> tids;
    for (auto& clause : *tailClauses)
    {
        assert(clause->getPopCount() > 0);
        BitCollection* tidsInClause (new dbitvec(rt.getDB().getDescriptorFeatures().numRows)); // not shared pointer as setinterMany needs normal pointer
        std::unique_ptr<const std::vector<uint32_t> > cOnes (clause->getOnes());
        for(uint32_t item: *cOnes)
        {
            std::unique_ptr<BitCollection> tidsItemLabel (rt.getStFeaturesBaseline()[item].tids->setinter(*rt.getStLabels()[labelIdx].tids));
            tidsInClause->thisUnion(*tidsItemLabel); // TODO Make more efficient by replacing it with setunionmany operation
        }

        // Remove transactions that break the xor assumption within a clause, i.e. which overlap in the clause
        // We can not do this while building the union because a transaction might overlap for multiple items (rare but happens)
        for(uint32_t item1: *cOnes)
        {
            for(uint32_t item2: *cOnes)
            {
                if(item1 == item2) // Items are ordered
                    break;
                // this overlap is actually bigger (over whole data; but we just use it for removal, so does not matter for result and we save an operation)
                std::unique_ptr<BitCollection> itemPairOverlap (rt.getStFeaturesBaseline()[item1].tids->setinter(*rt.getStFeaturesBaseline()[item2].tids));
                if(itemPairOverlap->getPopCount() > 0)
                {
                    tidsInClause->thisAndNot(*itemPairOverlap);
                }
            }
        }
        tids.push_back(tidsInClause);
    }
    std::shared_ptr<BitCollection> inter(tids[0]->setinterMany(tids.begin(), tids.end()));

    // the pointers in tids have to be deleted as they are not shared pointers and created in this method
    for(uint32_t i = 0; i < tids.size(); ++i)
    {
        delete tids[i];
    }
    return inter;
}

double MDLAlgo::gainBiasedStRule(uint32_t numTransactions,
                                    std::shared_ptr<BitCollection> clause,
                                    uint32_t label1Idx,
                                    uint32_t label2Idx,
                                    RuleTable &rt)
{
    double res = 0;
    std::unique_ptr<const std::vector<uint32_t> > items (clause->getOnes());

    // we only have one clause here
    std::shared_ptr<TailClauses> tailClauses = std::make_shared<TailClauses>();
    tailClauses->push_back(clause);

    std::shared_ptr<BitCollection> overlapL1 (MDLAlgo::computeTidsBiased(label1Idx, tailClauses, rt));
    std::shared_ptr<BitCollection> overlapL2 (MDLAlgo::computeTidsBiased(label2Idx, tailClauses, rt));
    
    // costs for sending where clause holds
    res += MDLMath::logBinom(rt.getStLabels()[label1Idx].tids->getPopCount(), overlapL1->getPopCount());
    res += MDLMath::logBinom(rt.getStLabels()[label2Idx].tids->getPopCount(), overlapL2->getPopCount());
    res += MDLMath::paramCompApprox(rt.getStLabels()[label1Idx].tids->getPopCount()) + MDLMath::paramCompApprox(rt.getStLabels()[label2Idx].tids->getPopCount());
    
    // costs for sending where each item in the clause holds
    res += MDLAlgo::computeCostSubmitItemGivenClause(rt, *tailClauses, overlapL1, label1Idx);
    res += MDLAlgo::computeCostSubmitItemGivenClause(rt, *tailClauses, overlapL2, label2Idx);
    
    for (const auto item : *items)
    {
        // old costs for the singleton
        res -= MDLMath::logBinom(numTransactions, rt.getStFeatures()[item].tids->getPopCount());
        res -= MDLMath::paramCompApprox(numTransactions);

        // not every item might be covered by the xor clause
        // submit the rest as singleton (error encoding)
        uint32_t stRem = rt.getStFeatures()[item].tids->thisAndNotCountAnyOthers({overlapL1.get(), overlapL2.get()});
        if (stRem > 0)
        {
            res += MDLMath::logBinom(numTransactions, stRem);
            res += MDLMath::paramCompApprox(numTransactions);
        }
    }

    res += computeRuleTransCost({clause}, rt.getStFeatures().size());
    res += computeGainNumRuleChange(rt.getRules().size(), 1);

    return res;
}

double MDLAlgo::gainEstBiasedStRulePair(uint32_t numTransactions,
                                std::shared_ptr<BitCollection> clause1,
                                std::shared_ptr<BitCollection> clause2,
                                uint32_t label1Idx,
                                uint32_t label2Idx,
                                RuleTable &rt)
{
    double res = 0;

    std::unique_ptr<const std::vector<uint32_t> > items1 (clause1->getOnes());
    std::unique_ptr<const std::vector<uint32_t> > items2 (clause2->getOnes());

    // where the clause holds for each label
    std::shared_ptr<TailClauses> tailClauses = std::make_shared<TailClauses>();
    tailClauses->push_back(clause1);
    tailClauses->push_back(clause2);
    std::shared_ptr<BitCollection> overlapL1 (MDLAlgo::computeTidsBiased(label1Idx, tailClauses, rt));
    std::shared_ptr<BitCollection> overlapL2 (MDLAlgo::computeTidsBiased(label2Idx, tailClauses, rt));

    // costs for sending where clause holds
    res += MDLMath::logBinom(rt.getStLabels()[label1Idx].tids->getPopCount(), overlapL1->getPopCount());
    res += MDLMath::logBinom(rt.getStLabels()[label2Idx].tids->getPopCount(), overlapL2->getPopCount());
    res += MDLMath::paramCompApprox(rt.getStLabels()[label1Idx].tids->getPopCount()) + MDLMath::paramCompApprox(rt.getStLabels()[label2Idx].tids->getPopCount());
    
    // costs for sending where each item in the clause holds
    res += MDLAlgo::computeCostSubmitItemGivenClause(rt, *tailClauses, overlapL1, label1Idx);
    res += MDLAlgo::computeCostSubmitItemGivenClause(rt, *tailClauses, overlapL2, label2Idx);
    
    for (const auto item : *items1)
    {
        // remaining transactions of the singleton not covered by rule
        uint32_t stRem = rt.getStFeatures()[item].tids->thisAndNotCountAnyOthers({overlapL1.get(), overlapL2.get()});
        // new respectively old cost of submitting the features as singletons
        res += MDLMath::logBinom(numTransactions, stRem);
        res -= MDLMath::logBinom(numTransactions, rt.getStFeatures()[item].tids->getPopCount());
        // old singleton regret (in case we covered all the singeltons now)
        if (rt.getStFeatures()[item].tids->getPopCount() > 0 && stRem == 0)
            res -= MDLMath::paramCompApprox(numTransactions);
    }

    for (const auto item : *items2)
    {
        // remaining transactions of the singleton not covered by rule
        uint32_t stRem = rt.getStFeatures()[item].tids->thisAndNotCountAnyOthers({overlapL1.get(), overlapL2.get()});
        // new respectively old cost of submitting the features as singletons
        res += MDLMath::logBinom(numTransactions, stRem);
        res -= MDLMath::logBinom(numTransactions, rt.getStFeatures()[item].tids->getPopCount());
        // old singleton regret  (in case we covered all the singeltons now)
        if (rt.getStFeatures()[item].tids->getPopCount() > 0 && stRem == 0)
            res -= MDLMath::paramCompApprox(numTransactions);
    }

    res += computeRuleTransCost({clause1, clause2}, rt.getStFeatures().size());
    res += computeGainNumRuleChange(rt.getRules().size(), 1);

    return res;
}

double MDLAlgo::gainEstBiasedComplexStRule(uint32_t numTransactions,
                                        const std::shared_ptr<BiasedRule> r,
                                        std::shared_ptr<BitCollection> newClause,
                                        uint32_t label1Idx,
                                        uint32_t label2Idx,
                                        RuleTable &rt)
{
    double res = 0;

    // where the clause holds for each label
    std::shared_ptr<TailClauses> newTailClauses = std::make_shared<TailClauses>();
    newTailClauses->insert(newTailClauses->end(), r->tail->getClauses().begin(), r->tail->getClauses().end());
    newTailClauses->push_back(newClause);

    std::shared_ptr<BitCollection> overlapL1 (MDLAlgo::computeTidsBiased(label1Idx, newTailClauses, rt));
    std::shared_ptr<BitCollection> overlapL2 (MDLAlgo::computeTidsBiased(label2Idx, newTailClauses, rt));

    // costs for sending where clause holds
    res += MDLMath::logBinom(rt.getStLabels()[label1Idx].tids->getPopCount(), overlapL1->getPopCount());
    res += MDLMath::logBinom(rt.getStLabels()[label2Idx].tids->getPopCount(), overlapL2->getPopCount());
    // parametric complexity stays the same between old and new rule

    // costs for sending where each item in the clauses hold
    res += MDLAlgo::computeCostSubmitItemGivenClause(rt, *newTailClauses, overlapL1, label1Idx);
    res += MDLAlgo::computeCostSubmitItemGivenClause(rt, *newTailClauses, overlapL2, label2Idx);

    //  --- ADAPTING COSTS FOR THE NEW PART GIVEN BY NEW CLAUSE ---

    std::unique_ptr<const std::vector<uint32_t> > items (newClause->getOnes());
    for (const auto item : *items)
    {
        // remaining transactions of the singleton not covered by rule
        uint32_t stRem = rt.getStFeatures()[item].tids->thisAndNotCountAnyOthers({overlapL1.get(), overlapL2.get()});

        // new respectively old cost of submitting the features as singletons
        res += MDLMath::logBinom(numTransactions, stRem);
        res -= MDLMath::logBinom(numTransactions, rt.getStFeatures()[item].tids->getPopCount());
        // old singleton regret
        if (rt.getStFeatures()[item].tids->getPopCount() > 0 && stRem == 0)
            res -= MDLMath::paramCompApprox(numTransactions);
    }

    //  --- ADAPTING COSTS OF OLD RULE THAT IS MERGED ---

    // old costs of sending where clauses hold
    res -= MDLMath::logBinom(rt.getStLabels()[label1Idx].tids->getPopCount(), r->tail->getTidsL1()->getPopCount());
    res -= MDLMath::logBinom(rt.getStLabels()[label2Idx].tids->getPopCount(), r->tail->getTidsL2()->getPopCount());
    // parametric complexity stays the same between old and new rule

    // old cost of sending where each item in each of the rule's closes hold
    res -= MDLAlgo::computeCostSubmitItemGivenClause(rt, r->tail->getClauses(), r->tail->getTidsL1(), label1Idx);
    res -= MDLAlgo::computeCostSubmitItemGivenClause(rt, r->tail->getClauses(), r->tail->getTidsL2(), label2Idx);

    for (const std::shared_ptr<BitCollection> oldRuleClause : r->tail->getClauses())
    {
        std::unique_ptr<const std::vector<uint32_t> > oldRuleItems (oldRuleClause->getOnes());
        for (const auto id : *oldRuleItems)
        {
            // remaining transactions not covered by new rule
            auto pairTidsCoveredOtherRules = computeTidsCoveredByRuleHavingSingleton(id, rt, {r});

            std::unique_ptr<BitCollection> rTids1Copy (r->tail->getTidsL1()->copy());
            rTids1Copy->thisAndNot(*overlapL1);
            rTids1Copy->thisAndNot(*pairTidsCoveredOtherRules.first);

            uint32_t stRem = rTids1Copy->intersectionCount(*rt.getStFeaturesBaseline()[id].tids);

            std::unique_ptr<BitCollection> rTids2Copy (r->tail->getTidsL2()->copy());
            rTids2Copy->thisAndNot(*overlapL2);
            rTids2Copy->thisAndNot(*pairTidsCoveredOtherRules.second);
            stRem += rTids2Copy->intersectionCount(*rt.getStFeaturesBaseline()[id].tids);

            // new respectively old cost of submitting the features as singletons
            res += MDLMath::logBinom(numTransactions, rt.getStFeatures()[id].tids->getPopCount() + stRem);
            res -= MDLMath::logBinom(numTransactions, rt.getStFeatures()[id].tids->getPopCount());
            // add regret if we added something to singleton encoding
            if (rt.getStFeatures()[id].tids->getPopCount() == 0 && stRem > 0)
                res += MDLMath::paramCompApprox(numTransactions);
        }
    }

    // old and new costs of sending rule (model costs)
    res -= computeRuleTransCost(r->tail->getClauses(), rt.getStFeatures().size());
    res += computeRuleTransCost(*newTailClauses, rt.getStFeatures().size());

    return res;

}

double MDLAlgo::gainEstBiasedComplexComplexRule(uint32_t numTransactions,
                                        const std::shared_ptr<BiasedRule> r1,
                                        const std::shared_ptr<BiasedRule> r2,
                                        uint32_t label1Idx,
                                        uint32_t label2Idx,
                                        RuleTable &rt)
{
    double res = 0;

    std::shared_ptr<TailClauses> newTailClauses = std::make_shared<TailClauses>();
    newTailClauses->insert(newTailClauses->end(), r1->tail->getClauses().begin(), r1->tail->getClauses().end());
    newTailClauses->insert(newTailClauses->end(), r2->tail->getClauses().begin(), r2->tail->getClauses().end());

    // where the rule holds for each label
    std::shared_ptr<BitCollection> overlapL1 (MDLAlgo::computeTidsBiased(label1Idx, newTailClauses, rt));
    std::shared_ptr<BitCollection> overlapL2 (MDLAlgo::computeTidsBiased(label2Idx, newTailClauses, rt));

    // costs for sending where rule holds
    res += MDLMath::logBinom(rt.getStLabels()[label1Idx].tids->getPopCount(), overlapL1->getPopCount());
    res += MDLMath::logBinom(rt.getStLabels()[label2Idx].tids->getPopCount(), overlapL2->getPopCount());

    // As we go from 2 rules to one rule, we only need one parametrix complexity for where rule holds
    // and can remove the second one
    res -= MDLMath::paramCompApprox(rt.getStLabels()[0].tids->getPopCount());
    res -= MDLMath::paramCompApprox(rt.getStLabels()[1].tids->getPopCount());

    // costs for sending where each item in the clauses hold
    res += MDLAlgo::computeCostSubmitItemGivenClause(rt, *newTailClauses, overlapL1, label1Idx);
    res += MDLAlgo::computeCostSubmitItemGivenClause(rt, *newTailClauses, overlapL2, label2Idx);

    //  --- ADAPTING COSTS OF EACH RULE THAT IS MERGED ---

    for(const BiasedRule& r: {*r1, *r2})
    {
        // old costs of sending where clauses hold
        res -= MDLMath::logBinom(rt.getStLabels()[label1Idx].tids->getPopCount(), r.tail->getTidsL1()->getPopCount());
        res -= MDLMath::logBinom(rt.getStLabels()[label2Idx].tids->getPopCount(), r.tail->getTidsL2()->getPopCount());
        // parametric complexity was removed above

        // old cost of sending where each item in each of the rule's closes hold
        res -= MDLAlgo::computeCostSubmitItemGivenClause(rt, r.tail->getClauses(), r.tail->getTidsL1(), label1Idx);
        res -= MDLAlgo::computeCostSubmitItemGivenClause(rt, r.tail->getClauses(), r.tail->getTidsL2(), label2Idx);

        for (const std::shared_ptr<BitCollection> clause : *r.tail)
        {
            std::unique_ptr<const std::vector<uint32_t> > oldRuleItems (clause->getOnes());
            for (const auto id : *oldRuleItems)
            {
                // remaining transactions not covered by new rule
                // TODO: make this more efficient
                auto pairTidsCoveredOtherRules = computeTidsCoveredByRuleHavingSingleton(id, rt, {r1, r2});

                std::unique_ptr<BitCollection> rTids1Copy (r.tail->getTidsL1()->copy());
                rTids1Copy->thisAndNot(*overlapL1);
                rTids1Copy->thisAndNot(*pairTidsCoveredOtherRules.first);
                uint32_t stRem = rTids1Copy->intersectionCount(*rt.getStFeaturesBaseline()[id].tids);
                std::unique_ptr<BitCollection> rTids2Copy (r.tail->getTidsL2()->copy());
                rTids2Copy->thisAndNot(*overlapL2);
                rTids2Copy->thisAndNot(*pairTidsCoveredOtherRules.second);
                stRem += rTids2Copy->intersectionCount(*rt.getStFeaturesBaseline()[id].tids);

                // new respectively old cost of submitting the features as singletons
                // no longer needed
                res += MDLMath::logBinom(numTransactions, rt.getStFeatures()[id].tids->getPopCount() + stRem);
                res -= MDLMath::logBinom(numTransactions, rt.getStFeatures()[id].tids->getPopCount());
                // add regret if we added something to singleton encoding
                if (rt.getStFeatures()[id].tids->getPopCount() == 0 && stRem > 0)
                    res += MDLMath::paramCompApprox(numTransactions);
            }
        }
    }

    // old and new costs of sending rule (model costs)
    res -= computeRuleTransCost(r1->tail->getClauses(), rt.getStFeatures().size());
    res -= computeRuleTransCost(r2->tail->getClauses(), rt.getStFeatures().size());
    res += computeRuleTransCost(*newTailClauses, rt.getStFeatures().size());

    // Adapt parametric complexity for sending data, and number of rules to send.
    res += MDLAlgo::computeGainNumRuleChange(rt.getRules().size(), -1);

    return res;
}

double MDLAlgo::gainEstBiasedStPairedRule(uint32_t numTransactions,
                                            uint32_t usgFeature1,
                                            uint32_t usgFeature2,
                                            uint32_t usgFeature1Rem,
                                            uint32_t usgFeature2Rem,
                                            uint32_t usgLabel1,
                                            uint32_t usgLabel2,
                                            uint32_t label1Overlap,
                                            uint32_t label2Overlap,
                                            uint32_t label1Idx,
                                            uint32_t label2Idx,
                                            uint32_t featureIdx,
                                            RuleTable &rt)
{
    assert(false);
    // old and new costs of sending singleton
    double res = -MDLMath::logBinom(numTransactions, usgFeature1);
    if (usgFeature1Rem == 0)
    {
        res -= MDLMath::paramCompApprox(numTransactions);
    } else {
        res += MDLMath::logBinom(numTransactions, usgFeature1Rem);
    }
    res -= MDLMath::logBinom(numTransactions, usgFeature2);
    if (usgFeature2Rem == 0)
    {
        res -= MDLMath::paramCompApprox(numTransactions);
    } else {
        res += MDLMath::logBinom(numTransactions, usgFeature2Rem);
    }

    // Instead of transmitting the transactions where the tail holds
    // according to the model (T_X over TM_Y|X) and then
    // transmitting where in the model range the 1s are,
    // we directly list the transactions (and therefore the 1s) given label1 or label2.
    // send with new rule Label->Feature
    res += MDLMath::logBinom(usgLabel1, label1Overlap);
    res += MDLMath::logBinom(usgLabel2, label2Overlap);

    // new models costs
    // cost for transmitting the binomials
    res += MDLMath::paramCompApprox(usgLabel1) + MDLMath::paramCompApprox(usgLabel2);

    // costs for transmitting the rule
    res += computeRuleTransCostOld(2, rt.getDB().getDescriptorFeatures().numCols);

    res += computeGainNumRuleChange(rt.getRules().size(), 1);

    return res;
}

double MDLAlgo::computeRuleTransCost(const TailClauses& clauses, const uint32_t numFeaturesTotal)
{
    double cl = 0;

    // Cost of sending the number of clauses in the tail
    cl += MDLMath::univCodeIntegerL(clauses.size());
    std::vector<uint32_t> clauseSizes;

    uint32_t featNum = numFeaturesTotal;
    for(auto& clause: clauses)
    {
        // Cost of sending the specific items in the clause
        // binomial + cost of sending the binomial
        // length of the clause is basically covered by parametric complexity term
        cl += MDLMath::logBinom(featNum, clause->getPopCount()) + MDLMath::paramCompApprox(featNum);
        // TODO
        // we could use the fact hat clauses are non-overlapping regarding items
        // if this was the case
        //featNum -= clause->getPopCount();
    }

    return cl;
}


std::pair<std::shared_ptr<BitCollection>, std::shared_ptr<BitCollection> > MDLAlgo::computeTidsCoveredByRuleHavingSingleton(uint32_t singletonIdx, RuleTable &rt, std::vector<std::shared_ptr<BiasedRule> > mergedRules)
{
    std::vector<std::shared_ptr<BiasedRule>> rulesCoveringSingleton;
    // TODO use iterators to be more efficient.
    for(const auto rule: rt.getRules())
    {
        // rules that would be deleted if this new rule is used
        // have to be skipped (those rules that are merged to create his rule would be deleted)
        bool skipIt = false;
        for (const auto rOld : mergedRules)
        {
            if (rule == rOld)
            {
                skipIt = true;
            }
        }
        if (skipIt)
        {
            continue;
        }
        if(rule->tail->getItems()->operator[](singletonIdx))
        {
            rulesCoveringSingleton.push_back(rule);
        }
    }

    // there are no rules to take into consideration
    // therefore, return empty tid lists
    if(rulesCoveringSingleton.size() == 0)
    {
        return std::pair<std::shared_ptr<BitCollection>, std::shared_ptr<BitCollection> >(
            new dbitvec(rt.getDB().getDescriptorFeatures().numRows), new dbitvec(rt.getDB().getDescriptorFeatures().numRows));
    }

    std::vector<BitCollection*> ruleTids1;
    std::vector<BitCollection*> ruleTids2;
    for (const auto rule : rulesCoveringSingleton)
    {
        ruleTids1.push_back(rule->tail->getTidsL1().get());
        ruleTids2.push_back(rule->tail->getTidsL2().get());
    }

    std::shared_ptr<BitCollection> tidsL1 (ruleTids1[0]->setunionMany(ruleTids1.begin(), ruleTids1.end()));
    std::shared_ptr<BitCollection> tidsL2 (ruleTids2[0]->setunionMany(ruleTids2.begin(), ruleTids2.end()));

    return std::pair<std::shared_ptr<BitCollection>, std::shared_ptr<BitCollection> >(tidsL1, tidsL2);
}

double MDLAlgo::computeRuleTransCostOld(const uint32_t tailnum, const uint32_t colnum)
{
    // TODO: Old implementation before the concept of clauses was added.
    // Used in the old search. New search should use computeRuleCost(clauses, numFeaturesTotal).
    double cl = 1;
    // Costs of sending which items we use in the tail
    cl += MDLMath::logBinom(colnum, tailnum) + MDLMath::paramCompApprox(colnum);
    return cl;
}

double MDLAlgo::computeGainNumRuleChange(uint32_t oldNumRules, int32_t deltaNewRules)
{
    double gain = MDLMath::univCodeIntegerL(oldNumRules + deltaNewRules);   // new cost to transmit the number of all rules after adding new rules
    gain -= (oldNumRules > 0) ? MDLMath::univCodeIntegerL(oldNumRules) : 0; // minus the cost for the number of all rules before adding
    return gain;
}