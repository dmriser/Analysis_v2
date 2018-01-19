///////////////////////////////////////////////////////////////////
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
#include <map>
#include <cmath>
using std::cout;
using std::endl;
using std::string; 

// my includes
#include "CommonTools.h"
#include "DataEventCut.h"
#include "h22Event.h"

#include "TMath.h"

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

void DataEventCut::Configure(Parameters *params){
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

void MomentumCut::Configure(Parameters *params){

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


void SampFracCut::Configure(Parameters *params){

  std::vector<std::string> requiredParameters;
  requiredParameters.push_back("EL_SF_MU_A"); 
  requiredParameters.push_back("EL_SF_MU_B"); 
  requiredParameters.push_back("EL_SF_MU_C"); 
  requiredParameters.push_back("EL_SF_MU_D"); 
  requiredParameters.push_back("EL_SF_SIGMA_A"); 
  requiredParameters.push_back("EL_SF_SIGMA_B"); 
  requiredParameters.push_back("EL_SF_SIGMA_C"); 
  requiredParameters.push_back("EL_SF_SIGMA_D"); 
  requiredParameters.push_back("EL_EC_NSIGMA"); 

  bool hasParams = true; 
  for(std::string param : requiredParameters){
    if (!params->hasParameter(param)){
      std::cerr << "[MomentumCut::Configure] Error searching for parameters in configuration " << param << std::endl; 
      hasParams = false; 
    }
  }

  // we're ok to set them 
  if (hasParams){
    am = params->getParameter("EL_SF_MU_A").getValue(sector-1); 
    bm = params->getParameter("EL_SF_MU_B").getValue(sector-1); 
    cm = params->getParameter("EL_SF_MU_C").getValue(sector-1); 
    dm = params->getParameter("EL_SF_MU_D").getValue(sector-1); 
    as = params->getParameter("EL_SF_SIGMA_A").getValue(sector-1); 
    bs = params->getParameter("EL_SF_SIGMA_B").getValue(sector-1); 
    cs = params->getParameter("EL_SF_SIGMA_C").getValue(sector-1); 
    ds = params->getParameter("EL_SF_SIGMA_D").getValue(sector-1); 
    nsigma = params->getParameter("EL_EC_NSIGMA").getValue(0); 
  }

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


void CCThetaMatchingCut::Configure(Parameters *params){

  std::vector<std::string> requiredParameters;
  requiredParameters.push_back("EL_CCT_MU_A"); 
  requiredParameters.push_back("EL_CCT_MU_B"); 
  requiredParameters.push_back("EL_CCT_MU_C"); 
  requiredParameters.push_back("EL_CCT_MU_D"); 
  requiredParameters.push_back("EL_CCT_SIGMA_A"); 
  requiredParameters.push_back("EL_CCT_SIGMA_B"); 
  requiredParameters.push_back("EL_CCT_SIGMA_C"); 
  requiredParameters.push_back("EL_CCT_SIGMA_D"); 
  requiredParameters.push_back("EL_CCT_NSIGMA"); 

  bool hasParams = true; 
  for(std::string param : requiredParameters){
    if (!params->hasParameter(param)){
      std::cerr << "[CCThetaMatchingCut::Configure] Error searching for parameters in configuration " << param << std::endl; 
      hasParams = false; 
    }
  }

  // we're ok to set them 
  if (hasParams){
    am = params->getParameter("EL_CCT_MU_A").getValue(sector-1); 
    bm = params->getParameter("EL_CCT_MU_B").getValue(sector-1); 
    cm = params->getParameter("EL_CCT_MU_C").getValue(sector-1); 
    dm = params->getParameter("EL_CCT_MU_D").getValue(sector-1); 
    as = params->getParameter("EL_CCT_SIGMA_A").getValue(sector-1); 
    bs = params->getParameter("EL_CCT_SIGMA_B").getValue(sector-1); 
    cs = params->getParameter("EL_CCT_SIGMA_C").getValue(sector-1); 
    ds = params->getParameter("EL_CCT_SIGMA_D").getValue(sector-1); 
    nsigma = params->getParameter("EL_CCT_NSIGMA").getValue(0); 
  }

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

void ECUCut::Configure(Parameters *params){
  if (params->hasParameter("EL_ECU_MIN") && 
      params->hasParameter("EL_ECU_MAX")){

    SetMin(params->getParameter("EL_ECU_MIN").getValue(0)); 
    SetMax(params->getParameter("EL_ECU_MAX").getValue(0)); 
  } else {
    std::cerr << "[ECUCut::Configure] Error finding parameters. " << std::endl; 
  }
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

void ECVCut::Configure(Parameters *params){
  if (params->hasParameter("EL_ECV_MIN") && 
      params->hasParameter("EL_ECV_MAX")){

    SetMin(params->getParameter("EL_ECV_MIN").getValue(0)); 
    SetMax(params->getParameter("EL_ECV_MAX").getValue(0)); 
  } else {
    std::cerr << "[ECUCut::Configure] Error finding parameters. " << std::endl; 
  }
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

void ECWCut::Configure(Parameters *params){
  if (params->hasParameter("EL_ECW_MIN") && 
      params->hasParameter("EL_ECW_MAX")){

    SetMin(params->getParameter("EL_ECW_MIN").getValue(0)); 
    SetMax(params->getParameter("EL_ECW_MAX").getValue(0)); 
  } else {
    std::cerr << "[ECWCut::Configure] Error finding parameters. " << std::endl; 
  }
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

void ZVertexCut::Configure(Parameters *params){
  if(params->hasParameter("EL_VZ_MIN") && params->hasParameter("EL_VZ_MAX")){
    SetMin(params->getParameter("EL_VZ_MIN").getValue(0));
    SetMax(params->getParameter("EL_VZ_MAX").getValue(0));
  } else {
    std::cerr << "[ZVertexCut::Configure] Error finding parameters. " << std::endl; 
  }
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

void CCFiducialCut::Configure(Parameters *params){
  if(params->hasParameter("EL_CC_FIDA") && 
     params->hasParameter("EL_CC_FIDB") && 
     params->hasParameter("EL_CC_FIDC")){

    a = params->getParameter("EL_CC_FIDA").getValue(0);
    b = params->getParameter("EL_CC_FIDB").getValue(0);
    c = params->getParameter("EL_CC_FIDC").getValue(0);
  } else {
    std::cerr << "[CCFiducialCut::Configure] Problem finidng parameters. " << std::endl; 
  }  
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

ECEdepInnerCut::ECEdepInnerCut(){
    SetName("EC Edep Inner Cut");
}

ECEdepInnerCut::~ECEdepInnerCut(){
}

void ECEdepInnerCut::Configure(Parameters *params){
  if(params->hasParameter("EL_EC_EDEP_MIN")){
    SetMin(params->getParameter("EL_EC_EDEP_MIN").getValue(0));
  } else {
    std::cerr << "[ECEdepInnerCut::Configure] Problem finding parameters. " << std::endl; 
  }
}

bool ECEdepInnerCut::IsPassed(h22Event &event, int index){

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

DCR3FiducialCut::DCR3FiducialCut(){
    angle = 0; height = 0;
    SetName("DC Region 3 Fid Cut");
}


DCR3FiducialCut::~DCR3FiducialCut(){

}

void DCR3FiducialCut::Configure(Parameters *params){
  if(params->hasParameter("EL_DCR3_FIDA") && params->hasParameter("EL_DCR3_FIDH")){
    angle  = params->getParameter("EL_DCR3_FIDA").getValue(0);
    height = params->getParameter("EL_DCR3_FIDH").getValue(0);
  } else {
    std::cerr << "[DCR3FiducialCut::Configure] Problem finding parameters. " << std::endl; 
  }
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

  if(event.GetElectronIndex() > -1 && event.GetElectronIndex() < event.gpart
     && hadronIndex > -1 && hadronIndex < event.gpart){
    double dvz = event.corr_vz[event.GetElectronIndex()]-event.corr_vz[hadronIndex];
    
    if (dvz > GetMin() && dvz < GetMax()){
      n_pass++;
      return true;
    } else {
      n_fail++;
      return false;
    }
    
  } else{
    printf("[DataEventCut_DeltaZVertexCut::IsPassed] Indices out of bounds electron (%d/%d), hadron (%d/%d)", event.GetElectronIndex(), event.gpart, hadronIndex, event.gpart);
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
  DataEventCut_BetaPLikelihood 
*/
///////////////////////////////////////////////////////////////

DataEventCut_BetaPLikelihood::DataEventCut_BetaPLikelihood(int pid) : fPid(pid) {
  SetName(Form("Beta P Likelihood Cut %d", fPid));

  // add possible particle
  AddPossibleParticle(fPid); 

  // by default any track that maximizes the likelihood ratio 
  // will be returned as true 
  fConfidenceMin = 0.0; 
}

DataEventCut_BetaPLikelihood::~DataEventCut_BetaPLikelihood(){
}

bool DataEventCut_BetaPLikelihood::CanBeApplied(h22Event &event, int index){
    return (event.q[index] != 0 && event.dc_sect[index] > 0);
}

void DataEventCut_BetaPLikelihood::AddPossibleParticle(int pid){
  
  // the current particle is not already being considered
  // so we should add it 
  if (fData.count(pid) == 0){
    beta_p_likelihood_data thisData;
    thisData.mass       = pid_to_mass(pid); 
    thisData.confidence = 0.0;

    for(int i=0; i<6; i++){
      for(int j=0; j<3; j++){
	thisData.mu[i][j]    = 0.0; 
	thisData.sigma[i][j] = 0.0; 
      }
    }

    fData[pid] = thisData; 
  } 

  // the current particle is already in consideration 
  else {
    std::cout << "[DataEventCut_BetaPLikelihood::AddPossibleParticle] Trying to add pid = " << pid 
	      << ".  That is already a possible particle (doing nothing). " << std::endl; 
  }

}

void DataEventCut_BetaPLikelihood::Configure(Parameters *params){

  std::map<int, std::string> pidToName; 
  pidToName[211]  = "PIP";
  pidToName[321]  = "KP";
  pidToName[2212] = "PROT";
  pidToName[-211] = "PIM";
  pidToName[-321] = "KM";

  std::vector<std::string> parNames; 
  parNames.push_back("_DBETA_MU_A"); 
  parNames.push_back("_DBETA_MU_B"); 
  parNames.push_back("_DBETA_MU_C"); 
  parNames.push_back("_DBETA_SIGMA_A"); 
  parNames.push_back("_DBETA_SIGMA_B"); 
  parNames.push_back("_DBETA_SIGMA_C"); 

  // doing this so that code doesnt die 
  std::vector<std::string> parsNeeded; 

  for(std::pair<int, beta_p_likelihood_data> particleData : fData){
    int pid = particleData.first; 

    for(std::string parName : parNames){
      std::string currentParameter(Form("%s%s", pidToName[pid].c_str(), parName.c_str()));
      parsNeeded.push_back(currentParameter);
    }
  }

  bool hasAllPars = true; 
  for(std::string par : parsNeeded){
    if(!params->hasParameter(par)){
      std::cout << "[DataEventCut_BetaPLikelihood::Configure] Parameter missing " << par << std::endl;
      hasAllPars = false; 
    } else {
      std::cout << "[DataEventCut_BetaPLikelihood::Configure] Parameter found " << par << std::endl;
    }
  }

  if(hasAllPars){

    for(std::pair<int, beta_p_likelihood_data> particleData : fData){
      int pid = particleData.first; 
      
      for(int isect=0; isect<6; isect++){
	fData[pid].mu[isect][0] = params->getParameter(pidToName[pid]+"_DBETA_MU_C").getValue(isect);
	fData[pid].mu[isect][1] = params->getParameter(pidToName[pid]+"_DBETA_MU_B").getValue(isect);
	fData[pid].mu[isect][2] = params->getParameter(pidToName[pid]+"_DBETA_MU_A").getValue(isect);

	fData[pid].sigma[isect][0] = params->getParameter(pidToName[pid]+"_DBETA_SIGMA_C").getValue(isect);
	fData[pid].sigma[isect][1] = params->getParameter(pidToName[pid]+"_DBETA_SIGMA_B").getValue(isect);
	fData[pid].sigma[isect][2] = params->getParameter(pidToName[pid]+"_DBETA_SIGMA_A").getValue(isect);
      }
    }
  }

  // try to set confidence level if user supplies 
  if (fPid == 211 && params->hasParameter("PIP_CONFIDENCE_LEVEL")){
    fConfidenceMin = params->getParameter("PIP_CONFIDENCE_LEVEL").getValue(0); 
  }
  else if (fPid == 321 && params->hasParameter("KP_CONFIDENCE_LEVEL")){
    fConfidenceMin = params->getParameter("KP_CONFIDENCE_LEVEL").getValue(0); 
  }
  else if (fPid == -321 && params->hasParameter("KM_CONFIDENCE_LEVEL")){
    fConfidenceMin = params->getParameter("KM_CONFIDENCE_LEVEL").getValue(0); 
  }
  else if (fPid == -211 && params->hasParameter("PIM_CONFIDENCE_LEVEL")){
    fConfidenceMin = params->getParameter("PIM_CONFIDENCE_LEVEL").getValue(0); 
  }
  else if (fPid == 2212 && params->hasParameter("PROT_CONFIDENCE_LEVEL")){
    fConfidenceMin = params->getParameter("PROT_CONFIDENCE_LEVEL").getValue(0); 
  }

}

bool DataEventCut_BetaPLikelihood::IsPassed(h22Event &event, int index){

  // normalize the pdf 
  double norm = sqrt(1/2.0/3.14159); 

  // This is used to calculate the total likelihood and 
  // normalize.
  double total = 0.0; 

  // get sector for the correct params 
  int sector = event.dc_sect[index]-1;

  std::map<int, beta_p_likelihood_data> updatedParticleData;
  for(std::pair<int, beta_p_likelihood_data> particleData : fData){
    int currentPid                     = particleData.first; 
    beta_p_likelihood_data currentData = particleData.second; 

    currentData.theory = 1/sqrt(1+pow(currentData.mass/event.p[index],2)); 
    currentData.mean   = currentData.theory 
      + currentData.mu[sector][2]*pow(event.p[index],2) 
      + currentData.mu[sector][1]*event.p[index] 
      + currentData.mu[sector][0];

    currentData.reso = currentData.sigma[sector][2]*pow(event.p[index],2) 
      + currentData.sigma[sector][1]*event.p[index] 
      + currentData.sigma[sector][0];
    
    currentData.residual   = event.corr_b[index]-currentData.mean; 
    currentData.likelihood = norm/currentData.reso * exp(-0.5 * pow(currentData.residual/currentData.reso, 2));
    currentData.confidence = 1.0 - TMath::Erf(fabs(currentData.residual)/currentData.reso/sqrt(2.0));  

    // add to total likelihood 
    total += currentData.likelihood; 

    // save calculations 
    updatedParticleData[currentPid] = currentData; 
  }  

  // normalize and choose max
  //
  for(std::pair<int, beta_p_likelihood_data> particleData : updatedParticleData){
    updatedParticleData[particleData.first].likelihood /= total; 
  }

  int max_index   = updatedParticleData.begin()->first; 
  double max_like = updatedParticleData.begin()->second.likelihood;
 
  for(std::pair<int, beta_p_likelihood_data> particleData : updatedParticleData){
    if (particleData.second.likelihood > max_like){
      max_like  = particleData.second.likelihood;
      max_index = particleData.first;
    }
  }

  // update with all information 
  fData = updatedParticleData; 

  if (fPid == max_index && fData[max_index].confidence > fConfidenceMin){
        n_pass++;
        return true;
    } else {
        n_fail++;
        return false;
    }

    return false;
}

float DataEventCut_BetaPLikelihood::GetConfidence(){
  return fData[fPid].confidence; 
}

float DataEventCut_BetaPLikelihood::GetFractionalDistance(h22Event &event, int index){
  return getFractionalDistance(event.corr_b[index], 0, 1);
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
