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

// my includes
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
    
    MC = false;
}

ParticleFilter::ParticleFilter(std::string efile)
{
    eparfile = efile;
    eid.load(eparfile);
    
    MC = false;
}

ParticleFilter::ParticleFilter(std::string efile, std::string hfile)
{
    eparfile = efile; hparfile = hfile;
    eid.load(eparfile);
    
    MC = false;
}


ParticleFilter::~ParticleFilter()
{
    // Don't need to save EID pars because it shouldnt be changing them.
}

void ParticleFilter::set_mc_by_runno(int runno)
{
    if (runno > 37657 && runno < 38752) MC = false;
    else MC = true;
}

int ParticleFilter::getByPID(h22Event event, int pid)
{
    if (pid == 11)
    {
        int e_index = -123;

        /* 
            Doing electron ID here based on parameters loaded from file. Trying to do cheap and easy cuts first which avoid doing the 
            expensive ones.  Only checking 0th particle because it's almost always where reconstruction puts the electron.  One can also 
            loop on ipart<gpart and then choose the fastest electron candidate.
         
         */
        
        if (event.q[0] < 0)                                  //! Particle is Neg.
            if (event.nphe[0] > eid.CCNPHE)                     //! Number of Photoelectrons
                if (event.ec_ei[0] > eid.ECEDEPMIN)
                    if (event.vz[0] > eid.VZMIN && event.vz[0] < eid.VZMAX)
                        if (event.ec_sect[0] > 0 && event.dc_sect[0] > 0 && event.cc_sect[0] > 0) //! Particle hit all major detector systems
                            if (event.rot_dc1x(0) > eid.dc_left(event.rot_dc1y(0),1) && event.rot_dc1x(0) > eid.dc_right(event.rot_dc1y(0),1)) //! DCR1 Fid
                                if (event.tl3_x[0] > eid.dc_left(event.tl3_y[0],3) && event.tl3_x[0] > eid.dc_right(event.tl3_y[0],3))//! DCR3 Fid
                                    if (event.etot[0]/event.p[0] > eid.ec_samp_min(event.p[0],event.ec_sect[0]) && event.etot[0]/event.p[0] < eid.ec_samp_max(event.p[0],event.ec_sect[0]))//! Ec Sampling
                                    {
                                        TVector3 uvw = event.uvw(0); //! Getting U, V, W Coords. for Particle 0
                                        int s=event.ec_sect[0];
                                        if (uvw.X() > eid.ECUMIN[s] && uvw.X() < eid.ECUMAX[s])         //! EC U, V, W Cuts
                                            if (uvw.Y() > eid.ECVMIN[s] && uvw.Y() < eid.ECVMAX[s])
                                                if (uvw.Z() > eid.ECWMIN[s] && uvw.Z() < eid.ECWMAX[s])
                                                    e_index = 0;
                            }
        
            return e_index;
    }
    
    return -123;   //! Default Case & Nathan Harrison Convention (for nothing found)
}

#endif