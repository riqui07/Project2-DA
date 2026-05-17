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
     * @brief Runs the free register allocation algorithm.
     * Checks if the interference graph has a shape for which heuristics have been implemented (star, cycle, complete, line),
     * if so, use the proper algorithms. If no known shape is found, or if the
     * result uses too many registers, falls back to linear scan. Always finds a solution.
     * @param nReg Number of registers available.
     * @return Number of registers used.
     *
     * @note Time Complexity: O(W^2) in the worst case due to linear scan fallback,
     * O(W) if a known shape is detected and fits within nReg registers.
     */
    int runFree(int nReg);

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

    /// @brief Returns the interference graph.
    const Graph<Web>& getGraph() const { return this->graph; }

    /// @brief Returns the list of webs that were spilled during allocation
    const vector<Web>& getSpilledResult() const { return spilledResult; }

    /// @brief Returns the number of splits performed during runSpilling
    const int getNumSplits() const { return nSplits; }

private:
    // === FIELDS ===
    const SpiderMan& peter_parker;
    Graph<Web> graph;
    vector<Web> spilledResult;
    int nSplits;
    std::map<Web, int> register_colors;

    // === METHODS ===
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

    /**
     * @brief Checks if the interference graph is a star (one central node connected to all others).
     * @param nReg Number of registers available.
     * @return True if the graph is a star, false otherwise.
     *
     * @note Time Complexity: O(W), W is the number of webs.
     */
    bool isStar(int nReg);

    /**
     * @brief Checks if the interference graph is a cycle (all nodes have degree 2).
     * @param nReg Number of registers available.
     * @return True if the graph is a cycle, false otherwise.
     *
     * @note Time Complexity: O(W), W is the number of webs.
     */
    bool isCycle(int nReg);

    /**
     * @brief Checks if the interference graph is a complete graph (all nodes connected to each other).
     * @param nReg Number of registers available.
     * @return True if the graph is complete, false otherwise.
     *
     * @note Time Complexity: O(W), W is the number of webs.
     */
    bool isComplete(int nReg);

    /**
     * @brief Checks if the interference graph is a line (path graph with exactly 2 endpoints).
     * @param nReg Number of registers available.
     * @return True if the graph is a line, false otherwise.
     *
     * @note Time Complexity: O(W), W is the number of webs.
     */
    bool isLine(int nReg);

    /**
     * @brief Linear scan register allocation algorithm.
     * Sorts webs by birth point and greedily assigns registers, spilling the web that dies
     * furthest when no register is available. Spilled webs are stored in spilledResult.
     * Always finds a valid allocation within nReg registers.
     * @param nReg Number of registers available.
     * @return Number of registers used.
     *
     * @note Time Complexity: O(W^2), W is the number of webs. Initial sorting takes O(W log W),
     * but checking which webs have expired for each new web takes O(W^2) overall, which dominates.
     */
    int runLinearScan(int nReg);

    /**
     * @brief Runs register allocation optimised for star-shaped interference graphs.
     * Assigns colour 0 to the centre node and colour 1 to all leaf nodes.
     * @return Number of registers used (always 2).
     *
     * @note Time Complexity: O(W), W is the number of webs.
     */
    int runStar();

    /**
     * @brief Runs register allocation optimised for cycle-shaped interference graphs.
     * Uses 2 colours for even cycles and 3 for odd cycles.
     * @return Number of registers used (2 or 3).
     *
     * @note Time Complexity: O(W), W is the number of webs.
     */
    int runCycle();

    /**
     * @brief Runs register allocation optimised for complete interference graphs.
     * Assigns a unique colour to each node.
     * @return Number of registers used (equals number of webs).
     *
     * @note Time Complexity: O(W), W is the number of webs.
     */
    int runComplete();

    /**
     * @brief Runs register allocation optimised for line-shaped interference graphs.
     * Alternates between 2 colours along the path.
     * @return Number of registers used (always 2).
     *
     * @note Time Complexity: O(W), W is the number of webs.
     */
    int runLine();
};

#endif //PROJECT2_INTERFERENCEMAN_H
