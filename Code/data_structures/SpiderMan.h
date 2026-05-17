#ifndef WEB_H
#define WEB_H

#include <string>
#include <vector>
#include <map>

#include "Parser.h"
#include "Web.h"

using namespace std;

class SpiderMan{
  public:
    SpiderMan(const map<string, vector<LiveRange>>& live_ranges);

    /**
     * @brief Builds the webs from the live ranges by merging overlapping ranges of the same variable.
     * @complexity O(W^3) where W is the number of webs, due to repeated scans of the web list until no more merges are possible.
     */
    void buildWebs();

    // getter
    const vector<Web>& getWebs() const { return webs; }

  private:

    /**
     * @brief Checks if two webs can be merged.
     * @param left_hand First web.
     * @param right_hand Second web.
     * @return True if the webs can be merged, false otherwise.
     * @complexity O(1).
     */
    static bool canMerge(const Web& left_hand, const Web& right_hand);

    const map<string, vector<LiveRange>>& live_ranges;
    vector<Web> webs;
};
#endif //WEB_H
