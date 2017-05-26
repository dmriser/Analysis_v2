#ifndef signal_h
#define signal_h

#include "Bins.h"
#include "Constants.h"
#include "PIDHistograms.h"
#include "Fits.h"
#include "Histograms.h"
#include "IntegratedHistograms.h"

#include "TF1.h"

class SignalBackgroundFitter { 

 public:
 SignalBackgroundFitter(PidHistos *h, std::string name, int index) : fHistos(h), fName(name), fMesonIndex(index) {
    fBins = Bins::GetInstance(fMesonIndex);
    Init(); 
  }
  
  double signal_p[constants::MAX_BINS_X][constants::MAX_BINS_Z][constants::MAX_BINS_PT];
  double signal_k[constants::MAX_BINS_X][constants::MAX_BINS_Z][constants::MAX_BINS_PT];
  double background_p[constants::MAX_BINS_X][constants::MAX_BINS_Z][constants::MAX_BINS_PT];
  double background_k[constants::MAX_BINS_X][constants::MAX_BINS_Z][constants::MAX_BINS_PT];

  TF1 *fit_p[constants::MAX_BINS_X][constants::MAX_BINS_Z][constants::MAX_BINS_PT];
  TF1 *fit_k[constants::MAX_BINS_X][constants::MAX_BINS_Z][constants::MAX_BINS_PT];
  TF1 *fit_bg[constants::MAX_BINS_X][constants::MAX_BINS_Z][constants::MAX_BINS_PT];
  TF1 *fit_tot[constants::MAX_BINS_X][constants::MAX_BINS_Z][constants::MAX_BINS_PT];

  void Fit(double min, double cut, double max){
    for(int i=0; i<fBins->GetXBins()->GetNumber()+1; i++){
      for(int j=0; j<fBins->GetZBins()->GetNumber()+1; j++){
	for(int k=0; k<fBins->GetPtBins()->GetNumber()+1; k++){
	  fHistos->h1_tof_mass[i][j][k]->Fit(Form("fit_p_%s_x%d_z%d_pt%d_%s", constants::Names::mesons[fMesonIndex].c_str(),
						  i, j, k, fName.c_str()), "RQ");
	  fHistos->h1_tof_mass[i][j][k]->Fit(Form("fit_k_%s_x%d_z%d_pt%d_%s", constants::Names::mesons[fMesonIndex].c_str(),
						  i, j, k, fName.c_str()), "RQ");

	  fit_tot[i][j][k]->SetParameter(0, fit_p[i][j][k]->GetParameter(0)); 
	  fit_tot[i][j][k]->SetParameter(1, fit_p[i][j][k]->GetParameter(1)); 
	  fit_tot[i][j][k]->SetParameter(2, fit_p[i][j][k]->GetParameter(2)); 
	  fit_tot[i][j][k]->SetParameter(3, fit_k[i][j][k]->GetParameter(0)); 
	  fit_tot[i][j][k]->SetParameter(4, fit_k[i][j][k]->GetParameter(1)); 
	  fit_tot[i][j][k]->SetParameter(5, fit_k[i][j][k]->GetParameter(2)); 

	  fHistos->h1_tof_mass[i][j][k]->Fit(Form("fit_tot_%s_x%d_z%d_pt%d_%s", constants::Names::mesons[fMesonIndex].c_str(),
						  i, j, k, fName.c_str()), "RQ");
	  
	  fit_p[i][j][k] ->SetRange(0.0, 1.0); 
	  fit_k[i][j][k] ->SetRange(0.0, 1.0); 
	  fit_bg[i][j][k]->SetRange(0.0, 1.0); 

	  fit_p[i][j][k] ->SetParameter(0, fit_tot[i][j][k]->GetParameter(0));
	  fit_p[i][j][k] ->SetParameter(1, fit_tot[i][j][k]->GetParameter(1));
	  fit_p[i][j][k] ->SetParameter(2, fit_tot[i][j][k]->GetParameter(2));
	  fit_k[i][j][k] ->SetParameter(0, fit_tot[i][j][k]->GetParameter(3));
	  fit_k[i][j][k] ->SetParameter(1, fit_tot[i][j][k]->GetParameter(4));
	  fit_k[i][j][k] ->SetParameter(2, fit_tot[i][j][k]->GetParameter(5));
	  fit_bg[i][j][k]->SetParameter(0, fit_tot[i][j][k]->GetParameter(6));
	  fit_bg[i][j][k]->SetParameter(1, fit_tot[i][j][k]->GetParameter(7));
	  fit_bg[i][j][k]->SetParameter(2, fit_tot[i][j][k]->GetParameter(8));

	  // initialize the stuff to zero 
	  signal_p[i][j][k]     = fit_p[i][j][k]->Integral(min, cut)/(fit_p[i][j][k]->Integral(min, cut)+fit_k[i][j][k]->Integral(min, cut)); 
	  signal_k[i][j][k]     = fit_k[i][j][k]->Integral(cut, max)/(fit_p[i][j][k]->Integral(cut, max)+fit_k[i][j][k]->Integral(cut, max)); 
	  background_p[i][j][k] = 1.0 - signal_k[i][j][k]; 
	  background_k[i][j][k] = 1.0 - signal_p[i][j][k]; 

	  std::cout << Form("Fitting bin %d %d %d, signal_p = %.3f, signal_k = %.3f, background_p = %.3f, background_k = %.3f ", i, j, k, signal_p[i][j][k], signal_k[i][j][k], 
			    background_p[i][j][k], background_k[i][j][k]) << std::endl; 
	}
      }
    }
  }

  void CorrectAsymmetry(Fits *pion, Fits *kaon){

    pionFit = new Fits(pion, "corrected"); 
    kaonFit = new Fits(kaon, "corrected"); 

    for(int i=0; i<fBins->GetXBins()->GetNumber()+1; i++){
      for(int j=0; j<fBins->GetZBins()->GetNumber()+1; j++){
	for(int k=0; k<fBins->GetPtBins()->GetNumber()+1; k++){
	  double obsP = pion->fit_asym[0][i][j][k]->GetParameter(0);  
	  double obsK = kaon->fit_asym[0][i][j][k]->GetParameter(0);  

	  double trueP = (obsK*background_k[i][j][k]-obsP*signal_k[i][j][k])/(background_k[i][j][k]*background_p[i][j][k]-signal_k[i][j][k]*signal_p[i][j][k]); 
	  double trueK = (signal_p[i][j][k]*obsK-obsP*background_p[i][j][k])/(signal_k[i][j][k]*signal_p[i][j][k]-background_k[i][j][k]*background_p[i][j][k]); 

	  pionFit->fit_asym[0][i][j][k]->SetParameter(0, trueP);
	  kaonFit->fit_asym[0][i][j][k]->SetParameter(0, trueK);

	  std::cout << Form("Correcting pion %.4f -> %.4f, kaon %.4f -> %.4f",obsP,trueP,obsK,trueK) << std::endl;
	}
      }
    }    

    pionInt = new IntegratedHistos(pionFit, "corr", Meson::kPionPositive); 
    kaonInt = new IntegratedHistos(kaonFit, "corr", Meson::kKaonPositive); 
  }

  Fits *GetPionFit() const {
    return pionFit; 
  }

  Fits *GetKaonFit() const {
    return kaonFit; 
  }

  IntegratedHistos *GetPionHistos() const {
    return pionInt; 
  }

  IntegratedHistos *GetKaonHistos() const {
    return kaonInt; 
  }

 protected:
  int         fMesonIndex; 
  Bins       *fBins; 
  PidHistos  *fHistos; 
  std::string fName; 

  Fits             *pionFit; 
  Fits             *kaonFit; 
  IntegratedHistos *pionInt; 
  IntegratedHistos *kaonInt; 

  void Init(){
    for(int i=0; i<fBins->GetXBins()->GetNumber()+1; i++){
      for(int j=0; j<fBins->GetZBins()->GetNumber()+1; j++){
	for(int k=0; k<fBins->GetPtBins()->GetNumber()+1; k++){
	  fit_p[i][j][k] = new TF1(Form("fit_p_%s_x%d_z%d_pt%d_%s", constants::Names::mesons[fMesonIndex].c_str(),
					i, j, k, fName.c_str()), "gaus", 0.05, 0.3);
	  fit_k[i][j][k] = new TF1(Form("fit_k_%s_x%d_z%d_pt%d_%s", constants::Names::mesons[fMesonIndex].c_str(),
					i, j, k, fName.c_str()), "gaus", 0.43, 0.60);
	  fit_bg[i][j][k] = new TF1(Form("fit_bg_%s_x%d_z%d_pt%d_%s", constants::Names::mesons[fMesonIndex].c_str(),
					i, j, k, fName.c_str()), "pol2");
	  fit_tot[i][j][k] = new TF1(Form("fit_tot_%s_x%d_z%d_pt%d_%s", constants::Names::mesons[fMesonIndex].c_str(),
					i, j, k, fName.c_str()), "gaus(0) + gaus(3) + pol2(6)");

	  // initialize the stuff to zero 
	  signal_p[i][j][k]     = 0.0; 
	  signal_k[i][j][k]     = 0.0; 
	  background_p[i][j][k] = 0.0; 
	  background_k[i][j][k] = 0.0; 
	}
      }
    }
  }
};

#endif
