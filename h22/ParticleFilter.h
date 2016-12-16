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
#include "Parameters.h"
#include "DataEventSelector.h"

// root includes

class ParticleFilter{
public:
    ParticleFilter(Parameters *params);               //! Passing EID Parameter filename
    ~ParticleFilter();
    
    // Data Members
    Parameters *pars;
    Corrections corr;
    bool MC; int runno;
    
    // Electron Selector can be accesed 
    // from outside the class to disable
    // cuts or whatever.
    DataEventSelector *electronSelector;

    // List of the cuts used for electrons.
    ChargeCut       *negativity_cut;
    CCFiducialCut   *cc_fid_cut;
    DCR1FiducialCut *dcr1_fid_cut;
    DCR3FiducialCut *dcr3_fid_cut;
    ECEdepInnerCut  *edep_cut;
    ECUCut          *ecu_cut;
    ECVCut          *ecv_cut;
    ECWCut          *ecw_cut;
    SampFracCut     *sf_s1_cut;
    SampFracCut     *sf_s2_cut;
    SampFracCut     *sf_s3_cut;
    SampFracCut     *sf_s4_cut;
    SampFracCut     *sf_s5_cut;
    SampFracCut     *sf_s6_cut;
    TrackQualityCut *qc_cut;
    ZVertexCut      *vz_cut;

    // Member Functions
    bool has_electron(h22Event);       //! yes or no, always particle 0
    int getByPID(h22Event, int);//! get by PID
    void set_info(bool, int);
    std::map<std::string,bool> eid_map(h22Event);
    std::map<std::string,bool> hid_map(h22Event, int);

};
#endif
