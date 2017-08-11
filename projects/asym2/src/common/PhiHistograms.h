#ifndef phi_histos_h
#define phi_histos_h

// standard includes 
#include <iostream>
#include <vector>

// this library includes
#include "DBins.h"
#include "PhysicsEvent.h"

// this project
#include "Config.h"
#include "Types.h"

// external 
#include "TFile.h"

class PhiHistos {
  
 public:
  PhiHistos(){
   
  }

  ~PhiHistos(){
    if(fFile){
      if(fFile->IsOpen()){
	fFile->Close();
      }
    }
    
  }

  void Init(std::map<int, DLineBins> bins,
	    std::string name){
    fName = name; 
    fBins = bins; 

    // there are more elegant ways, but im trying to 
    // be a physicist 
    for(int b=0; b<fBins[axis::x].GetNumber(); b++){
      std::string title(Form("phi_x%d_%s", b, fName.c_str()));

      histos[axis::x].push_back(new TH1F(title.c_str(), 
					 title.c_str(),
					 fBins[axis::phi].GetNumber(),
					 fBins[axis::phi].GetLimits().data()));      
    }
    for(int b=0; b<fBins[axis::q2].GetNumber(); b++){
      std::string title(Form("phi_q2%d_%s", b, fName.c_str()));

      histos[axis::q2].push_back(new TH1F(title.c_str(), 
					 title.c_str(),
					 fBins[axis::phi].GetNumber(),
					 fBins[axis::phi].GetLimits().data()));      
    }
    for(int b=0; b<fBins[axis::z].GetNumber(); b++){
      std::string title(Form("phi_z%d_%s", b, fName.c_str()));

      histos[axis::z].push_back(new TH1F(title.c_str(), 
					 title.c_str(),
					 fBins[axis::phi].GetNumber(),
					 fBins[axis::phi].GetLimits().data()));      
    }
    for(int b=0; b<fBins[axis::pt].GetNumber(); b++){
      std::string title(Form("phi_pt%d_%s", b, fName.c_str()));

      histos[axis::pt].push_back(new TH1F(title.c_str(), 
					  title.c_str(),
					  fBins[axis::phi].GetNumber(),
					  fBins[axis::phi].GetLimits().data()));      
    }
 

  }

  void Fill(PhysicsEvent ev){
    std::map<int,int> indices;
    

    // What bin does this event belong in?
    indices[axis::x]  = fBins[axis::x] .FindBin(ev.x); 
    indices[axis::q2] = fBins[axis::q2].FindBin(ev.qq); 
    indices[axis::z]  = fBins[axis::z] .FindBin(ev.z); 
    indices[axis::pt] = fBins[axis::pt].FindBin(ev.pT); 

    // Is this a safe bin?
    if(IsSafe(indices)){

      // Houston we have liftoff.
      histos[axis::x] [indices[axis::x]] ->Fill(ev.phiHadron);
      histos[axis::q2][indices[axis::q2]]->Fill(ev.phiHadron);
      histos[axis::z] [indices[axis::z]] ->Fill(ev.phiHadron);
      histos[axis::pt][indices[axis::pt]]->Fill(ev.phiHadron);
    }

  }

  // is the index safe ?
  bool IsSafe(std::map<int,int> indices){
    for(std::pair<int,int> index : indices){
      if( (index.second < 0) || (index.second >= fBins[index.first].GetNumber()) ){
	return false; 
      }
    }

    return true;
  }

  void Save(TFile *out){
    
    if (out->IsOpen()){
      out->mkdir("phi");
      out->cd("phi");

      for(int b=0; b<fBins[axis::x].GetNumber(); b++){
	histos[axis::x][b]->Write();
      }
      
      for(int b=0; b<fBins[axis::q2].GetNumber(); b++){
	histos[axis::q2][b]->Write();
      }
      
      for(int b=0; b<fBins[axis::z].GetNumber(); b++){
	histos[axis::z][b]->Write();
      }
      
      for(int b=0; b<fBins[axis::pt].GetNumber(); b++){
	histos[axis::pt][b]->Write();
      }
      
      out->Write();
      out->cd("/");
    }
  }

  void Load(){

  }
  

  // the histograms have index 
  // axis, heliciy 
  std::map<int, std::vector<TH1F*> > histos; 

 protected:
  std::string               fName; 
  std::map<int, DLineBins>  fBins; 

  TFile * fFile; 
};

#endif
