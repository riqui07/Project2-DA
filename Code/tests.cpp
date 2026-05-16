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

void rodar(){
    Parser parser;
    parser.parse("Input/generated_inputs/ranges/generated_ranges2.txt");
    parser.parse("Input/generated_inputs/registers/generated_registers2.txt");

    SpiderMan spiderman(parser.getLiveRanges());
    spiderman.buildWebs();

    InterferenceMan interference_man(spiderman);
    interference_man.startInterference();

    const Register& reg = parser.getRegister();

    if (reg.algorithm == "basic") {
        int colors_used = interference_man.runBasic(reg.num_registers);
        if (colors_used != -1)
            interference_man.outputResultsSuccess("maboy.txt");
        else
            interference_man.outputResultsFailure("maboy.txt");

    } else if (reg.algorithm == "spilling") {
        bool success = interference_man.runSpilling(reg.num_registers, reg.numeric_value);
        if (success)
            interference_man.outputResultsSuccess("maboy.txt");
        else
            interference_man.outputResultsFailure("maboy.txt");

    } else if (reg.algorithm == "splitting") {
        bool success = interference_man.runSplitting(reg.num_registers, reg.numeric_value);
        if (success)
            interference_man.outputResultsSuccess("maboy.txt");
        else
            interference_man.outputResultsFailure("maboy.txt");
    }
}

vector<string> validateOutput(const string& filename){
    vector<string> incorrect_lines;
    vector<string> output = parseLines("Output/" + filename);
    vector<string> expected = parseLines("Output/generated_outputs/generated_output2.txt");

    if (output.size() != expected.size()){incorrect_lines.push_back("Incorrect number of lines");}

    for (int i = 0; i < output.size(); i++)
    {
        if (output[i] != expected[i]){incorrect_lines.push_back(output[i]); incorrect_lines.push_back(expected[i]);}
    }
    return incorrect_lines;
}

int main() {
    rodar();
    vector<string> res = validateOutput("maboy.txt");
    if (res.empty()) {
        cout << "\033[32m" << "Correct!" << "\033[0m" << endl;
    } else {
        cout << "\033[31m" << "=======================================" << "\033[0m" << endl;
        cout << "\033[31m" << "Incorrect!" << "\033[0m" << endl;
        cout << "\033[31m" << "First Incorrect Line: " << res[0] << "\033[0m" << endl;
        cout << "\033[31m" << "Should be: " << res[1] << "\033[0m" << endl;
        cout << "\033[31m" << "=======================================" << "\033[0m" << endl;
    }
    return 0;
}