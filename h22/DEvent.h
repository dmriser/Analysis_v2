////////////////////////////////////////////////
/*

  David Riser, University of Connecticut 
  August 5, 2016 

  DEvent.h -> Naming the event class after myself shamelessly.

*/
////////////////////////////////////////////////

#ifndef devent_h
#define devent_h

// c++ includes
#include <iostream>

// root includes
#include "TLorentzVector.h"

class DEvent
{
  // Default Constr. Destr.
 public:
  DEvent();
  ~DEvent();

  DEvent(TLorentzVector);
  
  // Datatypes
 public:
  double QQ, MM2, W; 
  TLorentzVector target;
  TLorentzVector beam;
  TLorentzVector electron;
  
  // Member Functions
  void set_beam(TLorentzVector);
  void set_target(TLorentzVector);
  void set_electron(TLorentzVector);
  void set_beam_energy(double);
  void set_target_mass(double);
  void refresh();
  void print();
  
};

#endif
