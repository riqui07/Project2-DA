//
// Created by baguetes on 14/05/2026.
//

#include "InterferenceMan.h"

void InterferenceMan::startInterference() {
}

void InterferenceMan::runBasic(int nReg) {
}

void InterferenceMan::runSpilling(int nReg, int maxSpills) {
    if (runBasic(nReg,graph) != -1) return true;

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
        copy.removeVertex(bestSpill->getInfo()); 

        if(runBasic(nReg,copy) != -1){
            spilledResult = spilledWebs; 
            return true;
        }
    }
    return false;
}

void InterferenceMan::runSplitting(int nReg, int maxSplits) {
}
