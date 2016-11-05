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
  if ( !is_enabled ){ return false; } 

  cout << " Inside DCut::passes " << endl; 
  return true;
}

bool DCut::applies(h22Event event, int index)
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
bool ChargeCut::passes(h22Event event, int index)
{
  if ( event.q[index] > min() && event.q[index] < max() ) { n_pass++; return true; }
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
bool TrackQualityCut::passes(h22Event event, int index)
{
  if ( event.cc_sect[index] > 0 && event.dc_sect[index] > 0 && event.ec_sect[index] > 0 ) { n_pass++; return true; }
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

bool MomentumCut::passes(h22Event event, int index)
{
  if ( event.p[index] > min() && event.p[index] < max() ) { n_pass++; return true; }
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

bool NPheCut::passes(h22Event event, int index)
{
  if ( event.nphe[index] > min() ) { n_pass++; return true; }
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

bool SampFracCut::passes(h22Event event, int index)
{
  double samp = event.etot[index]/event.p[index];

  min = (am-nsigma*as)*pow(event.p[index],3) +  (bm-nsigma*bs)*pow(event.p[index],2) + (cm-nsigma*cs)*event.p[index] +  (dm-nsigma*ds); 
  max = (am+nsigma*as)*pow(event.p[index],3) +  (bm+nsigma*bs)*pow(event.p[index],2) + (cm+nsigma*cs)*event.p[index] +  (dm+nsigma*ds); 

  if (samp > min && samp < max){ n_pass++; return true; } 
  else { n_fail++; return false; } 
}

bool SampFracCut::applies(h22Event event, int index)
{
  int s = event.dc_sect[index];
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

bool ECUCut::passes(h22Event event, int index)
{

  double u = event.uvw(index).X();
    
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

bool ECVCut::passes(h22Event event, int index)
{

  double v = event.uvw(index).Y();
    
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

bool ECWCut::passes(h22Event event, int index)
{

  double w = event.uvw(index).Z();
    
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

bool ZVertexCut::passes(h22Event event, int index)
{

  if ( event.vz[index] > min() && event.vz[index] < max() ) { n_pass++; return true; }
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

bool CCFiducialCut::passes(h22Event event, int index)
{
  set_min(a - b*sqrt(1 - pow(event.rphi(index),2)/360));
    
  if ( event.theta_cc(index) > min() ) { n_pass++; return true; }
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

bool ECEdepInnerCut::passes(h22Event event, int index)
{

  if ( event.ec_ei[index] > min() ) { n_pass++; return true; }
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

bool DCR1FiducialCut::passes(h22Event event, int index)
{

  double slope = 1/tan(0.5*to_radians*angle);
  double left  = (height - slope*event.rot_dc1y(index));
  double right = (height + slope*event.rot_dc1y(index));
  
  if (event.rot_dc1x(index) > left && event.rot_dc1x(index) > right) { n_pass++; return true; }
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

bool DCR3FiducialCut::passes(h22Event event, int index)
{

  double slope = 1/tan(0.5*to_radians*angle);
  double left  = (height - slope*event.tl3_y[index]);
  double right = (height + slope*event.tl3_y[index]);
  
  if (event.tl3_x[index] > left && event.tl3_x[index] > right) { n_pass++; return true; }
  else { n_fail++; return false; }
}
 

#endif
