//
// Created by baguetes on 14/05/2026.
//

#include "InterferenceMan.h"
#include <stack>
#include <set>

void InterferenceMan::startInterference() {
    // in this case, each Web will be represented by a node in the graph and,
    // for variables where there is an interference, we need to add an edge between their nodes

    // fetch webs from SpiderMan
    const vector<Web>& webs = peter_parker.getWebs();

    // add nodes
    for (const Web& web: webs) {
        graph.addVertex(web);
    }

    // add edges
    for (int i = 0; i < webs.size(); i++) {
        for (int j = i + 1; j < webs.size(); j++) {
            if (webs[i].interferesWith(webs[j])) graph.addBidirectionalEdge(webs[i], webs[j], 1.0);
        }
    }
}

int InterferenceMan::runBasic(int nReg) {
    // make a deep copy of the graph - this copy is where the algorithm will run
    Graph copy(graph);

    // === ALGORITHM ===

    std::stack<Web> stack;

    // 1. Clearing Cliques
    while (copy.getNumVertex() != 0) {
        for (const Vertex<Web>* node : copy.getVertexSet()) {
            if (node->getIndegree() < nReg) {
                copy.removeVertex(node->getInfo());
                stack.push(node->getInfo());
            }
        }

        // this next loop is to check if all nodes in G have degree >= N
        bool all_nodes_too_much_degree = true;
        for (const Vertex<Web>* node : copy.getVertexSet()) {
            if (node->getAdj().size() < nReg) {
               all_nodes_too_much_degree = false;
            }
        }

        if (all_nodes_too_much_degree) {
            return -1; // not able to colour with the provided number of registers
        }
    }

    // 2. Colouring phase
    while (!stack.empty()) {
        Web w = stack.top();
        stack.pop();

        // this set will hold the colours of the neighbours of the current node
        std::set<int> neighbour_colours;


    }


    // if colouring succeeded
    return colorsUsed;

    // if colouring failed
    return -1;
}

int InterferenceMan::runBasic(int nReg, Graph<Web> g) {
}

bool InterferenceMan::runSpilling(int nReg, int maxSpills) {
    if (runBasic(nReg) != -1) return true;

    vector<Web> spilledWebs;
    Graph<Web> copy = graph;

    for(int numSpills = 0; numSpills <= maxSpills; numSpills++){
        Vertex<Web>* highestDegree = nullptr;
        int currentHighest = -1;

        for(auto v : copy.getVertexSet()){
            int degree = (int)v->getAdj().size();
            if (degree > currentHighest) {
                currentHighest = degree;
                highestDegree = v;
            }
        }

        if(highestDegree == nullptr) break;

        spilledWebs.push_back(highestDegree->getInfo());
        copy.removeVertex(highestDegree->getInfo());

        if(runBasic(nReg,copy) != -1){
            spilledResult = spilledWebs;
            return true;
        }
    }
    return false;
}

void InterferenceMan::runSplitting(int nReg, int maxSplits) {
}
