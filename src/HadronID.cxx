#ifndef hadron_id_cxx
#define hadron_id_cxx

#include "HadronID.h"

// c++ standard
#include <iostream>
#include <fstream>
#include <cmath>

// root 
#include "TString.h"

// h22 libraries 
#include "h22Event.h"
#include "NathanArchive.h"

namespace Nathan {


  // --------------------------------------------
  //
  // --------------------------------------------
  HadronIDResult::HadronIDResult(){
    protonIndex = -1; 
    pipIndex    = -1; 
    pimIndex    = -1;
  } 
  
  HadronIDResult::~HadronIDResult(){
  } 

  bool HadronIDResult::hasProton(){
    return (protonIndex > -1);
  }

  bool HadronIDResult::hasPip(){
    return (pipIndex > -1);
  }

  bool HadronIDResult::hasPim(){
    return (pimIndex > -1);
  }

  BetaPCutLimits::BetaPCutLimits(){
    min   = 0.0; 
    max   = 1.0; 
    sigma = 0.0; 
  } 
  
  BetaPCutLimits::~BetaPCutLimits(){
  }
    
  BetaPTable::BetaPTable(){
    fPath = ""; 
  }

  BetaPTable::~BetaPTable(){
  }

  void BetaPTable::loadValues(){

  }

  int BetaPTable::getMomentumBin(float p) const {
    int bin = ((p-pMin)/((pMax-pMin)/fNumberBins));
    if(bin < 0) bin = 0;
    if(bin >= fNumberBins) bin = fNumberBins-1;
    return bin; 
  }

  BetaPCutLimits & BetaPTable::getValues(int type, int sect, int bin){
    return fValues[type][sect][bin];
  }

  void PipBetaPTable::loadValues(){

    if ( fPath != "" ){

      float min, max, sigma; 

      for(int type=0; type<2; type++){
	for(int sector=1; sector<7; sector++){
	  for(int bin=0; bin<fNumberBins; bin++){
      
	    std::ifstream infile;
	    infile.open(Form("%s/pipVelocityCuts/t%i_pipVelocityCut_es%i_pBin%i.txt", fPath.c_str(), type, sector, bin)); 
	    if(infile) {
	      infile >> min >> max >> sigma;
	      sigma = fabs(sigma); 
	      
	      BetaPCutLimits limits; 
	      limits.min   = min; 
	      limits.max   = max; 
	      limits.sigma = sigma; 
	      
	      fValues[type][sector-1][bin] = limits; 
	    }
	    infile.close();
	    
	  }
	}
      }
      

    } else {
      std::cerr << "[BetaPTable::loadValues] Path hasn't been set." << std::endl; 
    }

  }
    
  void PimBetaPTable::loadValues(){

    if ( fPath != "" ){

      float min, max, sigma; 

      for(int type=0; type<2; type++){
	for(int sector=1; sector<7; sector++){
	  for(int bin=0; bin<fNumberBins; bin++){
      
	    std::ifstream infile;
	    infile.open(Form("%s/pimVelocityCuts/t%i_pimVelocityCut_es%i_pBin%i.txt", fPath.c_str(), type, sector, bin)); 
	    if(infile) {
	      infile >> min >> max >> sigma;
	      sigma = fabs(sigma); 
	      
	      BetaPCutLimits limits; 
	      limits.min   = min; 
	      limits.max   = max; 
	      limits.sigma = sigma; 
	      
	      fValues[type][sector-1][bin] = limits; 
	    }
	    infile.close();
	    
	  }
	}
      }
      

    } else {
      std::cerr << "[BetaPTable::loadValues] Path hasn't been set." << std::endl; 
    }

  }


  // --------------------------------------------
  //   PipMethods 
  // --------------------------------------------
  PipMethods::PipMethods(PipBetaPTable table) : fTable(table) {
    height[0] =  9; 
    height[1] = 10; 
    height[2] = 11; 
    angle[0]  = 61; 
    angle[1]  = 60; 
    angle[2]  = 59; 
    
    for(int s = 0; s < NSTRICT; s++){
	slope[s] = 1.0/tan(0.5*(3.141592653/180.0)*angle[s]);
      }


    // default strictnesses 
    fStrictFid   = 0; 
    fStrictBetaP = 0; 
  }

  PipMethods::~PipMethods(){

  }
  
  bool PipMethods::MXCut(int strict, float MX){
    if(strict == 9) return 1;
    if(MX > 1.35 + strict*0.025) return 1;
    return 0;
  }

  bool PipMethods::R1Fid(h22Event & event, int index){
    if(fStrictFid == 9) return 1;
    int strictIndex = fStrictFid + 1;

    return (event.GetRotatedDCR1PosX(index) > (height[strictIndex] - slope[strictIndex]*event.GetRotatedDCR1PosY(index)) 
	    && event.GetRotatedDCR1PosX(index) > height[strictIndex] + slope[strictIndex]*event.GetRotatedDCR1PosY(index));
  }

  bool PipMethods::BetaP(h22Event & event, int index, int expOrSim){
    if(fStrictBetaP == 9) return 1;

    int bin = fTable.getMomentumBin(event.p[index]);
    BetaPCutLimits limits = fTable.getValues(expOrSim, event.GetSector(index), bin);

    float Nsig_systematics = 0.25;

    return (event.corr_b[index] > limits.min + fStrictBetaP*Nsig_systematics*limits.sigma  && 
	    event.corr_b[index] < limits.max - fStrictBetaP*Nsig_systematics*limits.sigma);
  }

  // --------------------------------------------
  //   PimMethods 
  // --------------------------------------------
  PimMethods::PimMethods(PimBetaPTable table) : fTable(table) {    
  }

  PimMethods::~PimMethods(){
  }

  // --------------------------------------------
  //
  // --------------------------------------------
  HadronID::HadronID(PipBetaPTable pipTable, PimBetaPTable pimTable){
    fPipMethods = new PipMethods(pipTable); 
    fPimMethods = new PimMethods(pimTable); 
  }

  HadronID::~HadronID(){
  }

  HadronIDResult HadronID::getIDResult(h22Event & event){
    HadronIDResult result; 

    // default values 
    result.protonIndex = -1; 
    result.pipIndex    = -1; 
    result.pimIndex    = -1; 

    // search for pi+ 
    std::vector<int> pips; 
    for (int ipart=0; ipart<event.gpart; ipart++){

      // strictness needs to be added 
      if(event.q[ipart] == 1 && event.sc_sect[ipart] != 0 && event.dc_sect[ipart] != 0 
	 && goodORbadSCpaddle(event.dc_sect[ipart], event.sc_pd[ipart]))
	if (fPipMethods->BetaP(event, ipart, !fGSIM) && 
	    fPipMethods->R1Fid(event, ipart)){
	  pips.push_back(ipart); 
      }

      // if we found some, return fastest 
      if(pips.size() > 0){
	pips = event.SortByMomentum(pips);
	result.pipIndex = pips[0]; 
      }
    }

    // spit back results 
    return result; 
  }

}; 

#endif
