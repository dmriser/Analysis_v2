#ifndef EventMonitor_h
#define EventMonitor_h


 // Put your includes here 
#include "DBins.h" 
#include "DCut.h" 
#include "DEvent.h" 
#include "DSelection.h" 
#include "h22Event.h" 
#include "h22Reader.h" 

#include "histos.h"
#include "histos.cxx"

class EventMonitor : public h22Reader {
    public:
        EventMonitor();
        ~EventMonitor();

	 Histograms histos;

    void Loop();
    void ProcessEvent();
};
#endif
