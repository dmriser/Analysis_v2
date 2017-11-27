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
    DataEventSelector *positiveKaonSelector;
    DataEventSelector *negativeKaonSelector;
    DataEventSelector *positiveMesonCandidateSelector; 
    DataEventSelector *protonSelector;

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

    // Testing 
    TestCut *testCut; 

    // List of cuts used for pions
    DataEventCut_TOFMassCut      *pip_tofmass_cut, *pim_tofmass_cut, *kp_tofmass_cut, *km_tofmass_cut;

    DataEventCut_DeltaBetaCut    *pp_dbeta_cut; 
    DataEventCut_DeltaBetaCut    *kp_dbeta_cut; 
    DataEventCut_DeltaBetaCut    *proton_dbeta_cut; 

    // new cut from dan carman 
    DataEventCut_DBetaMinimizerCut *dbeta_minimizer_211; 
    DataEventCut_DBetaMinimizerCut *dbeta_minimizer_321; 
    DataEventCut_DBetaMinimizerCut *dbeta_minimizer_2212; 
    DataEventCut_DBetaMinimizerCut *dbeta_minimizer_neg_211; 
    DataEventCut_DBetaMinimizerCut *dbeta_minimizer_neg_321; 

    DataEventCut_BetaPLikelihood *betap_likelihood_211; 
    DataEventCut_BetaPLikelihood *betap_likelihood_321; 
    DataEventCut_BetaPLikelihood *betap_likelihood_2212; 

    DataEventCut_DeltaZVertexCut *dvz_cut;
    DCR1FiducialCut              *dcr1_meson_fid_cut;

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
    std::map<std::string,bool> eid_map(h22Event&, int);
    std::map<std::string,bool> hid_map(h22Event, int);
    std::map<std::string,float> eid_distance_map(h22Event &event, int index); 
    std::vector<TLorentzVector> getVectorOfTLorentzVectors(h22Event &event, int pid);
    std::map<int, TLorentzVector> getMapOfIndexAndTLorentzVector(h22Event, int pid); 

};
#endif
