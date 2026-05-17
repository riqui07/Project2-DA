#include <iostream>

#include "data_structures/InterferenceMan.h"
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

    // batch mode
    if (string(argv[1]) != "-b")
    {
        usage();
        return 1;
    }

    return 0;
}