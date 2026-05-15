//
// Created by baguetes on 14/05/2026.
//

#ifndef PROJECT2_INTERFERENCEMAN_H
#define PROJECT2_INTERFERENCEMAN_H

#include "Graph.h"
#include "SpiderMan.h"
#include "Web.h"

class InterferenceMan {
public:
    InterferenceMan(const SpiderMan& spiderMan) : peter_parker(spiderMan) {};

    void startInterference();

    int runBasic(int nReg);
    int runBasic(int nReg, Graph<Web> g);
    bool runSpilling(int nReg, int maxSpills);
    bool runSplitting(int nReg, int maxSplits);

    // getter
    const Graph<Web>& getGraph() const { return this->graph; }
    const vector<Web>& getSpilledResult() const { return spilledResult; }

private:
    const SpiderMan& peter_parker;
    Graph<Web> graph;
    vector<Web> spilledResult;
    map<Web, int> color_assignment;
};

#endif //PROJECT2_INTERFERENCEMAN_H
