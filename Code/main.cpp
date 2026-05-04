#include <iostream>
#include "data_structures/Parser.h"
#include "data_structures/SpiderMan.h"

using namespace std;

int main() {
    Parser parser;

    try {
        parser.parse("Input/ranges/ranges1.txt");

        // print live ranges
        cout << "=== LIVE RANGES ===" << endl;
        for (auto& [var, ranges] : parser.getLiveRanges()) {
            cout << "\nVariable: " << var << endl;
            int i = 0;
            for (const auto& [lines, birth, death] : ranges) {
                cout << "  LiveRange " << i++ << ":" << endl;
                cout << "    birth: " << birth << endl;
                cout << "    death: " << death << endl;
                cout << "    lines: ";
                for (const int& l : lines) cout << l << " ";
                cout << endl;
            }
        }

        // build and print webs
        auto live_ranges = parser.getLiveRanges();
        SpiderMan spiderman(live_ranges);
        spiderman.buildWebs();

        cout << "\n=== WEBS ===" << endl;
        int i = 0;
        for (const auto& web : spiderman.getWebs()) {
            cout << "\nWeb " << i++ << ":" << endl;
            cout << "  variable: " << web.getVariable() << endl;
            cout << "  birth: " << web.getBirth() << endl;
            cout << "  death: " << web.getDeath() << endl;
            cout << "  lines: ";
            for (const int& l : web.getLines()) cout << l << " ";
            cout << endl;
        }

    } catch (runtime_error& e) {
        cerr << "Erro: " << e.what() << endl;
    }

    return 0;
}