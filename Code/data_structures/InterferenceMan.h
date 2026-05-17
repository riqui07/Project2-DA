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

    /**
     * @brief Builds the interference graph from the webs held by peter_parker.
     * Adds a node for each web and an edge between two nodes if the corresponding webs interfere.
     *
     * @note Time Complexity: O(W^2), W is the number of webs.
     */
    void startInterference();

    /**
     * @brief Runs the basic graph colouring register allocation algorithm.
     * @param nReg Number of registers available.
     * @return Number of registers used, -1 if allocation is not possible.
     *
     * @note Time Complexity: O(W^2 + I), W is the number of webs, I is the number of interferences.
     */
    int runBasic(int nReg);

    /**
     * @brief Runs the basic graph colouring algorithm with web spilling.
     * If basic allocation fails, iteratively spills the highest degree web and retries, up to maxSpills times.
     * @param nReg Number of registers available.
     * @param maxSpills Maximum number of webs that can be spilled to memory.
     * @return True if allocation succeeded, false otherwise.
     *
     * @note Time Complexity: O(maxSpills * W^2), W is the number of webs.
     */
    bool runSpilling(int nReg, int maxSpills);

    /**
     * @brief Runs the basic graph colouring algorithm with web splitting.
     * If basic allocation fails, iteratively splits the highest degree web in half and retries, up to maxSplits times.
     * @param nReg Number of registers available.
     * @param maxSplits Maximum number of webs that can be split.
     * @return True if allocation succeeded, false otherwise.
     *
     * @note Time Complexity: O(maxSplits * W^2), W is the number of webs.
     */
    bool runSplitting(int nReg, int maxSplits);

    /**
     * @brief Runs the free register allocation algorithm based on linear scan.
     * @param nReg Maximum number of registers available.
     * @return Number of registers used, or -1 if allocation is not possible.
     */
    int runFree(int nReg);

    // getter
    const Graph<Web>& getGraph() const { return this->graph; }
    const vector<Web>& getSpilledResult() const { return spilledResult; }

    /**
     * @brief Outputs the results to the output file in case of success
     * @param output_filename Name of the output file.
     *
     * @note Time Complexity: O(W^2), W is the number of webs.
     */
    void outputResultsSuccess(string output_filename) const;

    /**
    * @brief Outputs the results to the output file in case of failure
    * @param output_filename Name of the output file.
    *
    * @note Time Complexity: O(W), W is the number of webs.
    */
    void outputResultsFailure(string output_filename) const;


private:
    // === FIELDS ===
    const SpiderMan& peter_parker;
    Graph<Web> graph;
    vector<Web> spilledResult;
    std::map<Web, int> register_colors;

    // === METHODS ===

    /**
     * @brief Linear scan register allocation algorithm.
     * Sorts webs by birth point and greedily assigns registers, spilling the web that dies furthest when no register is available.
     * @param nReg Number of registers available.
     * @return Number of registers used, or -1 if any web was spilled.
     *
     * @note Time Complexity: O(W^2), W is the number of webs. Initial sorting takes O(W log W), but checking which webs have expired for each new web takes O(W^2) overall, which dominates.
     */
    int runLinearScan(int nReg);

    /**
     * @brief Internal helper for runBasic that operates on a given graph instead of the class graph.
     * Used by runSpilling and runSplitting to test colouring on modified graph copies.
     * @param nReg Number of registers available.
     * @param g Graph to colour.
     * @return Number of registers used, or -1 if allocation is not possible.
     *
     * @note Time Complexity: O(W^2), W is the number of webs.
     */
    int runBasic(int nReg, Graph<Web> g);
};

#endif //PROJECT2_INTERFERENCEMAN_H
