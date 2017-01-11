/////////////////////////////////////////////////////
/*

  David Riser, University of Connecticut 
  September 21, 2016

*/
/////////////////////////////////////////////////////

#ifndef physics_event_cut_h
#define physics_event_cut_h

// c++ includes
#include <iostream>

// my includes
#include "PhysicsEvent.h"

class PhysicsEventCut
{
 public:
  PhysicsEventCut();
  ~PhysicsEventCut();

  int n_pass, n_fail;
  
  // Data Types
 private:
  bool is_enabled;
  double min_value, max_value;
  std::string cut_name;
  
  // Member functions 
 public:
  void disable(){ is_enabled = false; }
  void enable(){ is_enabled  = true; }
  void set_min( double x ) { min_value = x; }
  void set_max( double x ) { max_value = x; }
  void set_name( std::string n ){ cut_name = n; }
  bool is_on(){ return is_enabled; }
  
  int number_pass(){ return n_pass; }
  int number_fail(){ return n_fail; }
  int number_total(){ return n_pass+n_fail; }
  
  double min(){ return min_value; }
  double max(){ return max_value; }
  
  std::string name(){ return cut_name; }
  
  virtual bool applies(PhysicsEvent); /** Pass in the event, and the particle index of interest */
  virtual bool passes(PhysicsEvent); /** Pass in the event, and the particle index of interest */

};

////////////////////////////////////////////////////////////////////////////////////////////////
/*
  
  Missing Mass Cut 

*/
////////////////////////////////////////////////////////////////////////////////////////////////

class PhysicsEventCut_MissingMass : public PhysicsEventCut {
 public:
  PhysicsEventCut_MissingMass();
  ~PhysicsEventCut_MissingMass();

  bool passes(PhysicsEvent event); 

};

////////////////////////////////////////////////////////////////////////////////////////////////
/*
  
  W Cut 

*/
////////////////////////////////////////////////////////////////////////////////////////////////

class PhysicsEventCut_w : public PhysicsEventCut {
 public:
  PhysicsEventCut_w();
  ~PhysicsEventCut_w();

  bool passes(PhysicsEvent event); 

};

////////////////////////////////////////////////////////////////////////////////////////////////
/*
  
  P Cut 

*/
////////////////////////////////////////////////////////////////////////////////////////////////

class PhysicsEventCut_p : public PhysicsEventCut {
 public:
  PhysicsEventCut_p();
  ~PhysicsEventCut_p();

  bool passes(PhysicsEvent event); 

};

////////////////////////////////////////////////////////////////////////////////////////////////
/*
  
  x Cut 

*/
////////////////////////////////////////////////////////////////////////////////////////////////

class PhysicsEventCut_x : public PhysicsEventCut {
 public:
  PhysicsEventCut_x();
  ~PhysicsEventCut_x();

  bool passes(PhysicsEvent event); 

};

////////////////////////////////////////////////////////////////////////////////////////////////
/*
  
  y Cut 

*/
////////////////////////////////////////////////////////////////////////////////////////////////

class PhysicsEventCut_y : public PhysicsEventCut {
 public:
  PhysicsEventCut_y();
  ~PhysicsEventCut_y();

  bool passes(PhysicsEvent event); 

};

////////////////////////////////////////////////////////////////////////////////////////////////
/*
  
  z Cut 

*/
////////////////////////////////////////////////////////////////////////////////////////////////

class PhysicsEventCut_z : public PhysicsEventCut {
 public:
  PhysicsEventCut_z();
  ~PhysicsEventCut_z();

  bool passes(PhysicsEvent event); 

};

////////////////////////////////////////////////////////////////////////////////////////////////
/*
  
  qq Cut 

*/
////////////////////////////////////////////////////////////////////////////////////////////////

class PhysicsEventCut_qq : public PhysicsEventCut {
 public:
  PhysicsEventCut_qq();
  ~PhysicsEventCut_qq();

  bool passes(PhysicsEvent event); 

};

////////////////////////////////////////////////////////////////////////////////////////////////
/*
  
  p perp Cut 

*/
////////////////////////////////////////////////////////////////////////////////////////////////

class PhysicsEventCut_pTransverse : public PhysicsEventCut {
 public:
  PhysicsEventCut_pTransverse();
  ~PhysicsEventCut_pTransverse();

  bool passes(PhysicsEvent event); 

};

////////////////////////////////////////////////////////////////////////////////////////////////
/*
  
  x,y line cut
  y = mx + b

*/
////////////////////////////////////////////////////////////////////////////////////////////////

class PhysicsEventCut_xyLineCut : public PhysicsEventCut {
 public:
  PhysicsEventCut_xyLineCut();
  ~PhysicsEventCut_xyLineCut();

  double m, b;

  bool passes(PhysicsEvent event); 

};

#endif
