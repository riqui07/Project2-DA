//
// Created by baguetes on 14/05/2026.
//

#include "RegAllocator.h"
#include "Web.h"
#include <stack>
#include <vector>
#include <map>
#include <fstream>
#include <set>

using namespace std;

bool RegAllocator::isStar(int nReg) const {
    if (nReg < 2) return false;
    int n = graph.getNumVertex();
    if (n < 2) return false;
    int numCenter = 0;
    for (auto v : graph.getVertexSet()) {
        int degree = static_cast<int>(v->getAdj().size());
        if (degree == n-1) numCenter++;
        else if (degree != 1) return false;
    }
    return numCenter == 1;
}

int RegAllocator::runStar(){
    register_colors.clear();
    for (auto v : graph.getVertexSet()){
        if (static_cast<int>(v->getAdj().size()) != 1) register_colors[v->getInfo()] = 0;
        else register_colors[v->getInfo()] = 1;
    }
    return 2;
}

bool RegAllocator::isCycle(int nReg) const {
    if (graph.getNumVertex() < 3 || nReg < 2) return false;
    for (auto v : graph.getVertexSet()) {
        if (static_cast<int>(v->getAdj().size()) != 2) return false;
    }
    //odd cycle needs 3 registers
    if (nReg < 3 && graph.getNumVertex() % 2 != 0) return false;
    return true;
}

int RegAllocator::runCycle(){
    register_colors.clear();
    auto start = graph.getVertexSet()[0];
    Vertex<Web>* previous = nullptr;
    Vertex<Web>* current = start;
    int color = 0;
    while(register_colors.find(current->getInfo()) == register_colors.end()) {
        register_colors[current->getInfo()] = color;
        if (color == 0) color = 1;
        else color = 0;
        for(auto e : current->getAdj()) {
            if (e->getDest() != previous) {
                previous = current;
                current = e->getDest();
                break;
            }
        }
    }
    //odd cycles
    if (register_colors[current->getInfo()] == register_colors[previous->getInfo()]){
        register_colors[previous->getInfo()] = 2;
        return 3;
    }
    return 2;
}

bool RegAllocator::isComplete(int nReg) const {
    int n = graph.getNumVertex();
    if (nReg < n) return false; 
    for (auto v : graph.getVertexSet()) {
        if (static_cast<int>(v->getAdj().size()) != n-1) return false;
    }
    return true;
}

int RegAllocator::runComplete(){
    register_colors.clear();
    int current = 0;
    for (auto v : graph.getVertexSet()) register_colors[v->getInfo()] = current++;
    return current;
}

bool RegAllocator::isLine(int nReg) const {
    if (nReg < 2) return false;
    if (graph.getNumVertex() < 2) return false;
    int endPoints = 0;
    for (auto v : graph.getVertexSet()) {
        int degree = static_cast<int>(v->getAdj().size());
        if (degree == 1) endPoints++;
        else if (degree != 2) return false;
    }
    return endPoints == 2; 
}

int RegAllocator::runLine(){
    register_colors.clear();
    Vertex<Web>* start = nullptr;
    for (auto v : graph.getVertexSet()){
        if (static_cast<int>(v->getAdj().size()) == 1) {
            start = v;
            break;
        }
    }

    Vertex<Web>* previous = nullptr;
    Vertex<Web>* current = start;
    int color = 0;
    while(current != nullptr) {
        register_colors[current->getInfo()] = color;
        if (color == 0) color = 1;
        else color = 0;
        Vertex<Web>* next = nullptr;
        for (auto e : current->getAdj()) {
            if (e->getDest() != previous) {
                next = e->getDest();
                break;
            }
        }
        previous = current;
        current = next;
    }
    return 2;
}

bool RegAllocator::isNull(int nReg) const {
    return graph.getNumVertex() == 0;
}

int RegAllocator::runNull() {
    register_colors.clear();
    return 0;
}

bool RegAllocator::isTrivial(int nReg) const {
    return nReg >= 1 && graph.getNumVertex() == 1;
}

int RegAllocator::runTrivial() {
    register_colors.clear();
    register_colors[graph.getVertexSet()[0]->getInfo()] = 0;
    return 1;
}

bool RegAllocator::isEmpty(int nReg) const{
    if (nReg < 1) return false;
    for (auto v : graph.getVertexSet()) {
        if (static_cast<int>(v->getAdj().size()) != 0) return false;
    }
    return true;
}

int RegAllocator::runEmpty() {
    register_colors.clear();
    for (auto v : graph.getVertexSet()) register_colors[v->getInfo()] = 0;
    return 1;
}

bool RegAllocator::isTree(int nReg) const {
    if (nReg < 2) return false;
    int n = graph.getNumVertex();
    if (n < 2) return false;
    int edgeCount = 0;
    for (auto v : graph.getVertexSet()) edgeCount += static_cast<int>(v->getAdj().size());
    edgeCount /= 2;
    return edgeCount == n - 1;
}

int RegAllocator::runTree() {
    register_colors.clear();
    auto root = graph.getVertexSet()[0];
    std::map<Vertex<Web>*, int> visited;
    std::queue<Vertex<Web>*> q;
    visited[root] = 0;
    q.push(root);
    while (!q.empty()) {
        auto current = q.front();
        q.pop();
        register_colors[current->getInfo()] = visited[current];
        for (auto e : current->getAdj()) {
            auto dinizz = e->getDest();
            if(visited.find(dinizz) == visited.end()) {
                visited[dinizz] = 1 - visited[current];
                q.push(dinizz);
            }
        }
    }
    return 2;
}

void RegAllocator::startInterference() {
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

int RegAllocator::runBasic(int nReg) {
    return runBasic(nReg, this->graph);
}

int RegAllocator::runLinearScan(int nReg) {
    // === SETUP ===
    vector<Web> webs = this->peter_parker.getWebs();

    // sort webs by birth
    std::sort(webs.begin(), webs.end(), [](const Web&a, const Web& b) { return a.getBirth() < b.getBirth(); });

    // create vector active (holds the variables currently being hold by registers)
    std::vector<Web> active;
    std::vector<Web> spilled;

    // map to store final register assignments
    std::map<Web, int> assigned_registers;

    // which registers are free (true)
    std::vector<bool> free_regs(nReg, true);

    int maxRegUsed = 0;

    // === ALGORITHM ===
    // for each web
    // if any variable from the active vector died before the current web's birth
    // free their register

    for (const Web& web : webs) {
        // here I decided to iterate backwards because of removing while iterating
        for (int i = active.size() - 1; i >= 0; i--) {
            if (active.at(i).getDeath() <= web.getBirth()) {

                int reg_to_free = assigned_registers[active.at(i)];
                free_regs[reg_to_free] = true;

                active.erase(active.begin() + i);
            }
        }

        // if size of the active list is less than the maxReg, give currentWeb a register and add to the active list
        if (active.size() < nReg) {
            int given_reg = -1;
            for (int r = 0; r < nReg; r++) {
                if (free_regs[r]) {
                    given_reg = r;
                    break;
                }
            }

            // storoe
            free_regs[given_reg] = false;
            assigned_registers[web] = given_reg;

            active.push_back(web);

            if (given_reg + 1 > maxRegUsed) {
                maxRegUsed = given_reg + 1;
            }
        }
        // Spilling
        else {
            // find the active web that dies furthest
            int furthest_active_death = -1;
            int furthest_active_index = -1;

            for (int i = 0; i < active.size(); i++) {
                if (active[i].getDeath() > furthest_active_death) {
                    furthest_active_death = active[i].getDeath();
                    furthest_active_index = i;
                }
            }

            // compare with the current web's death
            if (furthest_active_death > web.getDeath()) {
                // spill active web
                Web web_to_spill = active[furthest_active_index];
                spilled.push_back(web_to_spill);

                // use the same register from the spilled web
                int stolen_reg = assigned_registers[web_to_spill];
                assigned_registers.erase(web_to_spill);
                assigned_registers[web] = stolen_reg;

                active.erase(active.begin() + furthest_active_index);
                active.push_back(web);
            } else {
                // spill current web
                spilled.push_back(web);
            }
        }
    }

    if (spilled.empty()) {
        this->register_colors = assigned_registers;
        return maxRegUsed;
    }

    this->spilledResult = spilled;

    this->register_colors = assigned_registers;

    return maxRegUsed;
}

int RegAllocator::runBasic(int nReg, const Graph<Web>& g) {
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

    this->register_colors = assignedColours;
    return coloursUsed;
}

bool RegAllocator::runSpilling(int nReg, int maxSpills) {
    spilledResult.clear();
    if (runBasic(nReg) != -1) return true;

    vector<Web> spilledWebs;
    Graph<Web> copy = graph;

    for(int numSpills = 0; numSpills <= maxSpills; numSpills++){
        Vertex<Web>* highestDegree = nullptr;
        int currentHighest = -1;

        for(auto v : copy.getVertexSet()){
            int degree = static_cast<int>(v->getAdj().size());
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

bool RegAllocator::runSplitting(int nReg, int maxSplits) {
    if (runBasic(nReg) != -1) return true;

    splitWebs.clear();

    Graph<Web> copy = graph;

    for(int numSplits = 0; numSplits <= maxSplits; numSplits++){
        Vertex<Web>* highestDegree = nullptr;
        int currentHighest = -1;

        for(auto v : copy.getVertexSet()){
            int degree = static_cast<int>(v->getAdj().size());
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
        left.birth = left.lines.front();
        left.death = left.lines.back();

        right.lines = vector<int>(lines.begin() + mid, lines.end());
        right.birth = right.lines.front();
        right.death = right.lines.back();

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
            nSplits = numSplits + 1;

            // store the final web list from the copy
            splitWebs.clear();
            for (auto v : copy.getVertexSet()) {
                splitWebs.push_back(v->getInfo());
            }

            return true;
        }
    }
    return false;
}

int RegAllocator::runFree(int nReg) {
    if (isNull(nReg)) {algorithmUsed = "Null"; return runNull();}
    if (isTrivial(nReg)) {algorithmUsed = "Trivial"; return runTrivial();}
    if (isEmpty(nReg)) {algorithmUsed = "Empty"; return runEmpty();}
    if (isComplete(nReg)) {algorithmUsed = "Complete"; return runComplete();}
    if (isStar(nReg)) {algorithmUsed = "Star"; return runStar();}
    if (isCycle(nReg)) {algorithmUsed = "Cycle"; return runCycle();}
    if (isLine(nReg)) {algorithmUsed = "Line"; return runLine();}
    if (isTree(nReg)) {algorithmUsed = "Tree"; return runTree();}
    algorithmUsed = "Linear Scan";
    return runLinearScan(nReg);
}

void RegAllocator::outputResultsSuccess(string output_filename) const{
    ofstream output_file("Output/" + output_filename);

    // use splitWebs if splitting was performed, otherwise use original webs
    const vector<Web>& webs = splitWebs.empty() ? peter_parker.getWebs() : splitWebs;

    // não sei se estas duas linhas têm de estar mas é ok yolo
    output_file << "# Total number of webs followed by the listing of the program points of each one" << endl;
    output_file << "# program points in each web are sorted in ascending order" << endl;

    output_file << "webs: " << webs.size() << endl;

    for (int i = 0; i < webs.size(); i++){
        output_file << "web" << i << ": ";
        const vector<int>& lines = webs[i].getLines();
        for (auto& line : lines){
            if (line != lines.back()){
                if (line == webs[i].getBirth()) output_file << line << "+,";
                else if (line == webs[i].getDeath()) output_file << line << "-,";
                else output_file << line << ",";
            }
        }
        if (lines.back() == webs[i].getBirth()) output_file << lines.back() << "+" << endl;
        else if (lines.back() == webs[i].getDeath()) output_file << lines.back() << "-" << endl;
        else output_file << lines.back() << endl;
    }

    output_file << "# Total number of registers used, followed by assignment to webs" << endl;

    set<int> colors;
    for (auto& [web, color] : register_colors){
        colors.insert(color);
    }

    output_file << "registers: " << colors.size() << endl;

    for (auto& color: colors){
        vector<Web> temp;

        for (auto& [web, color_reg] : register_colors){
            if (color_reg == color) temp.push_back(web);
        }

        for (auto& value : temp) {
            for (int i = 0; i < webs.size(); i++) {
                if (webs[i] == value) {
                    output_file << "r" << color << ": web" << i << endl;
                    break;
                }
            }
        }
    }
}

void RegAllocator::outputResultsFailure(string output_filename) const{
    ofstream output_file("Output/" + output_filename);

    const vector<Web>& webs = peter_parker.getWebs();

    // não sei se estas duas linhas têm de estar mas é ok yolo
    output_file << "# Total number of webs followed by the listing of the program points of each one" << endl;
    output_file << "# program points in each web are sorted in ascending order" << endl;

    output_file << "webs: " << webs.size() << endl;

    for (int i = 0; i < webs.size(); i++){
        output_file << "web" << i << ": ";
        const vector<int>& lines = webs[i].getLines();
        for (auto& line : lines){
            if (line != lines.back()){
                if (line == webs[i].getBirth()) output_file << line << "+,";
                else if (line == webs[i].getDeath()) output_file << line << "-,";
                else output_file << line << ",";
            }
        }
        if (lines.back() == webs[i].getBirth()) output_file << lines.back() << "+" << endl;
        else if (lines.back() == webs[i].getDeath()) output_file << lines.back() << "-" << endl;
        else output_file << lines.back() << endl;
    }

    output_file << "# Total number of registers used, followed by assignment to webs" << endl;

    set<int> colors;
    for (auto& [web, color] : register_colors){
        colors.insert(color);
    }

    output_file << "registers: 0" << endl;

    for (int i = 0; i < webs.size(); i++) {
        output_file << "M: web" << i << endl;
    }
}

void RegAllocator::printResultsSuccess() const{

    const vector<Web>& webs = peter_parker.getWebs();

    cout << "       webs: " << webs.size() << endl;

    for (int i = 0; i < webs.size(); i++){
        cout << "       web" << i << ": ";
        const vector<int>& lines = webs[i].getLines();
        for (auto& line : lines){
            if (line != lines.back()){
                if (line == webs[i].getBirth()) cout << line << "+,";
                else if (line == webs[i].getDeath()) cout << line << "-,";
                else cout << line << ",";
            }
        }
        if (lines.back() == webs[i].getBirth()) cout << lines.back() << "+" << endl;
        else if (lines.back() == webs[i].getDeath()) cout << lines.back() << "-" << endl;
        else cout << lines.back() << endl;
    }

    cout << endl;

    set<int> colors;
    for (auto& [web, color] : register_colors){
        colors.insert(color);
    }

    cout << "       registers: " << colors.size() << endl;

    for (auto& color: colors){
        vector<Web> temp;

        for (auto& [web, color_reg] : register_colors){
            if (color_reg == color) temp.push_back(web);
        }

        for (auto& value : temp) {
            for (int i = 0; i < webs.size(); i++) {
                if (webs[i] == value) {
                    cout << "       r" << color << ": web" << i << endl;
                    break;
                }
            }
        }
    }
}

void RegAllocator::printResultsFailure() const{

    const vector<Web>& webs = peter_parker.getWebs();

    cout << "       webs: " << webs.size() << endl;

    for (int i = 0; i < webs.size(); i++){
        cout << "       web" << i << ": ";
        const vector<int>& lines = webs[i].getLines();
        for (auto& line : lines){
            if (line != lines.back()){
                if (line == webs[i].getBirth()) cout << line << "+,";
                else if (line == webs[i].getDeath()) cout << line << "-,";
                else cout << line << ",";
            }
        }
        if (lines.back() == webs[i].getBirth()) cout << lines.back() << "+" << endl;
        else if (lines.back() == webs[i].getDeath()) cout << lines.back() << "-" << endl;
        else cout << lines.back() << endl;
    }

    cout << endl;

    set<int> colors;
    for (auto& [web, color] : register_colors){
        colors.insert(color);
    }

    cout << "       registers: 0" << endl;

    for (int i = 0; i < webs.size(); i++) {
        cout << "       M: web" << i << endl;
    }
}
