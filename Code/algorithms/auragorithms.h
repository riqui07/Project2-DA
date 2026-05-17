//
// Created by baguetes on 16/05/2026.
//

#ifndef PROJECT2_FREE_H
#define PROJECT2_FREE_H

#include "../data_structures/Web.h"
#include <algorithm>

int free() {
    // line()

    // circle()

    // linear_scan()
}

int linear_scan(vector<Web> webs, int maxReg) {
    // === SETUP ===
    // sort webs by birth
    std::sort(webs.begin(), webs.end(), [](const Web&a, const Web& b) { return a.getBirth() < b.getBirth(); });

    // create vector active (holds the variables currently being hold by registers)
    std::vector<Web> active;
    std::vector<Web> spilled;

    // map to store final register assignments
    std::map<Web, int> assigned_registers;

    // which registers are free (true)
    std::vector<bool> free_regs(maxReg, true);

    int maxRegUsed = 0;

    // === ALGORITHM ===
    // for each web
        // if any variable from the active vector died before the current web's birth
            // free their register

    for (const Web& web : webs) {
        // here I decided to iterate backwards because of removing while iterating
        for (int i = active.size() - 1; i >= 0; i--) {
            if (active.at(i).getDeath() < web.getBirth()) {

                int reg_to_free = assigned_registers[active.at(i)];
                free_regs[reg_to_free] = true;

                active.erase(active.begin() + i);
            }
        }

        // if size of the active list is less than the maxReg, give currentWeb a register and add to the active list
        if (active.size() < maxReg) {
            int given_reg = -1;
            for (int r = 0; r < maxReg; r++) {
                if (free_regs[r]) {
                    given_reg = r;
                    break;
                }
            }

            // storoe
            free_regs[given_reg] = false;
            assigned_registers[web] = given_reg;

            active.push_back(web);

            if (given_reg + 1 > maxRegUsed) {
                maxRegUsed = given_reg + 1;
            }
        }
        // Spilling
        else {
            // find the active web that dies furthest
            int furthest_active_death = -1;
            int furthest_active_index = -1;

            for (int i = 0; i < active.size(); i++) {
                if (active[i].getDeath() > furthest_active_death) {
                    furthest_active_death = active[i].getDeath();
                    furthest_active_index = i;
                }
            }

            // compare with the current web's death
            if (furthest_active_death > web.getDeath()) {
                // spill active web
                Web web_to_spill = active[furthest_active_index];
                spilled.push_back(web_to_spill);

                // use the same register from the spilled web
                int stolen_reg = assigned_registers[web_to_spill];
                assigned_registers.erase(web_to_spill);
                assigned_registers[web] = stolen_reg;

                active.erase(active.begin() + furthest_active_index);
                active.push_back(web);
            } else {
                // spill current web
                spilled.push_back(web);
            }
        }
    }

    return spilled.empty() ? maxRegUsed : -1;
}

#endif //PROJECT2_FREE_H
