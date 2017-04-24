#ifndef meson_histos_h
#define meson_histos_h

#include <iostream>

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"

#include "h22Event.h"
#include "PhysicsEvent.h"

class MesonHistograms {
 public:
  MesonHistograms(std::string name, int pid);
  
  ~MesonHistograms(){ 
    if(fInputFile){ 
      if (fInputFile->IsOpen())
	fInputFile->Close(); 
    }
  }

  // h1_xname
  TH1D *h1_p[7];
  TH1D *h1_b[7];
  TH1D *h1_tofmass[7];
  TH1D *h1_dcx[7];
  TH1D *h1_dcy[7];
  TH1D *h1_edep[7];
  TH1D *h1_dbeta[7];
  TH1D *h1_vz[7];
  TH1D *h1_dvz[7];
  TH1D *h1_theta[7];
  TH1D *h1_dtheta[7];
  TH1D *h1_phi[7];

  // h2_xname_yname
  TH2D *h2_phi_theta[7];
  TH2D *h2_dcx_dcy[7];
  TH2D *h2_p_beta[7];
  TH2D *h2_p_dbeta[7];
  TH2D *h2_p_tofmass[7];
  TH2D *h2_nphe_tofmass[7];
  TH2D *h2_p_mm2[7];

  void Fill(h22Event &event, int index);
  void Fill(PhysicsEvent &physicsEvent);
  void Fill(h22Event &event, PhysicsEvent &physicsEvent, int index);
  void Load(std::string filename);
  void Save(TFile *outputFile);
  
  int GetPid() const {
    return fPid;
  }
  
 protected:
  TFile       *fInputFile; 
  std::string  fName; 
  int          fPid; 

};

#endif
