#include <iostream>
#include "data_structures/Parser.h"
#include "data_structures/SpiderMan.h"
#include "data_structures/InterferenceMan.h"

using namespace std;

int main() {
    Parser parser;

    try {
        parser.parse("Input/generated_inputs/ranges/generated_ranges1.txt");
        parser.parse("Input/generated_inputs/registers/generated_registers1.txt");

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

    // print register config
    const Register& reg = parser.getRegister();
    cout << "\n=== REGISTERS ===" << endl;
    cout << "num_registers: " << reg.num_registers << endl;
    cout << "algorithm: " << reg.algorithm << endl;
    if (reg.numeric_value != -1)
        cout << "numeric_value: " << reg.numeric_value << endl;

    SpiderMan spiderman(parser.getLiveRanges());
    spiderman.buildWebs();

    InterferenceMan interference_man(spiderman);
    interference_man.startInterference();


    cout << "\n=== ALGORITHM RESULTS ===" << endl;

    if (reg.algorithm == "basic") {
        int colors_used = interference_man.runBasic(reg.num_registers);

        if (colors_used != -1) {
            cout << "Result: SUCCESS!" << endl;
            cout << "The graph was successfully colored using " << colors_used << " registers." << endl;
        } else {
            cout << "Result: FAILED." << endl;
            cout << "Could not color the graph with the provided " << reg.num_registers << " registers." << endl;
        }

    } else if (reg.algorithm == "spilling") {
        cout << "Running Spilling with max spills: " << reg.numeric_value << "..." << endl;

        // Catch the boolean result
        bool success = interference_man.runSpilling(reg.num_registers, reg.numeric_value);

        if (success) {
            cout << "Result: SUCCESS! The graph was colored after spilling." << endl;
        } else {
            cout << "Result: FAILED. Could not color the graph even after spilling." << endl;
        }
    } else if (reg.algorithm == "splitting") {
        cout << "Running Splitting with max splits: " << reg.numeric_value << "..." << endl;
        interference_man.runSplitting(reg.num_registers, reg.numeric_value);

    } else {
        cout << "Unknown algorithm specified in the registers file." << endl;
    }
    return 0;
}