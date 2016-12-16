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
#include <vector>

// my includes
#include "CommonTools.h"
#include "Corrections.h"
#include "h22Event.h"
#include "Parameters.h"
#include "ParticleFilter.h"
#include "DataEventSelector.h"
#include "DataEventCut.h"

// root includes
#include "TVector3.h"

ParticleFilter::ParticleFilter(Parameters *params) : pars(params){
    MC=false; runno = 0;

    // Setup the selector 
    electronSelector = new DataEventSelector();

    // Call cut constructors 
    // These could be split into 2 selectors, 
    // one for fid cuts that might be used on 
    // other particles and one for just elec
    // specific cuts.
    negativity_cut = new ChargeCut();
    cc_fid_cut     = new CCFiducialCut();
    dcr1_fid_cut   = new DCR1FiducialCut();
    dcr3_fid_cut   = new DCR3FiducialCut();
    edep_cut       = new ECEdepInnerCut();
    ecu_cut        = new ECUCut();
    ecv_cut        = new ECVCut();
    ecw_cut        = new ECWCut();
    sf_s1_cut      = new SampFracCut(1);
    sf_s2_cut      = new SampFracCut(2);
    sf_s3_cut      = new SampFracCut(3);
    sf_s4_cut      = new SampFracCut(4);
    sf_s5_cut      = new SampFracCut(5);
    sf_s6_cut      = new SampFracCut(6);
    qc_cut         = new TrackQualityCut();
    vz_cut         = new ZVertexCut();

    // Set limits on cuts from parameters 
    negativity_cut->set_min(-1.1);
    negativity_cut->set_max(-0.9);

    cc_fid_cut->a = params->getParameter("EL_CC_FIDA").getValue(0);
    cc_fid_cut->b = params->getParameter("EL_CC_FIDB").getValue(0);

    dcr1_fid_cut->height = params->getParameter("EL_DCR1_FIDH").getValue(0);
    dcr1_fid_cut->angle  = params->getParameter("EL_DCR1_FIDA").getValue(0);
    dcr3_fid_cut->height = params->getParameter("EL_DCR3_FIDH").getValue(0);
    dcr3_fid_cut->angle  = params->getParameter("EL_DCR3_FIDA").getValue(0);

    edep_cut->set_min( params->getParameter("EL_EC_EDEP_MIN").getValue(0) );
    ecu_cut->set_min( params->getParameter("EL_ECU_MIN").getValue(0) );
    ecu_cut->set_max( params->getParameter("EL_ECU_MAX").getValue(0) ); 
    ecv_cut->set_min( params->getParameter("EL_ECV_MIN").getValue(0) );
    ecv_cut->set_max( params->getParameter("EL_ECV_MAX").getValue(0) ); 
    ecw_cut->set_min( params->getParameter("EL_ECW_MIN").getValue(0) );
    ecw_cut->set_max( params->getParameter("EL_ECW_MAX").getValue(0) ); 

    sf_s1_cut->am     = params->getParameter("EL_SF_MU_A").getValue(0);
    sf_s1_cut->bm     = params->getParameter("EL_SF_MU_B").getValue(0);
    sf_s1_cut->cm     = params->getParameter("EL_SF_MU_C").getValue(0);
    sf_s1_cut->dm     = params->getParameter("EL_SF_MU_D").getValue(0);
    sf_s1_cut->as     = params->getParameter("EL_SF_SIGMA_A").getValue(0);
    sf_s1_cut->bs     = params->getParameter("EL_SF_SIGMA_B").getValue(0);
    sf_s1_cut->cs     = params->getParameter("EL_SF_SIGMA_C").getValue(0);
    sf_s1_cut->ds     = params->getParameter("EL_SF_SIGMA_D").getValue(0);
    sf_s1_cut->nsigma = params->getParameter("EL_EC_NSIGMA").getValue(0);

    sf_s2_cut->am     = params->getParameter("EL_SF_MU_A").getValue(1);
    sf_s2_cut->bm     = params->getParameter("EL_SF_MU_B").getValue(1);
    sf_s2_cut->cm     = params->getParameter("EL_SF_MU_C").getValue(1);
    sf_s2_cut->dm     = params->getParameter("EL_SF_MU_D").getValue(1);
    sf_s2_cut->as     = params->getParameter("EL_SF_SIGMA_A").getValue(1);
    sf_s2_cut->bs     = params->getParameter("EL_SF_SIGMA_B").getValue(1);
    sf_s2_cut->cs     = params->getParameter("EL_SF_SIGMA_C").getValue(1);
    sf_s2_cut->ds     = params->getParameter("EL_SF_SIGMA_D").getValue(1);
    sf_s2_cut->nsigma = params->getParameter("EL_EC_NSIGMA").getValue(0);

    sf_s3_cut->am     = params->getParameter("EL_SF_MU_A").getValue(2);
    sf_s3_cut->bm     = params->getParameter("EL_SF_MU_B").getValue(2);
    sf_s3_cut->cm     = params->getParameter("EL_SF_MU_C").getValue(2);
    sf_s3_cut->dm     = params->getParameter("EL_SF_MU_D").getValue(2);
    sf_s3_cut->as     = params->getParameter("EL_SF_SIGMA_A").getValue(2);
    sf_s3_cut->bs     = params->getParameter("EL_SF_SIGMA_B").getValue(2);
    sf_s3_cut->cs     = params->getParameter("EL_SF_SIGMA_C").getValue(2);
    sf_s3_cut->ds     = params->getParameter("EL_SF_SIGMA_D").getValue(2);
    sf_s3_cut->nsigma = params->getParameter("EL_EC_NSIGMA").getValue(0);

    sf_s4_cut->am     = params->getParameter("EL_SF_MU_A").getValue(3);
    sf_s4_cut->bm     = params->getParameter("EL_SF_MU_B").getValue(3);
    sf_s4_cut->cm     = params->getParameter("EL_SF_MU_C").getValue(3);
    sf_s4_cut->dm     = params->getParameter("EL_SF_MU_D").getValue(3);
    sf_s4_cut->as     = params->getParameter("EL_SF_SIGMA_A").getValue(3);
    sf_s4_cut->bs     = params->getParameter("EL_SF_SIGMA_B").getValue(3);
    sf_s4_cut->cs     = params->getParameter("EL_SF_SIGMA_C").getValue(3);
    sf_s4_cut->ds     = params->getParameter("EL_SF_SIGMA_D").getValue(3);
    sf_s4_cut->nsigma = params->getParameter("EL_EC_NSIGMA").getValue(0);

    sf_s5_cut->am     = params->getParameter("EL_SF_MU_A").getValue(4);
    sf_s5_cut->bm     = params->getParameter("EL_SF_MU_B").getValue(4);
    sf_s5_cut->cm     = params->getParameter("EL_SF_MU_C").getValue(4);
    sf_s5_cut->dm     = params->getParameter("EL_SF_MU_D").getValue(4);
    sf_s5_cut->as     = params->getParameter("EL_SF_SIGMA_A").getValue(4);
    sf_s5_cut->bs     = params->getParameter("EL_SF_SIGMA_B").getValue(4);
    sf_s5_cut->cs     = params->getParameter("EL_SF_SIGMA_C").getValue(4);
    sf_s5_cut->ds     = params->getParameter("EL_SF_SIGMA_D").getValue(4);
    sf_s5_cut->nsigma = params->getParameter("EL_EC_NSIGMA").getValue(0);

    sf_s6_cut->am     = params->getParameter("EL_SF_MU_A").getValue(5);
    sf_s6_cut->bm     = params->getParameter("EL_SF_MU_B").getValue(5);
    sf_s6_cut->cm     = params->getParameter("EL_SF_MU_C").getValue(5);
    sf_s6_cut->dm     = params->getParameter("EL_SF_MU_D").getValue(5);
    sf_s6_cut->as     = params->getParameter("EL_SF_SIGMA_A").getValue(5);
    sf_s6_cut->bs     = params->getParameter("EL_SF_SIGMA_B").getValue(5);
    sf_s6_cut->cs     = params->getParameter("EL_SF_SIGMA_C").getValue(5);
    sf_s6_cut->ds     = params->getParameter("EL_SF_SIGMA_D").getValue(5);
    sf_s6_cut->nsigma = params->getParameter("EL_EC_NSIGMA").getValue(0);

    vz_cut->set_min( params->getParameter("EL_VZ_MIN").getValue(0) );
    vz_cut->set_max( params->getParameter("EL_VZ_MAX").getValue(0) );

    // Push back the cuts 
    electronSelector->add_cut( negativity_cut );
    electronSelector->add_cut( cc_fid_cut );
    electronSelector->add_cut( dcr1_fid_cut );
    electronSelector->add_cut( dcr3_fid_cut );
    electronSelector->add_cut( edep_cut );
    electronSelector->add_cut( ecu_cut );
    electronSelector->add_cut( ecv_cut );
    electronSelector->add_cut( ecw_cut );
    electronSelector->add_cut( sf_s1_cut );
    electronSelector->add_cut( sf_s2_cut );
    electronSelector->add_cut( sf_s3_cut );
    electronSelector->add_cut( sf_s4_cut );
    electronSelector->add_cut( sf_s5_cut );
    electronSelector->add_cut( sf_s6_cut );
    electronSelector->add_cut( qc_cut );
    electronSelector->add_cut( vz_cut );
}

ParticleFilter::~ParticleFilter(){
  electronSelector->summarize();
}

// setting mc and run number status internally, mostly used for interfacing with corrections class
void ParticleFilter::set_info(bool GSIM, int run){
    runno = run; MC = GSIM;
}

bool ParticleFilter::has_electron(h22Event event){
    
  // Simply check 0 for now, later add support for 
  // loop over gpart.
  if (electronSelector->passesFast(event, 0)) 
    return true;
  
  return false;
}

int ParticleFilter::getByPID(h22Event event, int pid){
    int index = -123;
    return index;   //! Default Case & Nathan Harrison Convention (-123 -> for nothing found)
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

    return eid_results;    
}

#endif
