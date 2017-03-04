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
    if (dims[d]->getName() == name ){ return dims[d]; }
  }

  return new Dimension(); 
}


Dimension* PhaseSpace::getDimension(int index){
  if (index < getRank()){
    return dims[index];
  }

  return new Dimension(); 
}

vector<double> PhaseSpace::getRandom(){
  vector<double> point; 

  for(int d=0; d<getRank(); d++){
    point.push_back(dims[d]->getRandom()); 
  }

  return point; 
}

double PhaseSpace::getPhaseSpaceVolume(){
  double vol = 1.0; 

  for(int d=0; d<getRank(); d++){
    vol *= dims[d]->getRange(); 
  }

  return vol; 
}



#endif
