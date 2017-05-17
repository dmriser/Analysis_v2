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

#include "Constants.h"
#include "Types.h"

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
  const static int nz_km  = 3; 
  const static int npt_km = 3; 
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

  TH1D *h1_x_kp; 
  TH1D *h1_z_kp;
  TH1D *h1_pt_kp; 

  TH1D *h1_x_km; 
  TH1D *h1_z_km;
  TH1D *h1_pt_km; 

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
		h1_asym_km[s][i][j][k]->SetBinContent(b,diff/sum/constants::BEAM_POL);
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
		h1_asym_kp[s][i][j][k]->SetBinContent(b,diff/sum/constants::BEAM_POL);
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

    // set the histogram points in the x,z,pt histograms 
    h1_x_kp  = new TH1D(Form("h1_x_kp_%s",fName.c_str()),"",xbins[1].GetNumber(), xbins[1].GetLimits().data()); 
    h1_z_kp  = new TH1D(Form("h1_z_kp_%s",fName.c_str()),"",zbins[1].GetNumber(), zbins[1].GetLimits().data()); 
    h1_pt_kp = new TH1D(Form("h1_pt_kp_%s",fName.c_str()),"",ptbins[1].GetNumber(), ptbins[1].GetLimits().data()); 

    h1_x_km  = new TH1D(Form("h1_x_km_%s",fName.c_str()),"",xbins[0].GetNumber(), xbins[0].GetLimits().data()); 
    h1_z_km  = new TH1D(Form("h1_z_km_%s",fName.c_str()),"",zbins[0].GetNumber(), zbins[0].GetLimits().data()); 
    h1_pt_km = new TH1D(Form("h1_pt_km_%s",fName.c_str()),"",ptbins[0].GetNumber(), ptbins[0].GetLimits().data()); 


    for(int b=1; b<=nx_kp; b++){
      h1_x_kp->SetBinContent(b, fit_kp[0][b][0][0]->GetParameter(0));
      h1_x_kp->SetBinError(b, fit_kp[0][b][0][0]->GetParError(0));
    }

    for(int b=1; b<=nz_kp; b++){
      h1_z_kp->SetBinContent(b, fit_kp[0][0][b][0]->GetParameter(0));
      h1_z_kp->SetBinError(b, fit_kp[0][0][b][0]->GetParError(0));
    }

    for(int b=1; b<=npt_kp; b++){
      h1_pt_kp->SetBinContent(b, fit_kp[0][0][0][b]->GetParameter(0));
      h1_pt_kp->SetBinError(b, fit_kp[0][0][0][b]->GetParError(0));
    }

    for(int b=1; b<=nx_km; b++){
      h1_x_km->SetBinContent(b, fit_km[0][b][0][0]->GetParameter(0));
      h1_x_km->SetBinError(b, fit_km[0][b][0][0]->GetParError(0));
    }

    for(int b=1; b<=nz_km; b++){
      h1_z_km->SetBinContent(b, fit_km[0][0][b][0]->GetParameter(0));
      h1_z_km->SetBinError(b, fit_km[0][0][b][0]->GetParError(0));
    }

    for(int b=1; b<=npt_km; b++){
      h1_pt_km->SetBinContent(b, fit_km[0][0][0][b]->GetParameter(0));
      h1_pt_km->SetBinError(b, fit_km[0][0][0][b]->GetParError(0));
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
    
    h1_x_kp->Write(); 
    h1_z_kp->Write(); 
    h1_pt_kp->Write(); 
    h1_x_km->Write(); 
    h1_z_km->Write(); 
    h1_pt_km->Write(); 

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
      h1_x_kp  = (TH1D*) inputFile->Get(Form("h1_x_kp_%s", fName.c_str()));
      h1_z_kp  = (TH1D*) inputFile->Get(Form("h1_z_kp_%s", fName.c_str()));
      h1_pt_kp = (TH1D*) inputFile->Get(Form("h1_pt_kp_%s",fName.c_str()));
      h1_x_km  = (TH1D*) inputFile->Get(Form("h1_x_km_%s", fName.c_str()));
      h1_z_km  = (TH1D*) inputFile->Get(Form("h1_z_km_%s", fName.c_str()));
      h1_pt_km = (TH1D*) inputFile->Get(Form("h1_pt_km_%s",fName.c_str()));

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
 
    // this binning scheme was produced by running 
    // the code ./src/find_binning out/file.root 
    // on a file from this code that has standard 
    // histograms inside of it. 
    std::vector<double> vxbins_kp; 
    vxbins_kp.push_back(0.1);
    vxbins_kp.push_back(0.195);
    vxbins_kp.push_back(0.255);
    vxbins_kp.push_back(0.325);
    vxbins_kp.push_back(0.600);
 
    std::vector<double> vzbins_kp; 
    vzbins_kp.push_back(0.3);
    vzbins_kp.push_back(0.355);
    vzbins_kp.push_back(0.415);
    vzbins_kp.push_back(0.495);
    vzbins_kp.push_back(0.700);
 
    std::vector<double> vptbins_kp; 
    vptbins_kp.push_back(0.0);
    vptbins_kp.push_back(0.264);
    vptbins_kp.push_back(0.387);
    vptbins_kp.push_back(0.519);
    vptbins_kp.push_back(1.0);
 
    std::vector<double> vxbins_km; 
    vxbins_km.push_back(0.1);
    vxbins_km.push_back(0.215);
    vxbins_km.push_back(0.295);
    vxbins_km.push_back(0.600);
 
    std::vector<double> vzbins_km; 
    vzbins_km.push_back(0.3);
    vzbins_km.push_back(0.365);
    vzbins_km.push_back(0.465);
    vzbins_km.push_back(0.700);
 
    std::vector<double> vptbins_km; 
    vptbins_km.push_back(0.0);
    vptbins_km.push_back(0.412);
    vptbins_km.push_back(0.556);
    vptbins_km.push_back(1.0);


    xbins[0]   = DLineBins(nx_km,   vxbins_km); 
    zbins[0]   = DLineBins(nz_km,   vzbins_km);
    ptbins[0]  = DLineBins(npt_km,  vptbins_km);
    phibins[0] = DLineBins(nphi_km, -180.0, 180.0); 
 
    xbins[1]   = DLineBins(nx_kp,   vxbins_kp); 
    zbins[1]   = DLineBins(nz_kp,   vzbins_kp); 
    ptbins[1]  = DLineBins(npt_kp,  vptbins_kp); 
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
