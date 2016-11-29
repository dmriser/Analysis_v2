#ifndef EventMonitor_cxx
#define EventMonitor_cxx

#include "EventMonitor.h"
#include "histos.h"
#include "histos.cxx"

 // Put your includes here 
#include "DCut.h" 
#include "DSelection.h" 
#include "h22Event.h" 
#include "h22Reader.h" 

 // Class Constructor/Destructor 
EventMonitor::EventMonitor(){ 
 }

EventMonitor::~EventMonitor(){ 
 }

void EventMonitor::Loop(){

 // Event loop below. 
	 Init(); 
	 histos.Init(); 
        for(int ievent=0; ievent<GetEntries(); ievent++){
                GetEntry(ievent); 
                ProcessEvent(); 
        } 
}

void EventMonitor::ProcessEvent(){
	 // Do your stuff here. 
	 histos.Fill(event);
}
#endif
