//////////////////////////////////////
/*
 
 David Riser, University of Connecticut
 
 test.cxx -> test the usage of libraries
 
    July 9, 2016
 
 */
//////////////////////////////////////

// c++ includes
#include <iostream>
#include <vector>

// my includes
#include "h22Event.h"
#include "h22Option.h"
#include "h22Reader.h"
#include "pars.h"   

// root includes
#include "TVector3.h"

int main(int argc, char * argv[])
{
    
    
    // Setup Options
    h22Options opts;
    opts.set(argc,argv);
    
    // Testing
    epars pars;
    pars.load(opts.args["EPARS_FILE"].args);
    
    // Setup Reader
    int GSIM = 0;                             //! Set to false, not initializing MC banks.
    h22Reader * fReader = new h22Reader(GSIM);
    
    for (auto it=opts.ifiles.begin(); it<opts.ifiles.end(); it++) { fReader->AddFile(*it); }
    fReader->Init();
    
    int pass, fail;
    pass = 0; fail = 0;
    
    // Sample event loop.
    for (int iev=0; iev<opts.args["N"].arg; iev++)
    {
        fReader->GetEntry(iev);
        h22Event event = fReader->GetEvent();
        //event.printEvent();
        
        if (event.tl3_x[0] > pars.dc_left(3,event.tl3_y[0]) &&
            event.tl3_x[0] > pars.dc_right(3, event.tl3_y[0])) pass++;
        else { fail++; }
        
    }
    
    std::cout << " Pass: " << pass << " Fail: " << fail << std::endl;
    std::cout << " Done! " << std::endl;
    
    
    return 0;
}
