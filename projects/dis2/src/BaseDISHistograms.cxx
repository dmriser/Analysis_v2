#include <iostream>
using std::string;
using std::cout;
using std::endl;

// me
#include "PhysicsEvent.h"

// root
#include "TFile.h"
#include "TH2.h"

#include "BaseDISHistograms.h"

#ifndef base_dis_histograms_cxx
#define base_dis_histograms_cxx

//ClassImp(BaseDISHistograms);

BaseDISHistograms::BaseDISHistograms(){

}

BaseDISHistograms::~BaseDISHistograms(){

}

void BaseDISHistograms::Init(string name, string title){
  for (int isect = 0; isect < 7; isect++){
    xByQQ[isect] = new TH2I(Form("%s_xByQQ_s%d",name.c_str(),isect),Form("%s x vs. Q^{2} Sect. %d",title.c_str(),isect), numberOfXBins, xMin, xMax, numberOfQQBins, qqMin, qqMax);
    wByQQ[isect] = new TH2I(Form("%s_wByQQ_s%d",name.c_str(),isect),Form("%s w vs. Q^{2} Sect. %d",title.c_str(),isect), numberOfWBins, wMin, wMax, numberOfQQBins, qqMin, qqMax);
  }
}

void BaseDISHistograms::Save(string filename, string option){

  TFile * file = TFile::Open(filename.c_str(),option.c_str());

  if (file->IsOpen()){
    for (int isect = 0; isect < 7; isect++){
      xByQQ[isect]->Write();
      wByQQ[isect]->Write();
    }
  } else {
    cout << " Error opening file... " << filename << endl; 
  }

  file->Write();
  file->Close();
}

void BaseDISHistograms::Load(string filename, string name){

  TFile * file = TFile::Open(filename.c_str());

  if (file->IsOpen()){
    for (int isect = 0; isect < 7; isect++){
      xByQQ[isect] = (TH2I*) file->Get(Form("%s_xByQQ_s%d",name.c_str(),isect));
      wByQQ[isect] = (TH2I*) file->Get(Form("%s_wByQQ_s%d",name.c_str(),isect));
    }
  } else {
    cout << " Error opening file... " << filename << endl; 
  }

  file->Close();
   
}

void BaseDISHistograms::Fill(PhysicsEvent event, int sector){
  // We assume that nobody sends us sector > 6 or sector < 1
  xByQQ[0]      ->Fill(event.x, event.qq);
  xByQQ[sector] ->Fill(event.x, event.qq);
  wByQQ[0]      ->Fill(event.w, event.qq);
  wByQQ[sector] ->Fill(event.w, event.qq);
}

void BaseDISHistograms::FillxByQQ(double x, double qq, int sector){
  xByQQ[0]      ->Fill(x, qq);
  xByQQ[sector] ->Fill(x, qq);
}

void BaseDISHistograms::FillwByQQ(double w, double qq, int sector){
  wByQQ[0]      ->Fill(w, qq);
  wByQQ[sector] ->Fill(w, qq);
}

void BaseDISHistograms::Rebin2D(int xFactor, int yFactor){
  for (int ihist = 0; ihist < 7; ihist++) {
    xByQQ[ihist]->Rebin2D(xFactor, yFactor);
    wByQQ[ihist]->Rebin2D(xFactor, yFactor); 
  }
}

#endif
