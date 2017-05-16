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

#define BEAM_POL 0.751


/**
   The histograms are indexed for helicity and kaon charge 
   by assuming that, 
   positive = 1, negative = 0

*/

class AsymmetryHistograms {
  
 public:
 AsymmetryHistograms(std::string name) : fName(name) {
    SetupBinning();
    Init(); 
  }

  ~AsymmetryHistograms(){
    if (inputFile)
      if (inputFile->IsOpen()){
	inputFile->Write(); 
	inputFile->Close(); 
      }
  }

  // binning setup 
  const static int nsect  = 6; 
  const static int nx_kp  = 4; 
  const static int nz_kp  = 4; 
  const static int npt_kp = 4; 
  const static int nx_km  = 3; 
  const static int nz_km  = 2; 
  const static int npt_km = 4; 
  const static int nphi_kp = 18; 
  const static int nphi_km = 18; 
  const static int nmeson = 2; 
  const static int nhel   = 2; 

  DLineBins xbins[2], zbins[2], ptbins[2], phibins[2]; 

  // the order is always x, z, pt, phi
  TH1D *h1_phi_kp[nsect+1][nhel][nx_kp+1][nz_kp+1][npt_kp+1]; 
  TH1D *h1_phi_km[nsect+1][nhel][nx_km+1][nz_km+1][npt_km+1]; 
  TH1D *h1_asym_kp[nsect+1][nx_kp+1][nz_kp+1][npt_kp+1]; 
  TH1D *h1_asym_km[nsect+1][nx_km+1][nz_km+1][npt_km+1]; 
  TF1  *fit_kp[nsect+1][nx_kp+1][nz_kp+1][npt_kp+1]; 
  TF1  *fit_km[nsect+1][nx_km+1][nz_km+1][npt_km+1]; 

  //  TH1D *h1_x[nmeson][nsect+1][nhel][nz+1][npt+1][nphi+1]; 
  //  TH1D *h1_z[nmeson][nsect+1][nhel][npt+1][nphi+1][nx+1]; 
  //  TH1D *h1_pt[nmeson][nsect+1][nhel][nphi+1][nx+1][nz+1]; 

  // m is meson (0 = km, 1 = kp)
  // h is helicity (0 = -1, 1 = 1)
  void Fill(PhysicsEvent &ev, int m, int h){
    int s   = phiToSector(ev.detectedElectron.Phi()*to_degrees); 
    int x   = 1+xbins[m].FindBin(ev.x);
    int z   = 1+zbins[m].FindBin(ev.z);
    int pt  = 1+ptbins[m].FindBin(ev.pT);
    int phi = 1+phibins[m].FindBin(ev.phiHadron); 

    if (IndexIsSafe(s, h, x, z, pt, phi)){

      if (m == 0){
	h1_phi_km[0][h][0][0][0]->Fill(ev.phiHadron); 
	
	h1_phi_km[0][h][x][0][0]->Fill(ev.phiHadron); 
	h1_phi_km[0][h][0][z][0]->Fill(ev.phiHadron); 
	h1_phi_km[0][h][0][0][pt]->Fill(ev.phiHadron); 
	
	h1_phi_km[0][h][0][z][pt]->Fill(ev.phiHadron); 
	h1_phi_km[0][h][x][0][pt]->Fill(ev.phiHadron); 
	h1_phi_km[0][h][x][z][0]->Fill(ev.phiHadron); 
	
	h1_phi_km[s][h][x][0][0]->Fill(ev.phiHadron); 
	h1_phi_km[s][h][0][z][0]->Fill(ev.phiHadron); 
	h1_phi_km[s][h][0][0][pt]->Fill(ev.phiHadron); 
	
	h1_phi_km[s][h][0][z][pt]->Fill(ev.phiHadron); 
	h1_phi_km[s][h][x][0][pt]->Fill(ev.phiHadron); 
 	h1_phi_km[s][h][x][z][0]->Fill(ev.phiHadron); 
	
	h1_phi_km[s][h][x][z][pt]->Fill(ev.phiHadron); 
      }

      else if (m == 1){
	h1_phi_kp[0][h][0][0][0]->Fill(ev.phiHadron); 
	
	h1_phi_kp[0][h][x][0][0]->Fill(ev.phiHadron); 
	h1_phi_kp[0][h][0][z][0]->Fill(ev.phiHadron); 
	h1_phi_kp[0][h][0][0][pt]->Fill(ev.phiHadron); 
	
	h1_phi_kp[0][h][0][z][pt]->Fill(ev.phiHadron); 
	h1_phi_kp[0][h][x][0][pt]->Fill(ev.phiHadron); 
	h1_phi_kp[0][h][x][z][0]->Fill(ev.phiHadron); 
	
	h1_phi_kp[s][h][x][0][0]->Fill(ev.phiHadron); 
	h1_phi_kp[s][h][0][z][0]->Fill(ev.phiHadron); 
	h1_phi_kp[s][h][0][0][pt]->Fill(ev.phiHadron); 
	
	h1_phi_kp[s][h][0][z][pt]->Fill(ev.phiHadron); 
	h1_phi_kp[s][h][x][0][pt]->Fill(ev.phiHadron); 
 	h1_phi_kp[s][h][x][z][0]->Fill(ev.phiHadron); 
	
	h1_phi_kp[s][h][x][z][pt]->Fill(ev.phiHadron); 
      }
    }

  }

  void CalculateAsymmetry(){
    
    for (int s=0; s<nsect+1; s++){
      for(int i=0; i<nx_km+1; i++){
	for(int j=0; j<nz_km+1; j++){
	  for(int k=0; k<npt_km+1; k++){
	    
	    for (int b=1; b<=h1_phi_km[s][0][i][j][k]->GetXaxis()->GetNbins(); b++){
	      
	      double plus  = h1_phi_km[s][1][i][j][k]->GetBinContent(b);
	      double minus = h1_phi_km[s][0][i][j][k]->GetBinContent(b);
	      double sum   = plus+minus;
	      double diff  = plus-minus;
	      double err   = sqrt((1-pow(diff/sum,2))/sum);
	      
	      if (sum > 0.0) {
		h1_asym_km[s][i][j][k]->SetBinContent(b,diff/sum/BEAM_POL);
		h1_asym_km[s][i][j][k]->SetBinError(b, err);
	      }
	    }
	  }
	}
      }
    }

    for (int s=0; s<nsect+1; s++){
      for(int i=0; i<nx_kp+1; i++){
	for(int j=0; j<nz_kp+1; j++){
	  for(int k=0; k<npt_kp+1; k++){
	    
	    for (int b=1; b<=h1_phi_kp[s][0][i][j][k]->GetXaxis()->GetNbins(); b++){
	      
	      double plus  = h1_phi_kp[s][1][i][j][k]->GetBinContent(b);
	      double minus = h1_phi_kp[s][0][i][j][k]->GetBinContent(b);
	      double sum   = plus+minus;
	      double diff  = plus-minus;
	      double err   = sqrt((1-pow(diff/sum,2))/sum);
	      
	      if (sum > 0.0) {
		h1_asym_kp[s][i][j][k]->SetBinContent(b,diff/sum/BEAM_POL);
		h1_asym_kp[s][i][j][k]->SetBinError(b, err);
	      }
	    }
	  }
	}
      }
    }
    

  }

  void Fit(){
    
    for (int s=0; s<nsect+1; s++){
      for(int i=0; i<nx_km+1; i++){
	for(int j=0; j<nz_km+1; j++){
 	  for(int k=0; k<npt_km+1; k++){
	    std::string current_name(Form("fit_asym_km_sect%d_x%d_z%d_pt%d_%s", s, i, j, k, fName.c_str()));
	    
	    fit_km[s][i][j][k] = new TF1(current_name.c_str(),"[0]*sin((3.14159/180.0)*x)",phibins[0].GetMin(), phibins[0].GetMax());
	    //	    fit_km[s][i][j][k] = new TF1(current_name.c_str(),"[0]*sin((3.14159/180.0)*x)/(1 + [1]*cos((3.14159/180.0)*x)+ [2]*cos(2*(3.14159/180.0)*x))",phibins[0].GetMin(), phibins[0].GetMax());
	    h1_asym_km[s][i][j][k]->Fit(current_name.c_str(), "RQ"); 
	  }
	}
      }
    }
    
    for (int s=0; s<nsect+1; s++){
      for(int i=0; i<nx_kp+1; i++){
	for(int j=0; j<nz_kp+1; j++){
 	  for(int k=0; k<npt_kp+1; k++){
	    std::string current_name(Form("fit_asym_kp_sect%d_x%d_z%d_pt%d_%s", s, i, j, k, fName.c_str()));
	    
	    fit_kp[s][i][j][k] = new TF1(current_name.c_str(),"[0]*sin((3.14159/180.0)*x)",phibins[1].GetMin(), phibins[1].GetMax());
	    //	    fit_kp[s][i][j][k] = new TF1(current_name.c_str(),"[0]*sin((3.14159/180.0)*x)/(1 + [1]*cos((3.14159/180.0)*x)+ [2]*cos(2*(3.14159/180.0)*x))",phibins[1].GetMin(), phibins[1].GetMax());
	    h1_asym_kp[s][i][j][k]->Fit(current_name.c_str(), "RQ"); 
	  }
	}
      }
    }

  }

  
  void Save(std::string outfile, std::string saveOpts, int stage){
    TFile *out = new TFile(outfile.c_str(), saveOpts.c_str()); 

    if (stage == 1){
      out->mkdir("phi");
      out->cd("phi/");
      for(int s=0; s<nsect+1; s++){
	for(int i=0; i<nx_km+1; i++){
	  for(int j=0; j<nz_km+1; j++){
	    for(int k=0; k<npt_km+1; k++){
	      h1_phi_km[s][0][i][j][k]->Write(); 
	      h1_phi_km[s][1][i][j][k]->Write(); 
	    }
	  }
	}
	for(int i=0; i<nx_kp+1; i++){
	  for(int j=0; j<nz_kp+1; j++){
	    for(int k=0; k<npt_kp+1; k++){
	      h1_phi_kp[s][0][i][j][k]->Write(); 
	      h1_phi_kp[s][1][i][j][k]->Write(); 
	    }
	  }
	}
      }
      
      out->cd("/");
      
      out->mkdir("asym");
      out->cd("asym/");
      for(int s=0; s<nsect+1; s++){
	for(int i=0; i<nx_km+1; i++){
	  for(int j=0; j<nz_km+1; j++){
	    for(int k=0; k<npt_km+1; k++){
	      h1_asym_km[s][i][j][k]->Write(); 
	    }
	  }
	}
	for(int i=0; i<nx_kp+1; i++){
	  for(int j=0; j<nz_kp+1; j++){
	    for(int k=0; k<npt_kp+1; k++){
	      h1_asym_kp[s][i][j][k]->Write(); 
	    }
	  }
	}
      }
    }

    if (stage == 2){          
      
      out->mkdir("fit"); 
      out->cd("fit");
      for (int s=0; s<nsect+1; s++){
	for(int i=0; i<nx_km+1; i++){
	  for(int j=0; j<nz_km+1; j++){
	    for(int k=0; k<npt_km+1; k++){
	      fit_km[s][i][j][k]->Write();
	    }
	  }
	}
	for(int i=0; i<nx_kp+1; i++){
	  for(int j=0; j<nz_kp+1; j++){
	    for(int k=0; k<npt_kp+1; k++){
	      fit_kp[s][i][j][k]->Write();
	    }
	  }
	}
      }
      
    } 
    
    out->cd("/");
    out->Close(); 
  }

  void Load(std::string file, int stage){
    inputFile = TFile::Open(file.c_str()); 

    std::string names[2] = {"km", "kp"};
    if (stage == 1 || stage == 2)
      for (int s=0; s<nsect+1; s++){
	for(int i=0; i<nx_km+1; i++){
	  for(int j=0; j<nz_km+1; j++){
	    for(int k=0; k<npt_km+1; k++){
	      h1_phi_km[s][0][i][j][k] = (TH1D*) inputFile->Get(Form("phi/h1_phi_km_helMinus_sect%d_x%d_z%d_pt%d_%s", 
								     s, i, j, k, fName.c_str()));
	      
	      h1_phi_km[s][1][i][j][k] = (TH1D*) inputFile->Get(Form("phi/h1_phi_km_helPlus_sect%d_x%d_z%d_pt%d_%s", 
								     s, i, j, k, fName.c_str()));
	      
	      h1_asym_km[s][i][j][k] = (TH1D*) inputFile->Get(Form("asym/h1_asym_km_sect%d_x%d_z%d_pt%d_%s",
								   s, i, j, k, fName.c_str()));
	    }
	  }
	}
	
	for(int i=0; i<nx_kp+1; i++){
	  for(int j=0; j<nz_kp+1; j++){
	    for(int k=0; k<npt_kp+1; k++){
		h1_phi_kp[s][0][i][j][k] = (TH1D*) inputFile->Get(Form("phi/h1_phi_kp_helMinus_sect%d_x%d_z%d_pt%d_%s", 
								       s, i, j, k, fName.c_str()));
		
		h1_phi_kp[s][1][i][j][k] = (TH1D*) inputFile->Get(Form("phi/h1_phi_kp_helPlus_sect%d_x%d_z%d_pt%d_%s", 
								       s, i, j, k, fName.c_str()));
		
		h1_asym_kp[s][i][j][k] = (TH1D*) inputFile->Get(Form("asym/h1_asym_kp_sect%d_x%d_z%d_pt%d_%s",
								     s, i, j, k, fName.c_str()));
	    }
	  }
	}
      }

    if (stage == 2) {
      for (int s=0; s<nsect+1; s++){
	for(int i=0; i<nx_km+1; i++){
	  for(int j=0; j<nz_km+1; j++){
	    for(int k=0; k<npt_km+1; k++){
	      fit_km[s][i][j][k] = (TF1*) inputFile->Get(Form("fit/fit_asym_km_sect%d_x%d_z%d_pt%d_%s",
							       s, i, j, k, fName.c_str()));
	    }
	  }
	}

	for(int i=0; i<nx_kp+1; i++){
	  for(int j=0; j<nz_kp+1; j++){
	    for(int k=0; k<npt_kp+1; k++){
	      fit_kp[s][i][j][k] = (TF1*) inputFile->Get(Form("fit/fit_asym_kp_sect%d_x%d_z%d_pt%d_%s",
							       s, i, j, k, fName.c_str()));
	    }
	  }
	}
      }      
    }
  }

 protected:
  std::string fName; 
  TFile *inputFile; 

  void SetupBinning(){

    // This can be customized later by passing in 
    // std::vector<double> vxbins; 
    // vxbins.push_back(0.01); 
    // vxbins.push_back(0.07); 
    // vxbins.push_back(0.14); 
    // vxbins.push_back(0.27); 
    // vxbins.push_back(0.48); 
    // xbins = DLineBins(nx, vxbins);
 
    xbins[0]   = DLineBins(nx_km,      0.0,   0.7); 
    zbins[0]   = DLineBins(nz_km,      0.3,   0.8); 
    ptbins[0]  = DLineBins(npt_km,     0.0,   1.0); 
    phibins[0] = DLineBins(nphi_km, -180.0, 180.0); 
 
    xbins[1]   = DLineBins(nx_kp,      0.0,   0.7); 
    zbins[1]   = DLineBins(nz_kp,      0.3,   0.8); 
    ptbins[1]  = DLineBins(npt_kp,     0.0,   1.0); 
    phibins[1] = DLineBins(nphi_kp, -180.0, 180.0); 
  }

  void Init(){    
    std::string names[2] = {"km", "kp"};

    for (int s=0; s<nsect+1; s++){
      for(int i=0; i<nx_km+1; i++){
	for(int j=0; j<nz_km+1; j++){
	  for(int k=0; k<npt_km+1; k++){
	    h1_phi_km[s][0][i][j][k] = new TH1D(Form("h1_phi_km_helMinus_sect%d_x%d_z%d_pt%d_%s", 
							s, i, j, k, fName.c_str()),"", phibins[0].GetNumber(), 
						   phibins[0].GetMin(), phibins[0].GetMax());
	    
	    h1_phi_km[s][1][i][j][k] = new TH1D(Form("h1_phi_km_helPlus_sect%d_x%d_z%d_pt%d_%s", 
							s, i, j, k, fName.c_str()),"", phibins[0].GetNumber(), 
						   phibins[0].GetMin(), phibins[0].GetMax());
	    
	    h1_asym_km[s][i][j][k] = new TH1D(Form("h1_asym_km_sect%d_x%d_z%d_pt%d_%s", 
						      s, i, j, k, fName.c_str()),"", phibins[0].GetNumber(), 
						 phibins[0].GetMin(), phibins[0].GetMax());
	    
	    }
	  }
	}

      for(int i=0; i<nx_kp+1; i++){
	for(int j=0; j<nz_kp+1; j++){
	  for(int k=0; k<npt_kp+1; k++){
	    h1_phi_kp[s][0][i][j][k] = new TH1D(Form("h1_phi_kp_helMinus_sect%d_x%d_z%d_pt%d_%s", 
						     s, i, j, k, fName.c_str()),"", phibins[1].GetNumber(), 
						phibins[1].GetMin(), phibins[1].GetMax());
	    
	    h1_phi_kp[s][1][i][j][k] = new TH1D(Form("h1_phi_kp_helPlus_sect%d_x%d_z%d_pt%d_%s", 
							s, i, j, k, fName.c_str()),"", phibins[1].GetNumber(), 
						   phibins[1].GetMin(), phibins[1].GetMax());
	    
	    h1_asym_kp[s][i][j][k] = new TH1D(Form("h1_asym_kp_sect%d_x%d_z%d_pt%d_%s", 
						      s, i, j, k, fName.c_str()),"", phibins[1].GetNumber(), 
						 phibins[1].GetMin(), phibins[1].GetMax());
	    
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
