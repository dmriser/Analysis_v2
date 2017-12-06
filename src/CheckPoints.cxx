#ifndef checkpoints_cxx
#define checkpoints_cxx

// standard
#include <iostream>
#include <map>

// root 
#include "TFile.h"
#include "TH1.h"

// this 
#include "CheckPoints.h"

CheckPoints::CheckPoints(){
  fCurrentAxis  = 0; 
  fNumberOfAxes = 0; 
} 

CheckPoints::~CheckPoints(){

}

void CheckPoints::Reset(){
  fCurrentAxis = 0; 
}

void CheckPoints::AddCheckPoint(std::string name){
  CheckPoint cp; 
  cp.name   = name; 
  cp.counts = 0; 

  fCheckPoints[fNumberOfAxes] = cp; 
  fNumberOfAxes++; 
}

void CheckPoints::CheckIn(){
  fCheckPoints[fCurrentAxis].counts++; 
  fCurrentAxis++; 

  // reset on last checkpoint 
  if(fCurrentAxis == fNumberOfAxes){
    fCurrentAxis = 0; 
  }
}

void CheckPoints::PrintSummary(){
  for(std::pair<int,CheckPoint> cp : fCheckPoints){
    std::cout << cp.second.name << " " << cp.second.counts << std::endl; 
  }
}

void CheckPoints::Save(TFile *out){
  if(out->IsOpen()){
    TH1I *h = new TH1I("checkPoints","checkPoints",fNumberOfAxes,1,fNumberOfAxes); 

    for(int axis=0; axis<fNumberOfAxes; axis++){
      h->SetBinContent(axis+1, fCheckPoints[axis].counts); 
    }

    out->cd("/"); 
    h->Write(); 
  } else {
    std::cerr << "[CheckPoints::Save] Trouble accessing file. " << std::endl; 
  }
}

#endif 
