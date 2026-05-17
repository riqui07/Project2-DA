#include <iostream>

#include "data_structures/RegAllocator.h"
#include "data_structures/Parser.h"
#include "data_structures/SpiderMan.h"
#include "data_structures/Menu.h"

using namespace std;

void usage() {
    std::cerr << "Usage CLI: myProg\nUsage Batch: myProg -b [ranges] [registers] [allocation]" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 1 && argc != 5) {
        usage();
        return 1;
    }

    if (argc == 1) {

        Parser parser;
        bool parsedInfo = false;
        bool allocatedInfo = false;


        //show options
        //show action based on the option
        while (true)
        {
            showMenu();
            int option_num;
            string opt_list;

            while (true)
            {
                cin >> option_num;

                if (cin.fail() || option_num < 1 || option_num > 6) {
                    cin.clear();
                    cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
                    cout << " Invalid input, please enter a number 1-6: ";
                } else {
                    break;
                }
            }

            switch (option_num)
            {
            case 1:
                input_handler(parser);
                parsedInfo = true;
                break;
            case 2:
                if (parsedInfo) displayParsedInfo(parser);
                else {cout << endl << "  You need to Load the Input Files before Running this Option." << endl;}
                break;
            case 3:
                if (parsedInfo) {runAllocation(parser); allocatedInfo = true;}
                else {cout << endl << "  You need to Load the Input Files before Running this Option." << endl;}

                break;
            case 4:
                if (parsedInfo && allocatedInfo) displayAllocationResults(parser);
                else {cout << endl << "  You need to Load the Input Files and Run the Register Allocation before Running this Option." << endl;}
                break;
            case 5:
                if (parsedInfo && allocatedInfo) outputHandler(parser);
                else {cout << endl << "  You need to Load the Input Files and Run the Register Allocation before Running this Option." << endl;}
                break;
            case 6:
                //quit
                    byebye();
                exit(EXIT_SUCCESS);
            }

        }
    }

    // batch mode: -b ranges1.txt registers2.txt batch.txt
    if (string(argv[1]) != "-b")
    {
        usage();
        return 1;
    }

    Parser parser;
    parser.parse("Input/ranges/" + string(argv[2]));
    parser.parse("Input/registers/" + string(argv[3]));

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

  if (int spills = interference_man.getSpilledResult().size()) { cout << "       Result: SUCCESS! The graph was colored after spilling " << spills << " time(s)." << endl; }
  else { cout << "Result: SUCCESS! Graph colored "; cout << "       Time taken: " << duration.count() << " µs" << endl;}

 } else {
  success = false;
  cout << "       Unknown algorithm specified in the registers file." << endl;
 }
  cout << "  ╚════════════════════════════════════════════════════════════════════╝" << endl;

    cout << endl;
    cout << "\033[34m" << "  ╔══════════════════════════════════════════════════════════════╗" << endl << "\033[0m";
    cout << "\033[34m" << "  ║                   ALGORITHM RESULTS                          ║" << endl << "\033[0m";
    cout << "\033[34m" << "  ╠══════════════════════════════════════════════════════════════╣" << endl << "\033[0m";
    if (success) interference_man.printResultsSuccess();
    else{interference_man.printResultsFailure();}
    cout << "  ╚══════════════════════════════════════════════════════════════╝" << endl;

    if (success) interference_man.outputResultsSuccess(string(argv[4]));
    else{interference_man.outputResultsFailure(string(argv[4]));}

    cout << endl;
    cout << "  This information is available in Output/" + string(argv[4]) << endl;
    return 0;
}