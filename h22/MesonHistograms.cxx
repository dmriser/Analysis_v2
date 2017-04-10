#ifndef meson_histos_cxx
#define meson_histos_cxx

#include <iostream>

#include "TVector3.h"

#include "CommonTools.h"
#include "MesonHistograms.h"

MesonHistograms::MesonHistograms(std::string name, int pid) : fName(name), fPid(pid) {

  for(int s=0; s<7; s++){
    h1_p[s] = new TH1D(Form("h1_p_%d_%s",s, fName.c_str()), Form("h1_p_%d_%s",s, fName.c_str()), 100, 0.0, 4.5);
    h1_b[s] = new TH1D(Form("h1_b_%d_%s",s, fName.c_str()), Form("h1_b_%d_%s",s, fName.c_str()), 100, 0.0, 1.2);
    h1_tofmass[s] = new TH1D(Form("h1_tofmass_%d_%s",s, fName.c_str()), Form("h1_tofmass_%d_%s",s, fName.c_str()), 100, -0.6, 1.5);
    h1_dcx[s] = new TH1D(Form("h1_dcx_%d_%s",s, fName.c_str()), Form("h1_dcx_%d_%s",s, fName.c_str()), 100, -150, 150);
    h1_dcy[s] = new TH1D(Form("h1_dcy_%d_%s",s, fName.c_str()), Form("h1_dcy_%d_%s",s, fName.c_str()), 100, -150, 150);
    h1_edep[s] = new TH1D(Form("h1_edep_%d_%s",s, fName.c_str()), Form("h1_edep_%d_%s",s, fName.c_str()), 100, 0.0, 4.5);
    h1_dbeta[s] = new TH1D(Form("h1_dbeta_%d_%s",s, fName.c_str()), Form("h1_dbeta_%d_%s",s, fName.c_str()), 100, -2.5, 2.5);
    h1_vz[s] = new TH1D(Form("h1_vz_%d_%s",s, fName.c_str()), Form("h1_vz_%d_%s",s, fName.c_str()), 100, -40.0, 0.0);
    h1_dvz[s] = new TH1D(Form("h1_dvz_%d_%s",s, fName.c_str()), Form("h1_dvz_%d_%s",s, fName.c_str()), 100, -20.0, 20.0);
    h1_theta[s] = new TH1D(Form("h1_theta_%d_%s",s, fName.c_str()), Form("h1_theta_%d_%s",s, fName.c_str()), 100, 0.0, 180.0);
    h1_dtheta[s] = new TH1D(Form("h1_dtheta_%d_%s",s, fName.c_str()), Form("h1_dtheta_%d_%s",s, fName.c_str()), 100, -80.0, 80.0);
    h1_phi[s] = new TH1D(Form("h1_phi_%d_%s",s, fName.c_str()), Form("h1_phi_%d_%s",s, fName.c_str()), 100, -30, 30);
    
    h2_phi_theta[s] = new TH2D(Form("h2_phi_theta_%d_%s",s, fName.c_str()), Form("h2_phi_theta_%d_%s",s, fName.c_str()), 100, -30, 30, 100, 0.0, 60.0); 
    h2_dcx_dcy[s] = new TH2D(Form("h2_dcx_dcy_%d_%s",s, fName.c_str()), Form("h2_dcx_dcy_%d_%s",s, fName.c_str()), 100, -150, 150, 100, -150, 150); 
    h2_p_beta[s] = new TH2D(Form("h2_p_beta_%d_%s",s, fName.c_str()), Form("h2_p_beta_%d_%s",s, fName.c_str()), 100, 0.0, 4.5, 100, 0.0, 1.2); 
    h2_p_tofmass[s] = new TH2D(Form("h2_p_tofmass_%d_%s",s, fName.c_str()), Form("h2_p_tofmass_%d_%s",s, fName.c_str()), 100, 0.0, 4.5, 100, -0.6, 1.5); 
    h2_p_dbeta[s] = new TH2D(Form("h2_p_dbeta_%d_%s",s, fName.c_str()), Form("h2_p_dbeta_%d_%s",s, fName.c_str()), 100, 0.0, 4.5, 100, -0.5, 0.5); 
  }

}

void MesonHistograms::Fill(h22ElectronEvent event, int index) {
  h1_p[0]->Fill(event.p[index]);
  h1_dcx[0]->Fill(event.tl1_x[index]);
  h1_dcy[0]->Fill(event.tl1_y[index]);
  h1_edep[0]->Fill(event.edep[index]);
  h1_theta[0]->Fill(event.GetTheta(index));
  h1_dtheta[0]->Fill(event.GetTheta(event.GetElectronIndex()) - event.GetTheta(index));
  h1_phi[0] ->Fill(event.GetRelativePhi(index));
  
  h2_phi_theta[0]->Fill(event.GetRelativePhi(index), event.GetTheta(index));
  h2_dcx_dcy[0]->Fill(event.tl1_x[index], event.tl1_y[index]);
 
  int s = event.dc_sect[index];
  h1_p[s]->Fill(event.p[index]);
  h1_dcx[s]->Fill(event.tl1_x[index]);
  h1_dcy[s]->Fill(event.tl1_y[index]);
  h1_edep[s]->Fill(event.edep[index]);
  h1_theta[s]->Fill(event.GetTheta(index));
  h1_dtheta[s]->Fill(event.GetTheta(event.GetElectronIndex()) - event.GetTheta(index));
  h1_phi[s] ->Fill(event.GetRelativePhi(index));

  h2_phi_theta[s]->Fill(event.GetRelativePhi(index), event.GetTheta(index));
  h2_dcx_dcy[s]->Fill(event.tl1_x[index], event.tl1_y[index]);

  if(event.IsCorrected()){
    h1_vz[0] ->Fill(event.corr_vz[index]);
    h1_dvz[0]->Fill(event.corr_vz[event.GetElectronIndex()]-event.corr_vz[index]);
    h1_b[0]->Fill(event.corr_b[index]);    

    double beta = event.p[index]/sqrt( pow(event.p[index],2) + pow(pid_to_mass(fPid),2) );

    h1_dbeta[0]->Fill(beta-event.corr_b[index]);
    h2_p_beta[0]->Fill(event.p[index], event.corr_b[index]);
    h2_p_dbeta[0]->Fill(event.p[index], beta-event.corr_b[index]);

    h1_vz[s] ->Fill(event.corr_vz[index]);
    h1_dvz[s]->Fill(event.corr_vz[event.GetElectronIndex()]-event.corr_vz[index]);
    h1_b[s]->Fill(event.corr_b[index]);    

    h1_dbeta[s]->Fill(beta-event.corr_b[index]);
    h2_p_beta[s]->Fill(event.p[index], event.corr_b[index]);
    h2_p_dbeta[s]->Fill(event.p[index], beta-event.corr_b[index]);

    double tofmass = event.p[index]*sqrt((1-pow(event.corr_b[index],2))/pow(event.corr_b[index],2));
    h1_tofmass[0]->Fill(tofmass);
    h2_p_tofmass[0]->Fill(event.p[index], tofmass);
    h1_tofmass[s]->Fill(tofmass);
    h2_p_tofmass[s]->Fill(event.p[index], tofmass);
  }

}

void MesonHistograms::Load(std::string filename) {
  fInputFile = TFile::Open(filename.c_str());

  if (fInputFile->IsOpen()){
    for (int s=0; s<7; s++){
      h1_p[s] = (TH1D*) fInputFile->Get(Form("MesonHistograms/h1_p_%d_%s",s, fName.c_str()));
      h1_b[s] = (TH1D*) fInputFile->Get(Form("MesonHistograms/h1_b_%d_%s",s, fName.c_str()));
      h1_tofmass[s] = (TH1D*) fInputFile->Get(Form("MesonHistograms/h1_tofmass_%d_%s",s, fName.c_str()));
      h1_dcx[s] = (TH1D*) fInputFile->Get(Form("MesonHistograms/h1_dcx_%d_%s",s, fName.c_str()));
      h1_dcy[s] = (TH1D*) fInputFile->Get(Form("MesonHistograms/h1_dcy_%d_%s",s, fName.c_str()));
      h1_edep[s] = (TH1D*) fInputFile->Get(Form("MesonHistograms/h1_edep_%d_%s",s, fName.c_str()));
      h1_dbeta[s] = (TH1D*) fInputFile->Get(Form("MesonHistograms/h1_dbeta_%d_%s",s, fName.c_str()));
      h1_vz[s] = (TH1D*) fInputFile->Get(Form("MesonHistograms/h1_vz_%d_%s",s, fName.c_str()));
      h1_dvz[s] = (TH1D*) fInputFile->Get(Form("MesonHistograms/h1_dvz_%d_%s",s, fName.c_str()));
      h1_theta[s] = (TH1D*) fInputFile->Get(Form("MesonHistograms/h1_theta_%d_%s",s, fName.c_str()));
      h1_dtheta[s] = (TH1D*) fInputFile->Get(Form("MesonHistograms/h1_dtheta_%d_%s",s, fName.c_str()));
      h1_phi[s] = (TH1D*) fInputFile->Get(Form("MesonHistograms/h1_phi_%d_%s",s, fName.c_str()));

      h2_phi_theta[s] = (TH2D*) fInputFile->Get(Form("MesonHistograms/h2_phi_theta_%d_%s",s, fName.c_str()));
      h2_dcx_dcy[s] = (TH2D*) fInputFile->Get(Form("MesonHistograms/h2_dcx_dcy_%d_%s",s, fName.c_str()));
      h2_p_beta[s] = (TH2D*) fInputFile->Get(Form("MesonHistograms/h2_p_beta_%d_%s",s, fName.c_str()));
      h2_p_tofmass[s] = (TH2D*) fInputFile->Get(Form("MesonHistograms/h2_p_tofmass_%d_%s",s, fName.c_str()));
      h2_p_dbeta[s] = (TH2D*) fInputFile->Get(Form("MesonHistograms/h2_p_dbeta_%d_%s",s, fName.c_str()));
    }    
  }

}

void MesonHistograms::Save(TFile *outputFile) {

  outputFile->mkdir("MesonHistograms");
  outputFile->cd("MesonHistograms");

  for(int s=0; s<7; s++){
    h1_p[s]->Write(); 
    h1_b[s]->Write(); 
    h1_tofmass[s]->Write(); 
    h1_dcx[s]->Write(); 
    h1_dcy[s]->Write(); 
    h1_edep[s]->Write(); 
    h1_dbeta[s]->Write(); 
    h1_vz[s] ->Write(); 
    h1_dvz[s]->Write(); 
    h1_theta[s]->Write(); 
    h1_dtheta[s]->Write(); 
    h1_phi[s] ->Write(); 
    
    h2_phi_theta[s]->Write(); 
    h2_dcx_dcy[s]->Write(); 
    h2_p_beta[s]->Write(); 
    h2_p_tofmass[s]->Write(); 
    h2_p_dbeta[s]->Write(); 
  }

}

#endif
