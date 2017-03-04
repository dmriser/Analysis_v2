#ifndef dimension_cxx
#define dimension_cxx

// -------------------------------------
//    c++ standard 
// -------------------------------------
#include <iostream>
using std::cout; 
using std::endl; 
using std::string; 

// -------------------------------------
//   My Libs 
// -------------------------------------
#include "CommonTools.h"
#include "Dimension.h"

// -------------------------------------
//    ROOT Libs
// -------------------------------------
#include "TRandom3.h"

Dimension::Dimension(){
  min  = 0.0; 
  max  = 1.0; 
  name = "Unset";
}

Dimension::Dimension(string n, double mini, double maxi) : name(n), min(mini), max(maxi) {
}

string Dimension::toString(){
  return ("[Dimension] Name = " + name + "(" + stringify(min) + "," + stringify(max) + ")");
}

double Dimension::getRandom(){
  return random.Uniform(min,max); 
} 

#endif
