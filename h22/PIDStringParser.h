#ifndef PID_STRING_PARSER_H
#define PID_STRING_PARSER_H

#include <iostream>
#include <vector>

class PIDStringParser{
 public:
  PIDStringParser(){
  }
  
  ~PIDStringParser(){
  }

 public: 
  std::vector<int> getPIDTargets(std::string stringOfTargets);
};

#endif
