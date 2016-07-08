//////////////////////////////////////
/*
 
 David Riser, University of Connecticut
 
 test.C -> test the usage of libraries
 
 */
//////////////////////////////////////

// c++ includes
#include <iostream>
#include <vector>

// my includes
#include "h22Event.h"
#include "h22Option.h"
#include "h22Reader.h"

// root includes
#include "TVector3.h"

int main(int argc, char * argv[])
{
    
    
    // Setup Options
    h22Options OPT;
    OPT.set(argc,argv);
    
    // Setup Reader
    int GSIM = 0;                             //! Set to false, not initializing MC banks.
    h22Reader * fReader = new h22Reader(GSIM);
//    vector<string>::iterator it;
    for (auto it=OPT.ifiles.begin(); it<OPT.ifiles.end(); it++) { fReader->AddFile(*it); }
    fReader->Init();
    
    // Sample event loop.
    for (int iev=0; iev<OPT.args["N"].arg; iev++)
    {
        fReader->GetEntry(iev);
        h22Event event = fReader->GetEvent();
        event.printEvent();
        
    }
    
    std::cout << " Done! " << std::endl;
    
    
    return 0;
}
