#ifndef PID_STRING_PARSER_CXX
#define PID_STRING_PARSER_CXX

#include <cstdlib>
#include <iostream>
#include <vector>
using std::cout; 
using std::endl; 
using std::vector; 
using std::string; 

#include "CommonTools.h"
#include "PIDStringParser.h"

vector<int> PIDStringParser::getPIDTargets(string stringOfTargets){
  vector<int> targets; 
  vector<string> tokens = split(stringOfTargets, '+');

  for (int i=0; i<tokens.size(); i++){
    int thisTarget = atoi(tokens[i].c_str());
    targets.push_back(thisTarget);
  }

  return targets;
}

#endif
