#ifndef missing_mass_histos_h 
#define missing_mass_histos_h 

#include <iostream>

#include "Config.h"
#include "PhysicsEvent.h"

#include "TFile.h"
#include "TH1.h"

class MissingMassHistos {

 public:
 MissingMassHistos(Config c, std::string title) : fTitle(title) {
   fMin = c.cuts["missing_mass"]["min"] *0.8; 
   fMax = c.cuts["missing_mass"]["min"] *1.2; 

   h1_zoom = new TH1F(Form("h1_missingMass_zoom_%s",fTitle.c_str()), 
		      Form("h1_missingMass_zoom_%s",fTitle.c_str()), 
		      100, fMin, fMax);
   
   h1_wide = new TH1F(Form("h1_missingMass_wide_%s",fTitle.c_str()), 
		      Form("h1_missingMass_wide_%s",fTitle.c_str()), 
		      200, 0.5, 3.0);

   fFile = new TFile(); 
  }

 MissingMassHistos(std::string filename, std::string title) : fTitle(title) {
    Load(filename);
  }

  ~MissingMassHistos(){
    if(fFile->IsOpen()){
      fFile->Write(); 
      fFile->Close(); 
    }
  }

  void Fill(PhysicsEvent ev){
    h1_zoom->Fill(sqrt(ev.mm2)); 
    h1_wide->Fill(sqrt(ev.mm2)); 
  }

  void Load(std::string filename){
    fFile = TFile::Open(filename.c_str());

    h1_wide = (TH1F*) fFile->Get(Form("h1_missingMass_wide_%s",fTitle.c_str())); 
    h1_zoom = (TH1F*) fFile->Get(Form("h1_missingMass_zoom_%s",fTitle.c_str())); 
  }

  void Save(TFile *out){
    if(out->IsOpen()){
      h1_zoom->Write();
      h1_wide->Write();
    }
  }

 TH1F *h1_zoom, *h1_wide; 

 protected:
 TFile *fFile; 
 std::string fTitle; 
 float fMin, fMax; 

};

#endif
