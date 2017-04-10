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
    
    // Setup Reader
    h22Reader * fReader = new h22Reader(opts.args["MC"].arg);

    vector<string>::iterator it; 
    for (it=opts.ifiles.begin(); it<opts.ifiles.end(); it++) { fReader->AddFile(*it); }
    fReader->Init();
    
    // Setup particle filter
    ParticleFilter filter(opts.args["EPARS"].args,opts.args["HPARS"].args);

    int pip, pim, prot;
    pip = 0; pim = 0; prot = 0;
    
    int pass, fail;
    pass = 0; fail = 0;
    
    std::cout << " Total number of events loaded: " << fReader->GetEntries() << std::endl;
    
    long int nev = opts.args["N"].arg;
    if (nev > fReader->GetEntries()) nev = fReader->GetEntries();
    
    int runno = fReader->GetRunNumber();
    
    int n_ec_samp_fail, n_ec_geo_fail, n_ec_edep_fail;
    int n_dc_r1_fail, n_dc_r3_fail;
    int n_nphe_fail, n_vz_fail;
    int n_cc_fid_fail;
    
    n_ec_samp_fail = 0; n_ec_geo_fail = 0; n_ec_edep_fail = 0;
    n_dc_r1_fail = 0; n_dc_r3_fail = 0;
    n_nphe_fail = 0; n_vz_fail = 0;
    n_cc_fid_fail = 0;
    
    int prot_qc, prot_dvz, prot_dbeta, prot_dcfid;
    int pip_qc, pip_dvz, pip_dbeta, pip_dcfid;
    int pim_qc, pim_dvz, pim_dbeta, pim_dcfid;
    
    prot_qc = 0; prot_dvz = 0; prot_dbeta = 0; prot_dcfid = 0;
    pip_qc = 0; pip_dvz = 0; pip_dbeta = 0; pip_dcfid = 0;
    pim_qc = 0; pim_dvz = 0; pim_dbeta = 0; pim_dcfid = 0;

    
    // Sample event loop.
    for (int iev=0; iev<nev; iev++)
    {
        fReader->GetEntry(iev);
        h22Event event = fReader->GetEvent();
     
        // keep runno up to date
        if (runno != fReader->GetRunNumber() ) { runno = fReader->GetRunNumber(); filter.set_info(runno,opts.args["MC"].arg); std::cout << runno << std::endl;}
        

        if (filter.has_electron(event))
        {
            pass++;
            int pip_index  = filter.getByPID(event,211);
            int pim_index  = filter.getByPID(event,-211);
            int prot_index = filter.getByPID(event,2212);
            
            if (pip_index > -123) pip++;
            if (pim_index > -123) pim++;
            if (prot_index > -123) prot++;
            
            for (int ipart=1; ipart<event.gpart; ipart++)
            {
                std::map<std::string, bool> hid_map = filter.hid_map(event,ipart);

                if ( !hid_map["PROT_QC"] )    prot_qc++;
                if ( !hid_map["PROT_DVZ"] )   prot_dvz++;
                if ( !hid_map["PROT_DBETA"] ) prot_dbeta++;
                if ( !hid_map["PROT_DCFID"] ) prot_dcfid++;
                
                if ( !hid_map["PIP_QC"] )    pip_qc++;
                if ( !hid_map["PIP_DVZ"] )   pip_dvz++;
                if ( !hid_map["PIP_DBETA"] ) pip_dbeta++;
                if ( !hid_map["PIP_DCFID"] ) pip_dcfid++;

                if ( !hid_map["PIM_QC"] )    pim_qc++;
                if ( !hid_map["PIM_DVZ"] )   pim_dvz++;
                if ( !hid_map["PIM_DBETA"] ) pim_dbeta++;
                if ( !hid_map["PIM_DCFID"] ) pim_dcfid++;
            }
        }
        
        //! No Electron
        else { fail++; }
        
        std::map<std::string,bool> eid_map = filter.eid_map(event);

        if ( !eid_map["EC_GEO"] )     n_ec_geo_fail++;
        if ( !eid_map["EC_SAMPLING"]) n_ec_samp_fail++;
        if ( !eid_map["EC_EDEP"] )    n_ec_edep_fail++;
        if ( !eid_map["DCR1_FID"] )   n_dc_r1_fail++;
        if ( !eid_map["DCR3_FID"] )   n_dc_r3_fail++;
        if ( !eid_map["VZ"] )         n_vz_fail++;
        if ( !eid_map["CC_NPHE"] )     n_nphe_fail++;
        if ( !eid_map["CC_FID"] )     n_cc_fid_fail++;
    }
    
    std::cout << "Pass: " << pass << " Fail: " << fail << std::endl;
    std::cout << "ec geoemtry: " << (double) n_ec_geo_fail/fail << std::endl;
    std::cout << "ec sampling: " << (double) n_ec_samp_fail/fail << std::endl;
    std::cout << "ec en. dep : " << (double) n_ec_edep_fail/fail << std::endl;
    std::cout << "dc r1 fid  : " << (double) n_dc_r1_fail/fail << std::endl;
    std::cout << "dc r3 fid  : " << (double) n_dc_r3_fail/fail << std::endl;
    std::cout << "vz         : " << (double) n_vz_fail/fail << std::endl;
    std::cout << "cc nphe    : " << (double )n_nphe_fail/fail << std::endl;
    std::cout << "cc fid     : " << (double )n_cc_fid_fail/fail << std::endl;
    std::cout << " Done! " << std::endl;
    
    std::cout << " Quality Control Cuts: " << std::endl;
    std::cout << "    P: " << prot_qc << std::endl;
    std::cout << "  Pi+: " << pip_qc << std::endl;
    std::cout << "  Pi-: " << pim_qc << std::endl;
    
    std::cout << " Delta Z-Vertex Cuts: " << std::endl;
    std::cout << "    P: " << prot_dvz << std::endl;
    std::cout << "  Pi+: " << pip_dvz << std::endl;
    std::cout << "  Pi-: " << pim_dvz << std::endl;
    
    std::cout << " DC Fiducial Cuts: " << std::endl;
    std::cout << "    P: " << prot_dcfid << std::endl;
    std::cout << "  Pi+: " << pip_dcfid << std::endl;
    std::cout << "  Pi-: " << pim_dcfid << std::endl;
    
    std::cout << " Delta Beta Cuts: " << std::endl;
    std::cout << "    P: " << prot_dbeta << std::endl;
    std::cout << "  Pi+: " << pip_dbeta << std::endl;
    std::cout << "  Pi-: " << pim_dbeta << std::endl;
    
    std::cout << "Pi+: " << pip << "Pi-: " << pim << "Prot: " << prot << std::endl;
    
    return 0;
}
