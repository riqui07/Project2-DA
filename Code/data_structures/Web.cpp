//
// Created by baguetes on 04/05/2026.
//

#include "Web.h"
#include <algorithm>

Web::Web(const string &variable, const LiveRange &lr) {
    this->variable = variable;
    this->birth = lr.birth;
    this->death = lr.death;
    this->lines = lr.lines;
}

bool Web::interferesWith(const Web &other) const {
    for (auto& this_line : this->lines)
        for (auto& other_line : other.lines)
            if (this_line == other_line) return true;

    if (this->death != -1 && other.birth != -1 && this->death == other.birth) return true;
    if (this->birth != -1 && other.death != -1 && this->birth == other.death) return true;

    return false;
}

Web Web::mergeWith(const Web &other) const {
    LiveRange merged;

    // the merged Web will have the earliest birth and the latest death
    merged.birth = min(this->birth, other.birth);
    merged.death = max(this->death, other.death);

    // merge both Webs' lines
    merged.lines.insert(merged.lines.end(), this->lines.begin(), this->lines.end());
    merged.lines.insert(merged.lines.end(), other.lines.begin(), other.lines.end());
    std::sort(merged.lines.begin(), merged.lines.end());

    // delete duplicated entries from the merging
    merged.lines.erase(unique(merged.lines.begin(), merged.lines.end()), merged.lines.end());

    return Web(this->variable, merged);
}

const string & Web::getVariable() const {
    return this->variable;
}

const vector<int> & Web::getLines() const {
    return this->lines;
}

int Web::getBirth() const {
    return this->birth;
}

int Web::getDeath() const {
    return this->death;
}
