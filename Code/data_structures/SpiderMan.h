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
    void buildWebs();
    const vector<Web>& getWebs() const { return webs; }

  private:
    static bool canMerge(const Web& left_hand, const Web& right_hand);

    const map<string, vector<LiveRange>>& live_ranges;
    vector<Web> webs;
};
#endif //WEB_H
