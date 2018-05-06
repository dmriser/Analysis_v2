#ifndef corr_histos_h
#define corr_histos_h

#include <iostream>

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"

#include "h22Event.h"
#include "PhysicsEvent.h"

class CorrectionHistograms {
 public:
  CorrectionHistograms(std::string name);
  
  ~CorrectionHistograms(){ 
    if(fInputFile){ 
      if (fInputFile->IsOpen())
	fInputFile->Close(); 
    }
  }

  const static int NPADDLES = 50; 

  // timing 
  TH2D *h2_p_dbeta[2][NPADDLES][7];

  // vertex 
  TH1D *h1_vz[2][7];
  TH2D *h2_vz_phi[2][7];

  // momentum 
  TH2D *h2_theta_dw[2][7];
  TH2D *h2_phi_dw[2][7];

  void FillUncorrected(h22Event &event, PhysicsEvent &physicsEvent, int index);
  void FillCorrected(h22Event &event, PhysicsEvent &physicsEvent, int index);
  void Load(std::string filename);
  void Save(TFile *outputFile);
  
 protected:
  TFile       *fInputFile; 
  std::string  fName; 

};

#endif
