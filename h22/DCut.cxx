////////////////////////////////////////////////////////////////////
/*

  David Riser, University of Connecticut 
  August 14, 2016 

  DCut.cxx -> Writes class methods. 

*/
////////////////////////////////////////////////////////////////////

#ifndef dcut_cxx
#define dcut_cxx

// c++ includes 
#include <iostream>
using std::cout;
using std::endl;
using std::string; 

// my includes 
#include "DCut.h"
#include "h22Event.h"

DCut::DCut()
{
  // Setup procedure.
  is_enabled = true;
  n_pass = 0; n_fail = 0;
  cut_name = " DCut Unassigned ";
}

DCut::~DCut()
{

}

bool DCut::passes(h22Event event, int index)
{
  return true;
}

///////////////////////////////////////////////////////////////
/*

  ChargeCut 

*/
///////////////////////////////////////////////////////////////

ChargeCut::ChargeCut()
{
  set_name(" Charge Cut ");
}

ChargeCut::~ChargeCut()
{
  // Nothing to do 
}

/** Simply checking if the particle is negative */
bool ChargeCut::passes(h22Event event, int index)
{
  if ( event.q[index] > min() && event.q[index] < max() ) return true;
  return false; 
}


#endif
