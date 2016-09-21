////////////////////////////////////////////////////////////////////
/*

  David Riser, University of Connecticut 
  August 14, 2016 

  DCut.cxx -> Writes class methods. 

*/
////////////////////////////////////////////////////////////////////

#ifndef physics_event_cut_cxx
#define physics_event_cut_cxx

// c++ includes 
#include <iostream>
#include <cmath>
using std::cout;
using std::endl;
using std::string; 

// my includes
#include "CommonTools.h"
#include "PhysicsEventCut.h"
#include "PhysicsEvent.h"

PhysicsEventCut::PhysicsEventCut()
{
  // Setup procedure.
  is_enabled = true;
  n_pass = 0; n_fail = 0;
  cut_name = " PhysicsEventCut Unassigned ";
}

PhysicsEventCut::~PhysicsEventCut()
{

}


bool PhysicsEventCut::passes(PhysicsEvent event)
{
  if ( !is_enabled ){ return false; } 

  cout << " Inside PhysicsEventCut::passes " << endl; 
  return true;
}

bool PhysicsEventCut::applies(PhysicsEvent event)
{
  return true;
}

///////////////////////////////////////////////////////////////
/*

  PhysicsEventCut_MissingMass 

*/
///////////////////////////////////////////////////////////////
 

PhysicsEventCut_MissingMass::PhysicsEventCut_MissingMass()
{
  set_name("Missing Mass Cut");
}

PhysicsEventCut_MissingMass::~PhysicsEventCut_MissingMass()
{

}

bool PhysicsEventCut_MissingMass::passes(PhysicsEvent event)
{

  double missingMass = sqrt(event.mm2); 
  if ( missingMass > min() && missingMass < max()) { n_pass++; return true; }
  else { n_fail++; }
  
  return false; 
}

///////////////////////////////////////////////////////////////
/*

  PhysicsEventCut_W

*/
///////////////////////////////////////////////////////////////
 

PhysicsEventCut_w::PhysicsEventCut_w(){
  set_name("w Cut");
}

PhysicsEventCut_w::~PhysicsEventCut_w(){

}

bool PhysicsEventCut_w::passes(PhysicsEvent event){

  if ( event.w > min() && event.w < max()) { n_pass++; return true; }
  else { n_fail++; }
  
  return false; 
}
 
///////////////////////////////////////////////////////////////
/*

  PhysicsEventCut_x

*/
///////////////////////////////////////////////////////////////
 

PhysicsEventCut_x::PhysicsEventCut_x(){
  set_name("x Cut");
}

PhysicsEventCut_x::~PhysicsEventCut_x(){

}

bool PhysicsEventCut_x::passes(PhysicsEvent event){

  if ( event.x > min() && event.x < max()) { n_pass++; return true; }
  else { n_fail++; }
  
  return false; 
}
  
///////////////////////////////////////////////////////////////
/*

  PhysicsEventCut_y

*/
///////////////////////////////////////////////////////////////
 

PhysicsEventCut_y::PhysicsEventCut_y(){
  set_name("y Cut");
}

PhysicsEventCut_y::~PhysicsEventCut_y(){

}

bool PhysicsEventCut_y::passes(PhysicsEvent event){

  if ( event.y > min() && event.y < max()) { n_pass++; return true; }
  else { n_fail++; }
  
  return false; 
}
  
///////////////////////////////////////////////////////////////
/*

  PhysicsEventCut_z

*/
///////////////////////////////////////////////////////////////
 

PhysicsEventCut_z::PhysicsEventCut_z(){
  set_name("z Cut");
}

PhysicsEventCut_z::~PhysicsEventCut_z(){

}

bool PhysicsEventCut_z::passes(PhysicsEvent event){

  if ( event.z > min() && event.z < max()) { n_pass++; return true; }
  else { n_fail++; }
  
  return false; 
}
  
///////////////////////////////////////////////////////////////
/*

  PhysicsEventCut_qq

*/
///////////////////////////////////////////////////////////////
 

PhysicsEventCut_qq::PhysicsEventCut_qq(){
  set_name("qq Cut");
}

PhysicsEventCut_qq::~PhysicsEventCut_qq(){

}

bool PhysicsEventCut_qq::passes(PhysicsEvent event){

  if ( event.qq > min() && event.qq < max()) { n_pass++; return true; }
  else { n_fail++; }
  
  return false; 
}
 
#endif
