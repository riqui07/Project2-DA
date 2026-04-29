//
// Created by Henrique Reis on 29/04/2026.
//

#include <iostream>
#include "data_structures/Parser.h"

using namespace std;

int main() {
    Parser parser;

    try {
        parser.parse("Input/ranges/ranges3.txt");
        cout << "=== LIVE RANGES ===" << endl;
        for (auto& [var, ranges] : parser.getLiveRanges()) {
            cout << "\nVariavel: " << var << endl;
            int i = 0;
            for (auto& lr : ranges) {
                cout << "  LiveRange " << i++ << ":" << endl;
                cout << "    birth: " << lr.birth << endl;
                cout << "    death: " << lr.death << endl;
                cout << "    lines: ";
                for (int l : lr.lines) cout << l << " ";
                cout << endl;
            }
        }
    } catch (runtime_error& e) {
        cerr << "Erro: " << e.what() << endl;
    }

    return 0;
}