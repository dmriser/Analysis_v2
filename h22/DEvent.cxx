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
#include "h22Event.h"

DEvent::DEvent()
{
  w = 0.0; qq = 0.0; mm2 = 0.0; x = 0.0;
  beam   = TLorentzVector(0,0,5.498,5.498);
  target = TLorentzVector(0,0,0,proton_mass);
  e_index = -123; 
}

DEvent::DEvent(TLorentzVector kPrime)
{
  w = 0.0; qq = 0.0; mm2 = 0.0; x = 0.0; 
  beam     = TLorentzVector(0,0,5.498,5.498);
  target   = TLorentzVector(0,0,0,proton_mass);
  electron = kPrime;
  e_index = -123; 
  refresh();
}

DEvent::DEvent(h22Event event)
{
  w = 0.0; qq = 0.0; mm2 = 0.0; x = 0.0; 
  beam     = TLorentzVector(0,0,5.498,5.498);
  target   = TLorentzVector(0,0,0,proton_mass);
  tracks = event;
  e_index = -123;
}

DEvent::~DEvent()
{
  // Total Destruction 
}

void DEvent::refresh()
{
  TLorentzVector q      = beam-electron;
  TLorentzVector hadron = target + q; 

  qq  = -1*q.Mag2();
  w   = hadron.Mag();
  mm2 = hadron.E()*hadron.E()-hadron.P()*hadron.P();
  x   = qq/(2*target.E()*q.E());
}

void DEvent::set_e_index(int index)
{
  e_index = index;
  electron = TLorentzVector( tracks.p[e_index]*tracks.cx[e_index],
			     tracks.p[e_index]*tracks.cy[e_index],
			     tracks.p[e_index]*tracks.cz[e_index],
			     tracks.p[e_index]);
  refresh();
}

void DEvent::set_electron(TLorentzVector v)
{
  electron = v;
  refresh();
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
  cout.width(12); cout << qq;
  cout.width(12); cout << w;
  cout.width(12); cout << x;
  cout.width(12); cout << mm2 << endl;
}

#endif
