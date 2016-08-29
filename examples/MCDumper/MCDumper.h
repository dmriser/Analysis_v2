#ifndef MCDumper_h
#define MCDumper_h


 // Put your includes here 
#include "DBins.h" 
#include "DCut.h" 
#include "DEvent.h" 
#include "DSelection.h" 
#include "h22Event.h" 
#include "h22Reader.h" 

class MCDumper : public h22Reader {
    public:
        MCDumper();
        ~MCDumper();

    void Loop();
};
#endif
