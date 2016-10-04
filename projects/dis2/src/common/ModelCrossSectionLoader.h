#ifndef model_cs_loader_h
#define model_cs_loader_h

#include <iostream>
using std::cout; 
using std::endl; 
using std::string; 
using std::vector; 

#include "DIS1DHistograms.h"
#include "F1F209Wrapper.hh"

class ModelCrossSectionLoader{
 public:
  ModelCrossSectionLoader();
  ~ModelCrossSectionLoader();

  F1F209Wrapper * model; 
  DIS1DHistograms * crossSection; 
  DIS1DHistograms * crossSectionAverage; 

  void provideBinningTemplate(DIS1DHistograms * templateHistograms);
  void loadCrossSection(double A, double Z, double beamEnergy);
  void Save(string outputFilenameWithExtension, string saveOption); 
  DIS1DHistograms * getCrossSection(){ return crossSection; } 
  DIS1DHistograms * getCrossSectionAverage(){ return crossSectionAverage; }
 
};

ModelCrossSectionLoader::ModelCrossSectionLoader(){
  model = new F1F209Wrapper();
  crossSection = new DIS1DHistograms(); 
  crossSectionAverage = new DIS1DHistograms(); 
}

ModelCrossSectionLoader::~ModelCrossSectionLoader(){

}

void ModelCrossSectionLoader::provideBinningTemplate(DIS1DHistograms * templateHistograms){
  crossSection->CreateFromExisting(templateHistograms,"crossSection","crossSection");
  crossSectionAverage->CreateFromExisting(templateHistograms,"crossSectionAverage","crossSectionAverage");
}

void ModelCrossSectionLoader::loadCrossSection(double A, double Z, double beamEnergy){

  double CS_MIN_VALUE = 1e-10; 
  double CS_MAX_VALUE = 1e10;
  
  for (int sector=0; sector<7; sector++){

    for (int slice=0; slice<crossSection->xByQQ[sector].size(); slice++){
      for (int xBin=1; xBin<=crossSection->numberOfXBins; xBin++){
	double xLow = crossSection->xByQQ[sector][slice]->GetBinLowEdge(xBin);
	double xUp = xLow + crossSection->xByQQ[sector][slice]->GetBinWidth(xBin);
	double xMid = crossSection->xByQQ[sector][slice]->GetBinCenter(xBin);
	
	double qqLow = crossSection->qqMin + slice*crossSection->qqWidth; 
	double qqUp = crossSection->qqMin + (1+slice)*crossSection->qqWidth;
	double qqMid = crossSection->qqWidth/2 + qqLow;

	double csLow = model->GetXS(A, Z, beamEnergy, xLow, qqMid);
	double csUp = model->GetXS(A, Z, beamEnergy, xUp, qqMid);
	double csMid = model->GetXS(A, Z, beamEnergy, xMid, qqMid);
	double csAverage = (csUp-csLow)/2; 

	if (csMid > CS_MIN_VALUE && csMid < CS_MAX_VALUE) {
	  crossSection->xByQQ[sector][slice]->SetBinContent(xBin,csMid);
	  crossSection->xByQQ[sector][slice]->SetBinError(xBin,0.0);
	}
	if (csAverage > CS_MIN_VALUE && csAverage < CS_MAX_VALUE) {
	  crossSectionAverage->xByQQ[sector][slice]->SetBinContent(xBin,csAverage);
	  crossSectionAverage->xByQQ[sector][slice]->SetBinError(xBin,0.0);
	}      
      }
    }

    for (int slice=0; slice<crossSection->wByQQ[sector].size(); slice++){
      for (int wBin=1; wBin<=crossSection->numberOfWBins; wBin++){
	double wLow = crossSection->wByQQ[sector][slice]->GetBinLowEdge(wBin);
	double wUp = wLow + crossSection->wByQQ[sector][slice]->GetBinCenter(wBin);
	double wMid = crossSection->wByQQ[sector][slice]->GetBinCenter(wBin);

	double qqLow = crossSection->qqMin + slice*crossSection->qqWidth; 
	double qqUp = crossSection->qqMin + (1+slice)*crossSection->qqWidth;
	double qqMid = crossSection->qqWidth/2 + qqLow; 
	
	double csLow = model->GetXSByWQQ(A, Z, beamEnergy, wLow, qqMid);
	double csUp = model->GetXSByWQQ(A, Z, beamEnergy, wUp, qqMid);
	double csMid = model->GetXSByWQQ(A, Z, beamEnergy, wMid, qqMid);
	double csAverage = (csUp-csLow)/2; 
  
	if (csMid > CS_MIN_VALUE && csMid < CS_MAX_VALUE) {
	  crossSection->wByQQ[sector][slice]->SetBinContent(wBin,csMid);
	  crossSection->wByQQ[sector][slice]->SetBinError(wBin,0.0);
	}
	if (csAverage > CS_MIN_VALUE && csAverage < CS_MAX_VALUE) {
	  crossSectionAverage->wByQQ[sector][slice]->SetBinContent(wBin,csAverage);
	  crossSectionAverage->wByQQ[sector][slice]->SetBinError(wBin,0.0);
	}      

      }
    }
  }
}

void ModelCrossSectionLoader::Save(string outputFilenameWithExtension, string saveOption){

  // Don't do recreate twice it kills the first file.
  if (saveOption == "recreate" || saveOption == "RECREATE"){
    crossSection->Save(outputFilenameWithExtension,saveOption);
    crossSectionAverage->Save(outputFilenameWithExtension,"update");
  }
  
  else {
    crossSection->Save(outputFilenameWithExtension,saveOption);
    crossSectionAverage->Save(outputFilenameWithExtension,saveOption);
  }


}

#endif
