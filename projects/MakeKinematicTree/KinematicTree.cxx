#ifndef kinematic_tree_cxx
#define kinematic_tree_cxx

#include <iostream>
#include <fstream>
#include <map>
using std::cout; 
using std::endl; 


#include "KinematicTree.h"

#include "Corrections.h"
#include "PhysicsEvent.h"
#include "PhysicsEventBuilder.h"

#include "TTree.h"
#include "TBranch.h"

KinematicTree::KinematicTree(h22Options *opts, Parameters *p, Parameters *ptight, Parameters *ploose) : GenericAnalysis(opts), fPars(p) {
  fFilter      = new ParticleFilter(fPars); 
  fFilterLoose = new ParticleFilter(ploose); 
  fFilterTight = new ParticleFilter(ptight); 

  outfile.open("out.csv",std::ios::trunc);
}

KinematicTree::~KinematicTree(){
}

void KinematicTree::SetupTree(){

  // Create the structure of our kinematic tree 
  // and set the memory addresses of local vars.
  kinematicTree = new TTree("miniTree","Mininal Tree");
  kinematicTree->SetDirectory(0);

  b_qq        = kinematicTree->Branch("qq",        &qq);
  b_x         = kinematicTree->Branch("x",         &x);
  b_x         = kinematicTree->Branch("w",         &w);
  b_y         = kinematicTree->Branch("y",         &y);
  b_z         = kinematicTree->Branch("z",         &z);
  b_mm2       = kinematicTree->Branch("mm2",       &mm2);
  b_hel       = kinematicTree->Branch("hel",       &hel);
  b_pt        = kinematicTree->Branch("pt",        &pt);
  b_ele_vz    = kinematicTree->Branch("ele_vz",    &ele_vz);
  b_mes_vz    = kinematicTree->Branch("mes_vz",    &mes_vz);
  b_ele_p     = kinematicTree->Branch("ele_p",    &ele_p);
  b_mes_p     = kinematicTree->Branch("mes_p",    &mes_p);
  b_pass_sf   = kinematicTree->Branch("pass_sf",   &pass_sf);
  b_pass_dc1  = kinematicTree->Branch("pass_dc1",  &pass_dc1);
  b_pass_dc3  = kinematicTree->Branch("pass_dc3",  &pass_dc3);
  b_pass_vz   = kinematicTree->Branch("pass_vz",   &pass_vz);
  b_pass_ec   = kinematicTree->Branch("pass_ec",   &pass_ec);
  b_pt        = kinematicTree->Branch("pt",        &pt);
  b_meson_id  = kinematicTree->Branch("meson_id",  &meson_id);
  b_tof_mass  = kinematicTree->Branch("tof_mass",  &tof_mass);
  b_phiH      = kinematicTree->Branch("phi_h",     &phiH);
}

void KinematicTree::CheckForMesonAndFill(int index){
  
  std::vector<int> mesons = fFilter->getVectorOfParticleIndices(event, index); 

  if (mesons.size() > 0){    
    int electronIndex = event.GetElectronIndex(); 
    int mesonIndex    = mesons[0];

      TLorentzVector electron = event.GetTLorentzVector(electronIndex, 11); 
      TLorentzVector meson    = event.GetTLorentzVector(mesonIndex,  index); 
      PhysicsEvent ev         = builder.getPhysicsEvent(electron, meson);

      hel = event.corr_hel;
      meson_id = index; 

      ele_vz = event.corr_vz[electronIndex]; 
      mes_vz = event.corr_vz[mesonIndex]; 

      // this needs to be replaced 
      ele_p = event.p[electronIndex]; 
      mes_p = event.p[mesonIndex];

      x    = ev.x; 
      w    = ev.w; 
      y    = ev.y; 
      z    = ev.z; 
      qq   = ev.qq; 
      mm2  = ev.mm2; 
      phiH = ev.phiHadron; 
      pt   = ev.pT; 
      tof_mass = event.p[mesons[0]] * sqrt((1-pow(event.corr_b[mesons[0]],2))/pow(event.corr_b[mesons[0]],2)); 
      
      outfile << meson_id << ","; 
      outfile << hel << ","; 
      outfile << x << ","; 
      outfile << y << ","; 
      outfile << z << ","; 
      outfile << w << ","; 
      outfile << mm2 << ","; 
      outfile << pt << ","; 
      outfile << qq << ","; 
      outfile << phiH << ","; 
      outfile << tof_mass << ","; 
      outfile << ele_vz << ","; 
      outfile << mes_vz << ","; 
      outfile << ele_p << ","; 
      outfile << mes_p << ","; 
      outfile << pass_sf << ","; 
      outfile << pass_dc1 << ","; 
      outfile << pass_dc3 << ","; 
      outfile << pass_vz << ","; 
      outfile << pass_ec << std::endl; 

      kinematicTree->Fill(); 
    }
}

void KinematicTree::ProcessEvent(){

  std::vector<int> electrons = fFilterLoose->getVectorOfParticleIndices(event, 11); 
  
  if (!electrons.empty()){
    event.SetElectronIndex(electrons[0]);
    fCorr.correctEvent(&event, GetRunNumber(), GSIM);


    std::map<std::string, bool> results_nom   = fFilter     ->eid_map(event, electrons[0]); 
    std::map<std::string, bool> results_tight = fFilterTight->eid_map(event, electrons[0]); 
    
    // loose is always passed by this point 
    pass_sf  = -1; 
    pass_dc1 = -1; 
    pass_dc3 = -1; 
    pass_vz  = -1; 
    pass_ec  = -1; 

    if (results_nom["EC_SAMPLING"])  { pass_sf = 0; }
    if (results_tight["EC_SAMPLING"]){ pass_sf = 1; }

    if (results_nom["DC_R1_FID"])  { pass_dc1 = 0; }
    if (results_tight["DC_R1_FID"]){ pass_dc1 = 1; }

    if (results_nom["DC_R3_FID"])  { pass_dc3 = 0; }
    if (results_tight["DC_R3_FID"]){ pass_dc3 = 1; }

    if (results_nom["EC_FID"])  { pass_ec = 0; }
    if (results_tight["EC_FID"]){ pass_ec = 1; }

    if (results_nom["Z_VERTEX"])  { pass_vz = 0; }
    if (results_tight["Z_VERTEX"]){ pass_vz = 1; }
    
    CheckForMesonAndFill(211); 
    CheckForMesonAndFill(-211); 
    CheckForMesonAndFill(321); 
    CheckForMesonAndFill(-321); 
  }

}

void KinematicTree::Save(string outputFilename){

  TFile *outputFile = new TFile(outputFilename.c_str(), "recreate");
  kinematicTree->Write(); 
  outputFile   ->Close();

  outfile.close(); 
}

#endif
