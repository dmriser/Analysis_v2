#ifndef dis_1d_histograms_h
#define dis_1d_histograms_h

#include <iostream>
#include <vector>
using std::cout; 
using std::endl; 
using std::string; 
using std::vector; 

#include "BaseDISHistograms.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"

class DIS1DHistograms{
 public:
  DIS1DHistograms();
  ~DIS1DHistograms();

  TFile * inputFile; 
  TH1D * allxByQQ[7];
  TH1D * allwByQQ[7];
  vector<vector<TH1D*> > xByQQ; 
  vector<vector<TH1D*> > wByQQ; 

 public:
  void Create(BaseDISHistograms * baseHistograms); 
  void CreateFromExisting(DIS1DHistograms * sourceHistograms, string newName, string newTitle);
  void Load(string inputFilenameWithExtension, string title);
  void Save(string outputFilenameWithExtension, string saveOption); 
  void SetErrors(); 
  void Divide(DIS1DHistograms * denominator);
};

DIS1DHistograms::DIS1DHistograms(){

}

DIS1DHistograms::~DIS1DHistograms(){
  if (inputFile->IsOpen()){
    inputFile->Close();
  }
}

void DIS1DHistograms::Create(BaseDISHistograms * baseHistograms){

  for (int sector=0; sector<7; sector++){
    vector<TH1D*> xContainer; 
    vector<TH1D*> wContainer; 

    string xByQQBaseName = baseHistograms->xByQQ[sector]->GetName(); 
    string wByQQBaseName = baseHistograms->wByQQ[sector]->GetName(); 
    string xByQQBaseTitle = baseHistograms->xByQQ[sector]->GetTitle(); 
    string wByQQBaseTitle = baseHistograms->wByQQ[sector]->GetTitle(); 
  
    int numberOfXBins = baseHistograms->xByQQ[sector]->GetXaxis()->GetNbins();
    double xMin = baseHistograms->xByQQ[sector]->GetXaxis()->GetBinLowEdge(1);
    double xMax = baseHistograms->xByQQ[sector]->GetXaxis()->GetBinUpEdge(numberOfXBins);
  
    int numberOfWBins = baseHistograms->wByQQ[sector]->GetXaxis()->GetNbins();
    double wMin = baseHistograms->wByQQ[sector]->GetXaxis()->GetBinLowEdge(1);
    double wMax = baseHistograms->wByQQ[sector]->GetXaxis()->GetBinUpEdge(numberOfWBins);
  
    int numberOfQQBins = baseHistograms->wByQQ[sector]->GetYaxis()->GetNbins();
    double qqMin = baseHistograms->wByQQ[sector]->GetYaxis()->GetBinLowEdge(1);
    double qqMax = baseHistograms->wByQQ[sector]->GetYaxis()->GetBinUpEdge(numberOfQQBins);

    allxByQQ[sector] = new TH1D(xByQQBaseName.c_str(),xByQQBaseTitle.c_str(),numberOfXBins,xMin,xMax);
    allwByQQ[sector] = new TH1D(wByQQBaseName.c_str(),wByQQBaseTitle.c_str(),numberOfWBins,wMin,wMax);
    baseHistograms->xByQQ[sector]->ProjectionX(xByQQBaseName.c_str(),1,numberOfQQBins);
    baseHistograms->wByQQ[sector]->ProjectionX(wByQQBaseName.c_str(),1,numberOfQQBins);
    
    for (int slice=0; slice<numberOfQQBins; slice++){
      string xTitle = Form("%s Slice %d",xByQQBaseTitle.c_str(),slice); 
      string wTitle = Form("%s Slice %d",wByQQBaseTitle.c_str(),slice); 
      string xName = Form("%s_slice%d",xByQQBaseName.c_str(),slice); 
      string wName = Form("%s_slice%d",wByQQBaseName.c_str(),slice); 

      TH1D * xSlice = new TH1D(xName.c_str(),xTitle.c_str(),numberOfXBins,xMin,xMax);
      TH1D * wSlice = new TH1D(wName.c_str(),wTitle.c_str(),numberOfWBins,wMin,wMax);
    
      baseHistograms->xByQQ[sector]->ProjectionX(xName.c_str(),slice+1,slice+2);
      baseHistograms->wByQQ[sector]->ProjectionX(wName.c_str(),slice+1,slice+2);

      xContainer.push_back(xSlice);
      wContainer.push_back(wSlice);
    } 
    
    xByQQ.push_back(xContainer);
    wByQQ.push_back(wContainer);
  }  
}

void DIS1DHistograms::Load(string inputFilenameWithExtension, string title){
  
  if (!inputFile->IsOpen()){
    inputFile = new TFile(inputFilenameWithExtension.c_str());
  }

  // How do we load unspecified number of slices?

}

void DIS1DHistograms::Save(string outputFilenameWithExtension, string saveOption){
  TFile * outputFile = TFile::Open(outputFilenameWithExtension.c_str(),saveOption.c_str());

  if (outputFile->IsOpen()){
    for (int sector=0; sector<7; sector++){
      allxByQQ[sector]->Write();
      allwByQQ[sector]->Write();
    }
  

    for (int sector=0; sector<xByQQ.size(); sector++){
      for (int slice=0; slice<xByQQ[sector].size(); slice++){
	xByQQ[sector][slice]->Write();
      }
    }

    for (int sector=0; sector<wByQQ.size(); sector++){
      for (int slice=0; slice<wByQQ[sector].size(); slice++){
	wByQQ[sector][slice]->Write();
      }
    }
  }
 
  outputFile->Close();
}

void DIS1DHistograms::SetErrors(){
    for (int sector=0; sector<7; sector++){
      allxByQQ[sector]->Sumw2();
      allwByQQ[sector]->Sumw2();
    }
  

    for (int sector=0; sector<xByQQ.size(); sector++){
      for (int slice=0; slice<xByQQ[sector].size(); slice++){
	xByQQ[sector][slice]->Sumw2();
      }
    }

    for (int sector=0; sector<wByQQ.size(); sector++){
      for (int slice=0; slice<wByQQ[sector].size(); slice++){
	wByQQ[sector][slice]->Sumw2();
      }
    }
}

void DIS1DHistograms::Divide(DIS1DHistograms * denominator){

  for (int sector=0; sector<7; sector++){
    allxByQQ[sector]->Divide(denominator->allxByQQ[sector]); 
    allwByQQ[sector]->Divide(denominator->allwByQQ[sector]); 

    for (int slice=0; slice<xByQQ[sector].size(); slice++){
      xByQQ[sector][slice]->Divide(denominator->xByQQ[sector][slice]);
    }

    for (int slice=0; slice<wByQQ[sector].size(); slice++){
      wByQQ[sector][slice]->Divide(denominator->wByQQ[sector][slice]);
    }
  }

}

void DIS1DHistograms::CreateFromExisting(DIS1DHistograms * sourceHistograms, string newName, string newTitle){
  
  for (int sector=0; sector<7; sector++){
    allxByQQ[sector] = (TH1D*) sourceHistograms->allxByQQ[sector]->Clone();
    allxByQQ[sector]->SetName(Form("%s_xByQQ_s%d",newName.c_str(),sector));
    allxByQQ[sector]->SetName(Form("%s x vs. Q^{2} Sect. %d",newTitle.c_str(),sector));

    allwByQQ[sector] = (TH1D*) sourceHistograms->allwByQQ[sector]->Clone();
    allwByQQ[sector]->SetName(Form("%s_wByQQ_s%d",newName.c_str(),sector));
    allwByQQ[sector]->SetName(Form("%s w vs. Q^{2} Sect. %d",newTitle.c_str(),sector));
  
    vector<TH1D*> xContainer; 
    vector<TH1D*> wContainer; 
    for (int slice=0; slice<sourceHistograms->xByQQ[sector].size(); slice++){
      TH1D * xSlice = (TH1D*) sourceHistograms->xByQQ[sector][slice]->Clone();
      xSlice->SetName(Form("%s_xByQQ_s%d_slice%d",newName.c_str(),sector,slice));
      xSlice->SetTitle(Form("%s x vs. Q^{2} Sect. %d Slice %d",newTitle.c_str(),sector,slice));
      xContainer.push_back(xSlice);
    }

    for (int slice=0; slice<sourceHistograms->wByQQ[sector].size(); slice++){
      TH1D * wSlice = (TH1D*) sourceHistograms->wByQQ[sector][slice]->Clone();
      wSlice->SetName(Form("%s_wByQQ_s%d_slice%d",newName.c_str(),sector,slice));
      wSlice->SetTitle(Form("%s w vs. Q^{2} Sect. %d Slice %d",newTitle.c_str(),sector,slice));
      wContainer.push_back(wSlice);
    }
    
    xByQQ.push_back(xContainer);
    wByQQ.push_back(wContainer);
  }

}

#endif
