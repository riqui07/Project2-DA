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

    /**
      * @brief Checks if this web interferes with another web.
      * Two webs interfere if they share at least one program point, or if one starts where the other ends (i = i + 1 case).
      * @param other The other web.
      * @return True if the webs interfere, false otherwise.
      * @complexity O(L1 * L2) where L1 and L2 are the number of lines in each web.
      */
    bool interferesWith(const Web& other) const;

    /**
     * @brief Merges this web with another web of the same variable.
     * Merges the lines of both webs, keeping the earliest birth and the latest death.
     * @param other The other web to merge with.
     * @return A new merged Web object.
     * @complexity O(L log L) where L is the total number of lines in both webs, due to sorting.
     */
    Web mergeWith(const Web& other) const;

    const string& getVariable() const;
    const vector<int>& getLines() const;
    int getBirth() const;
    int getDeath() const;

    // implement operator== for Webs
    bool operator==(const Web& other) const {
        return this->variable == other.variable && this->lines == other.lines;
    }

    // implement operator< for Webs
    bool operator<(const Web& other) const {
        if (this->variable != other.variable) {
            return (this->variable < other.variable);
        }

        return (this->lines < other.lines);
    }

private:
    std::string variable;
    int birth;
    int death;
    vector<int> lines;
};


#endif //PROJECT2_WEB_H
