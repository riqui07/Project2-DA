#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include "Menu.h"

#include "RegAllocator.h"
#include "Parser.h"
#include "SpiderMan.h"
using namespace std;

//show the options for the menu
void showMenu(){
    cout << "\n";
    cout << "\033[34m" << "  ╔════════════════════════════════════════╗" << endl << "\033[0m";
    cout << "\033[34m" << "  ║        Choose an option (1-6):         ║" << endl << "\033[0m";
    cout << "\033[34m" << "  ╠════════════════════════════════════════╣" << endl << "\033[0m";
    cout << "  ║  1.  Load Input Files                  ║" << endl;
    cout << "  ║  2.  Display Loaded Data               ║" << endl;
    cout << "  ║  3.  Run Register Allocation           ║" << endl;
    cout << "  ║  4.  Show Allocation Results           ║" << endl;
    cout << "  ║  5.  Export Results to Output File     ║" << endl;
    cout << "  ║  6.  Exit                              ║" << endl;
    cout << "  ╚════════════════════════════════════════╝" << endl;
    cout << "  Choose an option (1-6): ";
}

void byebye()
{
    cout << R"(
                /|  /|  ---------------------------
                ||__||  |                         |
               /   O O\__        Bye bye!         |
              /          \                        |
             /      \     \                       |
            /   _    \     \ ----------------------
           /    |\____\     \      ||
          /     | | | |\____/      ||
         /       \| | | |/ |     __||
        /  /  \   -------  |_____| ||
       /   |   |           |       --|
       |   |   |           |_____  --|
       |  |_|_|_|          |     \----
       /\                  |
      / /\        |        /
     / /  |       |       |
 ___/ /   |       |       |
|____/    c_c_c_C/ \C_c_c_c
)";
}

void input_handler(Parser& parser){
 while (true){
  string ranges_filename;
  cout << "  Ranges filename: ";
  cin >> ranges_filename;

  string registers_filename;
  cout << "  Registers filename: ";
  cin >> registers_filename;

  if (ranges_filename.length() >= 4){
   string txt_rng = ranges_filename.substr(ranges_filename.length() - 4, 4);
   if (txt_rng.compare(".txt"))
   {
    ranges_filename += ".txt";
   }
  } else {ranges_filename += ".txt";}

  if (registers_filename.length() >= 4){
   string txt_reg = registers_filename.substr(registers_filename.length() - 4, 4);
   if (txt_reg.compare(".txt"))
   {
    registers_filename += ".txt";
   }
  } else {registers_filename += ".txt";}

  bool success = true;

  try {
   parser.parse("Input/ranges/" + ranges_filename);
  } catch (const exception& e) {
   cout << "=========== Error ===========" << endl << e.what() << endl << "=============================" << endl;
   success = false;
  }

  try {
   parser.parse("Input/registers/" + registers_filename);
  } catch (const exception& e) {
   cout << "=========== Error ===========" << endl << e.what() << endl << "=============================" << endl;
   success = false;
  }

  if (success){
   cout << endl;
   cout << "  Input Files Successfully Loaded." << endl;
   break;
  }
 }
}


void displayParsedInfo(Parser parser){

 map<string, vector<LiveRange>> live_ranges = parser.getLiveRanges();
 cout << "\n";
 cout << "\033[34m" << "  ╔══════════════════════════════════════════════════════════════╗" << endl << "\033[0m";
 cout << "\033[34m" << "  ║                      LIVE RANGES                             ║" << endl << "\033[0m";
 cout << "\033[34m" << "  ╠══════════════════════════════════════════════════════════════╣" << endl << "\033[0m";
 int count = 0;
 for (const auto& lrpair : live_ranges) {
  cout << "       Variable: " << lrpair.first << endl;
  int i = 0;
  for (const auto& lr : lrpair.second){
   cout << "         LiveRange " << i << ":" << endl;
   cout << "            Birth: " << lr.birth << endl;
   cout << "            Death: " << lr.death << endl;
   cout << "            Lines: ";
   for (const int& l : lr.lines) cout << l << " ";
   cout << endl;
  }
  count++;
  if (count < live_ranges.size()) cout << "  ╠══════════════════════════════════════════════════════════════╣" << endl;
  else{cout << "  ╚══════════════════════════════════════════════════════════════╝" << endl;}
 }
 cout << endl;
 cout << "\033[34m" << "  ╔══════════════════════════════════════════════════════════════╗" << endl << "\033[0m";
 cout << "\033[34m" << "  ║                       REGISTERS                              ║" << endl << "\033[0m";
 cout << "\033[34m" << "  ╠══════════════════════════════════════════════════════════════╣" << endl << "\033[0m";
 Register reg = parser.getRegister();
 cout << "       Number of Registers: " << reg.num_registers << endl;
 cout << "       Algorithm: " << reg.algorithm << endl;
 if (reg.numeric_value != -1) cout << "       Numeric Value: " << reg.numeric_value << endl;
 cout << "  ╚══════════════════════════════════════════════════════════════╝" << endl;
}

bool runAllocation(Parser parser){
 auto live_ranges = parser.getLiveRanges();
 SpiderMan spiderman(live_ranges);
 spiderman.buildWebs();

 RegAllocator interference_man(spiderman);
 interference_man.startInterference();

 const Register& reg = parser.getRegister();

 bool success;

 cout << "\n";
 cout << "\033[34m" << "  ╔════════════════════════════════════════════════════════════════════╗" << endl << "\033[0m";
 cout << "\033[34m" << "  ║                     ALLOCATION PROCESS                             ║" << endl << "\033[0m";
 cout << "\033[34m" << "  ╠════════════════════════════════════════════════════════════════════╣" << endl << "\033[0m";

 if (reg.algorithm == "basic") {
  auto start = chrono::high_resolution_clock::now();
  int colors_used = interference_man.runBasic(reg.num_registers);
  auto end = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

  if (colors_used != -1) {
   cout << "       Result: SUCCESS!" << endl;
   cout << "       The graph was successfully colored using " << colors_used << " registers." << endl;
   cout << "       Time taken: " << duration.count() << " µs" << endl;
   success = true;
  } else {
   cout << "       Result: FAILED." << endl;
   cout << "       Could not color the graph with the provided " << reg.num_registers << " registers." << endl;
   cout << "       Time taken: " << duration.count() << " µs" << endl;
   success = false;
  }

 } else if (reg.algorithm == "spilling") {
  cout << "       Running Spilling with max spills: " << reg.numeric_value << "..." << endl;

  auto start = chrono::high_resolution_clock::now();
  success = interference_man.runSpilling(reg.num_registers, reg.numeric_value);
  auto end = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

  if (success) {
   int spills = interference_man.getSpilledResult().size();
   cout << "       Result: SUCCESS! The graph was colored after spilling " << spills << " times." << endl;
   cout << "       Time taken: " << duration.count() << " µs" << endl;
  } else {
   cout << "       Result: FAILED. Could not color the graph even after spilling." << endl;
   cout << "       Time taken: " << duration.count() << " µs" << endl;
  }
 } else if (reg.algorithm == "splitting") {
  cout << "       Running Splitting with max splits: " << reg.numeric_value << "..." << endl;
  // Catch the boolean result
  auto start = chrono::high_resolution_clock::now();
  success = interference_man.runSplitting(reg.num_registers, reg.numeric_value);
  auto end = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

  if (success) {
   int splits = interference_man.getNumSplits();
   cout << "       Result: SUCCESS! The graph was colored after splitting " << splits << " times." << endl;
   cout << "       Time taken: " << duration.count() << " µs" << endl;
  } else{
   cout << "       Result: FAILED. Could not color the graph even after splitting." << endl;
   cout << "       Time taken: " << duration.count() << " µs" << endl;
  }

 } else if (reg.algorithm == "free") {
  auto start = chrono::high_resolution_clock::now();
  success = interference_man.runFree(reg.num_registers);
  auto end = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

  if (int spills = interference_man.getSpilledResult().size()) { cout << "       Result: SUCCESS! The graph was colored after spilling " << spills << " time(s)." << endl; cout << "       Algorithm used: " << interference_man.getAlgorithmUsed() << endl; cout << "       Time taken: " << duration.count() << " µs" << endl;}
  else { cout << "       Result: SUCCESS! Graph colored " << endl; cout << "       Algorithm used: " << interference_man.getAlgorithmUsed() << endl; cout << "       Time taken: " << duration.count() << " µs" << endl;}

 } else {
  success = false;
  cout << "       Unknown algorithm specified in the registers file." << endl;
 }
  cout << "  ╚════════════════════════════════════════════════════════════════════╝" << endl;

 return success;
}

void displayAllocationResults(Parser parser){
 auto live_ranges = parser.getLiveRanges();
 SpiderMan spiderman(live_ranges);
 spiderman.buildWebs();

 RegAllocator interference_man(spiderman);
 interference_man.startInterference();

 const Register& reg = parser.getRegister();
 bool success;

 if (reg.algorithm == "basic") {
  int colors = interference_man.runBasic(reg.num_registers);
  success = (colors != -1);
 } else if (reg.algorithm == "spilling") {
  success = interference_man.runSpilling(reg.num_registers, reg.numeric_value);
 } else if (reg.algorithm == "splitting") {
  success = interference_man.runSplitting(reg.num_registers, reg.numeric_value);
 } else if (reg.algorithm == "free") {
  success = interference_man.runFree(reg.num_registers);

 } else success = false;

 cout << endl;
 cout << "\033[34m" << "  ╔══════════════════════════════════════════════════════════════╗" << endl << "\033[0m";
 cout << "\033[34m" << "  ║                   ALGORITHM RESULTS                          ║" << endl << "\033[0m";
 cout << "\033[34m" << "  ╠══════════════════════════════════════════════════════════════╣" << endl << "\033[0m";
 if (success) interference_man.printResultsSuccess();
 else{interference_man.printResultsFailure();}
 cout << "  ╚══════════════════════════════════════════════════════════════╝" << endl;
}

void outputHandler(Parser parser){
 auto live_ranges = parser.getLiveRanges();
 SpiderMan spiderman(live_ranges);
 spiderman.buildWebs();

 RegAllocator interference_man(spiderman);
 interference_man.startInterference();

 const Register& reg = parser.getRegister();
 bool success;

 if (reg.algorithm == "basic") {
  int colors = interference_man.runBasic(reg.num_registers);
  success = (colors != -1);
 } else if (reg.algorithm == "spilling") {
  success = interference_man.runSpilling(reg.num_registers, reg.numeric_value);
 } else if (reg.algorithm == "splitting") {
  success = interference_man.runSplitting(reg.num_registers, reg.numeric_value);
 } else if (reg.algorithm == "free"){
  success = interference_man.runFree(reg.num_registers);
 }else success = false;

 cout << endl;
 cout << "\033[34m" << "  ╔══════════════════════════════════════════════════════════════════════════╗" << endl << "\033[0m";
 cout << "\033[34m" << "  ║                             OUTPUT EXPORT                                ║" << endl << "\033[0m";
 cout << "\033[34m" << "  ╠══════════════════════════════════════════════════════════════════════════╣" << endl << "\033[0m";
 cout << "       Type the name of the file you wish to export the Allocation Results" << endl;
 cout << "  ╚══════════════════════════════════════════════════════════════════════════╝" << endl;

 cout << endl;
 string output_filename;
 cout << "  Output Filename: ";
 cin >> output_filename;

 if (output_filename.length() >= 4){
  string txt = output_filename.substr(output_filename.length() - 4, 4);
  if (txt.compare(".txt"))
  {
   output_filename += ".txt";
  }
 } else {output_filename += ".txt";}
 if (success) interference_man.outputResultsSuccess(output_filename);
 else{interference_man.outputResultsFailure(output_filename);}

 cout << endl;
 cout << "  Your Results were exported to Output/" + output_filename << endl;
}