#ifndef signal_h
#define signal_h

#include "Bins.h"
#include "Constants.h"
#include "PIDHistograms.h"
#include "Fits.h"
#include "Histograms.h"
#include "IntegratedHistograms.h"

#include "TF1.h"
#include "TFitResult.h"
#include "TH1.h"
#include "TMatrixD.h"

class SignalBackgroundFitter { 

 public:
 SignalBackgroundFitter(PidHistos *h, std::string name, int index) : fHistos(h), fName(name), fMesonIndex(index) {
    fBins = Bins::GetInstance(fMesonIndex);
    Init(); 
    fInputFile = new TFile(); 
  }

 SignalBackgroundFitter(std::string name, int index) : fName(name), fMesonIndex(index) {
    fBins = Bins::GetInstance(fMesonIndex);
    Init(); 
    fInputFile = new TFile(); 
  }

  ~SignalBackgroundFitter(){
    if(fInputFile){
      if (fInputFile->IsOpen()){
	fInputFile->Close(); 
      }
    }
  }
  
  double signal_p[constants::MAX_BINS_X][constants::MAX_BINS_Z][constants::MAX_BINS_PT];
  double signal_k[constants::MAX_BINS_X][constants::MAX_BINS_Z][constants::MAX_BINS_PT];
  double err_p[constants::MAX_BINS_X][constants::MAX_BINS_Z][constants::MAX_BINS_PT];
  double err_k[constants::MAX_BINS_X][constants::MAX_BINS_Z][constants::MAX_BINS_PT];
  double background_p[constants::MAX_BINS_X][constants::MAX_BINS_Z][constants::MAX_BINS_PT];
  double background_k[constants::MAX_BINS_X][constants::MAX_BINS_Z][constants::MAX_BINS_PT];

  TF1 *fit_p[constants::MAX_BINS_X][constants::MAX_BINS_Z][constants::MAX_BINS_PT];
  TF1 *fit_k[constants::MAX_BINS_X][constants::MAX_BINS_Z][constants::MAX_BINS_PT];
  TF1 *fit_bg[constants::MAX_BINS_X][constants::MAX_BINS_Z][constants::MAX_BINS_PT];
  TF1 *fit_tot[constants::MAX_BINS_X][constants::MAX_BINS_Z][constants::MAX_BINS_PT];

  // pion signal 
  TH1D *h1_x_signal_p[constants::MAX_BINS_Z][constants::MAX_BINS_PT];
  TH1D *h1_z_signal_p[constants::MAX_BINS_PT][constants::MAX_BINS_X];
  TH1D *h1_pt_signal_p[constants::MAX_BINS_X][constants::MAX_BINS_Z];

  // kaon signal 
  TH1D *h1_x_signal_k[constants::MAX_BINS_Z][constants::MAX_BINS_PT];
  TH1D *h1_z_signal_k[constants::MAX_BINS_PT][constants::MAX_BINS_X];
  TH1D *h1_pt_signal_k[constants::MAX_BINS_X][constants::MAX_BINS_Z];

  double GetPionSignal(int x, int z, int pt) const {
    return signal_p[x][z][pt];
  }

  double GetKaonSignal(int x, int z, int pt) const {
    return signal_k[x][z][pt];
  }

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

	  TFitResultPtr resultOfFit = fHistos->h1_tof_mass[i][j][k]->Fit(Form("fit_tot_%s_x%d_z%d_pt%d_%s", constants::Names::mesons[fMesonIndex].c_str(),
						  i, j, k, fName.c_str()), "RQS");
	  
	  TMatrixD covMatrix = resultOfFit->GetCovarianceMatrix(); 	  
	  TMatrixD covMatrixPion(3,3); 
	  TMatrixD covMatrixKaon(3,3); 
	 
	  // the first one is easy 
	  for (int row=0; row<3; row++){
	    for (int col=0; col<3; col++){
	      covMatrixPion[row][col] = covMatrix[row][col];
	    }
	  }

	  // the next one is also easy, but different
	  for (int row=0; row<3; row++){
	    for (int col=0; col<3; col++){
	      covMatrixKaon[row][col] = covMatrix[row+3][col+3];
	    }
	  }

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
	  double p_below = fit_p[i][j][k]->Integral(min, cut); 
	  double p_above = fit_p[i][j][k]->Integral(cut, max); 
	  double k_below = fit_k[i][j][k]->Integral(min, cut); 
	  double k_above = fit_k[i][j][k]->Integral(cut, max); 

	  double p_below_err = fit_p[i][j][k]->IntegralError(min, cut, fit_p[i][j][k]->GetParameters(), covMatrixPion.GetMatrixArray()); 
	  double p_above_err = fit_p[i][j][k]->IntegralError(cut, max, fit_p[i][j][k]->GetParameters(), covMatrixPion.GetMatrixArray()); 
	  double k_below_err = fit_k[i][j][k]->IntegralError(min, cut, fit_k[i][j][k]->GetParameters(), covMatrixKaon.GetMatrixArray()); 
	  double k_above_err = fit_k[i][j][k]->IntegralError(cut, max, fit_k[i][j][k]->GetParameters(), covMatrixKaon.GetMatrixArray()); 

	  //	  signal_p[i][j][k]     = fit_p[i][j][k]->Integral(min, cut)/(fit_p[i][j][k]->Integral(min, cut)+fit_k[i][j][k]->Integral(min, cut)); 
	  //	  signal_k[i][j][k]     = fit_k[i][j][k]->Integral(cut, max)/(fit_p[i][j][k]->Integral(cut, max)+fit_k[i][j][k]->Integral(cut, max)); 
	  signal_p[i][j][k]     = p_below/(p_below+k_below); 
	  signal_k[i][j][k]     = k_above/(p_above+k_above);

	  err_p[i][j][k]        = sqrt(pow(p_below_err/(p_below+k_below),2) + pow(p_below*(p_below_err+k_below_err),2)/pow(p_below+k_below,4));
	  err_k[i][j][k]        = sqrt(pow(k_above_err/(p_above+k_above),2) + pow(k_above*(p_above_err+k_above_err),2)/pow(p_above+k_above,4));
	  background_p[i][j][k] = 1.0 - signal_k[i][j][k]; 
	  background_k[i][j][k] = 1.0 - signal_p[i][j][k]; 

	  std::cout << Form("[SignalBackgroundFitter::CorrectAsymmetry] Fitting bin %d %d %d, signal_p = %.3f, signal_k = %.3f, background_p = %.3f, background_k = %.3f ", i, j, k, signal_p[i][j][k], signal_k[i][j][k], 
			    background_p[i][j][k], background_k[i][j][k]) << std::endl; 
	}
      }
    }

    // fill the histograms for x, z, pt 
    for (int i=0; i<fBins->GetZBins()->GetNumber()+1; i++){
      for(int j=0; j<fBins->GetPtBins()->GetNumber()+1; j++){
	for(int b=1; b<fBins->GetXBins()->GetNumber()+1; b++){
	  h1_x_signal_p[i][j]->SetBinContent(b, signal_p[b][i][j]); 
	  h1_x_signal_p[i][j]->SetBinError(b, err_p[b][i][j]); 
	  h1_x_signal_k[i][j]->SetBinContent(b, signal_k[b][i][j]); 
	  h1_x_signal_k[i][j]->SetBinError(b, err_k[b][i][j]); 
	}
      }
    }

    for (int i=0; i<fBins->GetPtBins()->GetNumber()+1; i++){
      for(int j=0; j<fBins->GetXBins()->GetNumber()+1; j++){
	for(int b=1; b<fBins->GetZBins()->GetNumber()+1; b++){
	  h1_z_signal_p[i][j]->SetBinContent(b, signal_p[j][b][i]); 
	  h1_z_signal_p[i][j]->SetBinError(b, err_p[j][b][i]); 
	  h1_z_signal_k[i][j]->SetBinContent(b, signal_k[j][b][i]); 
	  h1_z_signal_k[i][j]->SetBinError(b, err_k[j][b][i]); 
	}
      }
    }

    for (int i=0; i<fBins->GetXBins()->GetNumber()+1; i++){
      for(int j=0; j<fBins->GetZBins()->GetNumber()+1; j++){
	for(int b=1; b<fBins->GetPtBins()->GetNumber()+1; b++){
	  h1_pt_signal_p[i][j]->SetBinContent(b, signal_p[i][j][b]); 
	  h1_pt_signal_p[i][j]->SetBinError(b, err_p[i][j][b]); 
	  h1_pt_signal_k[i][j]->SetBinContent(b, signal_k[i][j][b]); 
	  h1_pt_signal_k[i][j]->SetBinError(b, err_k[i][j][b]); 
	}
      }
    }
  }

  void Load(std::string inputFilename){
    TFile *inputFile = TFile::Open(inputFilename.c_str()); 
    
    if (inputFile->IsOpen()){
      for(int i=0; i<fBins->GetXBins()->GetNumber()+1; i++){
	for(int j=0; j<fBins->GetZBins()->GetNumber()+1; j++){
	  for(int k=0; k<fBins->GetPtBins()->GetNumber()+1; k++){
	    fit_p[i][j][k] = (TF1*) inputFile->Get(Form("signal_fits/fit_p_%s_x%d_z%d_pt%d_%s", constants::Names::mesons[fMesonIndex].c_str(),
							i, j, k, fName.c_str()));
	    fit_k[i][j][k] = (TF1*) inputFile->Get(Form("signal_fits/fit_k_%s_x%d_z%d_pt%d_%s", constants::Names::mesons[fMesonIndex].c_str(),
							i, j, k, fName.c_str()));
	    fit_bg[i][j][k] = (TF1*) inputFile->Get(Form("signal_fits/fit_bg_%s_x%d_z%d_pt%d_%s", constants::Names::mesons[fMesonIndex].c_str(),
							 i, j, k, fName.c_str()));
	    fit_tot[i][j][k] = (TF1*) inputFile->Get(Form("signal_fits/fit_tot_%s_x%d_z%d_pt%d_%s", constants::Names::mesons[fMesonIndex].c_str(),
							  i, j, k, fName.c_str()));

	    std::cout << "[SignalBackgroundFitter::Load] Loaded " << fit_p[i][j][k]->GetName() << std::endl; 
	    std::cout << "[SignalBackgroundFitter::Load] Loaded " << fit_k[i][j][k]->GetName() << std::endl; 
	    std::cout << "[SignalBackgroundFitter::Load] Loaded " << fit_bg[i][j][k]->GetName() << std::endl; 
	    std::cout << "[SignalBackgroundFitter::Load] Loaded " << fit_tot[i][j][k]->GetName() << std::endl; 
	  }
	}
      }
      
    // fill the histograms for x, z, pt 
    for (int i=0; i<fBins->GetZBins()->GetNumber()+1; i++){
      for(int j=0; j<fBins->GetPtBins()->GetNumber()+1; j++){
	h1_x_signal_p[i][j] = (TH1D*) inputFile->Get(Form("h1_x_signal_p_z%d_pt%d_%s", i, j, fName.c_str()));
	h1_x_signal_k[i][j] = (TH1D*) inputFile->Get(Form("h1_x_signal_k_z%d_pt%d_%s", i, j, fName.c_str()));

	// fill our local array 
	for (int k=1; k<fBins->GetXBins()->GetNumber()+1; k++){
	  signal_p[i][j][k] = h1_x_signal_p[i][j]->GetBinContent(k);
	  signal_k[i][j][k] = h1_x_signal_k[i][j]->GetBinContent(k);
	}

      }
    }
    
    for (int i=0; i<fBins->GetPtBins()->GetNumber()+1; i++){
      for(int j=0; j<fBins->GetXBins()->GetNumber()+1; j++){
	h1_z_signal_p[i][j] = (TH1D*) inputFile->Get(Form("h1_z_signal_p_z%d_pt%d_%s", i, j, fName.c_str()));
	h1_z_signal_k[i][j] = (TH1D*) inputFile->Get(Form("h1_z_signal_k_z%d_pt%d_%s", i, j, fName.c_str()));

	for (int k=1; k<fBins->GetZBins()->GetNumber()+1; k++){
	  signal_p[i][j][k] = h1_z_signal_p[i][j]->GetBinContent(k); 
	  signal_k[i][j][k] = h1_z_signal_k[i][j]->GetBinContent(k); 
	}

      }
    }
    
    for (int i=0; i<fBins->GetXBins()->GetNumber()+1; i++){
      for(int j=0; j<fBins->GetZBins()->GetNumber()+1; j++){
	h1_pt_signal_p[i][j] = (TH1D*) inputFile->Get(Form("h1_pt_signal_p_z%d_pt%d_%s", i, j, fName.c_str()));
	h1_pt_signal_k[i][j] = (TH1D*) inputFile->Get(Form("h1_pt_signal_k_z%d_pt%d_%s", i, j, fName.c_str()));

	for (int k=1; k<fBins->GetPtBins()->GetNumber()+1; k++){
	  signal_p[i][j][k] = h1_pt_signal_p[i][j]->GetBinContent(k); 
	  signal_k[i][j][k] = h1_pt_signal_k[i][j]->GetBinContent(k); 
	}
      }
    }

    } else {
      std::cerr << "[SignalBackgroundFitter::Load] Failed to open input file" << std::endl; 
    }
  }


  void Save(std::string outputFilename, std::string writeMode){
    TFile *outputFile = new TFile(outputFilename.c_str(), writeMode.c_str()); 

    if (outputFile->IsOpen()){

      outputFile->mkdir("signal/"); 
      outputFile->cd("signal/"); 
      for (int i=0; i<fBins->GetZBins()->GetNumber()+1; i++){
	for(int j=0; j<fBins->GetPtBins()->GetNumber()+1; j++){
	  h1_x_signal_p[i][j]->Write(); 
	  h1_x_signal_k[i][j]->Write(); 
	}
      }
      
      for (int i=0; i<fBins->GetPtBins()->GetNumber()+1; i++){
	for(int j=0; j<fBins->GetXBins()->GetNumber()+1; j++){
	  h1_z_signal_p[i][j]->Write(); 
	  h1_z_signal_k[i][j]->Write(); 
	}
      }
      
      for (int i=0; i<fBins->GetXBins()->GetNumber()+1; i++){
	for(int j=0; j<fBins->GetZBins()->GetNumber()+1; j++){
	  h1_pt_signal_p[i][j]->Write(); 
	  h1_pt_signal_k[i][j]->Write();
	}
      }

      outputFile->cd("/");       
      outputFile->mkdir("signal_fits/");      
      outputFile->cd("signal_fits/");
      for(int i=0; i<fBins->GetXBins()->GetNumber()+1; i++){
	for(int j=0; j<fBins->GetZBins()->GetNumber()+1; j++){
	  for(int k=0; k<fBins->GetPtBins()->GetNumber()+1; k++){
	    fit_p[i][j][k]  ->Write();
	    fit_k[i][j][k]  ->Write(); 
	    fit_bg[i][j][k] ->Write(); 
	    fit_tot[i][j][k]->Write(); 
	  }
	}
      }

      outputFile->cd("/");       
      outputFile->Write(); 
      outputFile->Close(); 
    }
    else {
      std::cerr << "[SignalBackgroundFitter::Save] File could not be opened for writing! " << outputFilename << std::endl; 
    }
  }
    
    void CorrectAsymmetry(Fits *pion, Fits *kaon){

    pionFit = new Fits(pion, "corrected"); 
    kaonFit = new Fits(kaon, "corrected"); 

    for(int i=0; i<fBins->GetXBins()->GetNumber()+1; i++){
      for(int j=0; j<fBins->GetZBins()->GetNumber()+1; j++){
	for(int k=0; k<fBins->GetPtBins()->GetNumber()+1; k++){
	  double obsP = pion->fit_asym[i][j][k]->GetParameter(0);  
	  double obsK = kaon->fit_asym[i][j][k]->GetParameter(0);  

	  double trueP = (obsK*background_k[i][j][k]-obsP*signal_k[i][j][k])/(background_k[i][j][k]*background_p[i][j][k]-signal_k[i][j][k]*signal_p[i][j][k]); 
	  double trueK = (signal_p[i][j][k]*obsK-obsP*background_p[i][j][k])/(signal_k[i][j][k]*signal_p[i][j][k]-background_k[i][j][k]*background_p[i][j][k]); 

	  pionFit->fit_asym[i][j][k]->SetParameter(0, trueP);
	  kaonFit->fit_asym[i][j][k]->SetParameter(0, trueK);

	  std::cout << Form("[SignalBackgroundFitter::CorrectAsymmetry] Correcting pion %.4f -> %.4f, kaon %.4f -> %.4f",obsP,trueP,obsK,trueK) << std::endl;
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
  TFile      *fInputFile; 

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
					  i, j, k, fName.c_str()), "gaus(0) + gaus(3) + pol2(6)", 0.0, 0.7);

	  // initialize the stuff to zero 
	  signal_p[i][j][k]     = 0.0; 
	  signal_k[i][j][k]     = 0.0; 
	  err_p[i][j][k]        = 0.0; 
	  err_k[i][j][k]        = 0.0; 
	  background_p[i][j][k] = 0.0; 
	  background_k[i][j][k] = 0.0; 
	}
      }
    }

    for (int i=0; i<fBins->GetZBins()->GetNumber()+1; i++){
      for(int j=0; j<fBins->GetPtBins()->GetNumber()+1; j++){
	h1_x_signal_p[i][j] = new TH1D(Form("h1_x_signal_p_z%d_pt%d_%s", i, j, fName.c_str()), "", fBins->GetXBins()->GetNumber(), fBins->GetXBins()->GetLimits().data());
	h1_x_signal_k[i][j] = new TH1D(Form("h1_x_signal_k_z%d_pt%d_%s", i, j, fName.c_str()), "", fBins->GetXBins()->GetNumber(), fBins->GetXBins()->GetLimits().data());
      }
    }

    for (int i=0; i<fBins->GetPtBins()->GetNumber()+1; i++){
      for(int j=0; j<fBins->GetXBins()->GetNumber()+1; j++){
	h1_z_signal_p[i][j] = new TH1D(Form("h1_z_signal_p_pt%d_x%d_%s", i, j, fName.c_str()), "", fBins->GetZBins()->GetNumber(), fBins->GetZBins()->GetLimits().data());
	h1_z_signal_k[i][j] = new TH1D(Form("h1_z_signal_k_pt%d_x%d_%s", i, j, fName.c_str()), "", fBins->GetZBins()->GetNumber(), fBins->GetZBins()->GetLimits().data());
      }
    }

    for (int i=0; i<fBins->GetXBins()->GetNumber()+1; i++){
      for(int j=0; j<fBins->GetZBins()->GetNumber()+1; j++){
	h1_pt_signal_p[i][j] = new TH1D(Form("h1_pt_signal_p_x%d_z%d_%s", i, j, fName.c_str()), "", fBins->GetPtBins()->GetNumber(), fBins->GetPtBins()->GetLimits().data());
	h1_pt_signal_k[i][j] = new TH1D(Form("h1_pt_signal_k_x%d_z%d_%s", i, j, fName.c_str()), "", fBins->GetPtBins()->GetNumber(), fBins->GetPtBins()->GetLimits().data());
      }
    }


  }
};

#endif
