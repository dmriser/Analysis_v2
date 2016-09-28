#ifndef AppendInformationToh22_cxx
#define AppendInformationToh22_cxx

#include "AppendInformationToh22.h"

 // Put your includes here 
#include "DBins.h" 
#include "DCut.h" 
#include "DEvent.h" 
#include "DSelection.h" 
#include "h22Event.h" 
#include "h22Reader.h" 

 // Class Constructor/Destructor 
AppendInformationToh22::AppendInformationToh22(){ 
 }

AppendInformationToh22::~AppendInformationToh22(){ 
 }

void AppendInformationToh22::Loop(){

 // Event loop below. 
        for(int ievent=0; ievent<GetEntries(); ievent++){
                GetEntry(ievent); 
                DEvent event = h22Event(GetEvent()); 
        } 
}
#endif
