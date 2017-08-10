#include "DBins.h"

#include <iostream>
#include <vector>

int main(){

  std::vector<double> x;
  x.push_back(0.0); 
  x.push_back(0.2); 
  x.push_back(0.3); 
  x.push_back(0.6); 

  DLineBins xBins(x);
  
  std::cout << Form("%d",xBins.GetNumber()) << std::endl; 
}
