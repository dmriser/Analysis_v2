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
#include <vector>

// my includes
#include "Corrections.h"
#include "h22Event.h"
#include "Parameters.h"
#include "DataEventSelector.h"

// root includes
#include "TLorentzVector.h"

class ParticleFilter{
public:
    ParticleFilter(Parameters *params);              
    ~ParticleFilter();
    
    // Data Members
    Parameters *pars;
    Corrections corr;
    bool MC; int runno;
    
    // Electron Selector can be accesed 
    // from outside the class to disable
    // cuts or whatever.
    DataEventSelector *electronSelector;
    DataEventSelector *positivePionSelector;
    DataEventSelector *negativePionSelector;

    // List of the cuts used for electrons.
    ChargeCut          *negativity_cut;
    CCFiducialCut      *cc_fid_cut;
    CCPhiMatchingCut   *cc_phi_match_cut;
    CCThetaMatchingCut *cc_theta_s1_cut;
    CCThetaMatchingCut *cc_theta_s2_cut;
    CCThetaMatchingCut *cc_theta_s3_cut;
    CCThetaMatchingCut *cc_theta_s4_cut;
    CCThetaMatchingCut *cc_theta_s5_cut;
    CCThetaMatchingCut *cc_theta_s6_cut;
    DCR1FiducialCut    *dcr1_fid_cut;
    DCR3FiducialCut    *dcr3_fid_cut;
    ECEdepInnerCut     *edep_cut;
    ECUCut             *ecu_cut;
    ECVCut             *ecv_cut;
    ECWCut             *ecw_cut;
    NPheCut            *nphe_cut; 
    MomentumCut        *p_cut; 
    SampFracCut        *sf_s1_cut;
    SampFracCut        *sf_s2_cut;
    SampFracCut        *sf_s3_cut;
    SampFracCut        *sf_s4_cut;
    SampFracCut        *sf_s5_cut;
    SampFracCut        *sf_s6_cut;
    TrackQualityCut    *qc_cut;
    ZVertexCut         *vz_cut;

    // List of cuts used for pions
    DataEventCut_TOFMassCut      *pip_tofmass_cut_s1;
    DataEventCut_TOFMassCut      *pip_tofmass_cut_s2;
    DataEventCut_TOFMassCut      *pip_tofmass_cut_s3;
    DataEventCut_TOFMassCut      *pip_tofmass_cut_s4;
    DataEventCut_TOFMassCut      *pip_tofmass_cut_s5;
    DataEventCut_TOFMassCut      *pip_tofmass_cut_s6;
    DataEventCut_TOFMassCut      *pim_tofmass_cut_s1;
    DataEventCut_TOFMassCut      *pim_tofmass_cut_s2;
    DataEventCut_TOFMassCut      *pim_tofmass_cut_s3;
    DataEventCut_TOFMassCut      *pim_tofmass_cut_s4;
    DataEventCut_TOFMassCut      *pim_tofmass_cut_s5;
    DataEventCut_TOFMassCut      *pim_tofmass_cut_s6;
    DataEventCut_DeltaZVertexCut *dvz_cut;

    // Parameters for Pi-0 Invariant Mass
    // There should be a better way to do it. 
    double pi0_invmass_mu[6], pi0_invmass_sigma[6], pi0_invmass_nsigma;
    
    // Member Functions
    bool has_electron(h22Event &event);
    bool isNotAnElectronCandidate(std::vector<int> electrons, int index);
    int getByPID(h22Event, int);
    void set_info(bool, int);
    
    DataEventSelector *GetSelector(int pid);
    std::vector<int> getVectorOfParticleIndices(h22Event &event, int pid);
    std::map<std::string,bool> eid_map(h22Event, int);
    std::map<std::string,bool> hid_map(h22Event, int);
    std::vector<TLorentzVector> getVectorOfTLorentzVectors(h22Event &event, int pid);
    std::map<int, TLorentzVector> getMapOfIndexAndTLorentzVector(h22Event, int pid); 

};
#endif
