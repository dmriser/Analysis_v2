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
#include "Corrections.h"
#include "h22Event.h"
#include "h22Option.h"
#include "h22Reader.h"
#include "pars.h"
#include "ParticleFilter.h"

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
    h22Reader * fReader = new h22Reader(opts.args["MC"].arg);
    
    for (auto it=opts.ifiles.begin(); it<opts.ifiles.end(); it++) { fReader->AddFile(*it); }
    fReader->Init();
    
    // Setup particle filter
    ParticleFilter filter(opts.args["EPARS_FILE"].args);
    int pass, fail;
    pass = 0; fail = 0;
    
    std::cout << " Total number of events loaded: " << fReader->GetEntries() << std::endl;
    
    long int nev = opts.args["N"].arg;
    if (nev > fReader->GetEntries()) nev = fReader->GetEntries();
    
    int runno = fReader->runno();
    
    // Sample event loop.
    for (int iev=0; iev<nev; iev++)
    {
        fReader->GetEntry(iev);
        h22Event event = fReader->GetEvent();
     
        // keep runno up to date
        if (runno != fReader->runno() ) { runno = fReader->runno(); }
        
        int e_index = filter.getByPID(event,11);
        if (e_index > -123) { pass++; }
        else fail++;
        
    }
    
    std::cout << "Pass: " << pass << " Fail: " << fail << std::endl;
    std::cout << " Done! " << std::endl;
    
    
    return 0;
}
