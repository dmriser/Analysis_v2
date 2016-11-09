#ifndef findElectronIDParameters_cxx
#define findElectronIDParameters_cxx

#include "findElectronIDParameters.h"
#include "histos.h"
#include "histos.cxx"

 // Put your includes here 
#include "DCut.h" 
#include "DSelection.h" 
#include "h22Event.h" 
#include "h22Reader.h" 

 // Class Constructor/Destructor 
findElectronIDParameters::findElectronIDParameters(){ 
 }

findElectronIDParameters::~findElectronIDParameters(){ 
 }

void findElectronIDParameters::Loop(){

 // Event loop below. 
        for(int ievent=0; ievent<GetEntries(); ievent++){
                GetEntry(ievent); 
                ProcessEvent(); 
        } 
}

void findElectronIDParameters::ProcessEvent(){
	 // Do your stuff here. 
	 histos.Fill(event);
}
#endif
