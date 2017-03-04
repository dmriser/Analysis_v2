#ifndef phase_space_h
#define phase_space_h

// ----------------------------------------
//       c++ standard 
// ----------------------------------------
#include <vector>

// ----------------------------------------
//       My Libs 
// ----------------------------------------
#include "Dimension.h"

// ----------------------------------------
//       ROOT 
// ----------------------------------------


class PhaseSpace{
 public:
  PhaseSpace();
  ~PhaseSpace(){}

 protected:
  std::string             name; 
  std::vector<Dimension*> dims;

 public:
  void addDimension(Dimension *d){ dims.push_back(d); } 
  void setName(std::string inputName){ name = inputName; }

  Dimension           *getDimension(std::string name);
  Dimension           *getDimension(int index);
  std::string          getName(){ return name; } 
  std::vector<double>  getRandom();
  double               getPhaseSpaceVolume();
  int                  getRank(){ return dims.size(); }
};

#endif
