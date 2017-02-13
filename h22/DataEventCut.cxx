////////////////////////////////////////////////////////////////////
/*
  David Riser, University of Connecticut 
  August 14, 2016 
  DataEventCut.cxx -> Writes class methods. 
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
#include "DataEventCut.h"
#include "h22Event.h"

DataEventCut::DataEventCut(){
  // Setup procedure.
  is_enabled = true;
  n_pass = 0; n_fail = 0;
  cut_name = " DataEventCut Unassigned ";
}

DataEventCut::~DataEventCut(){

}


bool DataEventCut::passes(h22Event event, int index){
  if ( !is_enabled ){ return false; } 

  cout << " Inside DataEventCut::passes " << endl; 
  return true;
}

bool DataEventCut::operator<(DataEventCut *otherCut){
  return (this->passFraction() < otherCut->passFraction());
}

bool DataEventCut::operator>(DataEventCut *otherCut){
  return (this->passFraction() > otherCut->passFraction());
}

bool DataEventCut::applies(h22Event event, int index)
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
  CC Phi Matching Cut 
*/
///////////////////////////////////////////////////////////////

CCPhiMatchingCut::CCPhiMatchingCut(){
  set_name("CCPhiPMT Match Cut");
}

CCPhiMatchingCut::~CCPhiMatchingCut(){

}

bool CCPhiMatchingCut::passes(h22Event event, int index){

  double rphi = event.rphi(index);

  // PMT is -1 left, +1 right, 0 both
  bool angleMatchesPMT = false; 
  int PMT              = event.cc_segm[index]/1000 -1; 

  if (PMT == -1 && rphi <= 0.0)     { angleMatchesPMT = true; } // Good match left
  else if (PMT ==  1 && rphi >= 0.0){ angleMatchesPMT = true; } // Good match right
  else if (PMT == 0 || rphi == 0.0) { angleMatchesPMT = true; } // Good match both

  if (angleMatchesPMT) { n_pass++; return true; }
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
  CCThetaMatchingCut 
*/
///////////////////////////////////////////////////////////////

CCThetaMatchingCut::CCThetaMatchingCut(int s){
  sector = s; 

  min = 0; max = 0;
  am = 0; as = 0;
  bm = 0; bs = 0;
  cm = 0; cs = 0;
  dm = 0; ds = 0;
  nsigma = 0;
  
  set_name(Form("CCTheta Cut %d",s));
}

CCThetaMatchingCut::~CCThetaMatchingCut(){
}

bool CCThetaMatchingCut::passes(h22Event event, int index){
  double thetaCC = event.theta_cc(index);
  int segment    = (event.cc_segm[index]%1000/10);

  min = (am-nsigma*as)*pow(segment,3) +  (bm-nsigma*bs)*pow(segment,2) + (cm-nsigma*cs)*segment +  (dm-nsigma*ds); 
  max = (am+nsigma*as)*pow(segment,3) +  (bm+nsigma*bs)*pow(segment,2) + (cm+nsigma*cs)*segment +  (dm+nsigma*ds); 

  if (thetaCC > min && thetaCC < max){ n_pass++; return true; } 
  else { n_fail++; return false; } 
}

bool CCThetaMatchingCut::applies(h22Event event, int index)
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
  a = 0; b = 0, c = 0;
  set_name("CC Fid Cut"); 
}

CCFiducialCut::~CCFiducialCut()
{

}

bool CCFiducialCut::passes(h22Event event, int index)
{
  set_min(a - b*sqrt(1 - pow(event.rphi(index),2)/c));
    
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

///////////////////////////////////////////////////////////////
/*
  DataEventCut_DeltaBetaCut
*/
///////////////////////////////////////////////////////////////

DataEventCut_DeltaBetaCut::DataEventCut_DeltaBetaCut(){
  set_name("Delta Beta Cut");
}

DataEventCut_DeltaBetaCut::~DataEventCut_DeltaBetaCut(){
}


bool DataEventCut_DeltaBetaCut::passes(h22Event event, int electronIndex, int hadronIndex, int PID){
  
  // It is very important to note that this 
  // routine expects CORRECTED variables.
  double startTime  = event.sc_t[electronIndex]-event.sc_r[electronIndex]/speed_of_light;
  double beta       = event.sc_r[hadronIndex]/(event.sc_t[hadronIndex]-startTime)/speed_of_light;  
  double betaCalc   = event.p[hadronIndex]/sqrt(pow(event.p[hadronIndex],2)+pow(pid_to_mass(PID),2));
  double deltaBeta  = betaCalc - beta; 

  if (deltaBeta > min() && deltaBeta < max()){
    n_pass++;
    return true; 
  } else {
    n_fail++; 
    return false;
  }

  return false;
}
 

#endif
