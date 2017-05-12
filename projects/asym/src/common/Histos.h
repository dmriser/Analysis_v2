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

enum MesonTypes {
  KP,
  KM
};

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
  const static int nx     = 4; 
  const static int nz     = 4; 
  const static int npt    = 1; 
  const static int nphi   = 18; 
  const static int nmeson = 2; 
  const static int nhel   = 2; 

  DLineBins xbins, zbins, ptbins, phibins; 

  // the order is always x, z, pt, phi
  TH1D *h1_phi[nmeson][nsect+1][nhel][nx+1][nz+1][npt+1]; 
  TH1D *h1_asym[nmeson][nsect+1][nx+1][nz+1][npt+1]; 
  TF1  *fit[nmeson][nsect+1][nx+1][nz+1][npt+1]; 
  //  TH1D *h1_x[nmeson][nsect+1][nhel][nz+1][npt+1][nphi+1]; 
  //  TH1D *h1_z[nmeson][nsect+1][nhel][npt+1][nphi+1][nx+1]; 
  //  TH1D *h1_pt[nmeson][nsect+1][nhel][nphi+1][nx+1][nz+1]; 

  // m is meson (0 = kp, 1 = km)
  // h is helicity (0 = -1, 1 = 1)
  void Fill(PhysicsEvent &ev, int m, int h){
    int s   = phiToSector(ev.detectedElectron.Phi()*to_degrees); 
    int x   = 1+xbins.FindBin(ev.x);
    int z   = 1+zbins.FindBin(ev.z);
    int pt  = 1+ptbins.FindBin(ev.pT);
    int phi = 1+phibins.FindBin(ev.phiHadron); 

    if (IndexIsSafe(m, s, h, x, z, pt, phi)){
      h1_phi[m][0][h][0][0][0]->Fill(ev.phiHadron); 

      h1_phi[m][0][h][x][0][0]->Fill(ev.phiHadron); 
      h1_phi[m][0][h][0][z][0]->Fill(ev.phiHadron); 
      h1_phi[m][0][h][0][0][pt]->Fill(ev.phiHadron); 

      h1_phi[m][0][h][0][z][pt]->Fill(ev.phiHadron); 
      h1_phi[m][0][h][x][0][pt]->Fill(ev.phiHadron); 
      h1_phi[m][0][h][x][z][0]->Fill(ev.phiHadron); 

      h1_phi[m][s][h][x][0][0]->Fill(ev.phiHadron); 
      h1_phi[m][s][h][0][z][0]->Fill(ev.phiHadron); 
      h1_phi[m][s][h][0][0][pt]->Fill(ev.phiHadron); 

      h1_phi[m][s][h][0][z][pt]->Fill(ev.phiHadron); 
      h1_phi[m][s][h][x][0][pt]->Fill(ev.phiHadron); 
      h1_phi[m][s][h][x][z][0]->Fill(ev.phiHadron); 

      h1_phi[m][s][h][x][z][pt]->Fill(ev.phiHadron); 
    }

  }

  void CalculateAsymmetry(){

    for(int m=0; m<nmeson; m++){
      for (int s=0; s<nsect+1; s++){
	for(int i=0; i<nx+1; i++){
	  for(int j=0; j<nz+1; j++){
	    for(int k=0; k<npt+1; k++){
	      
	      for (int b=1; b<=h1_phi[m][s][0][i][j][k]->GetXaxis()->GetNbins(); b++){

		double plus  = h1_phi[m][s][1][i][j][k]->GetBinContent(b);
		double minus = h1_phi[m][s][0][i][j][k]->GetBinContent(b);
		double sum   = plus+minus;
		double diff  = plus-minus;
		double err   = sqrt((1-pow(diff/sum,2))/sum);

		if (sum > 0.0) {
		  h1_asym[m][s][i][j][k]->SetBinContent(b,diff/sum/BEAM_POL);
		  h1_asym[m][s][i][j][k]->SetBinError(b, err);
		}
	      }
	      
	    }
	  }
	}
      }
    }

  }

  void Fit(){
    std::string names[2] = {"kp", "km"};
    
    for(int m=0; m<nmeson; m++){
      for (int s=0; s<nsect+1; s++){
	for(int i=0; i<nx+1; i++){
	  for(int j=0; j<nz+1; j++){
	    for(int k=0; k<npt+1; k++){
	      std::string current_name(Form("fit_asym_%s_sect%d_x%d_z%d_pt%d",names[m].c_str(), s, i, j, k));

	      fit[m][s][i][j][k] = new TF1(current_name.c_str(),"[0]*sin((3.14159/180.0)*x)",phibins.GetMin(), phibins.GetMax());
	      h1_asym[m][s][i][j][k]->Fit(current_name.c_str(), "RQ"); 
	    }
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
	for(int i=0; i<nx+1; i++){
	  for(int j=0; j<nz+1; j++){
	    for(int k=0; k<npt+1; k++){
	      h1_phi[0][s][0][i][j][k]->Write(); 
	      h1_phi[0][s][1][i][j][k]->Write(); 
	      h1_phi[1][s][0][i][j][k]->Write(); 
	      h1_phi[1][s][1][i][j][k]->Write(); 
	    }
	  }
	}
      }
      
      out->cd("/");
      
      out->mkdir("asym");
      out->cd("asym/");
      for(int s=0; s<nsect+1; s++){
	for(int i=0; i<nx+1; i++){
	  for(int j=0; j<nz+1; j++){
	    for(int k=0; k<npt+1; k++){
	      h1_asym[0][s][i][j][k]->Write(); 
	      h1_asym[1][s][i][j][k]->Write(); 
	    }
	  }
	}
      }
    }

    if (stage == 2){          
      
      out->mkdir("fit"); 
      out->cd("fit");
      for(int m=0; m<nmeson; m++){
	for (int s=0; s<nsect+1; s++){
	  for(int i=0; i<nx+1; i++){
	    for(int j=0; j<nz+1; j++){
	      for(int k=0; k<npt+1; k++){
		fit[m][s][i][j][k]->Write();
	      }
	    }
	  }
	}
      }
      
    } 
    
    out->cd("/");
    out->Close(); 
  }

  void Load(std::string file, int stage){
    //    inputFile = new TFile(file.c_str(), "update"); 
    inputFile = TFile::Open(file.c_str()); 

    std::string names[2] = {"kp", "km"};
    if (stage == 1){
      for(int m=0; m<nmeson; m++){
	for (int s=0; s<nsect+1; s++){
	  for(int i=0; i<nx+1; i++){
	    for(int j=0; j<nz+1; j++){
	      for(int k=0; k<npt+1; k++){
		h1_phi[m][s][0][i][j][k] = (TH1D*) inputFile->Get(Form("phi/h1_phi_%s_helMinus_sect%d_x%d_z%d_pt%d", 
								       names[m].c_str(), s, i, j, k));
		
		h1_phi[m][s][1][i][j][k] = (TH1D*) inputFile->Get(Form("phi/h1_phi_%s_helPlus_sect%d_x%d_z%d_pt%d", 
								       names[m].c_str(), s, i, j, k));
		
		h1_asym[m][s][i][j][k] = (TH1D*) inputFile->Get(Form("asym/h1_asym_%s_sect%d_x%d_z%d_pt%d",
								     names[m].c_str(),s, i, j, k));
	      }
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
 
    xbins   = DLineBins(nx,      0.0,   0.7); 
    zbins   = DLineBins(nz,      0.3,   0.8); 
    ptbins  = DLineBins(npt,     0.0,   1.0); 
    phibins = DLineBins(nphi, -180.0, 180.0); 
  }

  void Init(){    
    std::string names[2] = {"kp", "km"};

    for(int m=0; m<nmeson; m++){
      for (int s=0; s<nsect+1; s++){
	for(int i=0; i<nx+1; i++){
	  for(int j=0; j<nz+1; j++){
	    for(int k=0; k<npt+1; k++){
	      h1_phi[m][s][0][i][j][k] = new TH1D(Form("h1_phi_%s_helMinus_sect%d_x%d_z%d_pt%d", 
						       names[m].c_str(), s, i, j, k),"", phibins.GetNumber(), 
						  phibins.GetMin(), phibins.GetMax());
	      
	      h1_phi[m][s][1][i][j][k] = new TH1D(Form("h1_phi_%s_helPlus_sect%d_x%d_z%d_pt%d", 
						       names[m].c_str(), s, i, j, k),"", phibins.GetNumber(), 
						  phibins.GetMin(), phibins.GetMax());
	      
	      h1_asym[m][s][i][j][k] = new TH1D(Form("h1_asym_%s_sect%d_x%d_z%d_pt%d", 
						     names[m].c_str(), s, i, j, k),"", phibins.GetNumber(), 
						phibins.GetMin(), phibins.GetMax());
	      
	    }
	  }
	}
      }
    }
    
  }

  bool IndexIsSafe(int m, int s, int h, int x, int z, int pt, int phi){
    if (m>-1 && m<2)
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
