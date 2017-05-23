#ifndef kinematic_tree_h
#define kinematic_tree_h

#include <iostream>

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
 KinematicTree(h22Options *opts, Parameters *p);
  ~KinematicTree();

  // Structure of output tree
  TTree   *kinematicTree;
  TBranch *b_qq;
  TBranch *b_x;
  TBranch *b_y;
  TBranch *b_w;
  TBranch *b_nu;
  TBranch *b_phiH;
  TBranch *b_electron;
  TBranch *b_meson;

  Double_t qq; 
  Double_t x; 
  Double_t y; 
  Double_t w; 
  Double_t nu;
  Double_t phiH;
  TLorentzVector electron; 
  TLorentzVector meson; 

  bool isMonteCarlo;

  PhysicsEventBuilder builder;

 public:
  void SetupTree();
  void ProcessEvent();
  void Save(std::string outputFilename);

 protected:
  ParticleFilter *fFilter; 
  Parameters     *fPars; 

};

#endif
