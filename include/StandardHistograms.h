#ifndef standard_histograms_h
#define standard_histograms_h

#include <iostream>

// ----------------------------------
//              My 
// ----------------------------------
#include "PhysicsEvent.h"
#include "h22Event.h"

// ----------------------------------
//              Root 
// ----------------------------------
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"

class StandardHistograms{
 public:
  StandardHistograms(std::string inputName, int runMode); 
  ~StandardHistograms(); 

  // 1 for All 
  const static int numberSector = 7; 
  int              mode; 
  std::string      name; 
  TFile           *inputFile; 

  // Kinematic Histograms in 1-Dimension 
  TH1D *h1_ele_p[numberSector],   *h1_ele_phi[numberSector],   *h1_ele_relPhi[numberSector],   *h1_ele_theta[numberSector];
  TH1D *h1_part1_p[numberSector], *h1_part1_phi[numberSector], *h1_part1_relPhi[numberSector], *h1_part1_theta[numberSector];
  TH1D *h1_xbj[numberSector],     *h1_y[numberSector],         *h1_w[numberSector],            *h1_wProtonMass[numberSector],  *h1_q2[numberSector]; 
  TH1D *h1_nu[numberSector],      *h1_ele_rapidity[numberSector], *h1_part1_rapidity[numberSector]; 
  TH1D *h1_z[numberSector],       *h1_pt2[numberSector],          *h1_phiH[numberSector],  *h1_mm2[numberSector];

  // Kinematic Histograms in 2-Dimensions 
  TH2D *h2_ele_p_theta[numberSector],   *h2_ele_phi_theta[numberSector],   *h2_ele_relPhi_theta[numberSector]; 
  TH2D *h2_part1_p_theta[numberSector], *h2_part1_phi_theta[numberSector], *h2_part1_relPhi_theta[numberSector]; 
  TH2D *h2_xbj_q2[numberSector],        *h2_w_q2[numberSector],            *h2_y_w[numberSector], *h2_z_pt2[numberSector]; 
  
  // Detector Variables in 1-Dimension  
  TH1D *h1_ele_nphe[numberSector],   *h1_ele_ecInner[numberSector],   *h1_ele_ecOuter[numberSector],   *h1_ele_samplingFraction[numberSector]; 
  TH1D *h1_part1_nphe[numberSector], *h1_part1_ecInner[numberSector], *h1_part1_ecOuter[numberSector], *h1_part1_samplingFraction[numberSector]; 

  // Detector Variables in 2-Dimensions  
  TH2D *h2_ele_p_samplingFraction[numberSector],   *h2_ele_ecInner_ecOuter[numberSector],   *h2_ele_samplingFraction_nphe[numberSector]; 
  TH2D *h2_part1_p_samplingFraction[numberSector], *h2_part1_ecInner_ecOuter[numberSector], *h2_part1_samplingFraction_nphe[numberSector]; 
  TH2D *h2_ele_vz_phi[numberSector], *h2_part1_vz_phi[numberSector];

  // Member Functions
  void Fill(h22Event &event, int index);
  void Fill(h22Event &event, int index, PhysicsEvent &physicsEvent);
  void Fill(h22Event &event, int index1, int index2);
  void Fill(h22Event &event, int index1, int index2, PhysicsEvent &physicsEvent);
  void Fill(PhysicsEvent &physicsEvent);
  void Load(std::string inputFilename); 
  void Save(TFile *outputFile); 

 protected:
  void Initialize(); 


};


#endif
