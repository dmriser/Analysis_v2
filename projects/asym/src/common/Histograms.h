#ifndef histos_h 
#define histos_h 

#include <iostream>
#include <vector>

#include "TFile.h"
#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
 
#include "CommonTools.h"
#include "DBins.h"
#include "h22Event.h"
#include "PhysicsEvent.h"

#include "Bins.h"
#include "BinFactory.h"
#include "Constants.h"
#include "Types.h"

class Histos {
  
 public:
 Histos(std::string name, int index) : fName(name), fMesonIndex(index) {
    SetupBinning();
    Init(); 

    inputFile = new TFile(); 
  }
  
  ~Histos(){
    if (inputFile)
      if (inputFile->IsOpen()){
    	inputFile->Write(); 
    	inputFile->Close(); 
      }
  }

  // the order is always x, z, pt, phi
  TH1D *h1_phi[constants::NSECT+1][constants::NHEL][constants::MAX_BINS_X][constants::MAX_BINS_Z][constants::MAX_BINS_PT]; 
  TH1D *h1_asym[constants::NSECT+1][constants::MAX_BINS_X][constants::MAX_BINS_Z][constants::MAX_BINS_PT]; 

  TH1D *h1_x; 
  TH1D *h1_z;
  TH1D *h1_pt; 

  void Fill(PhysicsEvent &ev, int h){
    int s   = phiToSector(ev.detectedElectron.Phi()*to_degrees); 
    int x   = 1+bins->GetXBins()->FindBin(ev.x);
    int z   = 1+bins->GetZBins()->FindBin(ev.z);
    int pt  = 1+bins->GetPtBins()->FindBin(ev.pT);
    int phi = 1+bins->GetPhiBins()->FindBin(ev.phiHadron); 

    if (IndexIsSafe(s, h, x, z, pt, phi)){
	h1_phi[0][h][0][0][0]->Fill(ev.phiHadron); 
	
	h1_phi[0][h][x][0][0]->Fill(ev.phiHadron); 
	h1_phi[0][h][0][z][0]->Fill(ev.phiHadron); 
	h1_phi[0][h][0][0][pt]->Fill(ev.phiHadron); 
	
	h1_phi[0][h][0][z][pt]->Fill(ev.phiHadron); 
	h1_phi[0][h][x][0][pt]->Fill(ev.phiHadron); 
	h1_phi[0][h][x][z][0]->Fill(ev.phiHadron); 
	h1_phi[s][h][x][0][0]->Fill(ev.phiHadron); 
	h1_phi[s][h][0][z][0]->Fill(ev.phiHadron); 
	h1_phi[s][h][0][0][pt]->Fill(ev.phiHadron); 
	
	h1_phi[s][h][0][z][pt]->Fill(ev.phiHadron); 
	h1_phi[s][h][x][0][pt]->Fill(ev.phiHadron); 
 	h1_phi[s][h][x][z][0]->Fill(ev.phiHadron); 
	
	h1_phi[s][h][x][z][pt]->Fill(ev.phiHadron); 
    }
  }

  void CalculateAsymmetry(){
    for (int s=0; s<constants::NSECT+1; s++){
      for(int i=0; i<bins->GetXBins()->GetNumber()+1; i++){
	for(int j=0; j<bins->GetZBins()->GetNumber()+1; j++){
	  for(int k=0; k<bins->GetPtBins()->GetNumber()+1; k++){
	    h1_asym[s][i][j][k] = new TH1D(Form("h1_asym_%s_sect%d_x%d_z%d_pt%d_%s", 
						constants::Names::mesons[fMesonIndex].c_str(), s, i, j, k, fName.c_str()),"", bins->GetPhiBins()->GetNumber(), 
					   bins->GetPhiBins()->GetMin(), bins->GetPhiBins()->GetMax());
	    
	    for (int b=1; b<=h1_phi[s][Helicity::kNegative][i][j][k]->GetXaxis()->GetNbins(); b++){	      
	      double plus  = h1_phi[s][Helicity::kPositive][i][j][k]->GetBinContent(b);
	      double minus = h1_phi[s][Helicity::kNegative][i][j][k]->GetBinContent(b);
	      double sum   = plus+minus;
	      double diff  = plus-minus;
	      double err   = sqrt((1-pow(diff/sum,2))/sum);

	      if (sum > 0.0) {
		h1_asym[s][i][j][k]->SetBinContent(b,diff/sum/constants::BEAM_POL);
		h1_asym[s][i][j][k]->SetBinError(b, err);
	      }
	    }
	  }
	}
      }
    }
  }
  
  void Save(std::string outfile, std::string saveOpts, int stage){
    TFile *out = new TFile(outfile.c_str(), saveOpts.c_str()); 

    if (stage == 1){
      out->mkdir(Form("phi/%s", constants::Names::mesons[fMesonIndex].c_str()));
      out->cd(Form("phi/%s/", constants::Names::mesons[fMesonIndex].c_str()));
      for(int s=0; s<constants::NSECT+1; s++){
	for(int i=0; i<bins->GetXBins()->GetNumber()+1; i++){
	  for(int j=0; j<bins->GetZBins()->GetNumber()+1; j++){
	    for(int k=0; k<bins->GetPtBins()->GetNumber()+1; k++){
	      h1_phi[s][0][i][j][k]->Write(); 
	      h1_phi[s][1][i][j][k]->Write(); 
	    }
	  }
	}
      }
      out->cd("/");
    }

    if (stage == 2){      
      out->mkdir(Form("asym/%s", constants::Names::mesons[fMesonIndex].c_str()));
      out->cd(Form("asym/%s/", constants::Names::mesons[fMesonIndex].c_str()));
      for(int s=0; s<constants::NSECT+1; s++){
	for(int i=0; i<bins->GetXBins()->GetNumber()+1; i++){
	  for(int j=0; j<bins->GetZBins()->GetNumber()+1; j++){
	    for(int k=0; k<bins->GetPtBins()->GetNumber()+1; k++){
	      h1_asym[s][i][j][k]->Write(); 
	    }
	  }
	}
      }
    }
  
    
    out->cd("/");
    
    //    h1_x->Write(); 
    //    h1_z->Write(); 
    //    h1_pt->Write(); 
    
    out->Close(); 
  }

  void Load(std::string file, int stage){
    inputFile = TFile::Open(file.c_str()); 

    if (stage == 1 || stage == 2)
      for (int s=0; s<constants::NSECT+1; s++){
	for(int i=0; i<bins->GetXBins()->GetNumber()+1; i++){
	  for(int j=0; j<bins->GetZBins()->GetNumber()+1; j++){
	    for(int k=0; k<bins->GetPtBins()->GetNumber()+1; k++){
	      h1_phi[s][Helicity::kNegative][i][j][k] = (TH1D*) inputFile->Get(Form("phi/%s/h1_phi_%s_helMinus_sect%d_x%d_z%d_pt%d_%s", constants::Names::mesons[fMesonIndex].c_str(), 
										    constants::Names::mesons[fMesonIndex].c_str(), s, i, j, k, fName.c_str()));
 
	      h1_phi[s][Helicity::kPositive][i][j][k] = (TH1D*) inputFile->Get(Form("phi/%s/h1_phi_%s_helPlus_sect%d_x%d_z%d_pt%d_%s", constants::Names::mesons[fMesonIndex].c_str(), 
										    constants::Names::mesons[fMesonIndex].c_str(), s, i, j, k, fName.c_str()));
 
	      h1_asym[s][i][j][k] = (TH1D*) inputFile->Get(Form("asym/%s/h1_asym_%s_sect%d_x%d_z%d_pt%d_%s", constants::Names::mesons[fMesonIndex].c_str(), 
								constants::Names::mesons[fMesonIndex].c_str(), s, i, j, k, fName.c_str()));
	    }
	  }
	}
      }

    if (stage == 2) {
      h1_x   = (TH1D*) inputFile->Get(Form("h1_x_%s_%s", constants::Names::mesons[fMesonIndex].c_str(), fName.c_str()));
      h1_z   = (TH1D*) inputFile->Get(Form("h1_z_%s_%s", constants::Names::mesons[fMesonIndex].c_str(), fName.c_str()));
      h1_pt  = (TH1D*) inputFile->Get(Form("h1_pt_%s_%s",constants::Names::mesons[fMesonIndex].c_str(), fName.c_str()));
    }
  }

 protected:
  std::string  fName; 
  int          fMesonIndex; 
  Bins        *bins; 
  TFile       *inputFile; 

  void SetupBinning(){
    bins = Bins::GetInstance(fMesonIndex); 

    std::cout << "[Histos::SetupBinning] Setup bins properly. " << std::endl; 
    std::cout << "[Histos::SetupBinning] x-min = " << bins->GetXBins()->GetMin() << std::endl; 
  }

  void Init(){    
    for (int s=0; s<constants::NSECT+1; s++){
      for(int i=0; i<bins->GetXBins()->GetNumber()+1; i++){
	for(int j=0; j<bins->GetZBins()->GetNumber()+1; j++){
	  for(int k=0; k<bins->GetPtBins()->GetNumber()+1; k++){
	    h1_phi[s][Helicity::kNegative][i][j][k] = new TH1D(Form("h1_phi_%s_helMinus_sect%d_x%d_z%d_pt%d_%s", 
								    constants::Names::mesons[fMesonIndex].c_str(), s, i, j, k, fName.c_str()),"", bins->GetPhiBins()->GetNumber(), 
							       bins->GetPhiBins()->GetMin(), bins->GetPhiBins()->GetMax());
	    
	    h1_phi[s][Helicity::kPositive][i][j][k] = new TH1D(Form("h1_phi_%s_helPlus_sect%d_x%d_z%d_pt%d_%s", 
								   constants::Names::mesons[fMesonIndex].c_str(), s, i, j, k, fName.c_str()),"", bins->GetPhiBins()->GetNumber(), 
							      bins->GetPhiBins()->GetMin(), bins->GetPhiBins()->GetMax());
 	    
	    h1_asym[s][i][j][k] = new TH1D(Form("h1_asym_%s_sect%d_x%d_z%d_pt%d_%s", 
						constants::Names::mesons[fMesonIndex].c_str(), s, i, j, k, fName.c_str()),"", bins->GetPhiBins()->GetNumber(), 
					   bins->GetPhiBins()->GetMin(), bins->GetPhiBins()->GetMax());
	    
	    }
	  }
	}
    } 
  }

  bool IndexIsSafe(int s, int h, int x, int z, int pt, int phi){
    if (s>0 && s<7)
      if (h>-1 && h<2)
	if (x > 0)
	  if (z > 0)
	    if (pt > 0)
	      if (phi > 0)
		return true;
    
    return false; 
  }

};

#endif
