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
  std::string             fName;
  std::vector<Dimension*> fDims;

 public:
  void addDimension(Dimension *d){ fDims.push_back(d); }
  void setName(std::string inputName){ fName = inputName; }

  Dimension           *getDimension(std::string fName);
  Dimension           *getDimension(int index);
  std::string          getName(){ return fName; }
  std::vector<double>  getRandom();
  double               getPhaseSpaceVolume();
  int                  getRank(){ return fDims.size(); }
};

#endif
