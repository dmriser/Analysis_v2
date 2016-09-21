#include <iostream>
using std::string;
using std::cout;
using std::endl;

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
    TH2D * xByQQ[isect] = new TH2D(Form("%s_xByQQ_s%d",name.c_str(),isect),Form("%s x vs. Q^{2} Sect. %d",title.c_str(),isect), numberOfXBins, xMin, xMax, numberOfQQBins, qqMin, qqMax);
    TH2D * wByQQ[isect] = new TH2D(Form("%s_wByQQ_s%d",name.c_str(),isect),Form("%s w vs. Q^{2} Sect. %d",title.c_str(),isect), numberOfWBins, wMin, wMax, numberOfQQBins, qqMin, qqMax);
  }
}

void BaseDISHistograms::Save(string filename){

  TFile * file = TFile::Open(filename.c_str());

  if (file.IsOpen()){
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

  if (file.IsOpen()){
    for (int isect = 0; isect < 7; isect++){
      xByQQ[isect] = file->Get(Form("%s_xByQQ_s%d",name.c_str(),isect));
      wByQQ[isect] = file->Get(Form("%s_wByQQ_s%d",name.c_str(),isect));
    }
  } else {
    cout << " Error opening file... " << filename << endl; 
  }

  file->Close();
   
}



#endif
