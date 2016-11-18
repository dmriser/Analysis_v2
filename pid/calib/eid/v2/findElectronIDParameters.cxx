#ifndef findElectronIDParameters_cxx
#define findElectronIDParameters_cxx

#include "findElectronIDParameters.h"
#include "histos.h"
#include "histos.cxx"

#include <iostream>

 // Put your includes here 
#include "DCut.h" 
#include "DSelection.h" 
#include "h22Event.h" 
#include "h22Reader.h" 
#include "Parameters.h"
#include "ParameterSet.h"

 // Class Constructor/Destructor 
findElectronIDParameters::findElectronIDParameters(){ 
}

findElectronIDParameters::~findElectronIDParameters(){ 
}

void findElectronIDParameters::Loop(){

  // Event loop below. 
  int nen = GetEntries(); 

  for(int ievent=0; ievent<nen; ievent++){
    GetEntry(ievent); 
    ProcessEvent(); 
    if (ievent%1000 == 0) { std::cout << "\r done " << ievent << " of " << nen << std::flush; }
  } 
  
}

void findElectronIDParameters::ProcessEvent(){
  
  for (int ipart =0; ipart<event.gpart; ipart++){
    if (event.q[ipart] < 0) { histos.Fill(event, ipart); }
  }

}

void findElectronIDParameters::CalculateSamplingFractionCut(){

  // Routine is supposed to cut slices out of the etot/p vs. p 
  // then fit with gaussians in each bin and further fit the mean 
  // with pol3. 

  int numberOfSlices = histos.h2_ecSampling[0]->GetXaxis()->GetNbins();
  
  
}

void findElectronIDParameters::WriteHardCodedParameters(){
  ParameterSet zVertexLimitMin, zVertexLimitMax, ecUCoordMin, ecVCoordMin, ecWCoordMin,
    ecUCoordMax, ecVCoordMax, ecWCoordMax, ecEdepMin; 

  zVertexLimitMin.setName("ELECTRON_VZ_MIN"); 
  zVertexLimitMin.addValue(-27.7302); 
  zVertexLimitMin.addError(0.00); 

  zVertexLimitMax.setName("ELECTRON_VZ_MAX"); 
  zVertexLimitMax.addValue(-22.6864); 
  zVertexLimitMax.addError(0.00); 

  ecUCoordMin.setName("ELECTRON_ECU_MIN"); 
  ecUCoordMin.addValue(70.0);
  ecUCoordMin.addError(0.0);
  ecUCoordMax.setName("ELECTRON_ECU_MAX"); 
  ecUCoordMax.addValue(400.0);
  ecUCoordMax.addError(0.0);

  ecVCoordMin.setName("ELECTRON_ECV_MIN"); 
  ecVCoordMin.addValue(0.0);
  ecVCoordMin.addError(0.0);
  ecVCoordMax.setName("ELECTRON_ECV_MAX"); 
  ecVCoordMax.addValue(362.0);
  ecVCoordMax.addError(0.0);

  ecWCoordMin.setName("ELECTRON_ECW_MIN"); 
  ecWCoordMin.addValue(0.0);
  ecWCoordMin.addError(0.0);
  ecWCoordMax.setName("ELECTRON_ECW_MAX"); 
  ecWCoordMax.addValue(395.0);
  ecWCoordMax.addError(0.0);

  ecEdepMin.setName("ELECTRON_EC_EDEP_MIN");
  ecEdepMin.addValue(0.05); 
  ecEdepMin.addError(0.00); 

  eidParameters.addParameterSet(zVertexLimitMin);
  eidParameters.addParameterSet(zVertexLimitMax);
  eidParameters.addParameterSet(ecUCoordMin);
  eidParameters.addParameterSet(ecUCoordMax);
  eidParameters.addParameterSet(ecVCoordMin);
  eidParameters.addParameterSet(ecVCoordMax);
  eidParameters.addParameterSet(ecWCoordMin);
  eidParameters.addParameterSet(ecWCoordMax);
  eidParameters.addParameterSet(ecEdepMin); 
}

#endif
