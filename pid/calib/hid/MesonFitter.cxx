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

    //    for(std::vector<TH1D*>::iterator it=beta.begin(); it!=beta.end(); ++it){ delete *it; }
    //    for(std::vector<TH1D*>::iterator it=deltaBeta.begin(); it!=deltaBeta.end(); ++it){ delete *it; }
    //    for(std::vector<TH1D*>::iterator it=mass.begin(); it!=mass.end(); ++it){ delete *it; }
  }

  std::vector<std::vector<TH1D*> >  beta; 
  std::vector<std::vector<TH1D*> > deltaBeta; 
  std::vector<std::vector<TH1D*> > mass; 
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

    fPMin = 0.5; 
    fPMax = 3.5; 
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
  const static int fNSlices = 35; 
  std::vector<double> fBins; 
  double fPMin, fPMax, fPWidth; 

  void Slice(){
    for(int s=0; s<6; s++){
      std::vector<TH1D*> temp_beta; 
      std::vector<TH1D*> temp_deltaBeta; 
      std::vector<TH1D*> temp_mass; 

      for(int b=0; b<fNSlices; b++){
	double p = b*fPWidth + fPMin; 
	int startBin = histos->h2_p_beta[s+1]->GetXaxis()->FindBin(p); 
	int stopBin  = histos->h2_p_beta[s+1]->GetXaxis()->FindBin(p+fPWidth); 
	temp_beta     .push_back(histos->h2_p_beta[s+1]->ProjectionY(Form("h1_beta_pid%d_slice%d_sect%d",histos->GetPid(),b,s), startBin, stopBin));
	temp_deltaBeta.push_back(histos->h2_p_dbeta[s+1]->ProjectionY(Form("h1_dbeta_pid%d_slice%d_sect%d",histos->GetPid(),b,s), startBin, stopBin));
	temp_mass     .push_back(histos->h2_p_tofmass[s+1]->ProjectionY(Form("h1_mass_pid%d_slice%d_sect%d",histos->GetPid(),b,s), startBin, stopBin));
	std::cout << "[MesonFittingService::Slice] Finished momentum slice " << b << " for sector " << s << std::endl; 
      }

      slices->beta.push_back(temp_beta); 
      slices->deltaBeta.push_back(temp_deltaBeta); 
      slices->mass.push_back(temp_mass); 
    }
  }

};

class MesonPlottingService {
public:
  MesonPlottingService(MesonSlices *s) : slices(s) { 
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


  MesonSlices *slices; 

  void Execute(){
    if(fOutputPath != "unset"){

      gStyle->SetOptTitle(0);
      gStyle->SetOptStat(0);
      
      TCanvas *can = new TCanvas("can","",800, 800); 
      TLatex title; 
      title.SetNDC(); 
      title.SetTextFont(102); 
      title.SetTextSize(0.05); 

      int numberBins = slices->bins.size();
      int numberPages = ceil(numberBins/4); 

      for(int s=0; s<6; s++){
	can->Print(Form("%sCompareSlicesSector%d.pdf[", fOutputPath.c_str(), s));

	int bin=0; 
	for(int p=0; p<numberPages; p++){
	  can->Divide(3, 4); 

	  for(int row=1; row<5; row++){
	    int index = 3*row -2; 
	    
	    if (bin <numberBins){
	      can->cd(index);
	      gPad->SetMargin(0.2, 0.01, 0.2, 0.01); 
	      gPad->SetLogy(); 
	      slices->beta[s][bin]->SetFillColorAlpha(99,0.4);
	      slices->beta[s][bin]->SetLineColor(99);
	      slices->beta[s][bin]->Draw();
	      title.DrawLatex(0.24, 0.93, Form("p = %.3f (GeV/c)",slices->bins[bin])); 
	      title.DrawLatex(0.58, 0.08, "#beta");
	      
	      can->cd(index+1); 
	      gPad->SetMargin(0.2, 0.01, 0.2, 0.01); 
	      gPad->SetLogy(); 
	      slices->deltaBeta[s][bin]->SetFillColorAlpha(99,0.4);
	      slices->deltaBeta[s][bin]->SetLineColor(99);
	      slices->deltaBeta[s][bin]->Draw();
	      title.DrawLatex(0.24, 0.93, Form("p = %.3f (GeV/c)",slices->bins[bin])); 
	      title.DrawLatex(0.58, 0.08, "#Delta#beta");

	      can->cd(index+2); 
	      gPad->SetMargin(0.2, 0.01, 0.2, 0.01); 
	      gPad->SetLogy(); 
	      slices->mass[s][bin]->SetFillColorAlpha(99,0.4);
	      slices->mass[s][bin]->SetLineColor(99);
	      slices->mass[s][bin]->Draw();
	      title.DrawLatex(0.24, 0.93, Form("p = %.3f (GeV/c)",slices->bins[bin])); 
	      title.DrawLatex(0.58, 0.08, "M_{TOF}");
	    }

	    bin++; 
	  }
	  
	  can->Print(Form("%sCompareSlicesSector%d.pdf", fOutputPath.c_str(), s));
	  can->Clear();
	}
	can->Print(Form("%sCompareSlicesSector%d.pdf]", fOutputPath.c_str(), s));
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
      /*      
      MesonHistograms pim("pim", -211);
      pim.Load(opts.args["INPUT"].args);
      
      MesonHistograms kp("kp", 321);
      kp.Load(opts.args["INPUT"].args);
      
      MesonHistograms km("km", -321);
      km.Load(opts.args["INPUT"].args);
      */

      TFile *out = new TFile(opts.args["OUT"].args.c_str(), "recreate");

      MesonFittingService fitPip(&pip);
      fitPip.Execute();
      fitPip.Save(out);
      /*
      MesonFittingService fitPim(&pim);
      fitPim.Execute();
      fitPim.Save(out);

      MesonFittingService fitKp(&kp);
      fitKp.Execute();
      fitKp.Save(out);

      MesonFittingService fitKm(&km);
      fitKm.Execute();
      fitKm.Save(out);
      */

      MesonPlottingService plotPip(fitPip.slices);
      plotPip.SetOutputPath("/volatile/clas12/dmriser/plots/pid/"); 
      plotPip.Execute();

      out->Close(); 

    } else {
      std::cerr << "[main] No input file provided with flag -INPUT=file.root" << std::endl;
    }


    return 0;
}

