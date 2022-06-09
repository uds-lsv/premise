#ifndef MDLALGO_H
#define MDLALGO_H

#include "mdlmath.h"
#include "ruletable.h"

namespace MDLAlgo
{

    // Code length (in bits) of the data
    // given the RuleTable
    double computeCLData(RuleTable &rt);

    // Cost/code length of transmitting the items of a rule given all the clauses of a rule and a label
    // i.e. in a simplified form (L1 and clauses over item) for all items in the given clauses and label 1
    // see code for specific implementation
    double computeCostSubmitItemGivenClause(const RuleTable &rt, const TailClauses &clauses, std::shared_ptr<BitCollection> tidsLabelAndClauses,
                                                 uint32_t labelIndex);

    // Code length of the given rule table
    double computeCLRuleTable(RuleTable &rt);

    // Computes the cost of transmitting a rule (label -> clauses)
    double computeRuleTransCost(const TailClauses& clauses, const uint32_t numFeaturesTotal);

    // TODO: Old implementation before the concept of clauses was added.
    // Used in the old search. New search should use computeRuleCost(clauses, numFeaturesTotal).
    // computes the cost of transmitting a rule X->Y (label -> feature)
    // i.e. L_N(|X|) + L(X) + L_N(|Y|) + L(Y)
    // (cardinality of X and Y and then the items of X and Y)
    //
    // Args:
    //      tailnum     number of items in the tail
    //      colnum      number of columns in database
    double computeRuleTransCostOld(const uint32_t tailnum, const uint32_t colnum);

    // Computes the gain in codelength when the number of rules change
    // Args:
    //      oldNumRules: the old (current) number of rules
    //      deltaNewRules: how many new rules are added (or removed)
    double computeGainNumRuleChange(uint32_t oldNumRules, int32_t deltaNewRules);

    // for the set of rules (from the ruletable) that have the given singelton in their clauses,
    // computes their union of tids
    // Beware: Some of the tids might not contain the given singelton as the
    // rules (due to the -or-) can cover some transactions that do not contain the singelton
    std::pair<std::shared_ptr<BitCollection>, std::shared_ptr<BitCollection> > computeTidsCoveredByRuleHavingSingleton(uint32_t singletonIdx, RuleTable &rt, std::vector<std::shared_ptr<BiasedRule> > mergedRules);


    // computes the Tids where the label and tail holds (intersection of tids)
    std::shared_ptr<BitCollection> computeTidsBiased(uint32_t labelIdx, std::shared_ptr<TailClauses> tailClauses, RuleTable &rt);


    // estimate gain for single label -> single feature rule
    double gainEstSingleLabelSingleFeature(uint32_t numTransactions, uint32_t usgFeature, uint32_t usgLabel,
                                           uint32_t overlapFeatureLabelCount, uint32_t labelIdx, uint32_t featureIdx,
                                           RuleTable &rt);


    double gainEstBiasedComplexStRule(uint32_t numTransactions,
                                    const std::shared_ptr<BiasedRule> r,
                                    std::shared_ptr<BitCollection> newClause,
                                    uint32_t label1Idx,
                                    uint32_t label2Idx,
                                    RuleTable &rt);

    double gainEstBiasedComplexComplexRule(uint32_t numTransactions,
                                    const std::shared_ptr<BiasedRule> r1,
                                    const std::shared_ptr<BiasedRule> r2,
                                    uint32_t label1Idx,
                                    uint32_t label2Idx,
                                    RuleTable &rt);

    double gainBiasedStRule(uint32_t numTransactions,
                            std::shared_ptr<BitCollection> clause,
                            uint32_t label1Idx,
                            uint32_t label2Idx,
                            RuleTable &rt);

    double gainEstBiasedStRulePair(uint32_t numTransactions,
                                std::shared_ptr<BitCollection> clause1,
                                std::shared_ptr<BitCollection> clause2,
                                uint32_t label1Idx,
                                uint32_t label2Idx,
                                RuleTable &rt);

    double gainEstBiasedStPairedRule(uint32_t numTransactions,
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
                                        RuleTable &rt);


} // end namespace MDLAlgo

// add relatively exact estimates for all cases

#endif
