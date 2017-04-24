#ifndef photon_histos_h
#define photon_histos_h

#include <iostream>

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"

#include "h22Event.h"

class PhotonHistograms {
 public:
  PhotonHistograms(std::string name);
  
  ~PhotonHistograms(){ 
    if(fInputFile){ 
      if (fInputFile->IsOpen())
	fInputFile->Close(); 
    }
  }

  // h1_xname
  TH1D *h1_p[7];
  TH1D *h1_b[7];
  TH1D *h1_tofmass[7];
  TH1D *h1_ecx[7];
  TH1D *h1_ecy[7];
  TH1D *h1_ecu[7];
  TH1D *h1_ecv[7];
  TH1D *h1_ecw[7];
  TH1D *h1_edep[7];
  TH1D *h1_dbeta[7];
  TH1D *h1_vz[7];
  TH1D *h1_dvz[7];
  TH1D *h1_theta[7];
  TH1D *h1_dtheta[7];
  TH1D *h1_phi[7];

  // h2_xname_yname
  TH2D *h2_phi_theta[7];
  TH2D *h2_ecx_ecy[7];
  TH2D *h2_p_beta[7];
  TH2D *h2_p_dbeta[7];
  TH2D *h2_p_tofmass[7];

  void Fill(h22Event &event, int index);
  void Load(std::string filename);
  void Save(TFile *outputFile);
  
 protected:
  TFile       *fInputFile; 
  std::string  fName; 

};

#endif
