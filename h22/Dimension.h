#ifndef dimension_h
#define dimension_h

// -------------------------------------
//    c++ standard 
// -------------------------------------
#include <iostream>

// -------------------------------------
//   My Libs 
// -------------------------------------

// -------------------------------------
//    ROOT Libs
// -------------------------------------
#include "TRandom3.h"

class Dimension{
 public:
  Dimension();
  Dimension(std::string, double, double);
  ~Dimension(){ }

 protected:
  TRandom3    random; 
  std::string name;
  double      min, max; 
  
 public:
  void setName(std::string n){ name = n; } 
  void setMin(double m){ min = m; } 
  void setMax(double m){ max = m; } 
  std::string getName(){ return name; } 
  double getMin(){ return min; }
  double getMax(){ return max; }
  double getRange(){ return max-min; }
  std::string toString();
  double getRandom(); 
  
};

#endif
