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

    void buildWebs();

    const vector<WebOuAlgoAssim>& getWebs() const { return webs; }


  private:
    bool canMerge(const WebOuAlgoAssim& lra, const WebOuAlgoAssim& lrb);
    WebOuAlgoAssim Merge(const WebOuAlgoAssim& lra, const WebOuAlgoAssim& lrb);

    const map<string, vector<WebOuAlgoAssim>>& live_ranges;
    vector<WebOuAlgoAssim> webs;
};
#endif //WEB_H
