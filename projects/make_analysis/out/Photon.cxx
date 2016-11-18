#ifndef Photon_cxx
#define Photon_cxx

#include "Photon.h"
#include "histos.h"
#include "histos.cxx"

 // Put your includes here 
#include "DCut.h" 
#include "DSelection.h" 
#include "h22Event.h" 
#include "h22Reader.h" 

 // Class Constructor/Destructor 
Photon::Photon(){ 
 }

Photon::~Photon(){ 
 }

void Photon::Loop(){

 // Event loop below. 
        for(int ievent=0; ievent<GetEntries(); ievent++){
                GetEntry(ievent); 
                ProcessEvent(); 
        } 
}

void Photon::ProcessEvent(){
	 // Do your stuff here. 
	 histos.Fill(event);
}
#endif
