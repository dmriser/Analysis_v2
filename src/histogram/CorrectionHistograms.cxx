#ifndef corr_histos_cxx
#define corr_histos_cxx

#include <iostream>

#include "CorrectionHistograms.h"
#include "h22Event.h"
#include "PhysicsEvent.h"

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"

CorrectionHistograms::CorrectionHistograms(std::string name) : fName(name) {

  for(int s=0; s<7; s++){

    // timing 
    for (int p=0; p<NPADDLES; p++){
      h2_p_dbeta[0][p][s] = new TH2D(Form("h2_p_dbeta_uncorr_%d_%d_%s", p, s, fName.c_str()), 
				     Form("h2_p_dbeta_uncorr_%d_%d_%s", p, s, fName.c_str()), 
				     200,  0.0, 4.5, 
				     200, -0.4, 0.4);

      h2_p_dbeta[1][p][s] = new TH2D(Form("h2_p_dbeta_corr_%d_%d_%s", p, s, fName.c_str()), 
				     Form("h2_p_dbeta_corr_%d_%d_%s", p, s, fName.c_str()), 
				     200,  0.0, 4.5, 
				     200, -0.4, 0.4);
    }

    // vertex 
    h1_vz[0][s] = new TH1D(Form("h1_vz_uncorr_%d_%s", s, fName.c_str()), 
			   Form("h1_vz_uncorr_%d_%s", s, fName.c_str()), 
			   200, -30, -15); 
    h1_vz[1][s] = new TH1D(Form("h1_vz_corr_%d_%s", s, fName.c_str()), 
			   Form("h1_vz_corr_%d_%s", s, fName.c_str()), 
			   200, -30, -15); 
    h2_vz_phi[0][s] = new TH2D(Form("h2_vz_phi_uncorr_%d_%s", s, fName.c_str()), 
			       Form("h2_vz_phi_uncorr_%d_%s", s, fName.c_str()), 
			       200, -30, -15, 
			       200, -30, 360); 
    h2_vz_phi[1][s] = new TH2D(Form("h2_vz_phi_corr_%d_%s", s, fName.c_str()), 
			       Form("h2_vz_phi_corr_%d_%s", s, fName.c_str()), 
			       200, -30, -15, 
			       200, -30, 360); 

    // momentum 
    h2_theta_dw[0][s] = new TH2D(Form("h2_theta_dw_uncorr_%d_%s", s, fName.c_str()), 
				 Form("h2_theta_dw_uncorr_%d_%s", s, fName.c_str()), 
				 200, 5.0, 30.0, 
				 200, -0.2, 0.2);
    h2_theta_dw[1][s] = new TH2D(Form("h2_theta_dw_corr_%d_%s", s, fName.c_str()), 
				 Form("h2_theta_dw_corr_%d_%s", s, fName.c_str()), 
				 200,  5.0, 30.0, 
				 200, -0.2, 0.2);
    h2_phi_dw[0][s] = new TH2D(Form("h2_phi_dw_uncorr_%d_%s", s, fName.c_str()), 
				 Form("h2_phi_dw_uncorr_%d_%s", s, fName.c_str()), 
				 200, -30.0, 30.0, 
				 200, -0.2, 0.2);
    h2_phi_dw[1][s] = new TH2D(Form("h2_phi_dw_corr_%d_%s", s, fName.c_str()), 
				 Form("h2_phi_dw_corr_%d_%s", s, fName.c_str()), 
				 200, -30.0, 30.0, 
				 200, -0.2, 0.2);
  }
}
 
void CorrectionHistograms::FillCorrected(h22Event &event, PhysicsEvent &physicsEvent, int index) {

}

void CorrectionHistograms::FillUncorrected(h22Event &event, PhysicsEvent &physicsEvent, int index) {
  
}

void CorrectionHistograms::Load(std::string filename) {
  fInputFile = TFile::Open(filename.c_str());

  if (fInputFile->IsOpen()){
    for (int s=0; s<7; s++){
      for(int p=0; p<NPADDLES; p++){
	h2_p_dbeta[0][p][s] = (TH2D*) fInputFile->Get(Form("h2_p_dbeta_uncorr_%d_%d_%s", p, s, fName.c_str()));;
	h2_p_dbeta[1][p][s] = (TH2D*) fInputFile->Get(Form("h2_p_dbeta_corr_%d_%d_%s", p, s, fName.c_str()));;
      }
      
      // vertex
      h1_vz[0][s] = (TH1D*) fInputFile->Get(Form("h1_vz_uncorr_%d_%s", s, fName.c_str()));
      h1_vz[1][s] = (TH1D*) fInputFile->Get(Form("h1_vz_corr_%d_%s", s, fName.c_str()));
      h2_vz_phi[0][s] = (TH2D*) fInputFile->Get(Form("h2_vz_phi_uncorr_%d_%s", s, fName.c_str()));
      h2_vz_phi[1][s] = (TH2D*) fInputFile->Get(Form("h2_vz_phi_corr_%d_%s", s, fName.c_str()));

      // momentum
      h2_theta_dw[0][s] = (TH2D*) (Form("h2_theta_dw_uncorr_%d_%s", s, fName.c_str()));
      h2_theta_dw[1][s] = (TH2D*) (Form("h2_theta_dw_corr_%d_%s", s, fName.c_str()));
      h2_phi_dw[0][s] = (TH2D*) fInputFile->Get(Form("h2_phi_dw_uncorr_%d_%s", s, fName.c_str()));
      h2_phi_dw[1][s] = (TH2D*) fInputFile->Get(Form("h2_phi_dw_corr_%d_%s", s, fName.c_str()));
    }    
  }
}

void CorrectionHistograms::Save(TFile *outputFile) {

  outputFile->mkdir("CorrectionHistograms");
  outputFile->cd("CorrectionHistograms");

  for(int s=0; s<7; s++){
      for(int p=0; p<NPADDLES; p++){
	h2_p_dbeta[0][p][s]->Write();
	h2_p_dbeta[1][p][s]->Write();
      }
      
      // vertex
      h1_vz[0][s]->Write();
      h1_vz[1][s]->Write();
      h2_vz_phi[0][s]->Write();
      h2_vz_phi[1][s]->Write();

      // momentum
      h2_theta_dw[0][s]->Write();
      h2_theta_dw[1][s]->Write();
      h2_phi_dw[0][s]->Write();
      h2_phi_dw[1][s]->Write();
  }    

  outputFile->cd(); 
}


#endif
