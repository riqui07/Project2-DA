#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

struct LiveRange{
  vector<int> lines;
  int birth = -1;
  int death = -1;
};

struct Register{
  int num_registers;
  string algorithm;
  int numeric_value = -1;
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
  const map<string, vector<LiveRange>>& getLiveRanges() const { return live_ranges; }
  const Register& getRegister() const { return reg; }


private:
  map<string, vector<LiveRange>> live_ranges;
  Register reg;

  void parseLineRanges(const string& line);
  void parseLineRegisters(const string& line);
};
#endif //PARSER_H
