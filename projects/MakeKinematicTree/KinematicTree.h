#ifndef kinematic_tree_h
#define kinematic_tree_h

#include <iostream>
#include <fstream>

#include "Corrections.h"
#include "h22Option.h"
#include "GenericAnalysis.h"
#include "Parameters.h"
#include "ParticleFilter.h"
#include "PhysicsEventBuilder.h"

#include "TBranch.h"
#include "TLorentzVector.h"
#include "TTree.h"

class KinematicTree : public GenericAnalysis {
 public:
  KinematicTree(h22Options *opts, Parameters *p, Parameters *ptight, Parameters *ploose);
  ~KinematicTree();

  // Structure of output tree
  TTree   *kinematicTree;
  TBranch *b_qq;
  TBranch *b_x;
  TBranch *b_y;
  TBranch *b_z;
  TBranch *b_w;
  TBranch *b_hel;
  TBranch *b_meson_id;
  TBranch *b_tof_mass; 
  TBranch *b_pt;
  TBranch *b_phiH;
  TBranch *b_pass_sf;
  TBranch *b_pass_dc1;
  TBranch *b_pass_dc3;
  TBranch *b_pass_vz;
  TBranch *b_pass_ec;
  TBranch *b_ele_vz;
  TBranch *b_mes_vz;

  Double_t qq; 
  Double_t x; 
  Double_t w; 
  Double_t y; 
  Double_t z; 
  Double_t pt; 
  Double_t ele_vz; 
  Double_t mes_vz; 
  Double_t phiH;
  Double_t tof_mass; 
  Int_t    meson_id; 
  Int_t    hel; 
  Int_t    pass_sf; 
  Int_t    pass_dc1; 
  Int_t    pass_dc3; 
  Int_t    pass_vz; 
  Int_t    pass_ec; 

  PhysicsEventBuilder builder;

 public:
  void SetupTree();
  void ProcessEvent();
  void Save(std::string outputFilename);
  void CheckForMesonAndFill(int index); 

 protected:
  std::ofstream   outfile; 
  Corrections     fCorr; 
  ParticleFilter *fFilter; 
  ParticleFilter *fFilterTight; 
  ParticleFilter *fFilterLoose;
 
  Parameters     *fPars; 

};

#endif
