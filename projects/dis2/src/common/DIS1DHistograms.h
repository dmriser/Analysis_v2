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

  int numberOfXBins; 
  double xMin, xMax, xWidth;

  int numberOfWBins; 
  double wMin, wMax, wWidth;

  int numberOfQQBins; 
  double qqMin, qqMax, qqWidth;

 public:
  void Create(BaseDISHistograms * baseHistograms); 
  void CreateByDivision(DIS1DHistograms *numerator, DIS1DHistograms *demoninator, string name, string title);
  void CreateFromExisting(DIS1DHistograms * sourceHistograms, string newName, string newTitle);
  void Divide(DIS1DHistograms * denominator);
  void Load(string inputFilenameWithExtension, string title);
  void Save(string outputFilenameWithExtension, string saveOption); 
  void Scale(double scaleValue);
  void ScaleByBinWidth();
  void SetErrors(); 
};

DIS1DHistograms::DIS1DHistograms(){
  numberOfXBins = 0; 
  xMin = 0; xMax = 0; xWidth = 0; 
  numberOfWBins = 0; 
  wMin = 0; wMax = 0; wWidth = 0; 
  numberOfQQBins = 0; 
  qqMin = 0; qqMax = 0; qqWidth = 0; 
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
  
    numberOfXBins = baseHistograms->xByQQ[sector]->GetXaxis()->GetNbins();
    xMin = baseHistograms->xByQQ[sector]->GetXaxis()->GetBinLowEdge(1);
    xMax = baseHistograms->xByQQ[sector]->GetXaxis()->GetBinUpEdge(numberOfXBins);
    xWidth = (xMax-xMin)/numberOfXBins;
  
    numberOfWBins = baseHistograms->wByQQ[sector]->GetXaxis()->GetNbins();
    wMin = baseHistograms->wByQQ[sector]->GetXaxis()->GetBinLowEdge(1);
    wMax = baseHistograms->wByQQ[sector]->GetXaxis()->GetBinUpEdge(numberOfWBins);
    wWidth = (wMax-wMin)/numberOfWBins;

    numberOfQQBins = baseHistograms->wByQQ[sector]->GetYaxis()->GetNbins();
    qqMin = baseHistograms->wByQQ[sector]->GetYaxis()->GetBinLowEdge(1);
    qqMax = baseHistograms->wByQQ[sector]->GetYaxis()->GetBinUpEdge(numberOfQQBins);
    qqWidth = (qqMax-qqMin)/numberOfQQBins;

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

  numberOfXBins = sourceHistograms->numberOfXBins; 
  xMin = sourceHistograms->xMin; 
  xMax = sourceHistograms->xMax; 
  xWidth = sourceHistograms->xWidth; 

  numberOfWBins = sourceHistograms->numberOfWBins; 
  wMin = sourceHistograms->wMin; 
  wMax = sourceHistograms->wMax; 
  wWidth = sourceHistograms->wWidth; 

  numberOfQQBins = sourceHistograms->numberOfQQBins; 
  qqMin = sourceHistograms->qqMin; 
  qqMax = sourceHistograms->qqMax; 
  qqWidth = sourceHistograms->qqWidth; 

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

void DIS1DHistograms::CreateByDivision(DIS1DHistograms *numerator, DIS1DHistograms *denominator, string name, string title){

  numberOfXBins = numerator->numberOfXBins; 
  xMin = numerator->xMin; 
  xMax = numerator->xMax; 
  xWidth = numerator->xWidth; 

  numberOfWBins = numerator->numberOfWBins; 
  wMin = numerator->wMin; 
  wMax = numerator->wMax; 
  wWidth = numerator->wWidth; 

  numberOfQQBins = numerator->numberOfQQBins; 
  qqMin = numerator->qqMin; 
  qqMax = numerator->qqMax; 
  qqWidth = numerator->qqWidth; 

  CreateFromExisting(numerator,name,title);
  Divide(denominator);
}

void DIS1DHistograms::Scale(double scaleValue){

  for (int sector=0; sector<7; sector++){
    allxByQQ[sector]->Scale(scaleValue/6.0);
    allwByQQ[sector]->Scale(scaleValue/6.0);

    for (int slice=0; slice<xByQQ[sector].size(); slice++){
      xByQQ[sector][slice]->Scale(scaleValue);
    }

    for (int slice=0; slice<wByQQ[sector].size(); slice++){
      wByQQ[sector][slice]->Scale(scaleValue);
    }
  }
}

void DIS1DHistograms::ScaleByBinWidth(){

  for (int sector=0; sector<7; sector++){
    allxByQQ[sector]->Scale(1/xWidth);
    allwByQQ[sector]->Scale(1/wWidth);
  
    for (int slice=0; slice<xByQQ[sector].size(); slice++){
      xByQQ[sector][slice]->Scale(1/(xWidth*qqWidth));
    }

    for (int slice=0; slice<wByQQ[sector].size(); slice++){
      wByQQ[sector][slice]->Scale(1/(wWidth*qqWidth));
    }

  }
  

}

#endif
