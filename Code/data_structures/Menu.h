#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <string>
#include <vector>

#include "InterferenceMan.h"
#include "Parser.h"

using namespace std;

/**
 * @brief Prints the Menu Options
 */
void showMenu();

/**
 * @brief Prints Goodbye Message
 */
void byebye();

/**
 * @brief Handles the parsing of the input files
 */
void input_handler(Parser& parser);

/**
 * @brief Displays the Info parsed in the Input Handler
 */
void displayParsedInfo(Parser parser);

/**
 * @brief Displays whether the allocation was successful or not
 */
bool runAllocation(Parser parser);

/**
 * @brief Displays the Results of the Register Allocation
 */
void displayAllocationResults(Parser parser);

/**
 * @brief Handles the output to a file
 */
void outputHandler(Parser parser);
#endif //MENU_H