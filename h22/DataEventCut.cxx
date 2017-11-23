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
    fIsEnabled = true;
    n_pass = 0; n_fail = 0;
    fCutName = " DataEventCut Unassigned ";
}

DataEventCut::~DataEventCut(){

}


bool DataEventCut::IsPassed(h22Event &event, int index){
    if ( !fIsEnabled ){ return false; }

    cout << " Inside DataEventCut::passes " << endl;
    return true;
}

bool DataEventCut::operator<(DataEventCut *otherCut){
    return (this->GetPassFraction() < otherCut->GetPassFraction());
}

bool DataEventCut::operator>(DataEventCut *otherCut){
    return (this->GetPassFraction() > otherCut->GetPassFraction());
}

bool DataEventCut::CanBeApplied(h22Event &event, int index)
{
    return true;
}

float DataEventCut::GetFractionalDistance(h22Event &event, int index){
  return -1.0; 
}

///////////////////////////////////////////////////////////////
/*
  ChargeCut
*/
///////////////////////////////////////////////////////////////

ChargeCut::ChargeCut()
{
    SetName("Charge Cut");
}

ChargeCut::~ChargeCut()
{
    // Nothing to do
}

/** Simply checking if the particle is negative */
bool ChargeCut::IsPassed(h22Event &event, int index)
{
    if ( event.q[index] > GetMin() && event.q[index] < GetMax() ) { n_pass++; return true; }
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
    SetName("Track Quality Cut");
}

TrackQualityCut::~TrackQualityCut()
{
    // Nothing to do
}

/** Simply checking if the particle is negative */
bool TrackQualityCut::IsPassed(h22Event &event, int index)
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
    SetName("Momentum Cut");
}

MomentumCut::~MomentumCut()
{

}

bool MomentumCut::IsPassed(h22Event &event, int index)
{
    if ( event.p[index] > GetMin() && event.p[index] < GetMax() ) { n_pass++; return true; }
    else { n_fail++; }

    return false;
}

float MomentumCut::GetFractionalDistance(h22Event &event, int index){
  return getFractionalDistance(event.p[index], GetMin(), GetMax()); 
}


///////////////////////////////////////////////////////////////
/*
  Number of Photoelectrons Cut
*/
///////////////////////////////////////////////////////////////

NPheCut::NPheCut()
{
    SetName("NPhe Cut");
}

NPheCut::~NPheCut()
{

}

bool NPheCut::IsPassed(h22Event &event, int index)
{
    if ( event.nphe[index] > GetMin() ) { n_pass++; return true; }
    else { n_fail++; }

    return false;
}


///////////////////////////////////////////////////////////////
/*
  CC Phi Matching Cut
*/
///////////////////////////////////////////////////////////////

CCPhiMatchingCut::CCPhiMatchingCut(){
    SetName("CCPhiPMT Match Cut");
}

CCPhiMatchingCut::~CCPhiMatchingCut(){

}

bool CCPhiMatchingCut::IsPassed(h22Event &event, int index){

    double rphi = event.GetRelativePhi(index);

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

    GetMin = 0; GetMax = 0;
    am = 0; as = 0;
    bm = 0; bs = 0;
    cm = 0; cs = 0;
    dm = 0; ds = 0;
    nsigma = 0;

    SetName(Form("Samp Frac Cut %d",s));
}

SampFracCut::~SampFracCut()
{

}

bool SampFracCut::IsPassed(h22Event &event, int index)
{
    double samp = event.etot[index]/event.p[index];
    GetMin = (am-nsigma*as)*pow(event.p[index],3) +  (bm-nsigma*bs)*pow(event.p[index],2) + (cm-nsigma*cs)*event.p[index] +  (dm-nsigma*ds);
    GetMax = (am+nsigma*as)*pow(event.p[index],3) +  (bm+nsigma*bs)*pow(event.p[index],2) + (cm+nsigma*cs)*event.p[index] +  (dm+nsigma*ds);

    if (samp > GetMin && samp < GetMax){ n_pass++; return true; }
    else { n_fail++; return false; }
}

bool SampFracCut::CanBeApplied(h22Event &event, int index)
{
    int s = event.dc_sect[index];
    if (s == sector) return true;
    return false;
}

float SampFracCut::GetFractionalDistance(h22Event &event, int index){
    double samp = event.etot[index]/event.p[index];
    GetMin = (am-nsigma*as)*pow(event.p[index],3) +  (bm-nsigma*bs)*pow(event.p[index],2) + (cm-nsigma*cs)*event.p[index] +  (dm-nsigma*ds);
    GetMax = (am+nsigma*as)*pow(event.p[index],3) +  (bm+nsigma*bs)*pow(event.p[index],2) + (cm+nsigma*cs)*event.p[index] +  (dm+nsigma*ds);
  
    return (2*samp-GetMin-GetMax)/(GetMax-GetMin);
}

///////////////////////////////////////////////////////////////
/*
  CCThetaMatchingCut
*/
///////////////////////////////////////////////////////////////

CCThetaMatchingCut::CCThetaMatchingCut(int s){
    sector = s;

    GetMin = 0; GetMax = 0;
    am = 0; as = 0;
    bm = 0; bs = 0;
    cm = 0; cs = 0;
    dm = 0; ds = 0;
    nsigma = 0;

    SetName(Form("CCTheta Cut %d",s));
}

CCThetaMatchingCut::~CCThetaMatchingCut(){
}

bool CCThetaMatchingCut::IsPassed(h22Event &event, int index){
    double thetaCC = event.GetThetaCC(index);
    int segment    = (event.cc_segm[index]%1000/10);

    GetMin = (am-nsigma*as)*pow(segment,3) +  (bm-nsigma*bs)*pow(segment,2) + (cm-nsigma*cs)*segment +  (dm-nsigma*ds);
    GetMax = (am+nsigma*as)*pow(segment,3) +  (bm+nsigma*bs)*pow(segment,2) + (cm+nsigma*cs)*segment +  (dm+nsigma*ds);

    if (thetaCC > GetMin && thetaCC < GetMax){ n_pass++; return true; }
    else { n_fail++; return false; }
}

bool CCThetaMatchingCut::CanBeApplied(h22Event &event, int index)
{
    int s = event.dc_sect[index];
    if (s == sector) return true;
    return false;
}


float CCThetaMatchingCut::GetFractionalDistance(h22Event &event, int index){
    double thetaCC = event.GetThetaCC(index);
    int segment    = (event.cc_segm[index]%1000/10);

    GetMin = (am-nsigma*as)*pow(segment,3) +  (bm-nsigma*bs)*pow(segment,2) + (cm-nsigma*cs)*segment +  (dm-nsigma*ds);
    GetMax = (am+nsigma*as)*pow(segment,3) +  (bm+nsigma*bs)*pow(segment,2) + (cm+nsigma*cs)*segment +  (dm+nsigma*ds);

  return getFractionalDistance(thetaCC, GetMin, GetMax); 
}


///////////////////////////////////////////////////////////////
/*
  EC U, V, W Cuts
*/
///////////////////////////////////////////////////////////////
ECUCut::ECUCut()
{
    SetName("EC-U Cut");
}

ECUCut::~ECUCut()
{

}

bool ECUCut::IsPassed(h22Event &event, int index)
{

    double u = event.GetUVWVector(index).X();
    
    if ( u > GetMin() && u < GetMax() ) { n_pass++; return true; }
    else { n_fail++; }

    return false;
}

float ECUCut::GetFractionalDistance(h22Event &event, int index){
  double u = event.GetUVWVector(index).X();
  return getFractionalDistance(u, GetMin(), GetMax());
}

ECVCut::ECVCut()
{
    SetName("EC-V Cut");
}

ECVCut::~ECVCut()
{

}

bool ECVCut::IsPassed(h22Event &event, int index)
{

    double v = event.GetUVWVector(index).Y();
    
    if ( v > GetMin() && v < GetMax() ) { n_pass++; return true; }
    else { n_fail++; }

    return false;
}

float ECVCut::GetFractionalDistance(h22Event &event, int index){
  double v = event.GetUVWVector(index).Y();
  return getFractionalDistance(v, GetMin(), GetMax());
}

ECWCut::ECWCut()
{
    SetName("EC-W Cut");
}

ECWCut::~ECWCut()
{

}

bool ECWCut::IsPassed(h22Event &event, int index)
{

    double w = event.GetUVWVector(index).Z();
    
    if ( w > GetMin() && w < GetMax() ) { n_pass++; return true; }
    else { n_fail++; }

    return false;
}

float ECWCut::GetFractionalDistance(h22Event &event, int index){
  double w = event.GetUVWVector(index).Z();
  return getFractionalDistance(w, GetMin(), GetMax());
}

///////////////////////////////////////////////////////////////
/*
  Z-Vertex Cut
*/
///////////////////////////////////////////////////////////////

ZVertexCut::ZVertexCut()
{
    SetName("Z-Vertex Cut");
}

ZVertexCut::~ZVertexCut()
{

}

bool ZVertexCut::IsPassed(h22Event &event, int index)
{

    if ( event.vz[index] > GetMin() && event.vz[index] < GetMax() ) { n_pass++; return true; }
    else { n_fail++; }

    return false;
}

float ZVertexCut::GetFractionalDistance(h22Event &event, int index){
  return getFractionalDistance(event.vz[index], GetMin(), GetMax());
}

///////////////////////////////////////////////////////////////
/*
  CC Fid Cut
*/
///////////////////////////////////////////////////////////////

CCFiducialCut::CCFiducialCut()
{
    a = 0; b = 0, c = 0;
    SetName("CC Fid Cut");
}

CCFiducialCut::~CCFiducialCut()
{

}

bool CCFiducialCut::IsPassed(h22Event &event, int index)
{
    SetMin(a - b*sqrt(1 - pow(event.GetRelativePhi(index),2)/c));
    
    if ( event.GetThetaCC(index) > GetMin() ) { n_pass++; return true; }
    else                                        { n_fail++; return false;}
}

///////////////////////////////////////////////////////////////
/*
  EC Edep Inner Cut
*/
///////////////////////////////////////////////////////////////

ECEdepInnerCut::ECEdepInnerCut()
{
    SetName("EC Edep Inner Cut");
}

ECEdepInnerCut::~ECEdepInnerCut()
{

}

bool ECEdepInnerCut::IsPassed(h22Event &event, int index)
{

    if ( event.ec_ei[index] > GetMin() ) { n_pass++; return true; }
    else { n_fail++; }

    return false;
}

float ECEdepInnerCut::GetFractionalDistance(h22Event &event, int index){
    return (2*event.ec_ei[index]-GetMin()-GetMax())/(GetMax()-GetMin()); 
}

///////////////////////////////////////////////////////////////
/*
  DCR1FiducialCut
*/
///////////////////////////////////////////////////////////////

DCR1FiducialCut::DCR1FiducialCut()
{
    angle = 0; height = 0;
    SetName("DC Region 1 Fid Cut");
}


DCR1FiducialCut::~DCR1FiducialCut()
{

}

bool DCR1FiducialCut::IsPassed(h22Event &event, int index) {

    double slope = 1/tan(0.5*to_radians*angle);
    double left  = (height - slope*event.GetRotatedDCR1PosY(index));
    double right = (height + slope*event.GetRotatedDCR1PosY(index));

    if (event.GetRotatedDCR1PosX(index) > left && event.GetRotatedDCR1PosX(index) > right) { n_pass++; return true; }
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
    SetName("DC Region 3 Fid Cut");
}


DCR3FiducialCut::~DCR3FiducialCut()
{

}

bool DCR3FiducialCut::IsPassed(h22Event &event, int index)
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

DataEventCut_DeltaBetaCut::DataEventCut_DeltaBetaCut(int PID) : fPID(PID) {
    SetName("Delta Beta Cut");
}

DataEventCut_DeltaBetaCut::~DataEventCut_DeltaBetaCut(){
}


bool DataEventCut_DeltaBetaCut::IsPassed(h22Event &event, int hadronIndex){

    // It is very important to note that this
    // routine expects CORRECTED variables.
    double beta       = event.corr_b[hadronIndex]; 
    double betaCalc   = event.p[hadronIndex]/sqrt(pow(event.p[hadronIndex],2)+pow(pid_to_mass(fPID),2));
    double deltaBeta  = betaCalc - beta;

    if (deltaBeta > GetMin() && deltaBeta < GetMax()){
        n_pass++;
        return true;
    } else {
        n_fail++;
        return false;
    }

    return false;
}

///////////////////////////////////////////////////////////////
/*
  DataEventCut_DeltaZVertexCut
*/
///////////////////////////////////////////////////////////////

DataEventCut_DeltaZVertexCut::DataEventCut_DeltaZVertexCut(){
    SetName("Delta Z-Vertex Cut");
}

DataEventCut_DeltaZVertexCut::~DataEventCut_DeltaZVertexCut(){
}


bool DataEventCut_DeltaZVertexCut::IsPassed(h22Event &event, int hadronIndex){

  // using uncorrected position for now, something buggy 
  // about using the corrected ones
  //  double dvz = event.vz[event.GetElectronIndex()]-event.vz[hadronIndex];
  double dvz = event.corr_vz[event.GetElectronIndex()]-event.corr_vz[hadronIndex];
  
  if (dvz > GetMin() && dvz < GetMax()){
    n_pass++;
    return true;
  } else {
    n_fail++;
    return false;
  }
  
  return false;
}

float DataEventCut_DeltaZVertexCut::GetFractionalDistance(h22Event &event, int index){
  double dvz = event.corr_vz[event.GetElectronIndex()]-event.corr_vz[index];
  return getFractionalDistance(dvz, GetMin(), GetMax());
}

///////////////////////////////////////////////////////////////
/*
  DataEventCut_TOFMassCut
*/
///////////////////////////////////////////////////////////////

DataEventCut_TOFMassCut::DataEventCut_TOFMassCut() {
    SetName("TOF Mass Cut");
}

DataEventCut_TOFMassCut::~DataEventCut_TOFMassCut(){
}

bool DataEventCut_TOFMassCut::CanBeApplied(h22Event &event, int index){
  return true;
}

bool DataEventCut_TOFMassCut::IsPassed(h22Event &event, int hadronIndex){
    double tofmass = pow(event.p[hadronIndex],2)*(1-pow(event.corr_b[hadronIndex],2))/pow(event.corr_b[hadronIndex],2);

    if (tofmass > pow(GetMin(),2) && tofmass < pow(GetMax(),2)){
        n_pass++;
        return true;
    } else {
        n_fail++;
        return false;
    }

    return false;
}


///////////////////////////////////////////////////////////////
/*
  DataEventCut_DBetaMinimizerCut
*/
///////////////////////////////////////////////////////////////

DataEventCut_DBetaMinimizerCut::DataEventCut_DBetaMinimizerCut(){
  SetName("Delta Beta Minimizer Cut");
}

DataEventCut_DBetaMinimizerCut::~DataEventCut_DBetaMinimizerCut(){
  
}

bool DataEventCut_DBetaMinimizerCut::CanBeApplied(h22Event &event, int index){
  return true; 
}

bool DataEventCut_DBetaMinimizerCut::IsPassed(h22Event &event, int index){
  
  std::vector<float> diff; 
  
  for(int ipart=0; ipart<possibles.size(); ipart++){
    float beta = event.p[index]/sqrt(pow(event.p[index],2)+pow(pid_to_mass(possibles[ipart]),2));
    diff.push_back(fabs(event.corr_b[index]-beta)); 
  }

  int indexOfMin = GetMin(diff);  
  int pidOfMin   = possibles[indexOfMin];

  if (pidOfMin == target){
    n_pass++;
    return true;
  } else {
    n_fail++;
    return false;
  }
  
  return false;
}


///////////////////////////////////////////////////////////////
/*
  DataEventCut_BetaPCut
*/
///////////////////////////////////////////////////////////////

DataEventCut_BetaPCut::DataEventCut_BetaPCut(int s) : sector(s){
  SetName(Form("Beta vs. P Cut Sector %d",sector));
}

DataEventCut_BetaPCut::~DataEventCut_BetaPCut(){
}

// wont owrk for neutral
bool DataEventCut_BetaPCut::CanBeApplied(h22Event &event, int index){
    return (event.dc_sect[index] == sector);
}

bool DataEventCut_BetaPCut::IsPassed(h22Event &event, int hadronIndex){

  double current_min = (am-nsigma*as)*pow(event.p[hadronIndex],3) + 
    (bm-nsigma*bs)*pow(event.p[hadronIndex],2) + 
    (cm-nsigma*cs)*event.p[hadronIndex] + 
    (dm-nsigma*ds);

  double current_max = (am+nsigma*as)*pow(event.p[hadronIndex],3) + 
    (bm+nsigma*bs)*pow(event.p[hadronIndex],2) + 
    (cm+nsigma*cs)*event.p[hadronIndex] + 
    (dm+nsigma*ds);
  

    if (event.corr_b[hadronIndex] > current_min && event.corr_b[hadronIndex] < current_max){
        n_pass++;
        return true;
    } else {
        n_fail++;
        return false;
    }

    return false;
}

float DataEventCut_BetaPCut::GetFractionalDistance(h22Event &event, int index){

  double current_min = (am-nsigma*as)*pow(event.p[index],3) + 
    (bm-nsigma*bs)*pow(event.p[index],2) + 
    (cm-nsigma*cs)*event.p[index] + 
    (dm-nsigma*ds);

  double current_max = (am+nsigma*as)*pow(event.p[index],3) + 
    (bm+nsigma*bs)*pow(event.p[index],2) + 
    (cm+nsigma*cs)*event.p[index] + 
    (dm+nsigma*ds);

  return getFractionalDistance(event.corr_b[index], current_min, current_max);
}


///////////////////////////////////////////////////////////////
/*
  TestCut 
*/
///////////////////////////////////////////////////////////////

TestCut::TestCut(){
    SetName("Test Cut");
}

TestCut::~TestCut(){
}

bool TestCut::CanBeApplied(h22Event &event, int index){
  return true; 
}

bool TestCut::IsPassed(h22Event &event, int hadronIndex){
  std::cout << "[TestCut::IsPassed] hadron beta = " << event.corr_b[hadronIndex] << std::endl; 
}


#endif
