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
  const static int NSECT = 7; 
  int              mode; 
  std::string      name; 
  TFile           *inputFile; 

  // Kinematic Histograms in 1-Dimension 
  TH1D *h1_ele_p[NSECT],   *h1_ele_phi[NSECT],   *h1_ele_relPhi[NSECT],   *h1_ele_theta[NSECT];
  TH1D *h1_part1_p[NSECT], *h1_part1_phi[NSECT], *h1_part1_relPhi[NSECT], *h1_part1_theta[NSECT];
  TH1D *h1_xbj[NSECT],     *h1_y[NSECT],         *h1_w[NSECT],            *h1_wProtonMass[NSECT],  *h1_q2[NSECT]; 
  TH1D *h1_nu[NSECT],      *h1_ele_rapidity[NSECT], *h1_part1_rapidity[NSECT]; 
  TH1D *h1_z[NSECT],       *h1_pt2[NSECT],          *h1_phiH[NSECT],  *h1_mm2[NSECT];

  // Kinematic Histograms in 2-Dimensions 
  TH2D *h2_ele_p_theta[NSECT],   *h2_ele_phi_theta[NSECT],   *h2_ele_relPhi_theta[NSECT]; 
  TH2D *h2_part1_p_theta[NSECT], *h2_part1_phi_theta[NSECT], *h2_part1_relPhi_theta[NSECT]; 
  TH2D *h2_xbj_q2[NSECT],        *h2_w_q2[NSECT],            *h2_y_w[NSECT], *h2_z_pt2[NSECT]; 
  
  // Detector Variables in 1-Dimension  
  TH1D *h1_ele_nphe[NSECT],   *h1_ele_ecInner[NSECT],   *h1_ele_ecOuter[NSECT],   *h1_ele_samplingFraction[NSECT]; 
  TH1D *h1_part1_nphe[NSECT], *h1_part1_ecInner[NSECT], *h1_part1_ecOuter[NSECT], *h1_part1_samplingFraction[NSECT]; 

  // Detector Variables in 2-Dimensions  
  TH2D *h2_ele_p_samplingFraction[NSECT],   *h2_ele_ecInner_ecOuter[NSECT],   *h2_ele_samplingFraction_nphe[NSECT]; 
  TH2D *h2_part1_p_samplingFraction[NSECT], *h2_part1_ecInner_ecOuter[NSECT], *h2_part1_samplingFraction_nphe[NSECT]; 
  TH2D *h2_ele_vz_phi[NSECT], *h2_part1_vz_phi[NSECT];

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
