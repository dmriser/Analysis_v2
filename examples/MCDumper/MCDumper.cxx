#ifndef MCDumper_cxx
#define MCDumper_cxx

#include "MCDumper.h"

 // Put your includes here 
#include "DBins.h" 
#include "DCut.h" 
#include "DEvent.h" 
#include "DSelection.h" 
#include "h22Event.h" 
#include "h22Reader.h" 

 // Class Constructor/Destructor 
MCDumper::MCDumper(){ 
 }

MCDumper::~MCDumper(){ 
 }

void MCDumper::Loop(){

 // Event loop below. 
        for(int ievent=0; ievent<GetEntries(); ievent++){
                GetEntry(ievent); 
                DEvent event = h22Event(GetEvent()); 
		event.print(); 
        }
	cout << endl; 
}
#endif
