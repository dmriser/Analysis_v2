////////////////////////////////////////////////
/*

  David Riser, University of Connecticut 
  August 5, 2016 

  DEvent.cxx 
  -> Naming the event class after myself shamelessly.

*/
////////////////////////////////////////////////

#ifndef devent_cxx
#define devent_cxx

// c++ includes
#include <iostream>
using namespace std;

// root includes
#include "TLorentzVector.h"

// my includes
#include "CommonTools.h"
#include "DEvent.h"

DEvent::DEvent()
{
  W = 0.0; QQ = 0.0; MM2 = 0.0;
  beam   = TLorentzVector(0,0,5.498,5.498);
  target = TLorentzVector(0,0,0,proton_mass);
}

DEvent::DEvent(TLorentzVector kPrime)
{
  W = 0.0; QQ = 0.0; MM2 = 0.0;
  beam     = TLorentzVector(0,0,5.498,5.498);
  target   = TLorentzVector(0,0,0,proton_mass);
  electron = kPrime;  
  refresh();
}

DEvent::~DEvent()
{
  // Total Destruction 
}

void DEvent::refresh()
{
  TLorentzVector q      = beam-electron;
  TLorentzVector hadron = target + q; 

  QQ = -1*q.Mag2();
  W  = hadron.Mag();
  MM2 = hadron.E()*hadron.E()-hadron.P()*hadron.P();
}

void DEvent::set_electron(TLorentzVector v)
{
  electron = v;
}

void DEvent::set_beam(TLorentzVector v)
{
  beam = v;
  refresh();
}

void DEvent::set_target(TLorentzVector v)
{
  target = v;
  refresh();
}

void DEvent::set_beam_energy(double e)
{
  electron.SetE(e);
  refresh();
}

void DEvent::set_target_mass(double m)
{
  target.SetE(m);
  refresh();
}

void DEvent::print()
{
  cout.width(8); cout << QQ;
  cout.width(8); cout << W;
  cout.width(8); cout << MM2 << endl;
}

#endif
