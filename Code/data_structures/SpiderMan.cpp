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

SpiderMan::SpiderMan(map<string, vector<WebOuAlgoAssim>>& live_ranges): live_ranges(live_ranges){}

bool SpiderMan::canMerge(const WebOuAlgoAssim& lra, const WebOuAlgoAssim& lrb){
    // for each line of both live ranges, if there is a line in common then we can merge both live ranges
    if (lra.variable == lrb.variable){
        for (auto& linea : lra.lines){
            for (auto& lineb : lrb.lines){
                if (linea == lineb){
                    return true;
                }
            }
        }

        if (lra.death != -1 && lrb.birth != -1 && lra.death == lrb.birth) return true;
        if (lra.birth != -1 && lrb.death != -1 && lra.birth == lrb.death) return true;
    }

    return false;
}

WebOuAlgoAssim SpiderMan::Merge(const WebOuAlgoAssim& lra, const WebOuAlgoAssim& lrb){
    WebOuAlgoAssim result;
    result.birth = min(lra.birth, lrb.birth);
    result.death = max(lra.death, lrb.death);
    result.variable = lra.variable;

    result.lines.insert(result.lines.end(),lra.lines.begin(), lra.lines.end());
    result.lines.insert(result.lines.end(),lrb.lines.begin(), lrb.lines.end());
    sort(result.lines.begin(), result.lines.end());

    result.lines.erase(unique(result.lines.begin(), result.lines.end()), result.lines.end());

    return result;
}

void SpiderMan::buildWebs(){
    // insert every live range into the webs vector
    for (auto it = live_ranges.begin(); it != live_ranges.end(); it++){
        for (auto& live_range : it->second){
            webs.push_back(live_range);
        }
    }

    // merges live ranges until we can no longer
    int num_changes = -1; // doesn't make a lot of sense but we'll use this to enter into the while
    while (num_changes != 0){
        num_changes = 0;
        for (int curr = 0; curr < webs.size(); curr++){
            for (int next = curr + 1; next < webs.size(); next++){
                if (canMerge(webs[curr], webs[next])){
                    webs[curr] = Merge(webs[curr], webs[next]);
                    webs.erase(webs.begin() + next);
                    num_changes++;
                    break;
                }
            }

            if (num_changes != 0) break;
        }
    }
}