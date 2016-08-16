///////////////////////////////////////////
/*
 David Riser, University of Connecticut
 
 ParticleFilter.h -> Class def for particle
 filtering class to apply cuts from params.
 
 July 10, 2016
 
 */
///////////////////////////////////////////

#ifndef PARTICLE_FILTER_H
#define PARTICLE_FILTER_H

// c++ includes
#include <iostream>
#include <map>

// my includes
#include "Corrections.h"
#include "h22Event.h"
#include "Pars.h"

// root includes

class ParticleFilter
{
public:
    ParticleFilter();                          //! No parameter file passed
    ParticleFilter(std::string);               //! Passing EID Parameter filename
    ParticleFilter(std::string, std::string);  //! Passing both parfile names
    ~ParticleFilter();
    
    // Data Members
    epars eid;
    hpars hid;
    Corrections corr;
    bool MC; int runno;
    
    std::string eparfile;
    std::string hparfile;
    
    // Member Functions
    bool has_electron(h22Event);       //! yes or no, always particle 0
    int getByPID(h22Event, int);//! get by PID
    void set_info(bool, int);
    std::map<std::string,bool> eid_map(h22Event);
    std::map<std::string,bool> hid_map(h22Event, int);

};

#endif
