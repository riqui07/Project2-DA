//#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "data_structures/InterferenceMan.h"
using namespace std;

vector<string> parseLines(const string& filename)
{
    ifstream file(filename);
    vector<string> res;
    string line;

    while (getline(file, line))
    {
        res.push_back(line);
    }

    return res;
}

void rodar(const string& ran_filename, const string& reg_filename, const string& output_filename){
    Parser parser;
    parser.parse("Input/ranges/" + ran_filename);
    parser.parse("Input/registers/" + reg_filename);

    SpiderMan spiderman(parser.getLiveRanges());
    spiderman.buildWebs();

    InterferenceMan interference_man(spiderman);
    interference_man.startInterference();

    const Register& reg = parser.getRegister();

    if (reg.algorithm == "basic") {
        int colors_used = interference_man.runBasic(reg.num_registers);
        if (colors_used != -1)
            interference_man.outputResultsSuccess(output_filename);
        else
            interference_man.outputResultsFailure(output_filename);

    } else if (reg.algorithm == "spilling") {
        bool success = interference_man.runSpilling(reg.num_registers, reg.numeric_value);
        if (success)
            interference_man.outputResultsSuccess(output_filename);
        else
            interference_man.outputResultsFailure(output_filename);

    } else if (reg.algorithm == "splitting") {
        bool success = interference_man.runSplitting(reg.num_registers, reg.numeric_value);
        if (success)
            interference_man.outputResultsSuccess(output_filename);
        else
            interference_man.outputResultsFailure(output_filename);

    } else if (reg.algorithm == "free") {
        bool success = interference_man.runFree(reg.num_registers);
        if (success)
            interference_man.outputResultsSuccess(output_filename);
        else
            interference_man.outputResultsFailure(output_filename);
    }
}

vector<string> validateOutput(const string& gen_filename, const string& exp_filename){
    vector<string> incorrect_lines;
    vector<string> output = parseLines("Output/" + gen_filename);
    vector<string> expected = parseLines("Output/" + exp_filename);

    if (output.size() != expected.size()){incorrect_lines.push_back("Incorrect number of lines");}

    for (int i = 0; i < min(output.size(), expected.size()); i++)
    {
        if (output.at(i) != expected.at(i)){incorrect_lines.push_back(output.at(i)); incorrect_lines.push_back(expected.at(i));}
    }
    return incorrect_lines;
}

int main() {
    vector<vector<string>> testes{
        {"ranges1.txt", "registers2.txt", "allocation1.txt", "generated_allocation1.txt"},
        {"ranges2.txt", "registers2.txt", "allocation2.txt", "generated_allocation2.txt"},
        {"ranges3.txt", "registers2.txt", "allocation3.txt", "generated_allocation3.txt"},
        {"ranges4.txt", "registers1.txt", "allocation4.txt", "generated_allocation4.txt"},
        {"ranges5.txt", "registers1.txt", "allocation5.txt", "generated_allocation5.txt"},
        {"ranges6.txt", "registers3.txt", "allocation6.txt", "generated_allocation6.txt"},
    };

    for (int i = 0; i < testes.size(); i++){
        rodar(testes.at(i).at(0), testes.at(i).at(1), testes.at(i).at(2));
        vector<string> res = validateOutput(testes.at(i).at(2), testes.at(i).at(3));

        cout << "Test " << i + 1 << ":";
        if (res.empty()) {
            cout << "\033[32m" << " Correct!" << "\033[0m" << endl;
        } else {
            cout << "\033[31m" << " Incorrect!" << "\033[0m" << endl;
            cout << "\033[31m" << "First Incorrect Line: " << res.at(0) << "\033[0m" << endl;
        }
    }


    return 0;
}