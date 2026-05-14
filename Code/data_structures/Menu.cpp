#include <iostream>
#include <vector>
#include <string>
#include "Menu.h"
#include "Parser.h"
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