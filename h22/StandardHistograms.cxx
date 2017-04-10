#ifndef standard_histograms_cxx
#define standard_histograms_cxx

#include <iostream>
using std::cout; 
using std::endl; 

// ----------------------------------
//              My 
// ----------------------------------
#include "CommonTools.h"
#include "h22Event.h"
#include "PhysicsEvent.h"
#include "StandardHistograms.h"

// ----------------------------------
//              Root 
// ----------------------------------
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"

StandardHistograms::StandardHistograms(std::string inputName, int runMode) : name(inputName), mode(runMode){

  if(mode == 0){
    Initialize(); 
  } else if (mode == 1){
    inputFile = new TFile();
  } else {
    Color::Modifier red(Color::FG_RED); 
    Color::Modifier def(Color::FG_DEFAULT); 
    cout << red << "[StandardHistograms::StandardHistograms] Mode Unrecognized: " << mode << def << endl; 
  }

}

StandardHistograms::~StandardHistograms(){
  if(inputFile->IsOpen()){
    inputFile->Close(); 
  }
}


void StandardHistograms::Initialize(){
 
 for(int s=0; s<numberSector; s++){
   // Kinematic in 1-Dimension
   h1_ele_p[s]          = new TH1D(Form("h1_ele_p_sect%d_%s",s,name.c_str()),          Form("h1_ele_p_sect%d",s),         100,0.5,5.5); 
   h1_ele_phi[s]        = new TH1D(Form("h1_ele_phi_sect%d_%s",s,name.c_str()),        Form("h1_ele_phi_sect%d",s),       100,-180.0,180.0); 
   h1_ele_relPhi[s]     = new TH1D(Form("h1_ele_relPhi_sect%d_%s",s,name.c_str()),     Form("h1_ele_relPhi_sect%d",s),    100,-30.0, 30.0); 
   h1_ele_theta[s]      = new TH1D(Form("h1_ele_theta_sect%d_%s",s,name.c_str()),      Form("h1_ele_theta_sect%d",s),     100, 10.0, 60.0); 
   h1_part1_p[s]        = new TH1D(Form("h1_part1_p_sect%d_%s",s,name.c_str()),        Form("h1_part1_p_sect%d",s),       100,0.5,5.5); 
   h1_part1_phi[s]      = new TH1D(Form("h1_part1_phi_sect%d_%s",s,name.c_str()),      Form("h1_part1_phi_sect%d",s),     100,-180.0,180.0); 
   h1_part1_relPhi[s]   = new TH1D(Form("h1_part1_relPhi_sect%d_%s",s,name.c_str()),   Form("h1_part1_relPhi_sect%d",s),  100,-30.0, 30.0); 
   h1_part1_theta[s]    = new TH1D(Form("h1_part1_theta_sect%d_%s",s,name.c_str()),    Form("h1_part1_theta_sect%d",s),   100, 0.0, 180.0); 
   h1_xbj[s]            = new TH1D(Form("h1_xbj_sect%d_%s",s,name.c_str()),            Form("h1_xbj_sect%d",s),           100,0.0,1.0); 
   h1_y[s]              = new TH1D(Form("h1_y_sect%d_%s",s,name.c_str()),              Form("h1_y_sect%d",s),             100,0.0,1.0); 
   h1_z[s]              = new TH1D(Form("h1_z_sect%d_%s",s,name.c_str()),              Form("h1_z_sect%d",s),             100,0.0,1.0); 
   h1_pt2[s]            = new TH1D(Form("h1_pt2_sect%d_%s",s,name.c_str()),            Form("h1_pt2_sect%d",s),           100,0.0,2.0); 
   h1_w[s]              = new TH1D(Form("h1_w_sect%d_%s",s,name.c_str()),              Form("h1_w_sect%d",s),             100,0.5,3.5); 
   h1_phiH[s]           = new TH1D(Form("h1_phiH_sect%d_%s",s,name.c_str()),           Form("h1_phiH_sect%d",s),          100,-180.0,180.0); 
   h1_wProtonMass[s]    = new TH1D(Form("h1_wProtonMass_sect%d_%s",s,name.c_str()),    Form("h1_wProtonMass_sect%d",s),   100,0.7,1.2); 
   h1_q2[s]             = new TH1D(Form("h1_q2_sect%d_%s",s,name.c_str()),             Form("h1_q2_sect%d",s),            100,0.5,5.0); 
   h1_nu[s]             = new TH1D(Form("h1_nu_sect%d_%s",s,name.c_str()),             Form("h1_nu_sect%d",s),            100,0.0,5.5); 
   h1_ele_rapidity[s]   = new TH1D(Form("h1_ele_rapidity_sect%d_%s",s,name.c_str()),   Form("h1_ele_rapidity_sect%d",s),  100,-4.0,4.0); 
   h1_part1_rapidity[s] = new TH1D(Form("h1_part1_rapidity_sect%d_%s",s,name.c_str()), Form("h1_part1_rapidity_sect%d",s),100,-4.0,4.0); 

   // Detector Vars in 1-Dimension 
   h1_ele_nphe[s]             = new TH1D(Form("h1_ele_nphe_sect%d_%s",s,name.c_str()),Form("h1_ele_nphe_sect%d",s),100,0,200); 
   h1_ele_ecInner[s]          = new TH1D(Form("h1_ele_ecInner_sect%d_%s",s,name.c_str()),Form("h1_ele_ecInner_sect%d",s),100,0.0,0.5); 
   h1_ele_ecOuter[s]          = new TH1D(Form("h1_ele_ecOuter_sect%d_%s",s,name.c_str()),Form("h1_ele_ecOuter_sect%d",s),100,0.0,0.5); 
   h1_ele_samplingFraction[s] = new TH1D(Form("h1_ele_samplingFraction_sect%d_%s",s,name.c_str()),Form("h1_ele_samplingFraction_sect%d",s),100,0.0,0.5); 
   h1_part1_nphe[s]             = new TH1D(Form("h1_part1_nphe_sect%d_%s",s,name.c_str()),Form("h1_part1_nphe_sect%d",s),100,0,200); 
   h1_part1_ecInner[s]          = new TH1D(Form("h1_part1_ecInner_sect%d_%s",s,name.c_str()),Form("h1_part1_ecInner_sect%d",s),100,0.0,0.5); 
   h1_part1_ecOuter[s]          = new TH1D(Form("h1_part1_ecOuter_sect%d_%s",s,name.c_str()),Form("h1_part1_ecOuter_sect%d",s),100,0.0,0.5); 
   h1_part1_samplingFraction[s] = new TH1D(Form("h1_part1_samplingFraction_sect%d_%s",s,name.c_str()),Form("h1_part1_samplingFraction_sect%d",s),100,0.0,0.5); 

   // Kinematic in 2-Dimensions
   h2_ele_p_theta[s]        = new TH2D(Form("h2_ele_p_theta_sect%d_%s",s,name.c_str()),       Form("h2_ele_p_theta_sect%d",s),       100,0.5,5.5,100,10.0,60.0); 
   h2_ele_phi_theta[s]      = new TH2D(Form("h2_ele_phi_theta_sect%d_%s",s,name.c_str()),     Form("h2_ele_phi_theta_sect%d",s),     100,-180.0,180.0,100,10.0,60.0); 
   h2_ele_relPhi_theta[s]   = new TH2D(Form("h2_ele_relPhi_theta_sect%d_%s",s,name.c_str()),  Form("h2_ele_relPhi_theta_sect%d",s),  100,-30.0,30.0,100,10.0,60.0); 
   h2_part1_p_theta[s]      = new TH2D(Form("h2_part1_p_theta_sect%d_%s",s,name.c_str()),     Form("h2_part1_p_theta_sect%d",s),     100,0.5,5.5,100,10.0,60.0); 
   h2_part1_phi_theta[s]    = new TH2D(Form("h2_part1_phi_theta_sect%d_%s",s,name.c_str()),   Form("h2_part1_phi_theta_sect%d",s),   100,-180.0,180.0,100,0.0,180.0); 
   h2_part1_relPhi_theta[s] = new TH2D(Form("h2_part1_relPhi_theta_sect%d_%s",s,name.c_str()),Form("h2_part1_relPhi_theta_sect%d",s),100,-30.0,30.0,100,0.0,180.0); 
   h2_xbj_q2[s]             = new TH2D(Form("h2_xbj_q2_sect%d_%s",s,name.c_str()),            Form("h2_xbj_q2_sect%d",s),            100,0.0,1.0,100,0.5,5.0); 
   h2_w_q2[s]               = new TH2D(Form("h2_w_q2_sect%d_%s",s,name.c_str()),              Form("h2_w_q2_sect%d",s),              100,0.5,3.5,100,0.5,5.0); 
   h2_y_w[s]                = new TH2D(Form("h2_y_w_sect%d_%s",s,name.c_str()),               Form("h2_y_w_sect%d",s),               100,0.0,1.0,100,0.5,3.5); 
   h2_z_pt2[s]              = new TH2D(Form("h2_z_pt2_sect%d_%s",s,name.c_str()),             Form("h2_z_pt2_sect%d",s),             100,0.0,1.0,100,0.0,2.0); 

   // Detector Vars in 2-Dimensions 
   h2_ele_p_samplingFraction[s]    = new TH2D(Form("h2_ele_p_samplingFraction_sect%d_%s",s,name.c_str()),Form("h2_ele_p_samplingFraction_sect%d",s),100,0.5,5.5,100,0.0,0.5); 
   h2_ele_ecInner_ecOuter[s]       = new TH2D(Form("h2_ele_ecInner_ecOuter_sect%d_%s",s,name.c_str()),Form("h2_ele_ecInner_ecOuter_sect%d",s),100,0.0,0.5,100,0.0,0.5); 
   h2_ele_samplingFraction_nphe[s] = new TH2D(Form("h2_ele_samplingFraction_nphe_sect%d_%s",s,name.c_str()),Form("h2_ele_samplingFraction_nphe_sect%d",s),100,0.0,0.5,100,0,200); 
   h2_part1_p_samplingFraction[s]    = new TH2D(Form("h2_part1_p_samplingFraction_sect%d_%s",s,name.c_str()),Form("h2_part1_p_samplingFraction_sect%d",s),100,0.5,5.5,100,0.0,0.5); 
   h2_part1_ecInner_ecOuter[s]       = new TH2D(Form("h2_part1_ecInner_ecOuter_sect%d_%s",s,name.c_str()),Form("h2_part1_ecInner_ecOuter_sect%d",s),100,0.0,0.5,100,0.0,0.5); 
   h2_part1_samplingFraction_nphe[s] = new TH2D(Form("h2_part1_samplingFraction_nphe_sect%d_%s",s,name.c_str()),Form("h2_part1_samplingFraction_nphe_sect%d",s),100,0.0,0.5,100,0,200); 
 }

} 

void StandardHistograms::Load(std::string inputFilename){
  inputFile = TFile::Open(inputFilename.c_str()); 

  for(int s=0; s<numberSector; s++){
    // Kinematic in 1-Dimension
    h1_ele_p[s]          = (TH1D*) inputFile->Get(Form("h1_ele_p_sect%d_%s",s,name.c_str()));
    h1_ele_phi[s]        = (TH1D*) inputFile->Get(Form("h1_ele_phi_sect%d_%s",s,name.c_str()));
    h1_ele_relPhi[s]     = (TH1D*) inputFile->Get(Form("h1_ele_relPhi_sect%d_%s",s,name.c_str()));
    h1_ele_theta[s]      = (TH1D*) inputFile->Get(Form("h1_ele_theta_sect%d_%s",s,name.c_str()));
    h1_part1_p[s]        = (TH1D*) inputFile->Get(Form("h1_part1_p_sect%d_%s",s,name.c_str()));
    h1_part1_phi[s]      = (TH1D*) inputFile->Get(Form("h1_part1_phi_sect%d_%s",s,name.c_str()));
    h1_part1_relPhi[s]   = (TH1D*) inputFile->Get(Form("h1_part1_relPhi_sect%d_%s",s,name.c_str()));
    h1_part1_theta[s]    = (TH1D*) inputFile->Get(Form("h1_part1_theta_sect%d_%s",s,name.c_str()));
    h1_xbj[s]            = (TH1D*) inputFile->Get(Form("h1_xbj_sect%d_%s",s,name.c_str()));
    h1_y[s]              = (TH1D*) inputFile->Get(Form("h1_y_sect%d_%s",s,name.c_str()));
    h1_z[s]              = (TH1D*) inputFile->Get(Form("h1_z_sect%d_%s",s,name.c_str()));
    h1_pt2[s]            = (TH1D*) inputFile->Get(Form("h1_pt2_sect%d_%s",s,name.c_str()));
    h1_w[s]              = (TH1D*) inputFile->Get(Form("h1_w_sect%d_%s",s,name.c_str()));
    h1_phiH[s]           = (TH1D*) inputFile->Get(Form("h1_phiH_sect%d_%s",s,name.c_str()));
    h1_wProtonMass[s]    = (TH1D*) inputFile->Get(Form("h1_wProtonMass_sect%d_%s",s,name.c_str()));
    h1_q2[s]             = (TH1D*) inputFile->Get(Form("h1_q2_sect%d_%s",s,name.c_str()));
    h1_nu[s]             = (TH1D*) inputFile->Get(Form("h1_nu_sect%d_%s",s,name.c_str()));
    h1_ele_rapidity[s]   = (TH1D*) inputFile->Get(Form("h1_ele_rapidity_sect%d_%s",s,name.c_str()));
    h1_part1_rapidity[s] = (TH1D*) inputFile->Get(Form("h1_part1_rapidity_sect%d_%s",s,name.c_str()));
    
    // Detector Vars in 1-Dimension 
    h1_ele_nphe[s]               = (TH1D*) inputFile->Get(Form("h1_ele_nphe_sect%d_%s",s,name.c_str()));
    h1_ele_ecInner[s]            = (TH1D*) inputFile->Get(Form("h1_ele_ecInner_sect%d_%s",s,name.c_str()));
    h1_ele_ecOuter[s]            = (TH1D*) inputFile->Get(Form("h1_ele_ecOuter_sect%d_%s",s,name.c_str()));
    h1_ele_samplingFraction[s]   = (TH1D*) inputFile->Get(Form("h1_ele_samplingFraction_sect%d_%s",s,name.c_str()));
    h1_part1_nphe[s]             = (TH1D*) inputFile->Get(Form("h1_part1_nphe_sect%d_%s",s,name.c_str()));
    h1_part1_ecInner[s]          = (TH1D*) inputFile->Get(Form("h1_part1_ecInner_sect%d_%s",s,name.c_str()));
    h1_part1_ecOuter[s]          = (TH1D*) inputFile->Get(Form("h1_part1_ecOuter_sect%d_%s",s,name.c_str()));
    h1_part1_samplingFraction[s] = (TH1D*) inputFile->Get(Form("h1_part1_samplingFraction_sect%d_%s",s,name.c_str()));
    
    // Kinematic in 2-Dimensions
    h2_ele_p_theta[s]        = (TH2D*) inputFile->Get(Form("h2_ele_p_theta_sect%d_%s",s,name.c_str()));
    h2_ele_phi_theta[s]      = (TH2D*) inputFile->Get(Form("h2_ele_phi_theta_sect%d_%s",s,name.c_str()));
    h2_ele_relPhi_theta[s]   = (TH2D*) inputFile->Get(Form("h2_ele_relPhi_theta_sect%d_%s",s,name.c_str()));
    h2_part1_p_theta[s]      = (TH2D*) inputFile->Get(Form("h2_part1_p_theta_sect%d_%s",s,name.c_str()));
    h2_part1_phi_theta[s]    = (TH2D*) inputFile->Get(Form("h2_part1_phi_theta_sect%d_%s",s,name.c_str()));
    h2_part1_relPhi_theta[s] = (TH2D*) inputFile->Get(Form("h2_part1_relPhi_theta_sect%d_%s",s,name.c_str()));
    h2_xbj_q2[s]             = (TH2D*) inputFile->Get(Form("h2_xbj_q2_sect%d_%s",s,name.c_str()));
    h2_w_q2[s]               = (TH2D*) inputFile->Get(Form("h2_w_q2_sect%d_%s",s,name.c_str()));
    h2_y_w[s]                = (TH2D*) inputFile->Get(Form("h2_y_w_sect%d_%s",s,name.c_str()));
    h2_z_pt2[s]              = (TH2D*) inputFile->Get(Form("h2_z_pt2_sect%d_%s",s,name.c_str()));
    
    // Detector Vars in 2-Dimensions 
    h2_ele_p_samplingFraction[s]      = (TH2D*) inputFile->Get(Form("h2_ele_p_samplingFraction_sect%d_%s",s,name.c_str()));
    h2_ele_ecInner_ecOuter[s]         = (TH2D*) inputFile->Get(Form("h2_ele_ecInner_ecOuter_sect%d_%s",s,name.c_str()));
    h2_ele_samplingFraction_nphe[s]   = (TH2D*) inputFile->Get(Form("h2_ele_samplingFraction_nphe_sect%d_%s",s,name.c_str()));
    h2_part1_p_samplingFraction[s]    = (TH2D*) inputFile->Get(Form("h2_part1_p_samplingFraction_sect%d_%s",s,name.c_str()));
    h2_part1_ecInner_ecOuter[s]       = (TH2D*) inputFile->Get(Form("h2_part1_ecInner_ecOuter_sect%d_%s",s,name.c_str()));
    h2_part1_samplingFraction_nphe[s] = (TH2D*) inputFile->Get(Form("h2_part1_samplingFraction_nphe_sect%d_%s",s,name.c_str()));
  }
  
}

void StandardHistograms::Fill(h22Event event, int index){
  h1_ele_nphe[0]            ->Fill(event.nphe[index]);
  h1_ele_ecInner[0]         ->Fill(event.ec_ei[index]/event.p[index]);
  h1_ele_ecOuter[0]         ->Fill(event.ec_eo[index]/event.p[index]);
  h1_ele_samplingFraction[0]->Fill(event.etot[index] /event.p[index]);

  h2_ele_p_samplingFraction[0]   ->Fill(event.p[index], event.etot[index]/event.p[index]); 
  h2_ele_ecInner_ecOuter[0]      ->Fill(event.ec_ei[index]/event.p[index], event.ec_eo[index]/event.p[index]); 
  h2_ele_samplingFraction_nphe[0]->Fill(event.etot[index]/event.p[index], event.nphe[index]); 

  int s = event.dc_sect[index]; 
  h1_ele_nphe[s]            ->Fill(event.nphe[index]);
  h1_ele_ecInner[s]         ->Fill(event.ec_ei[index]/event.p[index]);
  h1_ele_ecOuter[s]         ->Fill(event.ec_eo[index]/event.p[index]);
  h1_ele_samplingFraction[s]->Fill(event.etot[index] /event.p[index]);

  h2_ele_p_samplingFraction[s]   ->Fill(event.p[index], event.etot[index]/event.p[index]); 
  h2_ele_ecInner_ecOuter[s]      ->Fill(event.ec_ei[index]/event.p[index], event.ec_eo[index]/event.p[index]); 
  h2_ele_samplingFraction_nphe[s]->Fill(event.etot[index]/event.p[index], event.nphe[index]); 
}

void StandardHistograms::Fill(h22Event event, int index1, int index2){
  h1_ele_nphe[0]              ->Fill(event.nphe[index1]);
  h1_ele_ecInner[0]           ->Fill(event.ec_ei[index1]/event.p[index1]);
  h1_ele_ecOuter[0]           ->Fill(event.ec_eo[index1]/event.p[index1]);
  h1_ele_samplingFraction[0]  ->Fill(event.etot[index1] /event.p[index1]);
  h1_part1_nphe[0]            ->Fill(event.nphe[index2]);
  h1_part1_ecInner[0]         ->Fill(event.ec_ei[index2]/event.p[index2]);
  h1_part1_ecOuter[0]         ->Fill(event.ec_eo[index2]/event.p[index2]);
  h1_part1_samplingFraction[0]->Fill(event.etot[index2] /event.p[index2]);

  h2_ele_p_samplingFraction[0]     ->Fill(event.p[index1], event.etot[index1]/event.p[index1]); 
  h2_ele_ecInner_ecOuter[0]        ->Fill(event.ec_ei[index1]/event.p[index1], event.ec_eo[index1]/event.p[index1]); 
  h2_ele_samplingFraction_nphe[0]  ->Fill(event.etot[index1]/event.p[index1], event.nphe[index1]); 
  h2_part1_p_samplingFraction[0]   ->Fill(event.p[index2], event.etot[index2]/event.p[index2]); 
  h2_part1_ecInner_ecOuter[0]      ->Fill(event.ec_ei[index2]/event.p[index2], event.ec_eo[index2]/event.p[index2]); 
  h2_part1_samplingFraction_nphe[0]->Fill(event.etot[index2]/event.p[index2], event.nphe[index2]); 

  int s  = event.dc_sect[index1]; 
  int s1 = event.dc_sect[index2]; 
  h1_ele_nphe[s]               ->Fill(event.nphe[index1]);
  h1_ele_ecInner[s]            ->Fill(event.ec_ei[index1]/event.p[index1]);
  h1_ele_ecOuter[s]            ->Fill(event.ec_eo[index1]/event.p[index1]);
  h1_ele_samplingFraction[s]   ->Fill(event.etot[index1] /event.p[index1]);
  h1_part1_nphe[s1]            ->Fill(event.nphe[index2]);
  h1_part1_ecInner[s1]         ->Fill(event.ec_ei[index2]/event.p[index2]);
  h1_part1_ecOuter[s1]         ->Fill(event.ec_eo[index2]/event.p[index2]);
  h1_part1_samplingFraction[s1]->Fill(event.etot[index2] /event.p[index2]);

  h2_ele_p_samplingFraction[s]      ->Fill(event.p[index1], event.etot[index1]/event.p[index1]); 
  h2_ele_ecInner_ecOuter[s]         ->Fill(event.ec_ei[index1]/event.p[index1], event.ec_eo[index1]/event.p[index1]); 
  h2_ele_samplingFraction_nphe[s]   ->Fill(event.etot[index1]/event.p[index1], event.nphe[index1]); 
  h2_part1_p_samplingFraction[s1]   ->Fill(event.p[index2], event.etot[index2]/event.p[index2]); 
  h2_part1_ecInner_ecOuter[s1]      ->Fill(event.ec_ei[index2]/event.p[index2], event.ec_eo[index2]/event.p[index2]); 
  h2_part1_samplingFraction_nphe[s1]->Fill(event.etot[index2]/event.p[index2], event.nphe[index2]); 
}

void StandardHistograms::Fill(h22Event event, int index1, int index2, PhysicsEvent physicsEvent){
  Fill(event, index1, index2);
  Fill(physicsEvent);
}

void StandardHistograms::Fill(h22Event event, int index, PhysicsEvent physicsEvent){
  Fill(event,index);
  Fill(physicsEvent);
}

void StandardHistograms::Fill(PhysicsEvent physicsEvent){
  h1_ele_p[0]         ->Fill(physicsEvent.detectedElectron.P()); 
  h1_ele_phi[0]       ->Fill(physicsEvent.detectedElectron.Phi()*to_degrees);
  h1_ele_relPhi[0]    ->Fill(getRelativePhi(physicsEvent.detectedElectron.Phi()*to_degrees)); 
  h1_ele_theta[0]     ->Fill(physicsEvent.detectedElectron.Theta()*to_degrees); 
  h1_part1_p[0]       ->Fill(physicsEvent.particle1.P()); 
  h1_part1_phi[0]     ->Fill(physicsEvent.particle1.Phi()*to_degrees);
  h1_part1_relPhi[0]  ->Fill(getRelativePhi(physicsEvent.particle1.Phi()*to_degrees)); 
  h1_part1_theta[0]   ->Fill(physicsEvent.particle1.Theta()*to_degrees);
  h1_xbj[0]           ->Fill(physicsEvent.x);
  h1_y[0]             ->Fill(physicsEvent.y);
  h1_z[0]             ->Fill(physicsEvent.z);
  h1_pt2[0]           ->Fill(physicsEvent.pT*physicsEvent.pT);
  h1_phiH[0]          ->Fill(physicsEvent.phiHadron);
  h1_w[0]             ->Fill(physicsEvent.w);
  h1_wProtonMass[0]   ->Fill(physicsEvent.w);
  h1_q2[0]            ->Fill(physicsEvent.qq);
  h1_nu[0]            ->Fill(physicsEvent.nu);
  h1_ele_rapidity[0]  ->Fill(physicsEvent.detectedElectron.Rapidity());
  h1_part1_rapidity[0]->Fill(physicsEvent.particle1.Rapidity());
  
  h2_ele_p_theta[0]       ->Fill(physicsEvent.detectedElectron.P(),physicsEvent.detectedElectron.Theta()*to_degrees); 
  h2_ele_phi_theta[0]     ->Fill(physicsEvent.detectedElectron.Phi()*to_degrees,physicsEvent.detectedElectron.Theta()*to_degrees); 
  h2_ele_relPhi_theta[0]  ->Fill(getRelativePhi(physicsEvent.detectedElectron.Phi()*to_degrees),physicsEvent.detectedElectron.Theta()*to_degrees); 
  h2_part1_p_theta[0]     ->Fill(physicsEvent.particle1.P(),physicsEvent.particle1.Theta()*to_degrees); 
  h2_part1_phi_theta[0]   ->Fill(physicsEvent.particle1.Phi()*to_degrees,physicsEvent.particle1.Theta()*to_degrees); 
  h2_part1_relPhi_theta[0]->Fill(getRelativePhi(physicsEvent.particle1.Phi()*to_degrees),physicsEvent.particle1.Theta()*to_degrees); 
  h2_xbj_q2[0]            ->Fill(physicsEvent.x, physicsEvent.qq); 
  h2_w_q2[0]              ->Fill(physicsEvent.w, physicsEvent.qq); 
  h2_y_w[0]               ->Fill(physicsEvent.y, physicsEvent.w);
  h2_z_pt2[0]             ->Fill(physicsEvent.z, physicsEvent.pT*physicsEvent.pT); 

  int s  = phiToSector(physicsEvent.detectedElectron.Phi()*to_degrees); 
  int s1 = phiToSector(physicsEvent.particle1.Phi()*to_degrees); 

  h1_ele_p[s]          ->Fill(physicsEvent.detectedElectron.P()); 
  h1_ele_phi[s]        ->Fill(physicsEvent.detectedElectron.Phi()*to_degrees);
  h1_ele_relPhi[s]     ->Fill(getRelativePhi(physicsEvent.detectedElectron.Phi()*to_degrees)); 
  h1_ele_theta[s]      ->Fill(physicsEvent.detectedElectron.Theta()*to_degrees); 
  h1_part1_p[s1]       ->Fill(physicsEvent.particle1.P()); 
  h1_part1_phi[s1]     ->Fill(physicsEvent.particle1.Phi()*to_degrees);
  h1_part1_relPhi[s1]  ->Fill(getRelativePhi(physicsEvent.particle1.Phi()*to_degrees)); 
  h1_part1_theta[s1]   ->Fill(physicsEvent.particle1.Theta()*to_degrees);
  h1_xbj[s]            ->Fill(physicsEvent.x);
  h1_y[s]              ->Fill(physicsEvent.y);
  h1_w[s]              ->Fill(physicsEvent.w);
  h1_z[s]              ->Fill(physicsEvent.z);
  h1_pt2[s]            ->Fill(physicsEvent.pT*physicsEvent.pT);
  h1_phiH[s]           ->Fill(physicsEvent.phiHadron);
  h1_wProtonMass[s]    ->Fill(physicsEvent.w);
  h1_q2[s]             ->Fill(physicsEvent.qq);
  h1_nu[s]             ->Fill(physicsEvent.nu);
  h1_ele_rapidity[s]   ->Fill(physicsEvent.detectedElectron.Rapidity());
  h1_part1_rapidity[s1]->Fill(physicsEvent.particle1.Rapidity());
  
  h2_ele_p_theta[s]       ->Fill(physicsEvent.detectedElectron.P(),physicsEvent.detectedElectron.Theta()*to_degrees); 
  h2_ele_phi_theta[s]     ->Fill(physicsEvent.detectedElectron.Phi()*to_degrees,physicsEvent.detectedElectron.Theta()*to_degrees); 
  h2_ele_relPhi_theta[s]  ->Fill(getRelativePhi(physicsEvent.detectedElectron.Phi()*to_degrees),physicsEvent.detectedElectron.Theta()*to_degrees); 
  h2_part1_p_theta[s1]     ->Fill(physicsEvent.particle1.P(),physicsEvent.particle1.Theta()*to_degrees); 
  h2_part1_phi_theta[s1]   ->Fill(physicsEvent.particle1.Phi()*to_degrees,physicsEvent.particle1.Theta()*to_degrees); 
  h2_part1_relPhi_theta[s1]->Fill(getRelativePhi(physicsEvent.particle1.Phi()*to_degrees),physicsEvent.particle1.Theta()*to_degrees); 
  h2_xbj_q2[s]             ->Fill(physicsEvent.x, physicsEvent.qq); 
  h2_w_q2[s]               ->Fill(physicsEvent.w, physicsEvent.qq); 
  h2_y_w[s]                ->Fill(physicsEvent.y, physicsEvent.w); 
  h2_z_pt2[s]              ->Fill(physicsEvent.z, physicsEvent.pT*physicsEvent.pT); 
}

void StandardHistograms::Save(TFile *outputFile){
  if(outputFile->IsOpen()){
    outputFile->cd(); 
 
    for(int s=0; s<numberSector; s++){
      h1_ele_p[s]                 ->Write(); 
      h1_ele_phi[s]               ->Write(); 
      h1_ele_relPhi[s]            ->Write(); 
      h1_ele_theta[s]             ->Write(); 
      h1_part1_p[s]               ->Write(); 
      h1_part1_phi[s]             ->Write(); 
      h1_part1_relPhi[s]          ->Write(); 
      h1_part1_theta[s]           ->Write(); 
      h1_xbj[s]                   ->Write(); 
      h1_y[s]                     ->Write(); 
      h1_w[s]                     ->Write(); 
      h1_z[s]                     ->Write(); 
      h1_pt2[s]                   ->Write(); 
      h1_phiH[s]                  ->Write(); 
      h1_wProtonMass[s]           ->Write(); 
      h1_q2[s]                    ->Write(); 
      h1_nu[s]                    ->Write(); 
      h1_ele_rapidity[s]          ->Write(); 
      h1_part1_rapidity[s]        ->Write(); 
      h1_ele_nphe[s]              ->Write(); 
      h1_ele_ecInner[s]           ->Write(); 
      h1_ele_ecOuter[s]           ->Write(); 
      h1_ele_samplingFraction[s]  ->Write(); 
      h1_part1_nphe[s]            ->Write(); 
      h1_part1_ecInner[s]         ->Write(); 
      h1_part1_ecOuter[s]         ->Write(); 
      h1_part1_samplingFraction[s]->Write(); 

      h2_ele_p_theta[s]                ->Write(); 
      h2_ele_phi_theta[s]              ->Write(); 
      h2_ele_relPhi_theta[s]           ->Write(); 
      h2_part1_p_theta[s]              ->Write(); 
      h2_part1_phi_theta[s]            ->Write(); 
      h2_part1_relPhi_theta[s]         ->Write(); 
      h2_xbj_q2[s]                     ->Write(); 
      h2_w_q2[s]                       ->Write(); 
      h2_y_w[s]                        ->Write(); 
      h2_z_pt2[s]                      ->Write(); 
      h2_ele_p_samplingFraction[s]     ->Write(); 
      h2_ele_ecInner_ecOuter[s]        ->Write(); 
      h2_ele_samplingFraction_nphe[s]  ->Write(); 
      h2_part1_p_samplingFraction[s]   ->Write(); 
      h2_part1_ecInner_ecOuter[s]      ->Write(); 
      h2_part1_samplingFraction_nphe[s]->Write(); 
    }
  } else {
    Color::Modifier red(Color::FG_RED); 
    Color::Modifier def(Color::FG_DEFAULT); 
    cout << red << "[StandardHistograms::Save] Output not open " << def << endl; 
  }
} 

#endif
