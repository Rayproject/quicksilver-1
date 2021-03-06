//
// Created by Nikolay Yakovets on 2018-02-01.
//

#include "SimpleGraph.h"
#include "SimpleEstimator.h"

SimpleEstimator::SimpleEstimator(std::shared_ptr<SimpleGraph> &g){

    // works only with SimpleGraph
    graph = g;
}

void SimpleEstimator::prepare() {
    for (uint32_t vertex = 0; vertex < graph->getNoVertices(); ++vertex) {
        for (auto out : graph->adj[vertex]) {
            const auto label = out.first;
            paths[label]++;
        }
    }
}

cardStat SimpleEstimator::estimate(RPQTree *q) {
    //If the query is divisible, first estimate the children and then combine
    if (q->isConcat()) {
        cardStat l = estimate(q->left);
        cardStat r = estimate(q->right);
        return join(q, l, r);
    }

    PathElement pathElement = getPathElement(q);

    return cardStat {paths[pathElement.label],
                     paths[pathElement.label],
                     paths[pathElement.label]};
}

cardStat SimpleEstimator::join(RPQTree *q, cardStat l, cardStat r) {
    if (!nonEmpty(l) || !nonEmpty(r)) {
        return cardStat {0, 0, 0};
    }
    uint32_t in = l.noIn/2;
    uint32_t out = r.noOut/2;
    auto paths = std::min(l.noPaths*r.noPaths/(l.noOut/4),l.noPaths * r.noPaths/(r.noIn/4));

    return cardStat {out, paths, in};
}

bool SimpleEstimator::nonEmpty(cardStat s) {
    return s.noIn != 0 && s.noPaths != 0 && s.noOut != 0;
}

PathElement SimpleEstimator::getPathElement(RPQTree *q) {
    PathElement pathElement = PathElement();
    pathElement.label = static_cast<uint32_t>(strtoll(q->data.c_str(), &pathElement.direction, 0));

    return pathElement;
}
