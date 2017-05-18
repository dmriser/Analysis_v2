#ifndef fitter_h
#define fitter_h

#include "Bins.h"
#include "Constants.h"
#include "Fits.h"
#include "Histos.h"

class Fitter{
 public:
 Fitter() {

  }

 virtual void Fit(Histos *h, Fits *f){

 }
  
};

class BasicFitter : public Fitter {
 public:
  void Fit(Histos *h, Fits *f){

   for (int s=0; s<constants::NSECT+1; s++){
     for (int i=0; i<f->GetBinning()->GetXBins()->GetNumber()+1; i++){
       for(int j=0; j<f->GetBinning()->GetZBins()->GetNumber()+1; j++){
	 for(int k=0; k<f->GetBinning()->GetPtBins()->GetNumber()+1; k++){
	   std::string current_name(Form("fit_asym_%s_sect%d_x%d_z%d_pt%d_%s", constants::Names::mesons[h->GetMesonIndex()].c_str(), s, i, j, k, h->GetName().c_str()));
 
	   f->fit_asym[s][i][j][k] = new TF1(current_name.c_str(),"[0]*sin((3.14159/180.0)*x)",f->GetBinning()->GetPhiBins()->GetMin(), f->GetBinning()->GetPhiBins()->GetMax());
	   h->h1_asym[s][i][j][k]->Fit(current_name.c_str(), "RQ");
	 }
       }
     }
   }
 
  }

};

class BasicAllMomentFitter : public Fitter {
 public:
  void Fit(Histos *h, Fits *f){
   std::string fitFunction("[0]*sin((3.14159/180.0)*x)/(1+[1]*cos((3.14159/180.0)*x)+[2]*cos(2*(3.14159/180.0)*x))"); 

   for (int s=0; s<constants::NSECT+1; s++){
     for (int i=0; i<f->GetBinning()->GetXBins()->GetNumber()+1; i++){
       for(int j=0; j<f->GetBinning()->GetZBins()->GetNumber()+1; j++){
	 for(int k=0; k<f->GetBinning()->GetPtBins()->GetNumber()+1; k++){
	   std::string current_name(Form("fit_asym_%s_sect%d_x%d_z%d_pt%d_%s", constants::Names::mesons[h->GetMesonIndex()].c_str(), s, i, j, k, h->GetName().c_str()));
	   
	   f->fit_asym[s][i][j][k] = new TF1(current_name.c_str(),fitFunction.c_str(),f->GetBinning()->GetPhiBins()->GetMin(), f->GetBinning()->GetPhiBins()->GetMax());
	   h->h1_asym[s][i][j][k]->Fit(current_name.c_str(), "RQ");
	 }
       }
     }
   }
 
  }
};

#endif
