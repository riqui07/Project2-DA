//
// Created by baguetes on 14/05/2026.
//

#include "InterferenceMan.h"
#include "Web.h"
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

bool InterferenceMan::runSplitting(int nReg, int maxSplits) {
    if (runBasic(nReg) != -1) return true;

    Graph<Web> copy = graph;

    for(int numSplits = 0; numSplits <= maxSplits; numSplits++){
        Vertex<Web>* highestDegree = nullptr;
        int currentHighest = -1;

        for(auto v : copy.getVertexSet()){
            int degree = (int)v->getAdj().size();
            // não podemos dar split a uma web com 1 só linha
            if (degree > currentHighest && v->getInfo().getLines().size() >= 2) {
                currentHighest = degree;
                highestDegree = v;
            }
        }

        if(highestDegree == nullptr) break;

        Web mary_jane = highestDegree->getInfo();
        const vector<int>& lines = mary_jane.getLines();
        int size_line = lines.size();
        int mid = size_line/2;

        LiveRange right, left;
        left.lines = vector<int>(lines.begin(), lines.begin() + mid);
        left.birth = left.lines.at(0);
        left.death = left.lines.at(mid - 1);

        right.lines = vector<int>(lines.begin() + mid, lines.end());
        right.birth = right.lines.at(0);
        right.death = right.lines.at(mid - 1);

        Web w_left(mary_jane.getVariable(), left);
        Web w_right(mary_jane.getVariable(), right);

        copy.removeVertex(mary_jane);
        copy.addVertex(w_left);
        copy.addVertex(w_right);

        for (auto v : copy.getVertexSet()) {
            if (v->getInfo() == w_left || v->getInfo() == w_right) continue;
            Web other = v->getInfo();
            if (w_left.interferesWith(other))
                copy.addBidirectionalEdge(w_left, other, 1.0);
            if (w_right.interferesWith(other))
                copy.addBidirectionalEdge(w_right, other, 1.0);
        }
        if (w_left.interferesWith(w_right))
            copy.addBidirectionalEdge(w_left, w_right, 1.0);

        if (runBasic(nReg, copy) != -1) {
            return true;
        }
    }
    return false;
}
