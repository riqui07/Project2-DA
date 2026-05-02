#include "SpiderMan.h"

#include <iostream>
#include <string>
#include <set>
#include <fstream>
#include <sstream>
#include <exception>
#include <vector>

#include "Parser.h"
#include "SpiderMan.h"

SpiderMan::SpiderMan(map<string, vector<WebOuAlgoAssim>>& live_ranges): live_ranges(live_ranges){}

bool SpiderMan::canMerge(WebOuAlgoAssim lra, WebOuAlgoAssim lrb){
    // for each line of both live ranges, if there is a line in common then we can merge both live ranges
    if (lra.variable == lrb.variable){
        for (auto& linea : lra.lines){
            for (auto& lineb : lrb.lines){
                if (linea == lineb){
                    return true;
                }
            }
        }

        if (lra.death == lrb.birth || lra.birth == lrb.death) return true;
    }

    return false;
}

WebOuAlgoAssim SpiderMan::Merge(WebOuAlgoAssim lra, WebOuAlgoAssim lrb){
    WebOuAlgoAssim result;
    result.birth = min(lra.birth, lrb.birth);
    result.death = max(lra.death, lrb.death);
    result.variable = lra.variable;

    result.lines.insert(result.lines.end(),lra.lines.begin(), lra.lines.end());
    result.lines.insert(result.lines.end(),lrb.lines.begin(), lrb.lines.end());
    sort(result.lines.begin(), result.lines.end());

    return result;
}

WebOuAlgoAssim SpiderMan::Merger(WebOuAlgoAssim lra, WebOuAlgoAssim lrb){
    if (canMerge(lra, lrb)) return Merge(lra, lrb);
    WebOuAlgoAssim newWeb;
    newWeb.variable = lrb.variable;
    newWeb.birth = lrb.birth;
    newWeb.death = lrb.death;
    newWeb.lines = lrb.lines;

    return newWeb;
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
                WebOuAlgoAssim res = Merger(webs[curr], webs[next]);
                if (/*condição para verificar se houve merge*/){
                    webs[curr] = res;
                    webs.erase(webs.begin() + next);
                    num_changes++;
                    break;
                }
            }

            if (num_changes != 0) break;
        }
    }
}