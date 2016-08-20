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
#include <cmath>
using std::cout;
using std::endl;
using std::string; 

// my includes
#include "CommonTools.h"
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

bool DCut::applies(DEvent event, int index)
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

  Track Quality Cut 

*/
///////////////////////////////////////////////////////////////

TrackQualityCut::TrackQualityCut()
{
  set_name("Track Quality Cut");
}

TrackQualityCut::~TrackQualityCut()
{
  // Nothing to do 
}

/** Simply checking if the particle is negative */
bool TrackQualityCut::passes(DEvent event, int index)
{
  if ( event.tracks.cc_sect[index] > 0 && event.tracks.dc_sect[index] > 0 && event.tracks.ec_sect[index] > 0 ) { n_pass++; return true; }
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

  Number of Photoelectrons Cut 

*/
///////////////////////////////////////////////////////////////

NPheCut::NPheCut()
{
  set_name("NPhe Cut");
}

NPheCut::~NPheCut()
{

}

bool NPheCut::passes(DEvent event, int index)
{
  if ( event.tracks.nphe[index] > min() ) { n_pass++; return true; }
  else { n_fail++; }
  
  return false; 
}
 
 
///////////////////////////////////////////////////////////////
/*

  Sampling Fraction Cut 

*/
///////////////////////////////////////////////////////////////

SampFracCut::SampFracCut(int s)
{
  sector = s; 

  min = 0; max = 0;
  am = 0; as = 0;
  bm = 0; bs = 0;
  cm = 0; cs = 0;
  dm = 0; ds = 0;
  nsigma = 0;
  
  set_name(Form("Samp Frac Cut %d",s));
}

SampFracCut::~SampFracCut()
{

}

bool SampFracCut::passes(DEvent event, int index)
{
  double samp = event.tracks.etot[index]/event.tracks.p[index];

  min = (am-nsigma*as)*pow(event.tracks.p[index],3) +  (bm-nsigma*bs)*pow(event.tracks.p[index],2) + (cm-nsigma*cs)*event.tracks.p[index] +  (dm-nsigma*ds); 
  max = (am+nsigma*as)*pow(event.tracks.p[index],3) +  (bm+nsigma*bs)*pow(event.tracks.p[index],2) + (cm+nsigma*cs)*event.tracks.p[index] +  (dm+nsigma*ds); 
  
  if (samp > min && samp < max){ n_pass++; return true; } 
  else { n_fail++; return false; } 
}

bool SampFracCut::applies(DEvent event, int index)
{
  int s = event.tracks.dc_sect[index];
  if (s == sector) return true;
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

  CC Fid Cut

*/
///////////////////////////////////////////////////////////////

CCFiducialCut::CCFiducialCut()
{
  a = 0; b = 0;
  set_name("CC Fid Cut"); 
}

CCFiducialCut::~CCFiducialCut()
{

}

bool CCFiducialCut::passes(DEvent event, int index)
{
  set_min(a - b*sqrt(1 - pow(event.tracks.rphi(index),2)/360));
    
  if ( event.tracks.theta_cc(index) > min() ) { n_pass++; return true; }
  else                                        { n_fail++; return false;}
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

  DCR1FiducialCut 

*/
///////////////////////////////////////////////////////////////

DCR1FiducialCut::DCR1FiducialCut()
{
  angle = 0; height = 0;
  set_name("DC Region 1 Fid Cut");
}


DCR1FiducialCut::~DCR1FiducialCut()
{

}

bool DCR1FiducialCut::passes(DEvent event, int index)
{

  double slope = 1/tan(0.5*to_radians*angle);
  double left  = (height - slope*event.tracks.rot_dc1y(index));
  double right = (height + slope*event.tracks.rot_dc1y(index));
  
  if (event.tracks.rot_dc1x(index) > left && event.tracks.rot_dc1x(index) > right) { n_pass++; return true; }
  else { n_fail++; return false; }
}


///////////////////////////////////////////////////////////////
/*

  DCR3FiducialCut 

*/
///////////////////////////////////////////////////////////////

DCR3FiducialCut::DCR3FiducialCut()
{
  angle = 0; height = 0;
  set_name("DC Region 3 Fid Cut");
}


DCR3FiducialCut::~DCR3FiducialCut()
{

}

bool DCR3FiducialCut::passes(DEvent event, int index)
{

  double slope = 1/tan(0.5*to_radians*angle);
  double left  = (height - slope*event.tracks.tl3_y[index]);
  double right = (height + slope*event.tracks.tl3_y[index]);
  
  if (event.tracks.tl3_x[index] > left && event.tracks.tl3_x[index] > right) { n_pass++; return true; }
  else { n_fail++; return false; }
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
