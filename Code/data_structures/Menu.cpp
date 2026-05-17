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


void displayParsedLiveRanges(Parser parser){

 map<string, vector<LiveRange>> live_ranges = parser.getLiveRanges();
 cout << "\n";
 cout << "\033[34m" << "  ╔══════════════════════════════════════════════════════════════╗" << endl << "\033[0m";
 cout << "\033[34m" << "  ║                      LIVE RANGES                             ║" << endl << "\033[0m";
 cout << "\033[34m" << "  ╠══════════════════════════════════════════════════════════════╣" << endl << "\033[0m";
 for (const auto& lr : live_ranges) {
  cout << "       Variable: " << lr.first << endl;
  cout << "       Title: " << s.title << endl;
  cout << "       Authors: " << s.authors << endl;
  cout << "       Email: " << s.email << endl;
  cout << "       Primary: " << s.primary << endl;
  cout << "       Secondary: " << s.secondary << endl;
  cout << "  ╠══════════════════════════════════════════════════════════════╣" << endl;
 }
}