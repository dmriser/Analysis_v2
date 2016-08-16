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
 
 
///////////////////////////////////////////////////////////////
/*

  Sampling Fraction Cut 

*/
///////////////////////////////////////////////////////////////

SampFracCut::SampFracCut()
{
  set_name("Samp Frac Cut");
}

SampFracCut::~SampFracCut()
{

}

bool SampFracCut::passes(DEvent event, int index)
{
  double samp = event.tracks.etot[index]/event.tracks.p[index];
  if (samp > min() && samp < max()){ n_pass++; return true; } 
  return false;
}

///////////////////////////////////////////////////////////////
/*

  EC U, V, W Cuts 

*/
///////////////////////////////////////////////////////////////

ECUCut::ECUCut()
{
  set_name("EC-U Cut");
}

ECUCut::~ECUCut()
{

}

bool ECUCut::passes(DEvent event, int index)
{

  double u = event.tracks.uvw(index).X();
    
  if ( u > min() && u < max() ) { n_pass++; return true; }
  else { n_fail++; }
  
  return false; 
}

ECVCut::ECVCut()
{
  set_name("EC-V Cut");
}

ECVCut::~ECVCut()
{

}

bool ECVCut::passes(DEvent event, int index)
{

  double v = event.tracks.uvw(index).Y();
    
  if ( v > min() && v < max() ) { n_pass++; return true; }
  else { n_fail++; }
  
  return false; 
}

ECWCut::ECWCut()
{
  set_name("EC-W Cut");
}

ECWCut::~ECWCut()
{

}

bool ECWCut::passes(DEvent event, int index)
{

  double w = event.tracks.uvw(index).Z();
    
  if ( w > min() && w < max() ) { n_pass++; return true; }
  else { n_fail++; }
  
  return false; 
}

///////////////////////////////////////////////////////////////
/*

  Z-Vertex Cut 

*/
///////////////////////////////////////////////////////////////

ZVertexCut::ZVertexCut()
{
  set_name("Z-Vertex Cut");
}

ZVertexCut::~ZVertexCut()
{

}

bool ZVertexCut::passes(DEvent event, int index)
{

  if ( event.tracks.vz[index] > min() && event.tracks.vz[index] < max() ) { n_pass++; return true; }
  else { n_fail++; }
  
  return false; 
}

///////////////////////////////////////////////////////////////
/*

  EC Edep Inner Cut 

*/
///////////////////////////////////////////////////////////////

ECEdepInnerCut::ECEdepInnerCut()
{
  set_name("EC Edep Inner Cut");
}

ECEdepInnerCut::~ECEdepInnerCut()
{

}

bool ECEdepInnerCut::passes(DEvent event, int index)
{

  if ( event.tracks.ec_ei[index] > min() ) { n_pass++; return true; }
  else { n_fail++; }
  
  return false; 
}

///////////////////////////////////////////////////////////////
/*

  W Cut 

*/
///////////////////////////////////////////////////////////////

WCut::WCut()
{
  set_name("W Cut");
}

WCut::~WCut()
{

}

bool WCut::passes(DEvent event, int index)
{

  if ( event.w > min() && event.w < max()) { n_pass++; return true; }
  else { n_fail++; }
  
  return false; 
}

///////////////////////////////////////////////////////////////
/*

  Virtuality Cut 

*/
///////////////////////////////////////////////////////////////

VirtualityCut::VirtualityCut()
{
  set_name("Virtuality Cut");
}

VirtualityCut::~VirtualityCut()
{

}

bool VirtualityCut::passes(DEvent event, int index)
{

  if ( event.qq > min() && event.qq < max()) { n_pass++; return true; }
  else { n_fail++; }
  
  return false; 
}


///////////////////////////////////////////////////////////////
/*

  Virtuality Cut 

*/
///////////////////////////////////////////////////////////////


MissingMassCut::MissingMassCut()
{
  set_name("Missing Mass Cut");
}

MissingMassCut::~MissingMassCut()
{

}

bool MissingMassCut::passes(DEvent event, int index)
{

  if ( event.mm2 > min() && event.mm2 < max()) { n_pass++; return true; }
  else { n_fail++; }
  
  return false; 
}


#endif
