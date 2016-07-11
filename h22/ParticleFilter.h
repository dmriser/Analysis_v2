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
#include "pars.h"

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
    Corrections corr;
    bool MC; int runno;
    
    std::string eparfile;
    std::string hparfile;
    
    // Member Functions
    int getByPID(h22Event, int);//! get by PID
    void set_info(bool, int);
    std::map<std::string,bool> eid_map(h22Event);
};

#endif