#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

struct WebOuAlgoAssim{
  vector<int> lines;
  string variable;
  int birth = -1;
  int death = -1;
};

struct Register{
  int num_registers;
  string algorithm;
};

enum ParseType{
  REGISTERS,
  RANGES
};

class Parser{
public:
  /**
 * @brief Constructor of a Parser Object
 */
  Parser();

  void parse(const string& filename);

  // getter
  const map<string, vector<WebOuAlgoAssim>>& getLiveRanges() const { return live_ranges; }


private:
  map<string, vector<WebOuAlgoAssim>> live_ranges;

  void parseLineRanges(const string& line);
  void parseLineRegisters(const string& line);
};
#endif //PARSER_H
