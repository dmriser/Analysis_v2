////////////////////////////////////////
/*
 David Riser, University of Connecticut
 
 July 13, 2016
 
 Modified:
 March 8, 2017
 April 14, 2017 

 kaon.cxx -> Write out K+ ID Params.
 
 */
////////////////////////////////////////

#include <iostream>
using namespace std;

#include "CommonTools.h"
#include "Corrections.h"
#include "DataEventCut.h"
#include "h22Event.h"
#include "h22ElectronEvent.h"
#include "h22Option.h"
#include "h22Reader.h"
#include "GenericAnalysis.h"
#include "MesonHistograms.h"
#include "MomCorr.h"
#include "Parameters.h"
#include "ParameterSet.h"
#include "Pars.h"
#include "ParticleFilter.h"
#include "PhysicsEvent.h"
#include "PhysicsEventBuilder.h"
#include "StandardHistograms.h"

#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraphErrors.h"
#include "TH1.h"
#include "TH2.h"
#include "TLorentzVector.h"

class HIDCalibration : public GenericAnalysis {

public:
    HIDCalibration(h22Options *opts, Parameters *params) : GenericAnalysis(opts), pars(params) { }
    ~HIDCalibration(){ }

public:
  Parameters *pars;
  ParticleFilter *filter;
  Corrections corr;
  PhysicsEventBuilder *builder;   
  MomCorr_e1f *momCorr; 

  MesonHistograms *allPosHistos; 
  MesonHistograms *dvzCutHistos; 
  MesonHistograms *dcFidCutHistos; 
  MesonHistograms *massCutHistos;   
  MesonHistograms *allCutHistos;   
  StandardHistograms *passedHistos;

#if __cplusplus >= 201103L
  static constexpr int NPBINS  = 10;
  static constexpr int NSLICES = NPBINS;
  static constexpr double PMIN = 0.5;
  static constexpr double PMAX = 2.0;
#else
  const static int NPBINS  = 10;
  const static int NSLICES = NPBINS;
  const static double PMIN = 0.5;
  const static double PMAX = 2.0;
#endif

  TH1D *h1_b_slice_kp[6][NPBINS];

  TF1 *f_pol3_kp_mu[6]; 
  TF1 *f_pol3_kp_sigma[6]; 

  double dummyAxis[NSLICES]; 
  double dummyAxisError[NSLICES]; 
  double mu[NSLICES];
  double sigma[NSLICES]; 
  double muError[NSLICES];
  double sigmaError[NSLICES];

  DCR1FiducialCut *dcFid; 

  void DoBetaPFits();
  void Initialize();
  void ProcessEvent();
  void Save(string outputFilename);
  void WriteHardCodedParameters();
  void InitHistos();
  bool CurrentParticleIsNotElectronCandidate(std::vector<int> &electronCandidates, int index);

};

void HIDCalibration::InitHistos(){
  allPosHistos = new MesonHistograms("allPositive", 321);
  dvzCutHistos = new MesonHistograms("dvzCut", 321);
  dcFidCutHistos = new MesonHistograms("dcFidCut", 321);
  massCutHistos = new MesonHistograms("massCut", 321);
  allCutHistos = new MesonHistograms("allCut", 321);
  passedHistos = new StandardHistograms("kaonEvents",0); 

  for (int s=0; s<6; s++) { 
    f_pol3_kp_mu[s]    = new TF1(Form("f_pol3_kp_mu_%d",s), "pol3", PMIN, PMAX); 
    f_pol3_kp_sigma[s] = new TF1(Form("f_pol3_kp_sigma_%d",s), "pol3", PMIN, PMAX); 
  }

  for (int b=0; b<NSLICES; b++){   
    dummyAxis[b]      = b*(PMAX-PMIN)/NSLICES + PMIN; 
    dummyAxisError[b] = 0.0; 

    for(int s=0; s<6; s++){
      h1_b_slice_kp[s][b] = new TH1D(Form("h1_b_slice%d_kp_%d",b,s), Form("h1_b_slice%d_kp_%d",b,s), 100, 0.4, 1.2); 
    }
  }
}

void HIDCalibration::Initialize(){
  InitHistos();
  
  dcFid = new DCR1FiducialCut(); 
  dcFid->angle = 60.0; 
  dcFid->height = 10.0; 

  filter = new ParticleFilter(pars);
  filter->set_info(GSIM, GetRunNumber());

  builder = new PhysicsEventBuilder(); 
  
  momCorr = new MomCorr_e1f("/u/home/dmriser/Analysis_v2/momCorr/");
}

void HIDCalibration::WriteHardCodedParameters(){
  ParameterSet deltaVz, dcr1Height, dcr1Angle, tofMassMinPars, tofMassMaxPars; 

  deltaVz.setName("KP_DVZ"); 
  dcr1Height.setName("KP_DCR1_HEIGHT"); 
  dcr1Angle.setName("KP_DCR1_ANGLE"); 
  tofMassMinPars.setName("KP_TOFMASS_MIN");
  tofMassMaxPars.setName("KP_TOFMASS_MAX");

  for(int s=0; s<6; s++){
    deltaVz.addValueAndError(4.00, 0.00); 
    dcr1Height.addValueAndError(10.00, 0.00);
    dcr1Angle.addValueAndError(60.00, 0.00);
    tofMassMinPars.addValueAndError(0.418, 0.00);
    tofMassMaxPars.addValueAndError(0.650, 0.00);
  }

  pars->addParameterSet(deltaVz);
  pars->addParameterSet(dcr1Height);
  pars->addParameterSet(dcr1Angle);
  pars->addParameterSet(tofMassMinPars);
  pars->addParameterSet(tofMassMaxPars);
}

void HIDCalibration::ProcessEvent(){

    // Load up hadrons if we've electron.
    vector<int> electronCandidates = filter->getVectorOfParticleIndices(event, 11);
    if ( !electronCandidates.empty() ){
      
      // Take the fastest one
      int electronIndex = electronCandidates[0];
      h22ElectronEvent electronEvent(event); 
      electronEvent.SetElectronIndex(electronIndex); 
      corr.CorrectElectronEvent(&electronEvent, GetRunNumber(), GSIM); 

      for (int ipart=0; ipart<event.gpart; ipart++){
	
	// This important line stops other electrons from
	// getting added to the plots.
	if (CurrentParticleIsNotElectronCandidate(electronCandidates, ipart)) {
	  if (event.q[ipart] == 1){ 
	    allPosHistos->Fill(electronEvent, ipart); 

	    double tofmass = electronEvent.p[ipart]*sqrt((1-pow(electronEvent.corr_b[ipart],2))/pow(electronEvent.corr_b[ipart],2)); 
	    double dvz = fabs(electronEvent.corr_vz[electronIndex] - electronEvent.corr_vz[ipart]);

	    if (dcFid->IsPassed(event, ipart)){
	      dcFidCutHistos->Fill(electronEvent, ipart); 

	      if (tofmass > 0.418 && tofmass < 0.65 && dvz < 4){
		allCutHistos->Fill(electronEvent, ipart); 

		TLorentzVector electron = event.GetTLorentzVector(electronIndex, 11);
		TLorentzVector kaon     = event.GetTLorentzVector(ipart, 321);

		electron = momCorr->PcorN(electron, -1, 11);
		kaon = momCorr->PcorN(kaon, 1, 321);

		PhysicsEvent physicsEvent = builder->getPhysicsEvent(electron, kaon);

		passedHistos->Fill(event, electronIndex, ipart, physicsEvent);
	      }

	    }
	    
	    if (tofmass > 0.418 && tofmass < 0.65){
	      massCutHistos->Fill(electronEvent, ipart); 
	    }

	    if (dvz < 4){
	      dvzCutHistos->Fill(electronEvent, ipart); 
	    }


	  }
	}
      }
    }

}

bool HIDCalibration::CurrentParticleIsNotElectronCandidate(std::vector<int> &electronCandidates,int index){
    return (std::find(electronCandidates.begin(), electronCandidates.end(), index) == electronCandidates.end());
}

void HIDCalibration::DoBetaPFits(){

  // ------------------------------------------------
  //   Setup ParameterSets
  // ------------------------------------------------
  ParameterSet mu_a, mu_b, mu_c, mu_d; 
  ParameterSet sigma_a, sigma_b, sigma_c, sigma_d; 
  ParameterSet nsigma; 

  mu_a.setName("KP_BVP_MU_A"); 
  mu_b.setName("KP_BVP_MU_B"); 
  mu_c.setName("KP_BVP_MU_C"); 
  mu_d.setName("KP_BVP_MU_D"); 
  sigma_a.setName("KP_BVP_SIGMA_A"); 
  sigma_b.setName("KP_BVP_SIGMA_B"); 
  sigma_c.setName("KP_BVP_SIGMA_C"); 
  sigma_d.setName("KP_BVP_SIGMA_D"); 
  nsigma.setName("KP_BVP_NSIGMA");

  nsigma.addValueAndError(3.0, 0.0);
  
  // ------------------------------------------------
  //   Calculate Parameters for beta vs. p 
  // ------------------------------------------------
  TF1 *fitGauss  = new TF1("fitGauss","gaus",0.0,0.6);
  
  for(int s=0; s<6; s++){
    for(int b=0; b<NSLICES; b++){
      double p = dummyAxis[b]; 
      double theoryBeta = p/sqrt( pow(p,2) + pow(pid_to_mass(321),2) );  
      double fitMin = theoryBeta*0.8; 
      double fitMax = theoryBeta*1.2; 

      std::cout << "Fitting bin = " << b << " fitMin = " << fitMin << " fitMax = " << fitMax << " beta = " << theoryBeta << std::endl; 
      
      int startBin = allPosHistos->h2_p_beta[s+1]->GetXaxis()->FindBin(p); 
      int stopBin  = allPosHistos->h2_p_beta[s+1]->GetXaxis()->FindBin(p + (PMAX-PMIN)/NSLICES); 

      fitGauss->SetParameter(1, theoryBeta);
      fitGauss->SetRange(fitMin, fitMax); 

      h1_b_slice_kp[s][b] = allCutHistos->h2_p_beta[s+1]->ProjectionY(Form("h1_b_slice%d_kp_%d",b,s), startBin, stopBin); 
      h1_b_slice_kp[s][b]->Fit("fitGauss", "RQ");

      mu[b] = fitGauss->GetParameter(1);
      muError[b] = fitGauss->GetParError(1); 
      sigma[b] = fitGauss->GetParameter(2);  
      sigmaError[b] = fitGauss->GetParError(2); 

      std::cout << "Fit mu = " << mu[b] << " sigma = " << sigma[b] << std::endl; 
    }

    TGraphErrors *g_mu = new TGraphErrors(NSLICES, dummyAxis, mu, dummyAxisError, muError); 
    TGraphErrors *g_sigma = new TGraphErrors(NSLICES, dummyAxis, sigma, dummyAxisError, sigmaError); 

    g_mu->Fit(Form("f_pol3_kp_mu_%d",s), "RQ"); 
    g_sigma->Fit(Form("f_pol3_kp_mu_%d",s), "RQ"); 

    mu_a.addValueAndError(f_pol3_kp_mu[s]->GetParameter(3), f_pol3_kp_mu[s]->GetParError(3)); 
    mu_b.addValueAndError(f_pol3_kp_mu[s]->GetParameter(2), f_pol3_kp_mu[s]->GetParError(2)); 
    mu_c.addValueAndError(f_pol3_kp_mu[s]->GetParameter(1), f_pol3_kp_mu[s]->GetParError(1)); 
    mu_d.addValueAndError(f_pol3_kp_mu[s]->GetParameter(0), f_pol3_kp_mu[s]->GetParError(0)); 

    sigma_a.addValueAndError(f_pol3_kp_sigma[s]->GetParameter(3), f_pol3_kp_sigma[s]->GetParError(3)); 
    sigma_b.addValueAndError(f_pol3_kp_sigma[s]->GetParameter(2), f_pol3_kp_sigma[s]->GetParError(2)); 
    sigma_c.addValueAndError(f_pol3_kp_sigma[s]->GetParameter(1), f_pol3_kp_sigma[s]->GetParError(1)); 
    sigma_d.addValueAndError(f_pol3_kp_sigma[s]->GetParameter(0), f_pol3_kp_sigma[s]->GetParError(0)); 
  }

  pars->addParameterSet(mu_a);
  pars->addParameterSet(mu_b);
  pars->addParameterSet(mu_c);
  pars->addParameterSet(mu_d);
  pars->addParameterSet(sigma_a);
  pars->addParameterSet(sigma_b);
  pars->addParameterSet(sigma_c);
  pars->addParameterSet(sigma_d);
  pars->addParameterSet(nsigma);
}

void HIDCalibration::Save(string outputFilename){
    TFile *outputFile = new TFile(outputFilename.c_str(), "recreate");

    allPosHistos->Save(outputFile);
    allCutHistos->Save(outputFile); 
    dvzCutHistos->Save(outputFile); 
    dcFidCutHistos->Save(outputFile); 
    massCutHistos->Save(outputFile); 
    passedHistos->Save(outputFile);
    
    outputFile->cd(); 
    outputFile->mkdir("betaSlices"); 
    outputFile->cd("betaSlices"); 
    for(int s=0; s<6; s++){
      
      f_pol3_kp_mu[s]->Write(); 
      f_pol3_kp_sigma[s]->Write(); 

      for(int b=0; b<NSLICES; b++){
	h1_b_slice_kp[s][b]->Write(); 
      }
    }

    outputFile->Write();
    outputFile->Close();
}

int main(int argc, char * argv[]){

    // Setup Options
    h22Options opts;
    opts.args["PARS"].args = "/u/home/dmriser/Analysis_v2/lists/data.pars";
    opts.args["PARS"].type = 1;
    opts.args["PARS"].name = "Parameter file";
    opts.set(argc,argv);

    int GSIM        = opts.args["MC"].arg;
    long int nev    = opts.args["N"].arg;
    string parfile  = opts.args["PARS"].args;

    Parameters pars;
    pars.loadParameters(opts.args["PARS"].args);

    HIDCalibration Analysis(&opts, &pars);
    for (std::vector<std::string>::iterator it=opts.ifiles.begin(); it<opts.ifiles.end(); it++) { Analysis.AddFile(*it); }
    Analysis.RunAnalysis();
    //    Analysis.DoBetaPFits();
    Analysis.WriteHardCodedParameters();
    Analysis.Save(opts.args["OUT"].args);

    //    pars.saveParameters(opts.args["PARS"].args); 
    pars.saveParameters("hid_run.pars"); 

    return 0;
}
