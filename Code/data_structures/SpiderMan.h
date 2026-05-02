#ifndef WEB_H
#define WEB_H

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "Parser.h"

using namespace std;

class SpiderMan{
  public:
    SpiderMan(map<string, vector<WebOuAlgoAssim>>& live_ranges);

    bool canMerge(WebOuAlgoAssim lra, WebOuAlgoAssim lrb);

    WebOuAlgoAssim Merge(WebOuAlgoAssim lra, WebOuAlgoAssim lrb);

    WebOuAlgoAssim Merger(WebOuAlgoAssim lra, WebOuAlgoAssim lrb);

    void buildWebs();

    const vector<WebOuAlgoAssim>& getWebs() const { return webs; }


  private:
    const map<string, vector<WebOuAlgoAssim>>& live_ranges;
    vector<WebOuAlgoAssim> webs;
};
#endif //WEB_H
