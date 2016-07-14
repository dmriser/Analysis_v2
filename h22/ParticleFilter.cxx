///////////////////////////////////////////
/*
 David Riser, University of Connecticut
 
 ParticleFilter.h -> Class def for particle
 filtering class to apply cuts from params.
 
 July 10, 2016
 
 */
///////////////////////////////////////////

#ifndef PARTICLE_FILTER_CXX
#define PARTICLE_FILTER_CXX

// c++ includes
#include <iostream>
#include <map>

// my includes
#include "Corrections.h"
#include "h22Event.h"
#include "pars.h"
#include "ParticleFilter.h"

// root includes
#include "TVector3.h"

ParticleFilter::ParticleFilter()
{
    eparfile = "eid_default_pars.dat";
    hparfile = "hid_default_pars.dat";
    eid.load(eparfile);
    
    MC = false; runno = 0;
}

ParticleFilter::ParticleFilter(std::string efile)
{
    eparfile = efile;
    eid.load(eparfile);
    
    MC = false; runno = 0;
}

ParticleFilter::ParticleFilter(std::string efile, std::string hfile)
{
    eparfile = efile; hparfile = hfile;
    eid.load(eparfile);
    
    MC = false; runno = 0;
}


ParticleFilter::~ParticleFilter()
{
    // Don't need to save EID pars because it shouldnt be changing them.
}

// setting mc and run number status internally, mostly used for interfacing with corrections class
void ParticleFilter::set_info(bool GSIM, int run)
{
    runno = run; MC = GSIM;
}

int ParticleFilter::getByPID(h22Event event, int pid)
{
    int index = -123;
    
    if (pid == 11)
    {
        /* 
            Doing electron ID here based on parameters loaded from file. Trying to do cheap and easy cuts first which avoid doing the 
            expensive ones.  Only checking 0th particle because it's almost always where reconstruction puts the electron.  One can also 
            loop on ipart<gpart and then choose the fastest electron candidate.
         
         */
        
        if (event.q[0] < 0)                                  //! Particle is Neg.
            if (event.nphe[0] > eid.CCNPHE)                     //! Number of Photoelectrons
                if (event.ec_ei[0] > eid.ECEDEPMIN)
                    if (corr.vz(event,0,runno,MC) > eid.VZMIN && corr.vz(event,0,runno,MC) < eid.VZMAX)
                        if (event.theta_cc(0) > eid.cc_min(event.rphi(0)))
                            if (event.ec_sect[0] > 0 && event.dc_sect[0] > 0 && event.cc_sect[0] > 0 && corr.good_sc_paddle(event,0)) //! Particle hit all major detector systems
                                if (event.rot_dc1x(0) > eid.dc_left(event.rot_dc1y(0),1) && event.rot_dc1x(0) > eid.dc_right(event.rot_dc1y(0),1)) //! DCR1 Fid
                                    if (event.tl3_x[0] > eid.dc_left(event.tl3_y[0],3) && event.tl3_x[0] > eid.dc_right(event.tl3_y[0],3))//! DCR3 Fid
                                        if (event.etot[0]/event.p[0] > eid.ec_samp_min(event.p[0],event.ec_sect[0]) && event.etot[0]/event.p[0] < eid.ec_samp_max(event.p[0],event.ec_sect[0]))//! Ec Sampling
                                        {
                                            TVector3 uvw = event.uvw(0); //! Getting U, V, W Coords. for Particle 0
                                            int s=event.ec_sect[0];
                                            if (uvw.X() > eid.ECUMIN[s] && uvw.X() < eid.ECUMAX[s])         //! EC U, V, W Cuts
                                                if (uvw.Y() > eid.ECVMIN[s] && uvw.Y() < eid.ECVMAX[s])
                                                    if (uvw.Z() > eid.ECWMIN[s] && uvw.Z() < eid.ECWMAX[s])
                                                        index = 0;
                                        }
    }
    
    if (pid == 211)
    { index = -123; }
    
    if (pid == -211)
    { index = -123; }
    
    if (pid == 2212)
    { index = -123; }
    
    return index;   //! Default Case & Nathan Harrison Convention (for nothing found)
}

std::map<std::string, bool> ParticleFilter::eid_map(h22Event event)
{
    bool ec_geo_pass, ec_edep_pass, ec_sampling_pass, vz_pass, dcr1_fid_pass, dcr3_fid_pass, cc_nphe_pass, cc_fid_pass;
    
    ec_geo_pass      = false;
    ec_edep_pass     = false;
    ec_sampling_pass = false;
    vz_pass          = false;
    dcr1_fid_pass    = false;
    dcr3_fid_pass    = false;
    cc_nphe_pass     = false;
    cc_fid_pass      = false;
    
    std::map<std::string, bool> eid_results;
    
    // Start doing Electron Identification Routines
    
    TVector3 uvw = event.uvw(0);
    int s=event.ec_sect[0];
    if (uvw.X() > eid.ECUMIN[s] && uvw.X() < eid.ECUMAX[s])         //! EC U, V, W Cuts
        if (uvw.Y() > eid.ECVMIN[s] && uvw.Y() < eid.ECVMAX[s])
            if (uvw.Z() > eid.ECWMIN[s] && uvw.Z() < eid.ECWMAX[s])
                ec_geo_pass = true;

    if (event.ec_ei[0] > eid.ECEDEPMIN) ec_edep_pass = true;
    if (event.etot[0]/event.p[0] > eid.ec_samp_min(event.p[0],event.ec_sect[0]) && event.etot[0]/event.p[0] < eid.ec_samp_max(event.p[0],event.ec_sect[0])) ec_sampling_pass = true;
    if (corr.vz(event,0,runno,MC) > eid.VZMIN && corr.vz(event,0,runno,MC) < eid.VZMAX) vz_pass = true;
    if (event.rot_dc1x(0) > eid.dc_left(event.rot_dc1y(0),1) && event.rot_dc1x(0) > eid.dc_right(event.rot_dc1y(0),1)) dcr1_fid_pass = true;
    if (event.tl3_x[0] > eid.dc_left(event.tl3_y[0],3) && event.tl3_x[0] > eid.dc_right(event.tl3_y[0],3)) dcr3_fid_pass = true;
    if (event.nphe[0] > eid.CCNPHE) cc_nphe_pass = true;
    if (event.theta_cc(0) > eid.cc_min(0)) cc_fid_pass = true;

    eid_results["EC_GEO"]      = ec_geo_pass;
    eid_results["EC_EDEP"]     = ec_edep_pass;
    eid_results["EC_SAMPLING"] = ec_sampling_pass;
    eid_results["VZ"]          = vz_pass;
    eid_results["DCR1_FID"]    = dcr1_fid_pass;
    eid_results["DCR3_FID"]    = dcr3_fid_pass;
    eid_results["CC_NPHE"]     = cc_nphe_pass;
    eid_results["CC_FID"]      = cc_fid_pass;
    
    return eid_results;
    
}

#endif