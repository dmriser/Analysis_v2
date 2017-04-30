////////////////////////////////////////
/*

 David Riser, University of Connecticut
 
 July 13, 2016
 
 Modified:
 March 8, 2017
 April 14, 2017 

 Make slices 
 
 */
////////////////////////////////////////

#include <iostream>

#include "CommonTools.h"
#include "h22Option.h"
#include "MesonHistograms.h"
#include "Parameters.h"
#include "ParameterSet.h"
#include "TF1Integrator.h"

#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraphErrors.h"
#include "TH1.h"
#include "TH2.h"
#include "TLatex.h"

/*
class HIDCalibration {

public:
    HIDCalibration(h22Options *opts) : GenericAnalysis(opts) { }
    ~HIDCalibration(){ }

public:
  MesonHistograms *pipHistos; 
  MesonHistograms *pimHistos; 
  MesonHistograms *kpHistos; 
  MesonHistograms *kmHistos; 

#if __cplusplus >= 201103L
  static constexpr int NPBINS  = 35;
  static constexpr int NSLICES = NPBINS;
  static constexpr double PMIN = 0.5;
  static constexpr double PMAX = 3.5;
#else
  const static int NPBINS  = 35;
  const static int NSLICES = NPBINS;
  const static double PMIN = 0.5;
  const static double PMAX = 3.5;
#endif

  TH1D *h1_b_slice_kp[6][NPBINS];

  TF1 *f_pol3_kp_mu[6]; 
  TF1 *f_pol3_kp_sigma[6]; 
  TF1 *f_pol3_kp_upper[6]; 
  TF1 *f_pol3_kp_lower[6]; 

  TGraphErrors *g_kp_mu[6]; 
  TGraphErrors *g_kp_sigma[6]; 

  double dummyAxis[NSLICES]; 
  double dummyAxisError[NSLICES]; 
  double mu[NSLICES];
  double sigma[NSLICES]; 
  double muError[NSLICES];
  double sigmaError[NSLICES];

  void DoBetaPFits();
  void Initialize();
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
    f_pol3_kp_upper[s] = new TF1(Form("f_pol3_kp_upper_%d",s), "pol3", PMIN, PMAX); 
    f_pol3_kp_lower[s] = new TF1(Form("f_pol3_kp_lower_%d",s), "pol3", PMIN, PMAX); 
  }


  double pMinHistos = allPosHistos->h2_p_beta[0]->GetXaxis()->GetBinLowEdge(1);
  double pMaxHistos = allPosHistos->h2_p_beta[0]->GetXaxis()->GetBinUpEdge( allPosHistos->h2_p_beta[0]->GetXaxis()->GetNbins() );

  for (int b=0; b<NSLICES; b++){   
    dummyAxis[b]      = b*(pMaxHistos-pMinHistos)/NSLICES + pMinHistos; 
    dummyAxisError[b] = 0.0; 

    for(int s=0; s<6; s++){
      h1_b_slice_kp[s][b] = new TH1D(Form("h1_b_slice%d_kp_%d",b,s), Form("h1_b_slice%d_kp_%d",b,s), 100, 0.4, 1.2); 
    }
  }
}

void HIDCalibration::Initialize(){
  InitHistos();
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

  nsigma.addValueAndError(1.8, 0.0);
  
  // ------------------------------------------------
  //   Calculate Parameters for beta vs. p 
  // ------------------------------------------------
  TF1 *fitGauss  = new TF1("fitGauss","gaus",0.0,0.6);
  
  for(int s=0; s<6; s++){
    for(int b=0; b<NSLICES; b++){
      double p = dummyAxis[b]; 
      double theoryBeta = p/sqrt( pow(p,2) + pow(pid_to_mass(321),2) );  
      double fitMin = theoryBeta*0.8; 
      double fitMax = theoryBeta*1.1; 

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

    g_kp_mu[s] = new TGraphErrors(NSLICES, dummyAxis, mu, dummyAxisError, muError); 
    g_kp_mu[s]->SetName(Form("g_kp_mu_%d",s)); 
    g_kp_mu[s]->Fit(Form("f_pol3_kp_mu_%d",s), "RQ"); 

    g_kp_sigma[s] = new TGraphErrors(NSLICES, dummyAxis, sigma, dummyAxisError, sigmaError); 
    g_kp_sigma[s]->SetName(Form("g_kp_sigma_%d",s)); 
    g_kp_sigma[s]->Fit(Form("f_pol3_kp_sigma_%d",s), "RQ"); 

    f_pol3_kp_upper[s]->SetParameter(3, f_pol3_kp_mu[s]->GetParameter(3)+3*f_pol3_kp_sigma[s]->GetParameter(3)); 
    f_pol3_kp_upper[s]->SetParameter(2, f_pol3_kp_mu[s]->GetParameter(2)+3*f_pol3_kp_sigma[s]->GetParameter(2)); 
    f_pol3_kp_upper[s]->SetParameter(1, f_pol3_kp_mu[s]->GetParameter(1)+3*f_pol3_kp_sigma[s]->GetParameter(1)); 
    f_pol3_kp_upper[s]->SetParameter(0, f_pol3_kp_mu[s]->GetParameter(0)+3*f_pol3_kp_sigma[s]->GetParameter(0)); 

    f_pol3_kp_lower[s]->SetParameter(3, f_pol3_kp_mu[s]->GetParameter(3)-3*f_pol3_kp_sigma[s]->GetParameter(3)); 
    f_pol3_kp_lower[s]->SetParameter(2, f_pol3_kp_mu[s]->GetParameter(2)-3*f_pol3_kp_sigma[s]->GetParameter(2)); 
    f_pol3_kp_lower[s]->SetParameter(1, f_pol3_kp_mu[s]->GetParameter(1)-3*f_pol3_kp_sigma[s]->GetParameter(1)); 
    f_pol3_kp_lower[s]->SetParameter(0, f_pol3_kp_mu[s]->GetParameter(0)-3*f_pol3_kp_sigma[s]->GetParameter(0)); 

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
    
    outputFile->cd(); 
    outputFile->mkdir("betaSlices"); 
    outputFile->cd("betaSlices"); 
    for(int s=0; s<6; s++){
      
      f_pol3_kp_mu[s]->Write(); 
      f_pol3_kp_sigma[s]->Write(); 
      f_pol3_kp_upper[s]->Write(); 
      f_pol3_kp_lower[s]->Write(); 

      g_kp_mu[s]->Write();
      g_kp_sigma[s]->Write();

      for(int b=0; b<NSLICES; b++){
	h1_b_slice_kp[s][b]->Write(); 
      }
    }

    outputFile->Write();
    outputFile->Close();
}
*/

class MesonSlices { 
public:

  MesonSlices(){

  }

  ~MesonSlices(){
  }

  std::vector<std::vector<TH1D*> > beta; 
  std::vector<std::vector<TH1D*> > deltaBeta; 
  std::vector<std::vector<TH1D*> > mass; 
  std::vector<std::vector<TF1*> >  mass_fit;
  std::vector<std::vector<TF1*> >  beta_fit;
  std::vector<double> bins; 

  void SetBins(std::vector<double> b){
    bins = b; 
  }

  void Save(TFile *outputFile){
    if (outputFile->IsOpen()){
      outputFile->mkdir("slices_beta");
      outputFile->mkdir("slices_dbeta");
      outputFile->mkdir("slices_mass");

      outputFile->cd();
      outputFile->cd("slices_beta");
      for(int s=0; s<beta.size(); s++){
	for(int b=0; b<beta[s].size(); b++){
	  beta[s][b]->Write();
	  beta_fit[s][b]->Write();
	}
      }

      outputFile->cd();
      outputFile->cd("slices_dbeta");
      for(int s=0; s<deltaBeta.size(); s++){
	for(int b=0; b<deltaBeta[s].size(); b++){
	  deltaBeta[s][b]->Write();
	}
      }

      outputFile->cd();
      outputFile->cd("slices_mass");
      for(int s=0; s<mass.size(); s++){
	for(int b=0; b<mass[s].size(); b++){
	  mass[s][b]->Write();
	  mass_fit[s][b]->Write();
	}
      }

      outputFile->cd();
    } else {
      std::cerr << "[MesonFittingService::Save] The output file was not opened successfully. " << std::endl; 
    }
  }

};

class MesonFittingService {
  
public:

  MesonFittingService(MesonHistograms *h) : histos(h) {
    slices = new MesonSlices(); 

    //    fPMin = 0.5; 
    //    fPMax = 3.5; 
    fPMin = 0.5; 
    fPMax = 1.8; 
    fPWidth = (fPMax-fPMin)/fNSlices; 

    for(int b=0; b<fNSlices; b++){
      fBins.push_back(fPMin +fPWidth*b);
    }

    slices->SetBins(fBins); 
  }

  ~MesonFittingService(){
  }

  MesonHistograms   *histos;   
  MesonSlices       *slices; 

  void SetPMin(double min) { fPMin = min; }
  void SetPMax(double max) { fPMax = max; }

  const int GetNumberOfSlices() const {
    return fNSlices; 
  }

  double GetBin(int index) {
    if(index < fNSlices){
      return fBins[index];
    } else {
      return 0.00; 
    }
  }

  double GetPMin() const { 
    return fPMin; 
  }
 
  double GetPMax() const { 
    return fPMax; 
  }
  
  void Execute(){
    Slice();
  }

  void Save(TFile *out){
    slices->Save(out);
  }

protected:
  const static int fNSlices = 24; 
  std::vector<double> fBins; 
  double fPMin, fPMax, fPWidth; 

  void Slice(){
    for(int s=0; s<6; s++){
      std::vector<TH1D*> temp_beta; 
      std::vector<TH1D*> temp_deltaBeta; 
      std::vector<TH1D*> temp_mass; 
      std::vector<TF1*> temp_mass_fit; 
      std::vector<TF1*> temp_beta_fit; 

      for(int b=0; b<fNSlices; b++){
	double p = b*fPWidth + fPMin; 
	int startBin = histos->h2_p_beta[s+1]->GetXaxis()->FindBin(p); 
	int stopBin  = histos->h2_p_beta[s+1]->GetXaxis()->FindBin(p+fPWidth); 
	temp_beta     .push_back(histos->h2_p_beta[s+1]->ProjectionY(Form("h1_beta_pid%d_slice%d_sect%d",histos->GetPid(),b,s), startBin, stopBin));
	temp_deltaBeta.push_back(histos->h2_p_dbeta[s+1]->ProjectionY(Form("h1_dbeta_pid%d_slice%d_sect%d",histos->GetPid(),b,s), startBin, stopBin));
	temp_mass     .push_back(histos->h2_p_tofmass[s+1]->ProjectionY(Form("h1_mass_pid%d_slice%d_sect%d",histos->GetPid(),b,s), startBin, stopBin));
	
	// Fitting TOF Mass in momentum bin
	// Cheating to help I do seperate fits first. 
	TF1 *pionMassFit = new TF1("pionFit","gaus", 0.08, 0.17); 
	pionMassFit->SetParameter(1, pid_to_mass(211)); 

	TF1 *kaonMassFit = new TF1("kaonFit","gaus", 0.4, 0.55); 
	kaonMassFit->SetParameter(1, pid_to_mass(321)); 

	temp_mass[b]->Fit("pionFit","RQ"); 
	temp_mass[b]->Fit("kaonFit","RQ"); 

	TF1 *temp_mfit = new TF1(Form("f_mass_pid%d_slice%d_sect%d",  histos->GetPid(), b, s),"gaus(0)+gaus(3)", 0.0, 0.75); 
	temp_mfit->SetParameter(0, pionMassFit->GetParameter(0)); 
	temp_mfit->SetParameter(1, pionMassFit->GetParameter(1)); 
	temp_mfit->SetParameter(2, pionMassFit->GetParameter(2)); 
	temp_mfit->SetParameter(3, kaonMassFit->GetParameter(0)); 
	temp_mfit->SetParameter(4, kaonMassFit->GetParameter(1)); 
	temp_mfit->SetParameter(5, kaonMassFit->GetParameter(2)); 
	//	temp_mass[b]->Fit(Form("f_mass_pid%d_slice%d_sect%d",  histos->GetPid(), b, s),"RQ"); 
	temp_mass_fit.push_back(temp_mfit); 

	// Now doing beta fits 
	// First we need to guess where the peak 
	// should be so that we can have a nice fit 
	double mom = slices->bins[b]; 
	double pionPeak = mom/sqrt(pow(mom,2)+pow(pid_to_mass(211),2)); 
	double kaonPeak = mom/sqrt(pow(mom,2)+pow(pid_to_mass(321),2)); 

	TF1 *pionBetaFit = new TF1("pionBetaFit","gaus", pionPeak *0.95, pionPeak*1.05); 
	pionMassFit->SetParameter(1, pionPeak); 

	TF1 *kaonBetaFit = new TF1("kaonBetaFit","gaus", kaonPeak *0.96, kaonPeak*1.04); 
	kaonMassFit->SetParameter(1, kaonPeak); 

	temp_beta[b]->Fit("pionBetaFit","RQ"); 
	temp_beta[b]->Fit("kaonBetaFit","RQ"); 

	TF1 *temp_bfit = new TF1(Form("f_beta_pid%d_slice%d_sect%d",  histos->GetPid(), b, s),"gaus(0)+gaus(3)", 0.2, 1.2); 
	temp_bfit->SetParameter(0, pionBetaFit->GetParameter(0)); 
	temp_bfit->SetParameter(1, pionBetaFit->GetParameter(1)); 
	temp_bfit->SetParameter(2, pionBetaFit->GetParameter(2)); 
	temp_bfit->SetParameter(3, kaonBetaFit->GetParameter(0)); 
	temp_bfit->SetParameter(4, kaonBetaFit->GetParameter(1)); 
	temp_bfit->SetParameter(5, kaonBetaFit->GetParameter(2)); 
	//	temp_mass[b]->Fit(Form("f_mass_pid%d_slice%d_sect%d",  histos->GetPid(), b, s),"RQ"); 
	temp_beta_fit.push_back(temp_bfit); 
	
	std::cout << "[MesonFittingService::Slice] Finished momentum slice " << b << " for sector " << s << std::endl; 
      }

      slices->beta.push_back(temp_beta); 
      slices->deltaBeta.push_back(temp_deltaBeta); 
      slices->mass.push_back(temp_mass); 
      slices->mass_fit.push_back(temp_mass_fit); 
      slices->beta_fit.push_back(temp_beta_fit); 
    }
  }

};

class MesonCutEfficiencyService {
public:
  MesonCutEfficiencyService(MesonSlices *s) : slices(s){
    fIntegrator.SetNumberSteps(10000);

    // integrate up to upper mass cut 
    fIntegrator.SetUpperLimit(0.75); 
  }

  ~MesonCutEfficiencyService(){
  }
  

  void Execute(){
    // If we haven't already 
    SetupCutValues(); 

    // setup the functions 
    for(int s=0; s<slices->mass.size(); s++){

      std::vector<TH1D*> temp_eff; 
      std::vector<TH1D*> temp_cont; 
      std::vector<TH1D*> temp_stat; 

      // keep track of best cut as funct of momentum 
      std::vector<double> bestCutValue; 

      for(int b=0; b<slices->mass[s].size(); b++){
	TF1 *pionFit = new TF1("pionFit", "gaus", slices->mass[s][b]->GetXaxis()->GetBinLowEdge(1), 
			       slices->mass[s][b]->GetXaxis()->GetBinUpEdge( slices->mass[s][b]->GetXaxis()->GetNbins() )); 
	TF1 *kaonFit = new TF1("kaonFit", "gaus", slices->mass[s][b]->GetXaxis()->GetBinLowEdge(1), 
			       slices->mass[s][b]->GetXaxis()->GetBinUpEdge( slices->mass[s][b]->GetXaxis()->GetNbins() )); 
	
	pionFit->SetParameter(0, slices->mass_fit[s][b]->GetParameter(0)); 
	pionFit->SetParameter(1, slices->mass_fit[s][b]->GetParameter(1)); 
	pionFit->SetParameter(2, slices->mass_fit[s][b]->GetParameter(2)); 
	kaonFit->SetParameter(0, slices->mass_fit[s][b]->GetParameter(3)); 
	kaonFit->SetParameter(1, slices->mass_fit[s][b]->GetParameter(4)); 
	kaonFit->SetParameter(2, slices->mass_fit[s][b]->GetParameter(5)); 

	TH1D *h1_eff = new TH1D(Form("h1_eff_slice%d_sect%d",b,s), Form("h1_eff_slice%d_sect%d",b,s), fNumberCutValues, fCutMin, fCutMax); 
	TH1D *h1_cont = new TH1D(Form("h1_cont_slice%d_sect%d",b,s), Form("h1_cont_slice%d_sect%d",b,s), fNumberCutValues, fCutMin, fCutMax); 
	TH1D *h1_stat = new TH1D(Form("h1_stat_slice%d_sect%d",b,s), Form("h1_stat_slice%d_sect%d",b,s), fNumberCutValues, fCutMin, fCutMax); 

	// total kaon count 
	fIntegrator.SetLowerLimit(-0.2);
	fIntegrator.SetFunction(kaonFit);
	double kaonTotal = fIntegrator.Integrate(); 
	
	for(int c=0; c<fCutValues.size(); c++){
	  fIntegrator.SetFunction(pionFit);
	  fIntegrator.SetLowerLimit(fCutValues[c]);
	  double pionVal = fIntegrator.Integrate(); 

	  fIntegrator.SetFunction(kaonFit);
	  double kaonVal = fIntegrator.Integrate(); 

	  double eff = kaonVal/kaonTotal; 
	  double cont = 1-kaonVal/(kaonVal + pionVal); 
	  double stat = eff-cont; 

	  h1_eff->SetBinContent(c+1, eff); 
	  h1_cont->SetBinContent(c+1, cont); 
	  h1_stat->SetBinContent(c+1, stat); 
	}

	temp_eff.push_back(h1_eff); 	
	temp_cont.push_back(h1_cont); 	
	temp_stat.push_back(h1_stat); 	
	
	bestCutValue.push_back(h1_stat->GetBinCenter( h1_stat->GetMaximumBin() )); 

	std::cout << "[MesonCutEfficiencyService] Integrating momentum bin = " << b << " for sector = " << s << std::endl;
      }

      efficiency.push_back(temp_eff); 
      contamination.push_back(temp_cont); 
      test_statistic.push_back(temp_stat); 

      fBestCutGraph[s] = new TGraph(slices->bins.size(), slices->bins.data(), bestCutValue.data());
      fBestCutGraph[s]->SetName(Form("g_bestcut_sect%d",s)); 
    }

  }

  void SetCutMin(double min){
    fCutMin = min; 

    fIntegrator.SetLowerLimit(fCutMin); 
  }

  void SetCutMax(double max){
    fCutMax = max; 
  }

  void SetupCutValues(){
    fCutValues.clear();

    for(int i=0; i<fNumberCutValues; i++){
      fCutValues.push_back(i*(fCutMax-fCutMin)/fNumberCutValues +fCutMin);
    }
  }

  void Save(TFile *out){
    out->cd();

    out->mkdir("efficiency"); 
    out->cd("efficiency/"); 

    for(int s=0; s<efficiency.size(); s++){
      for(int b=0; b<efficiency[s].size(); b++){
	efficiency[s][b]->Write(); 
      }
    }

    out->mkdir("contamination"); 
    out->cd("contamination/"); 

    for(int s=0; s<contamination.size(); s++){
      for(int b=0; b<contamination[s].size(); b++){
	contamination[s][b]->Write(); 
      }
    }

    out->mkdir("test_statistic"); 
    out->cd("test_statistic/"); 

    for(int s=0; s<test_statistic.size(); s++){
      for(int b=0; b<test_statistic[s].size(); b++){
	test_statistic[s][b]->Write(); 
      }
    }

    for(int s=0; s<6; s++){
      fBestCutGraph[s]->Write(); 
    }

    out->cd(); 
  } 

  MesonSlices *slices; 
  std::vector<std::vector<TH1D*> > efficiency; 
  std::vector<std::vector<TH1D*> > contamination; 
  std::vector<std::vector<TH1D*> > test_statistic; 
  TGraph                          *fBestCutGraph[6]; 

protected:
  const static int fNumberCutValues = 100;
  double fCutMin, fCutMax; 
  std::vector<double>              fCutValues; 
  TF1Integrator_Simpsons           fIntegrator;

};

class MesonCutEfficiencyPlottingService {
public:
  MesonCutEfficiencyPlottingService(MesonHistograms *h, MesonCutEfficiencyService *s) : histos(h), efficiencyService(s) {
    fOutputPath = "unset"; 
  }
  
  void SetOutputPath(std::string p){
    fOutputPath = p; 
  }

  std::string GetOutputPath() const {
    return fOutputPath; 
  }

  void Execute(){

    if(fOutputPath != "unset"){

      gStyle->SetOptTitle(0);
      gStyle->SetOptStat(0);
      
      TCanvas *can = new TCanvas("can","",800, 500); 
      TLatex title; 
      title.SetNDC(); 
      title.SetTextFont(102); 
      title.SetTextSize(0.05); 

      int fillColor = 13; 
      double fillAlpha = 0.4;

      Global::Visualization::SetCustomPalette(); 

      // draw best cut on 2-d distributions 
      for (int s=0; s<6; s++){
	histos->h2_p_tofmass[s+1]->Draw("colz"); 
	gPad->SetLogz(); 
	gPad->SetGridx(); 
	gPad->SetGridy(); 
	gPad->SetMargin(0.15,0.15, 0.15, 0.15);

	efficiencyService->fBestCutGraph[s]->SetLineStyle(8);
	efficiencyService->fBestCutGraph[s]->SetLineWidth(2);
	efficiencyService->fBestCutGraph[s]->Draw("lsame");

	title.DrawLatex(0.44, 0.92, Form("sector %d", s+1)); 
	title.DrawLatex(0.47, 0.08, "p (GeV/c)"); 

	can->Print(Form("%sBestTOFCutSector%d.png",fOutputPath.c_str(),s)); 
      }

      // draw the plots of eff/ cont/ stat 
      for(int s=0; s<efficiencyService->efficiency.size(); s++){
	for(int b=0; b<efficiencyService->efficiency[s].size(); b++){
	  efficiencyService->efficiency[s][b]->SetMaximum(1.0);
	  efficiencyService->efficiency[s][b]->SetMinimum(0.0);
	  efficiencyService->efficiency[s][b]->SetLineColor(99);
	  efficiencyService->contamination[s][b]->SetLineColor(77);
	  efficiencyService->test_statistic[s][b]->SetLineColor(55);

	  efficiencyService->efficiency[s][b]->Draw("l");
	  efficiencyService->contamination[s][b]->Draw("lsame");
	  efficiencyService->test_statistic[s][b]->Draw("lsame");

	  gPad->SetGridx();
	  gPad->SetGridy();
	  gPad->SetMargin(0.15,0.15, 0.15, 0.15);

	  title.DrawLatex(0.21, 0.92, Form("Efficiency K^{+}, Sector %d, p = %.3f (Gev/c)",s,efficiencyService->slices->bins[b])); 
	  title.DrawLatex(0.45, 0.08, "M_{cut}"); 

	  can->Print(Form("%sEffSlice%dSector%d.png",fOutputPath.c_str(),b,s)); 
	}
      }

    } else {
      std::cerr << "[MesonCutEfficiencyPlottingService] Output path not valid! " << std::endl;
    }
    
    
  }
  
  MesonCutEfficiencyService *efficiencyService;
  MesonHistograms           *histos; 
protected:
  std::string fOutputPath; 

};

class MesonPlottingService {
public:
  MesonPlottingService(MesonHistograms *h, MesonSlices *s) : histos(h), slices(s) { 
    fOutputPath = "unset"; 
  }
  ~MesonPlottingService(){

  }
  
  void SetOutputPath(std::string p){
    fOutputPath = p; 
  }

  std::string GetOutputPath() const {
    return fOutputPath; 
  }


  MesonSlices     *slices; 
  MesonHistograms *histos; 

  void Execute(){
    if(fOutputPath != "unset"){

      gStyle->SetOptTitle(0);
      gStyle->SetOptStat(0);
      
      TCanvas *can = new TCanvas("can","",800, 800); 
      TLatex title; 
      title.SetNDC(); 
      title.SetTextFont(102); 
      title.SetTextSize(0.05); 

      int fillColor = 13; 
      double fillAlpha = 0.4;

      int numberBins = slices->bins.size() +3;
      int numberPages = ceil(numberBins/4); 

      //      Global::Visualization::SetBentCoolWarmPalette();
      Global::Visualization::SetCustomPalette(); 


      for(int s=0; s<6; s++){
	can->Print(Form("%sCompareSlicesSector%d.pdf[", fOutputPath.c_str(), s));

	int bin=0; 
	for(int p=0; p<numberPages; p++){
	  can->Divide(3, 4); 

	  for(int row=1; row<5; row++){
	    int index = 3*row -2; 

	    if (p == 0 && row == 1){
	      can->cd(1); 
	      gPad->SetLogz();
	      gPad->SetMargin(0.2, 0.01, 0.2, 0.01); 
	      histos->h2_p_beta[s+1]->Draw("colz");
	      title.DrawLatex(0.58, 0.08, "p (GeV/c)");

	      can->cd(2); 
	      gPad->SetLogz();
	      gPad->SetMargin(0.2, 0.01, 0.2, 0.01); 
	      histos->h2_p_dbeta[s+1]->Draw("colz");
	      title.DrawLatex(0.58, 0.08, "p (GeV/c)");

	      can->cd(3); 
	      gPad->SetLogz();
	      gPad->SetMargin(0.2, 0.01, 0.2, 0.01); 
	      histos->h2_p_tofmass[s+1]->Draw("colz");
	      title.DrawLatex(0.58, 0.08, "p (GeV/c)");
	    }

	    else {
	      
	      if (bin <numberBins){
		can->cd(index);
		gPad->SetMargin(0.2, 0.01, 0.2, 0.01); 
		gPad->SetLogy(); 
		slices->beta[s][bin]->SetFillColorAlpha(fillColor,fillAlpha);
		slices->beta[s][bin]->SetLineColor(fillColor);
		slices->beta[s][bin]->Draw();
		title.DrawLatex(0.24, 0.93, Form("p = %.3f (GeV/c)",slices->bins[bin])); 
		title.DrawLatex(0.58, 0.08, "#beta");
		
		can->cd(index+1); 
		gPad->SetMargin(0.2, 0.01, 0.2, 0.01); 
		gPad->SetLogy(); 
		slices->deltaBeta[s][bin]->SetFillColorAlpha(fillColor,fillAlpha);
		slices->deltaBeta[s][bin]->SetLineColor(fillColor);
		slices->deltaBeta[s][bin]->Draw();
		title.DrawLatex(0.24, 0.93, Form("p = %.3f (GeV/c)",slices->bins[bin])); 
		title.DrawLatex(0.58, 0.08, "#Delta#beta");
		
		can->cd(index+2); 
		gPad->SetMargin(0.2, 0.01, 0.2, 0.01); 
		//		gPad->SetLogy(); 
		slices->mass[s][bin]->SetFillColorAlpha(fillColor,fillAlpha);
		slices->mass[s][bin]->SetLineColor(fillColor);
		slices->mass[s][bin]->Draw();

		TF1 *pionFit = new TF1("pionFit", "gaus", slices->mass[s][bin]->GetXaxis()->GetBinLowEdge(1), 
				       slices->mass[s][bin]->GetXaxis()->GetBinUpEdge( slices->mass[s][bin]->GetXaxis()->GetNbins() )); 
 		TF1 *kaonFit = new TF1("kaonFit", "gaus", slices->mass[s][bin]->GetXaxis()->GetBinLowEdge(1), 
				       slices->mass[s][bin]->GetXaxis()->GetBinUpEdge( slices->mass[s][bin]->GetXaxis()->GetNbins() )); 
 
		pionFit->SetParameter(0, slices->mass_fit[s][bin]->GetParameter(0)); 
		pionFit->SetParameter(1, slices->mass_fit[s][bin]->GetParameter(1)); 
		pionFit->SetParameter(2, slices->mass_fit[s][bin]->GetParameter(2)); 
		kaonFit->SetParameter(0, slices->mass_fit[s][bin]->GetParameter(3)); 
		kaonFit->SetParameter(1, slices->mass_fit[s][bin]->GetParameter(4)); 
		kaonFit->SetParameter(2, slices->mass_fit[s][bin]->GetParameter(5)); 

		pionFit->SetLineColor(99); 
		kaonFit->SetLineColor(77);
		slices->mass_fit[s][bin]->SetLineColor(55); 

		pionFit->Draw("same"); 
		kaonFit->Draw("same");
		//		slices->mass_fit[s][bin]->Draw("same"); 

		title.DrawLatex(0.24, 0.93, Form("p = %.3f (GeV/c)",slices->bins[bin])); 
		title.DrawLatex(0.58, 0.08, "M_{TOF}");
		
		bin++; 
	      }
	    }
	  }
	  
	  can->Print(Form("%sCompareSlicesSector%d.pdf", fOutputPath.c_str(), s));
	  can->Clear();
	}
	can->Print(Form("%sCompareSlicesSector%d.pdf]", fOutputPath.c_str(), s));
      }

      // print the fit mass distr. 
      TCanvas *sliceCan = new TCanvas("sliceCan", "", 800, 500); 
      
      for(int s=0; s<slices->mass.size(); s++){
	for(int b=0; b<slices->mass[s].size(); b++){
	  sliceCan->Clear(); 

	  slices->mass[s][b]->Draw();
	  gPad->SetGridx(); 
	  gPad->SetGridy(); 
	  gPad->SetMargin(0.15, 0.15, 0.15, 0.15); 
	  //		gPad->SetLogy(); 
	  slices->mass[s][b]->SetFillColorAlpha(fillColor,fillAlpha);
	  slices->mass[s][b]->SetLineColor(fillColor);
	  slices->mass[s][b]->Draw();
	  
	  TF1 *pionFit = new TF1("pionFit", "gaus", slices->mass[s][b]->GetXaxis()->GetBinLowEdge(1), 
				 slices->mass[s][b]->GetXaxis()->GetBinUpEdge( slices->mass[s][b]->GetXaxis()->GetNbins() )); 
	  TF1 *kaonFit = new TF1("kaonFit", "gaus", slices->mass[s][b]->GetXaxis()->GetBinLowEdge(1), 
				 slices->mass[s][b]->GetXaxis()->GetBinUpEdge( slices->mass[s][b]->GetXaxis()->GetNbins() )); 
	  
	  pionFit->SetParameter(0, slices->mass_fit[s][b]->GetParameter(0)); 
	  pionFit->SetParameter(1, slices->mass_fit[s][b]->GetParameter(1)); 
	  pionFit->SetParameter(2, slices->mass_fit[s][b]->GetParameter(2)); 
	  kaonFit->SetParameter(0, slices->mass_fit[s][b]->GetParameter(3)); 
	  kaonFit->SetParameter(1, slices->mass_fit[s][b]->GetParameter(4)); 
	  kaonFit->SetParameter(2, slices->mass_fit[s][b]->GetParameter(5)); 
	  
	  pionFit->SetLineColor(99); 
	  kaonFit->SetLineColor(77);
	  slices->mass_fit[s][b]->SetLineColor(55); 
	  
	  pionFit->Draw("same"); 
	  kaonFit->Draw("same");
	  //		slices->mass_fit[s][bin]->Draw("same"); 
	  
	  title.DrawLatex(0.24, 0.93, Form("p = %.3f (GeV/c)",slices->bins[b])); 
	  title.DrawLatex(0.58, 0.08, "M_{TOF}");
	  
	  sliceCan->Print(Form("%sFitTofMassSlice%dSector%d.png",fOutputPath.c_str(), b, s)); 
	}
      }
      
      
    } else {
      std::cerr << "[MesonPlottingService] Output path is not set. " << std::endl; 
    }
  }

protected: 
  std::string fOutputPath; 

};

int main(int argc, char * argv[]){

    // Setup Options
    h22Options opts;
    opts.args["INPUT"].args = "";
    opts.args["INPUT"].type = 1;
    opts.args["INPUT"].name = "Input file";
    opts.set(argc,argv);

    
    if (opts.args["INPUT"].args != ""){
      
      MesonHistograms pip("pip", 211);
      pip.Load(opts.args["INPUT"].args);
            
      MesonHistograms pim("pim", -211);
      pim.Load(opts.args["INPUT"].args);
      
      MesonHistograms kp("kp", 321);
      kp.Load(opts.args["INPUT"].args);
      
      MesonHistograms km("km", -321);
      km.Load(opts.args["INPUT"].args);
      

      TFile *out = new TFile(opts.args["OUT"].args.c_str(), "recreate");

      MesonFittingService fitPip(&pip);
      fitPip.Execute();
      fitPip.Save(out);

      MesonFittingService fitPim(&pim);
      fitPim.Execute();
      fitPim.Save(out);

      MesonFittingService fitKp(&kp);
      fitKp.Execute();
      fitKp.Save(out);

      MesonFittingService fitKm(&km);
      fitKm.Execute();
      fitKm.Save(out);

      MesonCutEfficiencyService effKp(fitKp.slices); 
      effKp.SetCutMin(0.2); 
      effKp.SetCutMax(0.6); 
      effKp.Execute(); 
      effKp.Save(out); 

      MesonCutEfficiencyPlottingService plotKpEff(&kp, &effKp);
      plotKpEff.SetOutputPath("/volatile/clas12/dmriser/plots/pid/kp/");
      plotKpEff.Execute(); 

      MesonPlottingService plotPip(&pip, fitPip.slices);
      plotPip.SetOutputPath("/volatile/clas12/dmriser/plots/pid/pip/"); 
      plotPip.Execute();

      MesonPlottingService plotPim(&pim, fitPim.slices);
      plotPim.SetOutputPath("/volatile/clas12/dmriser/plots/pid/pim/"); 
      plotPim.Execute();

      MesonPlottingService plotKp(&kp, fitKp.slices);
      plotKp.SetOutputPath("/volatile/clas12/dmriser/plots/pid/kp/"); 
      plotKp.Execute();

      MesonPlottingService plotKm(&km, fitKm.slices);
      plotKm.SetOutputPath("/volatile/clas12/dmriser/plots/pid/km/"); 
      plotKm.Execute();

      out->Close(); 

    } else {
      std::cerr << "[main] No input file provided with flag -INPUT=file.root" << std::endl;
    }


    return 0;
}

