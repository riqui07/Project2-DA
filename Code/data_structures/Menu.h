#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <string>
#include <vector>
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

void displayParsedInfo(const Parser& parser);
#endif //MENU_H