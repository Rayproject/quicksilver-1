//
// Created by Nikolay Yakovets on 2018-02-01.
//

#ifndef QS_SIMPLEESTIMATOR_H
#define QS_SIMPLEESTIMATOR_H

#include "Estimator.h"
#include "SimpleGraph.h"

struct PathElement {
    char* direction;
    uint32_t label;
};


class SimpleEstimator : public Estimator {

    std::shared_ptr<SimpleGraph> graph;

    std::unordered_map<uint32_t, uint32_t> starts, paths, ends;

    cardStat join(RPQTree *q, cardStat l, cardStat r);
    bool nonEmpty(cardStat s);
    PathElement getPathElement(RPQTree *q);

public:
    explicit SimpleEstimator(std::shared_ptr<SimpleGraph> &g);

    ~SimpleEstimator() = default;
    void prepare() override ;

    cardStat estimate(RPQTree *q) override ;
};


#endif //QS_SIMPLEESTIMATOR_H
