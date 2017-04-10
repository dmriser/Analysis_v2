#ifndef phase_space_cxx
#define phase_space_cxx

// ----------------------------------------
//       c++ standard 
// ----------------------------------------
#include <vector>
using std::vector; 

// ----------------------------------------
//       My Libs 
// ----------------------------------------
#include "Dimension.h"
#include "PhaseSpace.h"

PhaseSpace::PhaseSpace(){

}



Dimension* PhaseSpace::getDimension(std::string name){
  for(int d=0; d<getRank(); d++){
    if (fDims[d]->getName() == name ){ return fDims[d]; }
  }

  return new Dimension(); 
}


Dimension* PhaseSpace::getDimension(int index){
  if (index < getRank()){
    return fDims[index];
  }

  return new Dimension(); 
}

vector<double> PhaseSpace::getRandom(){
  vector<double> point; 

  for(int d=0; d<getRank(); d++){
    point.push_back(fDims[d]->getRandom()); 
  }

  return point; 
}

double PhaseSpace::getPhaseSpaceVolume(){
  double vol = 1.0; 

  for(int d=0; d<getRank(); d++){
    vol *= fDims[d]->getRange(); 
  }

  return vol; 
}



#endif
