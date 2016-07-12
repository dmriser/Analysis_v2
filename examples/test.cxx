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
#include <map>

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
    
    int n_ec_samp_fail, n_ec_geo_fail, n_ec_edep_fail;
    int n_dc_r1_fail, n_dc_r3_fail;
    int n_nphe_fail, n_vz_fail;
    
    n_ec_samp_fail = 0; n_ec_geo_fail = 0; n_ec_edep_fail = 0;
    n_dc_r1_fail = 0; n_dc_r3_fail = 0;
    n_nphe_fail = 0; n_vz_fail = 0;
    
    // Sample event loop.
    for (int iev=0; iev<nev; iev++)
    {
        fReader->GetEntry(iev);
        h22Event event = fReader->GetEvent();
     
        // keep runno up to date
        if (runno != fReader->runno() ) { runno = fReader->runno(); filter.set_info(runno,opts.args["MC"].arg); }
        
        int e_index = filter.getByPID(event,11);
        if (e_index > -123) { pass++; }
        else fail++;
    
        //! Testing h22Event::theta_cc(int ipart)
        std::cout << event.theta_cc(0) << " ";
        
        std::map<std::string,bool> eid_map = filter.eid_map(event);

        if ( !eid_map["EC_GEO"] )     n_ec_geo_fail++;
        if ( !eid_map["EC_SAMPLING"]) n_ec_samp_fail++;
        if ( !eid_map["EC_EDEP"] )    n_ec_edep_fail++;
        if ( !eid_map["DCR1_FID"] )   n_dc_r1_fail++;
        if ( !eid_map["DCR3_FID"] )   n_dc_r3_fail++;
        if ( !eid_map["VZ"] )         n_vz_fail++;
        if ( !eid_map["CC_NPHE"] )     n_nphe_fail++;
        
    }
    
    std::cout << "Pass: " << pass << " Fail: " << fail << std::endl;
    std::cout << "ec geoemtry: " << (double) n_ec_geo_fail/fail << std::endl;
    std::cout << "ec sampling: " << (double) n_ec_samp_fail/fail << std::endl;
    std::cout << "ec en. dep : " << (double) n_ec_edep_fail/fail << std::endl;
    std::cout << "dc r1 fid  : " << (double) n_dc_r1_fail/fail << std::endl;
    std::cout << "dc r3 fid  : " << (double) n_dc_r3_fail/fail << std::endl;
    std::cout << "vz         : " << (double) n_vz_fail/fail << std::endl;
    std::cout << "cc nphe    : " << (double )n_nphe_fail/fail << std::endl;
    std::cout << " Done! " << std::endl;
    
    
    return 0;
}
