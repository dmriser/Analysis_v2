#ifndef findElectronIDParameters_h
#define findElectronIDParameters_h


 // Put your includes here 
#include "DBins.h" 
#include "DCut.h" 
#include "DEvent.h" 
#include "DSelection.h" 
#include "h22Event.h" 
#include "h22Reader.h" 

#include "histos.h"
#include "histos.cxx"

class findElectronIDParameters : public h22Reader {
    public:
        findElectronIDParameters();
        ~findElectronIDParameters();

	 Histograms histos;

    void Loop();
    void ProcessEvent();
};
#endif
