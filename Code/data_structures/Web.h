//
// Created by baguetes on 04/05/2026.
//

#ifndef PROJECT2_WEB_H
#define PROJECT2_WEB_H

#include <string>

#include "Parser.h"

class Web {
public:
    Web(const string& variable, const LiveRange& lr);
    bool interferesWith(const Web& other) const;
    Web mergeWith(const Web& other) const;
    const string& getVariable() const;
    const vector<int>& getLines() const;

    // getters
    int getBirth() const;
    int getDeath() const;
private:
    std::string variable;
    int birth;
    int death;
    vector<int> lines;
};


#endif //PROJECT2_WEB_H
