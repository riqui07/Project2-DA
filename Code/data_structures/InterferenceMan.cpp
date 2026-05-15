//
// Created by baguetes on 14/05/2026.
//

#include "InterferenceMan.h"
#include "Web.h"
#include <stack>
#include <vector>
#include <map>

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
    return runBasic(nReg, this->graph);
}

int InterferenceMan::runBasic(int nReg, Graph<Web> g) {
    // make a deep copy of the graph - this copy is where the algorithm will run
    Graph copy(g);

    // === ALGORITHM ===

    std::stack<Web> stack;

    // 1. Clearing Cliques
    while (copy.getNumVertex() != 0) {
        bool removedNode = false;

        for (const Vertex<Web>* node : copy.getVertexSet()) {
            if (node->getAdj().size() < nReg) {
                stack.push(node->getInfo());
                copy.removeVertex(node->getInfo());
                removedNode = true;
                break;
            }
        }

        // if there is no removed node, then all nodes must have out degree >= nReg
        if (!removedNode) {
            return -1; // not able to colour with the provided number of registers
        }
    }

    // 2. Colouring phase
    std::map<Web, int> assignedColours;
    int coloursUsed = 0;

    while (!stack.empty()) {
        Web w = stack.top();
        stack.pop();

        Vertex<Web>* origWeb = g.findVertex(w);

        std::vector<bool> availableColours(nReg, true);

        // check the colours of all neighbours in the original graph
        for (auto edge : origWeb->getAdj()) {
            Web neighbourWeb = edge->getDest()->getInfo();

            // if the neighbour is already in our map, it has been coloured
            if (assignedColours.find(neighbourWeb) != assignedColours.end()) {
                int neighbourColour = assignedColours[neighbourWeb];

                // mark the colour of the neighbour as unavailable
                if (neighbourColour < nReg) { availableColours[neighbourColour] = false; }
            }
        }

        // find the first available colour for our current node
        int givenColour = -1; // here i will just use numbers to represent colour, -1 means no colour is assigned
        for (int i = 0; i < nReg; i++) {
            if (availableColours[i]) { givenColour = i; break; }
        }

        // assign the colour
        if (givenColour != -1) {
            assignedColours[w] = givenColour;

            // coloursUsed++;
            if (givenColour + 1 > coloursUsed) {
                coloursUsed = givenColour + 1;
            }
        } else { return -1; } // we should not reach this, but it is here just in case
    }

    return coloursUsed;
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
