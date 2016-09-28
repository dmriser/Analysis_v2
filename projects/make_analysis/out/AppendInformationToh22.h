#ifndef AppendInformationToh22_h
#define AppendInformationToh22_h


 // Put your includes here 
#include "DBins.h" 
#include "DCut.h" 
#include "DEvent.h" 
#include "DSelection.h" 
#include "h22Event.h" 
#include "h22Reader.h" 

class AppendInformationToh22 : public h22Reader {
    public:
        AppendInformationToh22();
        ~AppendInformationToh22();

    void Loop();
};
#endif
