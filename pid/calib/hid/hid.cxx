////////////////////////////////////////
/*
 David Riser, University of Connecticut
 
 July 13, 2016
 
 hid.cxx -> Write out Hadron ID Params.
 
 */
////////////////////////////////////////

// c++ includes
#include <iostream>
using namespace std;

// my includes
#include "CommonTools.h"
#include "Corrections.h"
#include "h22Event.h"
#include "h22Option.h"
#include "h22Reader.h"
#include "GenericAnalysis.h"
#include "ParticleFilter.h"
#include "Parameters.h"
#include "Pars.h"

// root includes
#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraphErrors.h"
#include "TH1.h"
#include "TH2.h"

class HIDCalibration : public GenericAnalysis {

public:
  HIDCalibration(Parameters *params){ pars = params; }
  ~HIDCalibration(){ }

public:
  Parameters *pars; 
  ParticleFilter *filter;
  Corrections corr;
  
  const static int NPBINS      = 50;
  const static int NSLICES     = NPBINS;
  constexpr static double PMIN = 0.5;
  constexpr static double PMAX = 2.0;
  
  // Histograms
  TH1F * h1_dvz[6];
  TH1F *h1_tofmass_pos[6];
  TH1F *h1_tofmass_neg[6];
  TH1F *h1_tofmass_neu[6];
  TH2F * h2_p_b_pos[6];
  TH2F * h2_p_b_neg[6];
  TH2F * h2_p_b_neu[6];
  TH2F *h2_tofmass_pos[6];
  TH2F *h2_tofmass_neg[6];
  TH2F *h2_tofmass_neu[6];
  TH2F * h2_p_db_prot[6];
  TH2F * h2_p_db_pim[6];
  TH2F * h2_p_db_pip[6];    
  TF1 * f_dvz_gauss;
  TF1 * f_db_gauss;
  TF1 * f_db_mean;
  TF1 * f_db_sigma;
  TH1D * h1_slice_prot[6][NSLICES];
  TH1D * h1_slice_pip[6][NSLICES];
  TH1D * h1_slice_pim[6][NSLICES];
  TGraphErrors * g_mean_prot[6];
  TGraphErrors * g_sigma_prot[6];
  TGraphErrors * g_mean_pip[6];
  TGraphErrors * g_sigma_pip[6];
  TGraphErrors * g_mean_pim[6];
  TGraphErrors * g_sigma_pim[6];
  double mean[NSLICES];
  double sigma[NSLICES];
  double mean_e[NSLICES];
  double sigma_e[NSLICES];
  double x[NSLICES];
  double dx[NSLICES];
  
  void DoFits();
  void Initialize();
  void ProcessEvent();
  void Save(string outputFilename);
  void WriteHardCodedParameters();
};

void HIDCalibration::Initialize(){

  double PSTEP = (PMAX-PMIN)/(NPBINS-1);
  
  f_dvz_gauss = new TF1("f_dvz_gauss","gaus",-10,10);
  f_db_gauss  = new TF1("f_db_gauss","gaus",-0.05,0.05);
  f_db_mean   = new TF1("f_db_mean","[0]",PMIN,PMAX);
  f_db_sigma  = new TF1("f_db_sigma","[0]",PMIN,PMAX);
  
  // For passing to TGraph constructor
    for (int b=0; b<NSLICES; b++) {x[b] = (double) b*PSTEP + PMIN; dx[b] = 0.0;}
    
    for (int s=0; s<6; s++){
      h1_dvz[s]       = new TH1F(Form("h1_dvz_%d",s),      Form(" Difference between E Z-Vertex #& Candidate Sector %d",s+1),200,-10,10);
      h2_p_b_pos[s]   = new TH2F(Form("h2_p_b_pos_%d",s),  Form(" Beta vs. Momentum (q>0) Sector %d ",s+1),NPBINS,PMIN,PMAX,100,0.2,1.2);
      h2_p_b_neg[s]   = new TH2F(Form("h2_p_b_neg_%d",s),  Form(" Beta vs. Momentum (q<0) Sector %d ",s+1),NPBINS,PMIN,PMAX,100,0.2,1.2);
      h2_p_b_neu[s]   = new TH2F(Form("h2_p_b_neu_%d",s),  Form(" Beta vs. Momentum (q=0) Sector %d ",s+1),NPBINS,PMIN,PMAX,100,0.2,1.2);
      h2_tofmass_pos[s] = new TH2F(Form("h2_tofmass_pos_%d",s),  Form(" TOF Mass (q= 1) Sector %d ",s+1),NPBINS,PMIN,PMAX,100,-0.01,1.5);
      h2_tofmass_neg[s] = new TH2F(Form("h2_tofmass_neg_%d",s),  Form(" TOF Mass (q=-1) Sector %d ",s+1),NPBINS,PMIN,PMAX,100,-0.01,1.5);
      h2_tofmass_neu[s] = new TH2F(Form("h2_tofmass_neu_%d",s),  Form(" TOF Mass (q= 0) Sector %d ",s+1),NPBINS,PMIN,PMAX,100,-0.01,1.5);
      h1_tofmass_pos[s] = new TH1F(Form("h1_tofmass_pos_%d",s),  Form(" TOF Mass (q= 1) Sector %d ",s+1),100,-0.1,1.5);
      h1_tofmass_neg[s] = new TH1F(Form("h1_tofmass_neg_%d",s),  Form(" TOF Mass (q=-1) Sector %d ",s+1),100,-0.1,1.5);
      h1_tofmass_neu[s] = new TH1F(Form("h1_tofmass_neu_%d",s),  Form(" TOF Mass (q= 0) Sector %d ",s+1),100,-0.4,1.5);
      h2_p_db_prot[s] = new TH2F(Form("h2_p_db_prot_%d",s),Form(" #Delta Beta vs. Momentum (Proton) Sector %d ",s+1),NPBINS,PMIN,PMAX,100,-0.6,0.2);
      h2_p_db_pip[s]  = new TH2F(Form("h2_p_db_pip_%d",s), Form(" #Delta Beta vs. Momentum (#pi^+) Sector %d ",s+1),NPBINS,PMIN,PMAX,100,-0.2,0.6);
      h2_p_db_pim[s]  = new TH2F(Form("h2_p_db_pim_%d",s), Form(" #Delta Beta vs. Momentum (#pi^-) Sector %d ",s+1),NPBINS,PMIN,PMAX,100,-0.2,0.2);
    }

    filter = new ParticleFilter(pars);
    filter->set_info(GSIM, runno());
    
}

void HIDCalibration::ProcessEvent(){
  
  // Load up hadrons if we've electron.
  vector<int> electronCandidates = filter->getVectorOfParticleIndices(event, 11);
  if ( !electronCandidates.empty() ){

    // Take the fastest one
    int electronIndex = electronCandidates[0];    
    double start_time = corr.electron_sct(event,electronIndex,runno(),GSIM) - event.sc_r[electronIndex]/speed_of_light;

    for (int ipart=0; ipart<event.gpart; ipart++){

      // This important line stops other electrons from 
      // getting added to the plots.
      if ( std::find(electronCandidates.begin(), electronCandidates.end(), ipart) == electronCandidates.end() ) {

	int sector = event.sc_sect[ipart]-1; 
	if (sector > -1){
	  double cbeta = event.sc_r[ipart]/(corr.hadron_sct(event,ipart,runno(),GSIM)-start_time);
	  double beta  = cbeta/speed_of_light;
	  double tofmass = sqrt(pow(event.p[ipart],2)*(1-pow(beta,2))/pow(beta,2)); 
	  
	  if (event.q[ipart] < 0) {
	    h2_p_b_neg[sector]  ->Fill(event.p[ipart],beta);
	    h2_p_db_pim[sector] ->Fill(event.p[ipart],(event.p[ipart])/sqrt(event.p[ipart]*event.p[ipart]+pi_mass*pi_mass)-beta);
	    h2_tofmass_neg[sector]->Fill(event.p[ipart], tofmass); 
	    h1_tofmass_neg[sector]->Fill(tofmass); 
	  }
	  
	  if (event.q[ipart] > 0) {
	    h2_p_b_pos[sector]   ->Fill(event.p[ipart],beta);
	    h2_p_db_prot[sector] ->Fill(event.p[ipart],(event.p[ipart])/sqrt(event.p[ipart]*event.p[ipart]+proton_mass*proton_mass)-beta);
	    h2_p_db_pip[sector]  ->Fill(event.p[ipart],(event.p[ipart])/sqrt(event.p[ipart]*event.p[ipart]+pi_mass*pi_mass)-beta);
	    h2_tofmass_pos[sector]->Fill(event.p[ipart], tofmass); 
	    h1_tofmass_pos[sector]->Fill(tofmass); 
	  }
	  
	  if (event.q[ipart] == 0) { 
	    h2_p_b_neu[sector]->Fill(event.p[ipart],beta); 
	    h2_tofmass_neu[sector]->Fill(event.p[ipart], tofmass); 
	    h1_tofmass_neu[sector]->Fill(tofmass); 
	  }
        
	  h1_dvz[sector]->Fill(corr.vz(event,electronIndex,runno(),GSIM)-corr.vz(event,ipart,runno(),GSIM));
	}
      }
      
    }
  }
}

void HIDCalibration::DoFits(){
    
  // Fits here

}

void HIDCalibration::Save(string outputFilename){
  TFile *outputFile = new TFile(outputFilename.c_str(), "recreate"); 
  
  for (int s=0; s<6; s++){
    h1_dvz[s]      ->Write();
    h2_p_b_pos[s]  ->Write();
    h2_p_b_neg[s]  ->Write();
    h2_p_b_neu[s]  ->Write();
    h2_p_db_prot[s]->Write();
    h2_p_db_pip[s] ->Write();
    h2_p_db_pim[s] ->Write();
    h2_tofmass_pos[s]->Write();
    h2_tofmass_neg[s]->Write();
    h2_tofmass_neu[s]->Write();
    h1_tofmass_pos[s]->Write();
    h1_tofmass_neg[s]->Write();
    h1_tofmass_neu[s]->Write();
  }
  
  outputFile->Write();
  outputFile->Close();

}

int main(int argc, char * argv[]){
  
  // Setup Options
  h22Options opts;
  opts.args["PARS"].args = "/u/home/dmriser/mydoc/analysis/root_scripts/Analysis_v2/lists/data.pars";
  opts.args["PARS"].type = 1;
  opts.args["PARS"].name = "Parameter file";
  opts.set(argc,argv);
  
  int GSIM        = opts.args["MC"].arg;
  long int nev    = opts.args["N"].arg;
  string parfile  = opts.args["PARS"].args;
  
  Parameters pars;
  pars.loadParameters(opts.args["PARS"].args);

  HIDCalibration Analysis(&pars); 
  for (auto it=opts.ifiles.begin(); it<opts.ifiles.end(); it++) { Analysis.AddFile(*it); }
  Analysis.RunAnalysis(nev); 
  Analysis.DoFits(); 
  Analysis.Save(opts.args["OUT"].args); 

  pars.saveParameters(opts.args["PARS"].args);  
  
  return 0;
}
