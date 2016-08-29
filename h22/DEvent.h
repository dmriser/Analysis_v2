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

// my includes
#include "h22Event.h"

// root includes
#include "TLorentzVector.h"

class DEvent
{
  // Default Constr. Destr.
 public:
  DEvent();
  ~DEvent();

  DEvent(TLorentzVector); /** Declare with TLorentzVector of final state electron */
  DEvent(h22Event); /** Declare with an event, later set the electron index and the kinematics are calculated */
  
  // Datatypes
 public:
  int e_index; 
  double qq, mm2, x, y, w; 
  TLorentzVector target;
  TLorentzVector beam;
  TLorentzVector electron;

  h22Event tracks; 
  
  // Member Functions
  void set_beam(TLorentzVector);
  void set_target(TLorentzVector);
  void set_electron(TLorentzVector);
  void set_beam_energy(double);
  void set_target_mass(double);
  void set_e_index(int);
  void set_event(h22Event ev){ tracks = ev; }
  void refresh();
  void print();
  
};

#endif
