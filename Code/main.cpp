#include <iostream>
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
                    cout << "Invalid input, please enter a number 1-6: ";
                } else {
                    break;
                }
            }

            switch (option_num)
            {
            case 1:
                cout << "Option 1 not yet Implemented" << endl;
                break;
            case 2:
                cout << "Option 2 not yet Implemented" << endl;
                break;
            case 3:
                cout << "Option 3 not yet Implemented" << endl;
                break;
            case 4:
                cout << "Option 4 not yet Implemented" << endl;
                break;
            case 5:
                cout << "Option 5 not yet Implemented" << endl;
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