#include <iostream>

#include "CommonTools.h"

int main(){

  std::cout << "Include: " << Global::Environment::GetIncludePath() << std::endl; 
  std::cout << "Analysis: " << Global::Environment::GetAnalysisPath() << std::endl; 

  return 0;
}
