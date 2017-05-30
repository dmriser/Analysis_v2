#ifndef int_histos_h
#define int_histos_h

#include "Bins.h"
#include "Constants.h"
#include "Fits.h"
#include "Types.h"

#include "TF1.h"
#include "TFile.h"
#include "TH1.h"

class IntegratedHistos {
 public:

  // Used when they dont exist
 IntegratedHistos(Fits *f, std::string name, int index): fFits(f), fName(name), fMesonIndex(index) {
    CreateHistograms(); 
    fInputFile = new TFile(); 
  }

  //  Used when they are being loaded.
 IntegratedHistos(std::string infile, std::string name, int index) : fName(name), fMesonIndex(index){
    Load(infile);
  }

  ~IntegratedHistos(){
    if(fInputFile->IsOpen()){
      fInputFile->Write();
      fInputFile->Close();
    }
  }

  TH1D *h1_x[constants::MAX_BINS_Z][constants::MAX_BINS_PT];
  TH1D *h1_z[constants::MAX_BINS_PT][constants::MAX_BINS_X];
  TH1D *h1_pt[constants::MAX_BINS_X][constants::MAX_BINS_Z];

  // assume nobody asks for weird values of x, z, pt 
  double GetAsymmetryX(int xbin, int zbin, int ptbin){
    return h1_x[zbin][ptbin]->GetBinContent(xbin);
  }
  
  double GetStatErrorX(int xbin, int zbin, int ptbin){
    return h1_x[zbin][ptbin]->GetBinError(xbin);
  }

  double GetAsymmetryZ(int xbin, int zbin, int ptbin){
    return h1_z[ptbin][xbin]->GetBinContent(zbin);
  }
  
  double GetStatErrorZ(int xbin, int zbin, int ptbin){
    return h1_z[ptbin][xbin]->GetBinError(zbin);
  }
  double GetAsymmetryPt(int xbin, int zbin, int ptbin){
    return h1_pt[xbin][zbin]->GetBinContent(ptbin);
  }
  
  double GetStatErrorPt(int xbin, int zbin, int ptbin){
    return h1_pt[xbin][zbin]->GetBinError(ptbin);
  }

  void Load(std::string infile){
    Bins *bins = Bins::GetInstance(fMesonIndex);

    fInputFile = TFile::Open(infile.c_str());
    
    
    for (int i=0; i<bins->GetZBins()->GetNumber()+1; i++){
      for(int j=0; j<bins->GetPtBins()->GetNumber()+1; j++){
	h1_x[i][j] = (TH1D*) fInputFile->Get(Form("integrated/%s/h1_x_z%d_pt%d_%s",constants::Names::mesons[fMesonIndex].c_str(), i, j, fName.c_str()));
      }
    }

    for (int i=0; i<bins->GetPtBins()->GetNumber()+1; i++){
      for(int j=0; j<bins->GetXBins()->GetNumber()+1; j++){
 	h1_z[i][j] = (TH1D*) fInputFile->Get(Form("integrated/%s/h1_z_pt%d_x%d_%s",constants::Names::mesons[fMesonIndex].c_str(), i, j, fName.c_str()));
      }
    } 

    for (int i=0; i<bins->GetXBins()->GetNumber()+1; i++){
      for(int j=0; j<bins->GetZBins()->GetNumber()+1; j++){
	h1_pt[i][j] = (TH1D*) fInputFile->Get(Form("integrated/%s/h1_pt_x%d_z%d_%s",constants::Names::mesons[fMesonIndex].c_str(), i, j, fName.c_str()));
      }
    }
  }

  void Save(std::string outfile, std::string mode){
    TFile *out = new TFile(outfile.c_str(), mode.c_str()); 
    
    std::string dir(Form("integrated/%s/", constants::Names::mesons[fMesonIndex].c_str()));
    out->mkdir(dir.c_str()); 
    out->cd(dir.c_str()); 
		    
    for (int i=0; i<fFits->GetBinning()->GetZBins()->GetNumber()+1; i++){
      for(int j=0; j<fFits->GetBinning()->GetPtBins()->GetNumber()+1; j++){
	h1_x[i][j]->Write(); 
      }
    }
    
    for (int i=0; i<fFits->GetBinning()->GetPtBins()->GetNumber()+1; i++){
      for(int j=0; j<fFits->GetBinning()->GetXBins()->GetNumber()+1; j++){
	h1_z[i][j]->Write();
      }
    } 
    
    for (int i=0; i<fFits->GetBinning()->GetXBins()->GetNumber()+1; i++){
      for(int j=0; j<fFits->GetBinning()->GetZBins()->GetNumber()+1; j++){
	h1_pt[i][j]->Write(); 
      }
    }

    out->cd("");
    out->Close(); 
  }

protected:
  std::string fName; 
  int         fMesonIndex; 
  Fits       *fFits; 
  TFile      *fInputFile; 

  void CreateHistograms(){

    for (int i=0; i<fFits->GetBinning()->GetZBins()->GetNumber()+1; i++){
      for(int j=0; j<fFits->GetBinning()->GetPtBins()->GetNumber()+1; j++){
	h1_x[i][j] = new TH1D(Form("h1_x_z%d_pt%d_%s", i, j, fName.c_str()), "", fFits->GetBinning()->GetXBins()->GetNumber(), fFits->GetBinning()->GetXBins()->GetLimits().data());

	// iterate on the bins in x, adding them to the histogram 
	for (int k=1; k<fFits->GetBinning()->GetXBins()->GetNumber()+1; k++){
	  h1_x[i][j]->SetBinContent(k, fFits->fit_asym[k][i][j]->GetParameter(0)); 	  
	  h1_x[i][j]->SetBinError(k, fFits->fit_asym[k][i][j]->GetParError(0)); 
	}
      }
    }

    for (int i=0; i<fFits->GetBinning()->GetPtBins()->GetNumber()+1; i++){
      for(int j=0; j<fFits->GetBinning()->GetXBins()->GetNumber()+1; j++){
	h1_z[i][j] = new TH1D(Form("h1_z_pt%d_x%d_%s", i, j, fName.c_str()), "", fFits->GetBinning()->GetZBins()->GetNumber(), fFits->GetBinning()->GetZBins()->GetLimits().data());    

	// iterate on the bins in x, adding them to the histogram 
	for (int k=1; k<fFits->GetBinning()->GetZBins()->GetNumber()+1; k++){
	  h1_z[i][j]->SetBinContent(k, fFits->fit_asym[j][k][i]->GetParameter(0)); 	  
	  h1_z[i][j]->SetBinError(k, fFits->fit_asym[j][k][i]->GetParError(0)); 
	}
      }
    } 

    for (int i=0; i<fFits->GetBinning()->GetXBins()->GetNumber()+1; i++){
      for(int j=0; j<fFits->GetBinning()->GetZBins()->GetNumber()+1; j++){
	h1_pt[i][j] = new TH1D(Form("h1_pt_x%d_z%d_%s", i, j, fName.c_str()), "", fFits->GetBinning()->GetPtBins()->GetNumber(), fFits->GetBinning()->GetPtBins()->GetLimits().data());    

	// iterate on the bins in x, adding them to the histogram 
	for (int k=1; k<fFits->GetBinning()->GetPtBins()->GetNumber()+1; k++){
	  h1_pt[i][j]->SetBinContent(k, fFits->fit_asym[i][j][k]->GetParameter(0)); 	  
	  h1_pt[i][j]->SetBinError(k, fFits->fit_asym[i][j][k]->GetParError(0)); 
	}
      }
    }


  }

};


#endif
