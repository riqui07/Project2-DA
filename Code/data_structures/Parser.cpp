#include <iostream>
#include <string>
#include <set>
#include <fstream>
#include <sstream>
#include <exception>
#include <vector>

#include "Parser.h"

using namespace std;

Parser::Parser(){}

void Parser::parse(const string& filename){
  ifstream read(filename);

  if (!read.is_open()) {
    throw runtime_error("Could not open file" + filename);
  }

  // check whether we should use the methods to parse a ranges or a registers file
  if (filename.find("ranges") != string::npos){
    for(string line; getline(read, line); ){
      if (!line.empty() && line.at(0) != '#') parseLineRanges(line);
    }
  } else{
    for(string line; getline(read, line); ){
      if (!line.empty() && line.at(0) != '#') parseLineRegisters(line);
    }
  }
}

void Parser::parseLineRanges(const string& line){
  stringstream ss(line);
  LiveRange lr;

  // create delimiters
  char tomas = ',', nicholas = ':';

  string token, variable;

  getline(ss, variable, nicholas);

  while(getline(ss, token, tomas)){
    if (token.find("+") != string::npos){
      lr.birth = stoi(token.substr(0, token.length() - 1));
      lr.lines.push_back(lr.birth);
    } else if (token.find("-") != string::npos){
      lr.death = stoi(token.substr(0, token.length() - 1));
      lr.lines.push_back(lr.death);
    } else{
      lr.lines.push_back(stoi(token));
    }
  }

  live_ranges[variable].push_back(lr);
}

void Parser::parseLineRegisters(const string& line){
  stringstream ss(line);
  Register reg;

  char nicholas = ':';

  string token;

  getline(ss, token, nicholas);
  getline(ss, token, nicholas);
  reg.num_registers = stoi(token);

  getline(ss, token, nicholas);
  getline(ss, token, nicholas);
  reg.algorithm = token;
}