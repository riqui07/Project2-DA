#include "SpiderMan.h"

#include <iostream>
#include <string>
#include <set>
#include <fstream>
#include <sstream>
#include <exception>
#include <vector>
#include <algorithm>

#include "Parser.h"
#include "SpiderMan.h"

SpiderMan::SpiderMan(const map<string, vector<LiveRange>>& live_ranges): live_ranges(live_ranges){}

bool SpiderMan::canMerge(const Web& left_hand, const Web& right_hand){
    return left_hand.getVariable() == right_hand.getVariable() && left_hand.interferesWith(right_hand);
}

void SpiderMan::buildWebs(){
    // insert every live range into the webs vector
    for (const auto& [variable, ranges] : live_ranges){
        for (const auto& live_range : ranges){
            webs.emplace_back(variable, live_range);
        }
    }

    // merges live ranges until we can no longer
    bool merged = true;
    while (merged) {
        merged = false;

        for (int curr = 0; curr < webs.size(); curr++){
            for (int next = curr + 1; next < webs.size(); next++){
                if (canMerge(webs[curr], webs[next])){
                    webs[curr] = webs[curr].mergeWith(webs[next]);
                    webs.erase(webs.begin() + next);
                    merged = true;
                    break;
                }
            }

            if (merged) break;
        }
    }
}