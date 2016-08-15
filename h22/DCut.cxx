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
#include "DEvent.h"

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


bool DCut::passes(DEvent event, int index)
{
  cout << " Inside DCut::passes " << endl; 
  return true;
}


///////////////////////////////////////////////////////////////
/*

  ChargeCut 

*/
///////////////////////////////////////////////////////////////

ChargeCut::ChargeCut()
{
  set_name("Charge Cut");
}

ChargeCut::~ChargeCut()
{
  // Nothing to do 
}

/** Simply checking if the particle is negative */
bool ChargeCut::passes(DEvent event, int index)
{
  if ( event.tracks.q[index] > min() && event.tracks.q[index] < max() ) { n_pass++; return true; }
  else { n_fail++; }
  
  return false; 
}


///////////////////////////////////////////////////////////////
/*

  MomentumCut 

*/
///////////////////////////////////////////////////////////////

MomentumCut::MomentumCut()
{
  set_name("Momentum Cut");
}

MomentumCut::~MomentumCut()
{

}

bool MomentumCut::passes(DEvent event, int index)
{
  if ( event.tracks.p[index] > min() && event.tracks.p[index] < max() ) { n_pass++; return true; }
  else { n_fail++; }
  
  return false; 
}


#endif
