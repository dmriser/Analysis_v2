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
using std::string; 
using std::vector; 

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
#include "TLorentzVector.h"

ParticleFilter::ParticleFilter(Parameters *params) : pars(params){
    MC=false; runno = 0;

    // Setup the selector
    electronSelector     = new DataEventSelector();
    positivePionSelector = new DataEventSelector();
    negativePionSelector = new DataEventSelector();
    positiveKaonSelector = new DataEventSelector();
    negativeKaonSelector = new DataEventSelector();

    // Call cut constructors
    // These could be split into 2 selectors,
    // one for fid cuts that might be used on
    // other particles and one for just elec
    // specific cuts.
    negativity_cut     = new ChargeCut();
    cc_fid_cut         = new CCFiducialCut();
    cc_phi_match_cut   = new CCPhiMatchingCut();
    cc_theta_s1_cut    = new CCThetaMatchingCut(1);
    cc_theta_s2_cut    = new CCThetaMatchingCut(2);
    cc_theta_s3_cut    = new CCThetaMatchingCut(3);
    cc_theta_s4_cut    = new CCThetaMatchingCut(4);
    cc_theta_s5_cut    = new CCThetaMatchingCut(5);
    cc_theta_s6_cut    = new CCThetaMatchingCut(6);
    dcr1_fid_cut       = new DCR1FiducialCut();
    dcr3_fid_cut       = new DCR3FiducialCut();
    edep_cut           = new ECEdepInnerCut();
    ecu_cut            = new ECUCut();
    ecv_cut            = new ECVCut();
    ecw_cut            = new ECWCut();
    nphe_cut           = new NPheCut();
    p_cut              = new MomentumCut();
    sf_s1_cut          = new SampFracCut(1);
    sf_s2_cut          = new SampFracCut(2);
    sf_s3_cut          = new SampFracCut(3);
    sf_s4_cut          = new SampFracCut(4);
    sf_s5_cut          = new SampFracCut(5);
    sf_s6_cut          = new SampFracCut(6);
    qc_cut             = new TrackQualityCut();
    vz_cut             = new ZVertexCut();

    // pion cuts
    dvz_cut            = new DataEventCut_DeltaZVertexCut();
    pim_tofmass_cut_s1 = new DataEventCut_TOFMassCut(1);
    pim_tofmass_cut_s2 = new DataEventCut_TOFMassCut(2);
    pim_tofmass_cut_s3 = new DataEventCut_TOFMassCut(3);
    pim_tofmass_cut_s4 = new DataEventCut_TOFMassCut(4);
    pim_tofmass_cut_s5 = new DataEventCut_TOFMassCut(5);
    pim_tofmass_cut_s6 = new DataEventCut_TOFMassCut(6);
    pip_tofmass_cut_s1 = new DataEventCut_TOFMassCut(1);
    pip_tofmass_cut_s2 = new DataEventCut_TOFMassCut(2);
    pip_tofmass_cut_s3 = new DataEventCut_TOFMassCut(3);
    pip_tofmass_cut_s4 = new DataEventCut_TOFMassCut(4);
    pip_tofmass_cut_s5 = new DataEventCut_TOFMassCut(5);
    pip_tofmass_cut_s6 = new DataEventCut_TOFMassCut(6);
 
    km_tofmass_cut_s1 = new DataEventCut_TOFMassCut(1);
    km_tofmass_cut_s2 = new DataEventCut_TOFMassCut(2);
    km_tofmass_cut_s3 = new DataEventCut_TOFMassCut(3);
    km_tofmass_cut_s4 = new DataEventCut_TOFMassCut(4);
    km_tofmass_cut_s5 = new DataEventCut_TOFMassCut(5);
    km_tofmass_cut_s6 = new DataEventCut_TOFMassCut(6);
    kp_tofmass_cut_s1 = new DataEventCut_TOFMassCut(1);
    kp_tofmass_cut_s2 = new DataEventCut_TOFMassCut(2);
    kp_tofmass_cut_s3 = new DataEventCut_TOFMassCut(3);
    kp_tofmass_cut_s4 = new DataEventCut_TOFMassCut(4);
    kp_tofmass_cut_s5 = new DataEventCut_TOFMassCut(5);
    kp_tofmass_cut_s6 = new DataEventCut_TOFMassCut(6);

    kp_betap_cut_s1 = new DataEventCut_BetaPCut(1); 
    kp_betap_cut_s2 = new DataEventCut_BetaPCut(2); 
    kp_betap_cut_s3 = new DataEventCut_BetaPCut(3); 
    kp_betap_cut_s4 = new DataEventCut_BetaPCut(4); 
    kp_betap_cut_s5 = new DataEventCut_BetaPCut(5); 
    kp_betap_cut_s6 = new DataEventCut_BetaPCut(6); 
    km_betap_cut_s1 = new DataEventCut_BetaPCut(1); 
    km_betap_cut_s2 = new DataEventCut_BetaPCut(2); 
    km_betap_cut_s3 = new DataEventCut_BetaPCut(3); 
    km_betap_cut_s4 = new DataEventCut_BetaPCut(4); 
    km_betap_cut_s5 = new DataEventCut_BetaPCut(5); 
    km_betap_cut_s6 = new DataEventCut_BetaPCut(6); 
    pp_betap_cut_s1 = new DataEventCut_BetaPCut(1); 
    pp_betap_cut_s2 = new DataEventCut_BetaPCut(2); 
    pp_betap_cut_s3 = new DataEventCut_BetaPCut(3); 
    pp_betap_cut_s4 = new DataEventCut_BetaPCut(4); 
    pp_betap_cut_s5 = new DataEventCut_BetaPCut(5); 
    pp_betap_cut_s6 = new DataEventCut_BetaPCut(6); 
    pm_betap_cut_s1 = new DataEventCut_BetaPCut(1); 
    pm_betap_cut_s2 = new DataEventCut_BetaPCut(2); 
    pm_betap_cut_s3 = new DataEventCut_BetaPCut(3); 
    pm_betap_cut_s4 = new DataEventCut_BetaPCut(4); 
    pm_betap_cut_s5 = new DataEventCut_BetaPCut(5); 
    pm_betap_cut_s6 = new DataEventCut_BetaPCut(6); 

    dcr1_meson_fid_cut = new DCR1FiducialCut();

    // Set limits on cuts from parameters
    negativity_cut->SetMin(-1.1);
    negativity_cut->SetMax(-0.9);

    cc_fid_cut->a = params->getParameter("EL_CC_FIDA").getValue(0);
    cc_fid_cut->b = params->getParameter("EL_CC_FIDB").getValue(0);
    cc_fid_cut->c = params->getParameter("EL_CC_FIDC").getValue(0);

    dcr1_fid_cut->height = params->getParameter("EL_DCR1_FIDH").getValue(0);
    dcr1_fid_cut->angle  = params->getParameter("EL_DCR1_FIDA").getValue(0);
    dcr3_fid_cut->height = params->getParameter("EL_DCR3_FIDH").getValue(0);
    dcr3_fid_cut->angle  = params->getParameter("EL_DCR3_FIDA").getValue(0);

    edep_cut->SetMin( params->getParameter("EL_EC_EDEP_MIN").getValue(0) );
    ecu_cut->SetMin( params->getParameter("EL_ECU_MIN").getValue(0) );
    ecu_cut->SetMax( params->getParameter("EL_ECU_MAX").getValue(0) );
    ecv_cut->SetMin( params->getParameter("EL_ECV_MIN").getValue(0) );
    ecv_cut->SetMax( params->getParameter("EL_ECV_MAX").getValue(0) );
    ecw_cut->SetMin( params->getParameter("EL_ECW_MIN").getValue(0) );
    ecw_cut->SetMax( params->getParameter("EL_ECW_MAX").getValue(0) );

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

    cc_theta_s1_cut->am     = params->getParameter("EL_CCT_MU_A").getValue(0);
    cc_theta_s1_cut->bm     = params->getParameter("EL_CCT_MU_B").getValue(0);
    cc_theta_s1_cut->cm     = params->getParameter("EL_CCT_MU_C").getValue(0);
    cc_theta_s1_cut->dm     = params->getParameter("EL_CCT_MU_D").getValue(0);
    cc_theta_s1_cut->as     = params->getParameter("EL_CCT_SIGMA_A").getValue(0);
    cc_theta_s1_cut->bs     = params->getParameter("EL_CCT_SIGMA_B").getValue(0);
    cc_theta_s1_cut->cs     = params->getParameter("EL_CCT_SIGMA_C").getValue(0);
    cc_theta_s1_cut->ds     = params->getParameter("EL_CCT_SIGMA_D").getValue(0);
    cc_theta_s1_cut->nsigma = params->getParameter("EL_CCT_NSIGMA").getValue(0);

    cc_theta_s2_cut->am     = params->getParameter("EL_CCT_MU_A").getValue(1);
    cc_theta_s2_cut->bm     = params->getParameter("EL_CCT_MU_B").getValue(1);
    cc_theta_s2_cut->cm     = params->getParameter("EL_CCT_MU_C").getValue(1);
    cc_theta_s2_cut->dm     = params->getParameter("EL_CCT_MU_D").getValue(1);
    cc_theta_s2_cut->as     = params->getParameter("EL_CCT_SIGMA_A").getValue(1);
    cc_theta_s2_cut->bs     = params->getParameter("EL_CCT_SIGMA_B").getValue(1);
    cc_theta_s2_cut->cs     = params->getParameter("EL_CCT_SIGMA_C").getValue(1);
    cc_theta_s2_cut->ds     = params->getParameter("EL_CCT_SIGMA_D").getValue(1);
    cc_theta_s2_cut->nsigma = params->getParameter("EL_CCT_NSIGMA").getValue(0);

    cc_theta_s3_cut->am     = params->getParameter("EL_CCT_MU_A").getValue(2);
    cc_theta_s3_cut->bm     = params->getParameter("EL_CCT_MU_B").getValue(2);
    cc_theta_s3_cut->cm     = params->getParameter("EL_CCT_MU_C").getValue(2);
    cc_theta_s3_cut->dm     = params->getParameter("EL_CCT_MU_D").getValue(2);
    cc_theta_s3_cut->as     = params->getParameter("EL_CCT_SIGMA_A").getValue(2);
    cc_theta_s3_cut->bs     = params->getParameter("EL_CCT_SIGMA_B").getValue(2);
    cc_theta_s3_cut->cs     = params->getParameter("EL_CCT_SIGMA_C").getValue(2);
    cc_theta_s3_cut->ds     = params->getParameter("EL_CCT_SIGMA_D").getValue(2);
    cc_theta_s3_cut->nsigma = params->getParameter("EL_CCT_NSIGMA").getValue(0);

    cc_theta_s4_cut->am     = params->getParameter("EL_CCT_MU_A").getValue(3);
    cc_theta_s4_cut->bm     = params->getParameter("EL_CCT_MU_B").getValue(3);
    cc_theta_s4_cut->cm     = params->getParameter("EL_CCT_MU_C").getValue(3);
    cc_theta_s4_cut->dm     = params->getParameter("EL_CCT_MU_D").getValue(3);
    cc_theta_s4_cut->as     = params->getParameter("EL_CCT_SIGMA_A").getValue(3);
    cc_theta_s4_cut->bs     = params->getParameter("EL_CCT_SIGMA_B").getValue(3);
    cc_theta_s4_cut->cs     = params->getParameter("EL_CCT_SIGMA_C").getValue(3);
    cc_theta_s4_cut->ds     = params->getParameter("EL_CCT_SIGMA_D").getValue(3);
    cc_theta_s4_cut->nsigma = params->getParameter("EL_CCT_NSIGMA").getValue(0);

    cc_theta_s5_cut->am     = params->getParameter("EL_CCT_MU_A").getValue(4);
    cc_theta_s5_cut->bm     = params->getParameter("EL_CCT_MU_B").getValue(4);
    cc_theta_s5_cut->cm     = params->getParameter("EL_CCT_MU_C").getValue(4);
    cc_theta_s5_cut->dm     = params->getParameter("EL_CCT_MU_D").getValue(4);
    cc_theta_s5_cut->as     = params->getParameter("EL_CCT_SIGMA_A").getValue(4);
    cc_theta_s5_cut->bs     = params->getParameter("EL_CCT_SIGMA_B").getValue(4);
    cc_theta_s5_cut->cs     = params->getParameter("EL_CCT_SIGMA_C").getValue(4);
    cc_theta_s5_cut->ds     = params->getParameter("EL_CCT_SIGMA_D").getValue(4);
    cc_theta_s5_cut->nsigma = params->getParameter("EL_CCT_NSIGMA").getValue(0);

    cc_theta_s6_cut->am     = params->getParameter("EL_CCT_MU_A").getValue(5);
    cc_theta_s6_cut->bm     = params->getParameter("EL_CCT_MU_B").getValue(5);
    cc_theta_s6_cut->cm     = params->getParameter("EL_CCT_MU_C").getValue(5);
    cc_theta_s6_cut->dm     = params->getParameter("EL_CCT_MU_D").getValue(5);
    cc_theta_s6_cut->as     = params->getParameter("EL_CCT_SIGMA_A").getValue(5);
    cc_theta_s6_cut->bs     = params->getParameter("EL_CCT_SIGMA_B").getValue(5);
    cc_theta_s6_cut->cs     = params->getParameter("EL_CCT_SIGMA_C").getValue(5);
    cc_theta_s6_cut->ds     = params->getParameter("EL_CCT_SIGMA_D").getValue(5);
    cc_theta_s6_cut->nsigma = params->getParameter("EL_CCT_NSIGMA").getValue(0);

    vz_cut->SetMin( params->getParameter("EL_VZ_MIN").getValue(0) );
    vz_cut->SetMax( params->getParameter("EL_VZ_MAX").getValue(0) );

    nphe_cut->SetMin(20);
    nphe_cut->SetMax(1000);

    p_cut->SetMin(0.5);
    p_cut->SetMax(100.0);

    // Push back the cuts
    electronSelector->AddCut( negativity_cut );
    electronSelector->AddCut( nphe_cut );
    electronSelector->AddCut( p_cut );
    electronSelector->AddCut( cc_fid_cut );
    electronSelector->AddCut( cc_phi_match_cut );
    electronSelector->AddCut( cc_theta_s1_cut );
    electronSelector->AddCut( cc_theta_s2_cut );
    electronSelector->AddCut( cc_theta_s3_cut );
    electronSelector->AddCut( cc_theta_s4_cut );
    electronSelector->AddCut( cc_theta_s5_cut );
    electronSelector->AddCut( cc_theta_s6_cut );
    electronSelector->AddCut( dcr1_fid_cut );
    electronSelector->AddCut( dcr3_fid_cut );
    electronSelector->AddCut( edep_cut );
    electronSelector->AddCut( ecu_cut );
    electronSelector->AddCut( ecv_cut );
    electronSelector->AddCut( ecw_cut );
    electronSelector->AddCut( sf_s1_cut );
    electronSelector->AddCut( sf_s2_cut );
    electronSelector->AddCut( sf_s3_cut );
    electronSelector->AddCut( sf_s4_cut );
    electronSelector->AddCut( sf_s5_cut );
    electronSelector->AddCut( sf_s6_cut );
    //    electronSelector->add_cut( qc_cut );
    electronSelector->AddCut( vz_cut );

    // This can be overridden by the user by just
    // turning off the ones you don't want. This
    // happens at the time of construction so your
    // call will be after.
    electronSelector->EnableAll();
    electronSelector->DisableByName("NPhe Cut");

    /*
    // Neutral Pion
    for(int s=0; s<6; s++){
        pi0_invmass_mu[s]    = params->getParameter("PI0_INVMASS_MU")   .getValue(s);
        pi0_invmass_sigma[s] = params->getParameter("PI0_INVMASS_SIGMA").getValue(s);
    }
    pi0_invmass_nsigma   = params->getParameter("PI0_INVMASS_NSIGMA").getValue(0);

    // Setting cut values for positive pions from file
    pip_tofmass_cut_s1->SetMin(params->getParameter("PIP_TOFMASS_MU").getValue(0) -
                                params->getParameter("PIP_TOFMASS_NSIGMA").getValue(0)*
                                params->getParameter("PIP_TOFMASS_SIGMA").getValue(0));
    pip_tofmass_cut_s1->SetMax(params->getParameter("PIP_TOFMASS_MU").getValue(0) +
                                params->getParameter("PIP_TOFMASS_NSIGMA").getValue(0)*
                                params->getParameter("PIP_TOFMASS_SIGMA").getValue(0));
 
    pip_tofmass_cut_s2->SetMin(params->getParameter("PIP_TOFMASS_MU").getValue(1) -
                                params->getParameter("PIP_TOFMASS_NSIGMA").getValue(0)*
                                params->getParameter("PIP_TOFMASS_SIGMA").getValue(1));
    pip_tofmass_cut_s2->SetMax(params->getParameter("PIP_TOFMASS_MU").getValue(1) +
                                params->getParameter("PIP_TOFMASS_NSIGMA").getValue(0)*
                                params->getParameter("PIP_TOFMASS_SIGMA").getValue(1));

    pip_tofmass_cut_s3->SetMin(params->getParameter("PIP_TOFMASS_MU").getValue(2) -
                                params->getParameter("PIP_TOFMASS_NSIGMA").getValue(0)*
                                params->getParameter("PIP_TOFMASS_SIGMA").getValue(2));
    pip_tofmass_cut_s3->SetMax(params->getParameter("PIP_TOFMASS_MU").getValue(2) +
                                params->getParameter("PIP_TOFMASS_NSIGMA").getValue(0)*
                                params->getParameter("PIP_TOFMASS_SIGMA").getValue(2));

    pip_tofmass_cut_s4->SetMin(params->getParameter("PIP_TOFMASS_MU").getValue(3) -
                                params->getParameter("PIP_TOFMASS_NSIGMA").getValue(0)*
                                params->getParameter("PIP_TOFMASS_SIGMA").getValue(3));
    pip_tofmass_cut_s4->SetMax(params->getParameter("PIP_TOFMASS_MU").getValue(3) +
                                params->getParameter("PIP_TOFMASS_NSIGMA").getValue(0)*
                                params->getParameter("PIP_TOFMASS_SIGMA").getValue(3));

    pip_tofmass_cut_s5->SetMin(params->getParameter("PIP_TOFMASS_MU").getValue(4) -
                                params->getParameter("PIP_TOFMASS_NSIGMA").getValue(0)*
                                params->getParameter("PIP_TOFMASS_SIGMA").getValue(4));
    pip_tofmass_cut_s5->SetMax(params->getParameter("PIP_TOFMASS_MU").getValue(4) +
                                params->getParameter("PIP_TOFMASS_NSIGMA").getValue(0)*
                                params->getParameter("PIP_TOFMASS_SIGMA").getValue(4));

    pip_tofmass_cut_s6->SetMin(params->getParameter("PIP_TOFMASS_MU").getValue(5) -
                                params->getParameter("PIP_TOFMASS_NSIGMA").getValue(0)*
                                params->getParameter("PIP_TOFMASS_SIGMA").getValue(5));
    pip_tofmass_cut_s6->SetMax(params->getParameter("PIP_TOFMASS_MU").getValue(5) +
                                params->getParameter("PIP_TOFMASS_NSIGMA").getValue(0)*
                                params->getParameter("PIP_TOFMASS_SIGMA").getValue(5));

    // Now for negative pions, this is so ugly.
    pim_tofmass_cut_s1->SetMin(params->getParameter("PIM_TOFMASS_MU").getValue(0) -
                                params->getParameter("PIM_TOFMASS_NSIGMA").getValue(0)*
                                params->getParameter("PIM_TOFMASS_SIGMA").getValue(0));
    pim_tofmass_cut_s1->SetMax(params->getParameter("PIM_TOFMASS_MU").getValue(0) +
                                params->getParameter("PIM_TOFMASS_NSIGMA").getValue(0)*
                                params->getParameter("PIM_TOFMASS_SIGMA").getValue(0));

    pim_tofmass_cut_s2->SetMin(params->getParameter("PIM_TOFMASS_MU").getValue(1) -
                                params->getParameter("PIM_TOFMASS_NSIGMA").getValue(0)*
                                params->getParameter("PIM_TOFMASS_SIGMA").getValue(1));
    pim_tofmass_cut_s2->SetMax(params->getParameter("PIM_TOFMASS_MU").getValue(1) +
                                params->getParameter("PIM_TOFMASS_NSIGMA").getValue(0)*
                                params->getParameter("PIM_TOFMASS_SIGMA").getValue(1));

    pim_tofmass_cut_s3->SetMin(params->getParameter("PIM_TOFMASS_MU").getValue(2) -
                                params->getParameter("PIM_TOFMASS_NSIGMA").getValue(0)*
                                params->getParameter("PIM_TOFMASS_SIGMA").getValue(2));
    pim_tofmass_cut_s3->SetMax(params->getParameter("PIM_TOFMASS_MU").getValue(2) +
                                params->getParameter("PIM_TOFMASS_NSIGMA").getValue(0)*
                                params->getParameter("PIM_TOFMASS_SIGMA").getValue(2));

    pim_tofmass_cut_s4->SetMin(params->getParameter("PIM_TOFMASS_MU").getValue(3) -
                                params->getParameter("PIM_TOFMASS_NSIGMA").getValue(0)*
                                params->getParameter("PIM_TOFMASS_SIGMA").getValue(3));
    pim_tofmass_cut_s4->SetMax(params->getParameter("PIM_TOFMASS_MU").getValue(3) +
                                params->getParameter("PIM_TOFMASS_NSIGMA").getValue(0)*
                                params->getParameter("PIM_TOFMASS_SIGMA").getValue(3));

    pim_tofmass_cut_s5->SetMin(params->getParameter("PIM_TOFMASS_MU").getValue(4) -
                                params->getParameter("PIM_TOFMASS_NSIGMA").getValue(0)*
                                params->getParameter("PIM_TOFMASS_SIGMA").getValue(4));
    pim_tofmass_cut_s5->SetMax(params->getParameter("PIM_TOFMASS_MU").getValue(4) +
                                params->getParameter("PIM_TOFMASS_NSIGMA").getValue(0)*
                                params->getParameter("PIM_TOFMASS_SIGMA").getValue(4));

    pim_tofmass_cut_s6->SetMin(params->getParameter("PIM_TOFMASS_MU").getValue(5) -
                                params->getParameter("PIM_TOFMASS_NSIGMA").getValue(0)*
                                params->getParameter("PIM_TOFMASS_SIGMA").getValue(5));
    pim_tofmass_cut_s6->SetMax(params->getParameter("PIM_TOFMASS_MU").getValue(5) +
                                params->getParameter("PIM_TOFMASS_NSIGMA").getValue(0)*
                                params->getParameter("PIM_TOFMASS_SIGMA").getValue(5));

    */

    // kaon min and max 
    kp_tofmass_cut_s1->SetMin(params->getParameter("KP_TOFMASS_MIN").getValue(0));
    kp_tofmass_cut_s2->SetMin(params->getParameter("KP_TOFMASS_MIN").getValue(1));
    kp_tofmass_cut_s3->SetMin(params->getParameter("KP_TOFMASS_MIN").getValue(2));
    kp_tofmass_cut_s4->SetMin(params->getParameter("KP_TOFMASS_MIN").getValue(3));
    kp_tofmass_cut_s5->SetMin(params->getParameter("KP_TOFMASS_MIN").getValue(4));
    kp_tofmass_cut_s6->SetMin(params->getParameter("KP_TOFMASS_MIN").getValue(5));
    kp_tofmass_cut_s1->SetMax(params->getParameter("KP_TOFMASS_MAX").getValue(0));
    kp_tofmass_cut_s2->SetMax(params->getParameter("KP_TOFMASS_MAX").getValue(1));
    kp_tofmass_cut_s3->SetMax(params->getParameter("KP_TOFMASS_MAX").getValue(2));
    kp_tofmass_cut_s4->SetMax(params->getParameter("KP_TOFMASS_MAX").getValue(3));
    kp_tofmass_cut_s5->SetMax(params->getParameter("KP_TOFMASS_MAX").getValue(4));
    kp_tofmass_cut_s6->SetMax(params->getParameter("KP_TOFMASS_MAX").getValue(5));

    km_tofmass_cut_s1->SetMin(params->getParameter("KM_TOFMASS_MIN").getValue(0));
    km_tofmass_cut_s2->SetMin(params->getParameter("KM_TOFMASS_MIN").getValue(1));
    km_tofmass_cut_s3->SetMin(params->getParameter("KM_TOFMASS_MIN").getValue(2));
    km_tofmass_cut_s4->SetMin(params->getParameter("KM_TOFMASS_MIN").getValue(3));
    km_tofmass_cut_s5->SetMin(params->getParameter("KM_TOFMASS_MIN").getValue(4));
    km_tofmass_cut_s6->SetMin(params->getParameter("KM_TOFMASS_MIN").getValue(5));
    km_tofmass_cut_s1->SetMax(params->getParameter("KM_TOFMASS_MAX").getValue(0));
    km_tofmass_cut_s2->SetMax(params->getParameter("KM_TOFMASS_MAX").getValue(1));
    km_tofmass_cut_s3->SetMax(params->getParameter("KM_TOFMASS_MAX").getValue(2));
    km_tofmass_cut_s4->SetMax(params->getParameter("KM_TOFMASS_MAX").getValue(3));
    km_tofmass_cut_s5->SetMax(params->getParameter("KM_TOFMASS_MAX").getValue(4));
    km_tofmass_cut_s6->SetMax(params->getParameter("KM_TOFMASS_MAX").getValue(5));

    // kaon beta vs. p values 
    kp_betap_cut_s1->am = params->getParameter("KP_BVP_MU_A").getValue(0);
    kp_betap_cut_s1->bm = params->getParameter("KP_BVP_MU_B").getValue(0);
    kp_betap_cut_s1->cm = params->getParameter("KP_BVP_MU_C").getValue(0);
    kp_betap_cut_s1->dm = params->getParameter("KP_BVP_MU_D").getValue(0);
    kp_betap_cut_s1->as = params->getParameter("KP_BVP_SIGMA_A").getValue(0);
    kp_betap_cut_s1->bs = params->getParameter("KP_BVP_SIGMA_B").getValue(0);
    kp_betap_cut_s1->cs = params->getParameter("KP_BVP_SIGMA_C").getValue(0);
    kp_betap_cut_s1->ds = params->getParameter("KP_BVP_SIGMA_D").getValue(0);
    kp_betap_cut_s1->nsigma = params->getParameter("KP_BVP_NSIGMA").getValue(0);

    kp_betap_cut_s2->am = params->getParameter("KP_BVP_MU_A").getValue(1);
    kp_betap_cut_s2->bm = params->getParameter("KP_BVP_MU_B").getValue(1);
    kp_betap_cut_s2->cm = params->getParameter("KP_BVP_MU_C").getValue(1);
    kp_betap_cut_s2->dm = params->getParameter("KP_BVP_MU_D").getValue(1);
    kp_betap_cut_s2->as = params->getParameter("KP_BVP_SIGMA_A").getValue(1);
    kp_betap_cut_s2->bs = params->getParameter("KP_BVP_SIGMA_B").getValue(1);
    kp_betap_cut_s2->cs = params->getParameter("KP_BVP_SIGMA_C").getValue(1);
    kp_betap_cut_s2->ds = params->getParameter("KP_BVP_SIGMA_D").getValue(1);
    kp_betap_cut_s2->nsigma = params->getParameter("KP_BVP_NSIGMA").getValue(0);

    kp_betap_cut_s3->am = params->getParameter("KP_BVP_MU_A").getValue(2);
    kp_betap_cut_s3->bm = params->getParameter("KP_BVP_MU_B").getValue(2);
    kp_betap_cut_s3->cm = params->getParameter("KP_BVP_MU_C").getValue(2);
    kp_betap_cut_s3->dm = params->getParameter("KP_BVP_MU_D").getValue(2);
    kp_betap_cut_s3->as = params->getParameter("KP_BVP_SIGMA_A").getValue(2);
    kp_betap_cut_s3->bs = params->getParameter("KP_BVP_SIGMA_B").getValue(2);
    kp_betap_cut_s3->cs = params->getParameter("KP_BVP_SIGMA_C").getValue(2);
    kp_betap_cut_s3->ds = params->getParameter("KP_BVP_SIGMA_D").getValue(2);
    kp_betap_cut_s3->nsigma = params->getParameter("KP_BVP_NSIGMA").getValue(0);

    kp_betap_cut_s4->am = params->getParameter("KP_BVP_MU_A").getValue(3);
    kp_betap_cut_s4->bm = params->getParameter("KP_BVP_MU_B").getValue(3);
    kp_betap_cut_s4->cm = params->getParameter("KP_BVP_MU_C").getValue(3);
    kp_betap_cut_s4->dm = params->getParameter("KP_BVP_MU_D").getValue(3);
    kp_betap_cut_s4->as = params->getParameter("KP_BVP_SIGMA_A").getValue(3);
    kp_betap_cut_s4->bs = params->getParameter("KP_BVP_SIGMA_B").getValue(3);
    kp_betap_cut_s4->cs = params->getParameter("KP_BVP_SIGMA_C").getValue(3);
    kp_betap_cut_s4->ds = params->getParameter("KP_BVP_SIGMA_D").getValue(3);
    kp_betap_cut_s4->nsigma = params->getParameter("KP_BVP_NSIGMA").getValue(0);

    kp_betap_cut_s5->am = params->getParameter("KP_BVP_MU_A").getValue(4);
    kp_betap_cut_s5->bm = params->getParameter("KP_BVP_MU_B").getValue(4);
    kp_betap_cut_s5->cm = params->getParameter("KP_BVP_MU_C").getValue(4);
    kp_betap_cut_s5->dm = params->getParameter("KP_BVP_MU_D").getValue(4);
    kp_betap_cut_s5->as = params->getParameter("KP_BVP_SIGMA_A").getValue(4);
    kp_betap_cut_s5->bs = params->getParameter("KP_BVP_SIGMA_B").getValue(4);
    kp_betap_cut_s5->cs = params->getParameter("KP_BVP_SIGMA_C").getValue(4);
    kp_betap_cut_s5->ds = params->getParameter("KP_BVP_SIGMA_D").getValue(4);
    kp_betap_cut_s5->nsigma = params->getParameter("KP_BVP_NSIGMA").getValue(0);

    kp_betap_cut_s6->am = params->getParameter("KP_BVP_MU_A").getValue(5);
    kp_betap_cut_s6->bm = params->getParameter("KP_BVP_MU_B").getValue(5);
    kp_betap_cut_s6->cm = params->getParameter("KP_BVP_MU_C").getValue(5);
    kp_betap_cut_s6->dm = params->getParameter("KP_BVP_MU_D").getValue(5);
    kp_betap_cut_s6->as = params->getParameter("KP_BVP_SIGMA_A").getValue(5);
    kp_betap_cut_s6->bs = params->getParameter("KP_BVP_SIGMA_B").getValue(5);
    kp_betap_cut_s6->cs = params->getParameter("KP_BVP_SIGMA_C").getValue(5);
    kp_betap_cut_s6->ds = params->getParameter("KP_BVP_SIGMA_D").getValue(5);
    kp_betap_cut_s6->nsigma = params->getParameter("KP_BVP_NSIGMA").getValue(0);

    // negative kaon
    km_betap_cut_s1->am = params->getParameter("KM_BVP_MU_A").getValue(0);
    km_betap_cut_s1->bm = params->getParameter("KM_BVP_MU_B").getValue(0);
    km_betap_cut_s1->cm = params->getParameter("KM_BVP_MU_C").getValue(0);
    km_betap_cut_s1->dm = params->getParameter("KM_BVP_MU_D").getValue(0);
    km_betap_cut_s1->as = params->getParameter("KM_BVP_SIGMA_A").getValue(0);
    km_betap_cut_s1->bs = params->getParameter("KM_BVP_SIGMA_B").getValue(0);
    km_betap_cut_s1->cs = params->getParameter("KM_BVP_SIGMA_C").getValue(0);
    km_betap_cut_s1->ds = params->getParameter("KM_BVP_SIGMA_D").getValue(0);
    km_betap_cut_s1->nsigma = params->getParameter("KM_BVP_NSIGMA").getValue(0);

    km_betap_cut_s2->am = params->getParameter("KM_BVP_MU_A").getValue(1);
    km_betap_cut_s2->bm = params->getParameter("KM_BVP_MU_B").getValue(1);
    km_betap_cut_s2->cm = params->getParameter("KM_BVP_MU_C").getValue(1);
    km_betap_cut_s2->dm = params->getParameter("KM_BVP_MU_D").getValue(1);
    km_betap_cut_s2->as = params->getParameter("KM_BVP_SIGMA_A").getValue(1);
    km_betap_cut_s2->bs = params->getParameter("KM_BVP_SIGMA_B").getValue(1);
    km_betap_cut_s2->cs = params->getParameter("KM_BVP_SIGMA_C").getValue(1);
    km_betap_cut_s2->ds = params->getParameter("KM_BVP_SIGMA_D").getValue(1);
    km_betap_cut_s2->nsigma = params->getParameter("KM_BVP_NSIGMA").getValue(0);

    km_betap_cut_s3->am = params->getParameter("KM_BVP_MU_A").getValue(2);
    km_betap_cut_s3->bm = params->getParameter("KM_BVP_MU_B").getValue(2);
    km_betap_cut_s3->cm = params->getParameter("KM_BVP_MU_C").getValue(2);
    km_betap_cut_s3->dm = params->getParameter("KM_BVP_MU_D").getValue(2);
    km_betap_cut_s3->as = params->getParameter("KM_BVP_SIGMA_A").getValue(2);
    km_betap_cut_s3->bs = params->getParameter("KM_BVP_SIGMA_B").getValue(2);
    km_betap_cut_s3->cs = params->getParameter("KM_BVP_SIGMA_C").getValue(2);
    km_betap_cut_s3->ds = params->getParameter("KM_BVP_SIGMA_D").getValue(2);
    km_betap_cut_s3->nsigma = params->getParameter("KM_BVP_NSIGMA").getValue(0);

    km_betap_cut_s4->am = params->getParameter("KM_BVP_MU_A").getValue(3);
    km_betap_cut_s4->bm = params->getParameter("KM_BVP_MU_B").getValue(3);
    km_betap_cut_s4->cm = params->getParameter("KM_BVP_MU_C").getValue(3);
    km_betap_cut_s4->dm = params->getParameter("KM_BVP_MU_D").getValue(3);
    km_betap_cut_s4->as = params->getParameter("KM_BVP_SIGMA_A").getValue(3);
    km_betap_cut_s4->bs = params->getParameter("KM_BVP_SIGMA_B").getValue(3);
    km_betap_cut_s4->cs = params->getParameter("KM_BVP_SIGMA_C").getValue(3);
    km_betap_cut_s4->ds = params->getParameter("KM_BVP_SIGMA_D").getValue(3);
    km_betap_cut_s4->nsigma = params->getParameter("KM_BVP_NSIGMA").getValue(0);

    km_betap_cut_s5->am = params->getParameter("KM_BVP_MU_A").getValue(4);
    km_betap_cut_s5->bm = params->getParameter("KM_BVP_MU_B").getValue(4);
    km_betap_cut_s5->cm = params->getParameter("KM_BVP_MU_C").getValue(4);
    km_betap_cut_s5->dm = params->getParameter("KM_BVP_MU_D").getValue(4);
    km_betap_cut_s5->as = params->getParameter("KM_BVP_SIGMA_A").getValue(4);
    km_betap_cut_s5->bs = params->getParameter("KM_BVP_SIGMA_B").getValue(4);
    km_betap_cut_s5->cs = params->getParameter("KM_BVP_SIGMA_C").getValue(4);
    km_betap_cut_s5->ds = params->getParameter("KM_BVP_SIGMA_D").getValue(4);
    km_betap_cut_s5->nsigma = params->getParameter("KM_BVP_NSIGMA").getValue(0);

    km_betap_cut_s6->am = params->getParameter("KM_BVP_MU_A").getValue(5);
    km_betap_cut_s6->bm = params->getParameter("KM_BVP_MU_B").getValue(5);
    km_betap_cut_s6->cm = params->getParameter("KM_BVP_MU_C").getValue(5);
    km_betap_cut_s6->dm = params->getParameter("KM_BVP_MU_D").getValue(5);
    km_betap_cut_s6->as = params->getParameter("KM_BVP_SIGMA_A").getValue(5);
    km_betap_cut_s6->bs = params->getParameter("KM_BVP_SIGMA_B").getValue(5);
    km_betap_cut_s6->cs = params->getParameter("KM_BVP_SIGMA_C").getValue(5);
    km_betap_cut_s6->ds = params->getParameter("KM_BVP_SIGMA_D").getValue(5);
    km_betap_cut_s6->nsigma = params->getParameter("KM_BVP_NSIGMA").getValue(0);
   
    // pion positive 
    pp_betap_cut_s1->am = params->getParameter("PIP_BVP_MU_A").getValue(0);
    pp_betap_cut_s1->bm = params->getParameter("PIP_BVP_MU_B").getValue(0);
    pp_betap_cut_s1->cm = params->getParameter("PIP_BVP_MU_C").getValue(0);
    pp_betap_cut_s1->dm = params->getParameter("PIP_BVP_MU_D").getValue(0);
    pp_betap_cut_s1->as = params->getParameter("PIP_BVP_SIGMA_A").getValue(0);
    pp_betap_cut_s1->bs = params->getParameter("PIP_BVP_SIGMA_B").getValue(0);
    pp_betap_cut_s1->cs = params->getParameter("PIP_BVP_SIGMA_C").getValue(0);
    pp_betap_cut_s1->ds = params->getParameter("PIP_BVP_SIGMA_D").getValue(0);
    pp_betap_cut_s1->nsigma = params->getParameter("PIP_BVP_NSIGMA").getValue(0);

    pp_betap_cut_s2->am = params->getParameter("PIP_BVP_MU_A").getValue(1);
    pp_betap_cut_s2->bm = params->getParameter("PIP_BVP_MU_B").getValue(1);
    pp_betap_cut_s2->cm = params->getParameter("PIP_BVP_MU_C").getValue(1);
    pp_betap_cut_s2->dm = params->getParameter("PIP_BVP_MU_D").getValue(1);
    pp_betap_cut_s2->as = params->getParameter("PIP_BVP_SIGMA_A").getValue(1);
    pp_betap_cut_s2->bs = params->getParameter("PIP_BVP_SIGMA_B").getValue(1);
    pp_betap_cut_s2->cs = params->getParameter("PIP_BVP_SIGMA_C").getValue(1);
    pp_betap_cut_s2->ds = params->getParameter("PIP_BVP_SIGMA_D").getValue(1);
    pp_betap_cut_s2->nsigma = params->getParameter("PIP_BVP_NSIGMA").getValue(0);

    pp_betap_cut_s3->am = params->getParameter("PIP_BVP_MU_A").getValue(2);
    pp_betap_cut_s3->bm = params->getParameter("PIP_BVP_MU_B").getValue(2);
    pp_betap_cut_s3->cm = params->getParameter("PIP_BVP_MU_C").getValue(2);
    pp_betap_cut_s3->dm = params->getParameter("PIP_BVP_MU_D").getValue(2);
    pp_betap_cut_s3->as = params->getParameter("PIP_BVP_SIGMA_A").getValue(2);
    pp_betap_cut_s3->bs = params->getParameter("PIP_BVP_SIGMA_B").getValue(2);
    pp_betap_cut_s3->cs = params->getParameter("PIP_BVP_SIGMA_C").getValue(2);
    pp_betap_cut_s3->ds = params->getParameter("PIP_BVP_SIGMA_D").getValue(2);
    pp_betap_cut_s3->nsigma = params->getParameter("PIP_BVP_NSIGMA").getValue(0);

    pp_betap_cut_s4->am = params->getParameter("PIP_BVP_MU_A").getValue(3);
    pp_betap_cut_s4->bm = params->getParameter("PIP_BVP_MU_B").getValue(3);
    pp_betap_cut_s4->cm = params->getParameter("PIP_BVP_MU_C").getValue(3);
    pp_betap_cut_s4->dm = params->getParameter("PIP_BVP_MU_D").getValue(3);
    pp_betap_cut_s4->as = params->getParameter("PIP_BVP_SIGMA_A").getValue(3);
    pp_betap_cut_s4->bs = params->getParameter("PIP_BVP_SIGMA_B").getValue(3);
    pp_betap_cut_s4->cs = params->getParameter("PIP_BVP_SIGMA_C").getValue(3);
    pp_betap_cut_s4->ds = params->getParameter("PIP_BVP_SIGMA_D").getValue(3);
    pp_betap_cut_s4->nsigma = params->getParameter("PIP_BVP_NSIGMA").getValue(0);

    pp_betap_cut_s5->am = params->getParameter("PIP_BVP_MU_A").getValue(4);
    pp_betap_cut_s5->bm = params->getParameter("PIP_BVP_MU_B").getValue(4);
    pp_betap_cut_s5->cm = params->getParameter("PIP_BVP_MU_C").getValue(4);
    pp_betap_cut_s5->dm = params->getParameter("PIP_BVP_MU_D").getValue(4);
    pp_betap_cut_s5->as = params->getParameter("PIP_BVP_SIGMA_A").getValue(4);
    pp_betap_cut_s5->bs = params->getParameter("PIP_BVP_SIGMA_B").getValue(4);
    pp_betap_cut_s5->cs = params->getParameter("PIP_BVP_SIGMA_C").getValue(4);
    pp_betap_cut_s5->ds = params->getParameter("PIP_BVP_SIGMA_D").getValue(4);
    pp_betap_cut_s5->nsigma = params->getParameter("PIP_BVP_NSIGMA").getValue(0);

    pp_betap_cut_s6->am = params->getParameter("PIP_BVP_MU_A").getValue(5);
    pp_betap_cut_s6->bm = params->getParameter("PIP_BVP_MU_B").getValue(5);
    pp_betap_cut_s6->cm = params->getParameter("PIP_BVP_MU_C").getValue(5);
    pp_betap_cut_s6->dm = params->getParameter("PIP_BVP_MU_D").getValue(5);
    pp_betap_cut_s6->as = params->getParameter("PIP_BVP_SIGMA_A").getValue(5);
    pp_betap_cut_s6->bs = params->getParameter("PIP_BVP_SIGMA_B").getValue(5);
    pp_betap_cut_s6->cs = params->getParameter("PIP_BVP_SIGMA_C").getValue(5);
    pp_betap_cut_s6->ds = params->getParameter("PIP_BVP_SIGMA_D").getValue(5);
    pp_betap_cut_s6->nsigma = params->getParameter("PIP_BVP_NSIGMA").getValue(0);

    // negative pion
    pm_betap_cut_s1->am = params->getParameter("PIM_BVP_MU_A").getValue(0);
    pm_betap_cut_s1->bm = params->getParameter("PIM_BVP_MU_B").getValue(0);
    pm_betap_cut_s1->cm = params->getParameter("PIM_BVP_MU_C").getValue(0);
    pm_betap_cut_s1->dm = params->getParameter("PIM_BVP_MU_D").getValue(0);
    pm_betap_cut_s1->as = params->getParameter("PIM_BVP_SIGMA_A").getValue(0);
    pm_betap_cut_s1->bs = params->getParameter("PIM_BVP_SIGMA_B").getValue(0);
    pm_betap_cut_s1->cs = params->getParameter("PIM_BVP_SIGMA_C").getValue(0);
    pm_betap_cut_s1->ds = params->getParameter("PIM_BVP_SIGMA_D").getValue(0);
    pm_betap_cut_s1->nsigma = params->getParameter("PIM_BVP_NSIGMA").getValue(0);

    pm_betap_cut_s2->am = params->getParameter("PIM_BVP_MU_A").getValue(1);
    pm_betap_cut_s2->bm = params->getParameter("PIM_BVP_MU_B").getValue(1);
    pm_betap_cut_s2->cm = params->getParameter("PIM_BVP_MU_C").getValue(1);
    pm_betap_cut_s2->dm = params->getParameter("PIM_BVP_MU_D").getValue(1);
    pm_betap_cut_s2->as = params->getParameter("PIM_BVP_SIGMA_A").getValue(1);
    pm_betap_cut_s2->bs = params->getParameter("PIM_BVP_SIGMA_B").getValue(1);
    pm_betap_cut_s2->cs = params->getParameter("PIM_BVP_SIGMA_C").getValue(1);
    pm_betap_cut_s2->ds = params->getParameter("PIM_BVP_SIGMA_D").getValue(1);
    pm_betap_cut_s2->nsigma = params->getParameter("PIM_BVP_NSIGMA").getValue(0);

    pm_betap_cut_s3->am = params->getParameter("PIM_BVP_MU_A").getValue(2);
    pm_betap_cut_s3->bm = params->getParameter("PIM_BVP_MU_B").getValue(2);
    pm_betap_cut_s3->cm = params->getParameter("PIM_BVP_MU_C").getValue(2);
    pm_betap_cut_s3->dm = params->getParameter("PIM_BVP_MU_D").getValue(2);
    pm_betap_cut_s3->as = params->getParameter("PIM_BVP_SIGMA_A").getValue(2);
    pm_betap_cut_s3->bs = params->getParameter("PIM_BVP_SIGMA_B").getValue(2);
    pm_betap_cut_s3->cs = params->getParameter("PIM_BVP_SIGMA_C").getValue(2);
    pm_betap_cut_s3->ds = params->getParameter("PIM_BVP_SIGMA_D").getValue(2);
    pm_betap_cut_s3->nsigma = params->getParameter("PIM_BVP_NSIGMA").getValue(0);

    pm_betap_cut_s4->am = params->getParameter("PIM_BVP_MU_A").getValue(3);
    pm_betap_cut_s4->bm = params->getParameter("PIM_BVP_MU_B").getValue(3);
    pm_betap_cut_s4->cm = params->getParameter("PIM_BVP_MU_C").getValue(3);
    pm_betap_cut_s4->dm = params->getParameter("PIM_BVP_MU_D").getValue(3);
    pm_betap_cut_s4->as = params->getParameter("PIM_BVP_SIGMA_A").getValue(3);
    pm_betap_cut_s4->bs = params->getParameter("PIM_BVP_SIGMA_B").getValue(3);
    pm_betap_cut_s4->cs = params->getParameter("PIM_BVP_SIGMA_C").getValue(3);
    pm_betap_cut_s4->ds = params->getParameter("PIM_BVP_SIGMA_D").getValue(3);
    pm_betap_cut_s4->nsigma = params->getParameter("PIM_BVP_NSIGMA").getValue(0);

    pm_betap_cut_s5->am = params->getParameter("PIM_BVP_MU_A").getValue(4);
    pm_betap_cut_s5->bm = params->getParameter("PIM_BVP_MU_B").getValue(4);
    pm_betap_cut_s5->cm = params->getParameter("PIM_BVP_MU_C").getValue(4);
    pm_betap_cut_s5->dm = params->getParameter("PIM_BVP_MU_D").getValue(4);
    pm_betap_cut_s5->as = params->getParameter("PIM_BVP_SIGMA_A").getValue(4);
    pm_betap_cut_s5->bs = params->getParameter("PIM_BVP_SIGMA_B").getValue(4);
    pm_betap_cut_s5->cs = params->getParameter("PIM_BVP_SIGMA_C").getValue(4);
    pm_betap_cut_s5->ds = params->getParameter("PIM_BVP_SIGMA_D").getValue(4);
    pm_betap_cut_s5->nsigma = params->getParameter("PIM_BVP_NSIGMA").getValue(0);

    pm_betap_cut_s6->am = params->getParameter("PIM_BVP_MU_A").getValue(5);
    pm_betap_cut_s6->bm = params->getParameter("PIM_BVP_MU_B").getValue(5);
    pm_betap_cut_s6->cm = params->getParameter("PIM_BVP_MU_C").getValue(5);
    pm_betap_cut_s6->dm = params->getParameter("PIM_BVP_MU_D").getValue(5);
    pm_betap_cut_s6->as = params->getParameter("PIM_BVP_SIGMA_A").getValue(5);
    pm_betap_cut_s6->bs = params->getParameter("PIM_BVP_SIGMA_B").getValue(5);
    pm_betap_cut_s6->cs = params->getParameter("PIM_BVP_SIGMA_C").getValue(5);
    pm_betap_cut_s6->ds = params->getParameter("PIM_BVP_SIGMA_D").getValue(5);
    pm_betap_cut_s6->nsigma = params->getParameter("PIM_BVP_NSIGMA").getValue(0);

    dcr1_meson_fid_cut->height = params->getParameter("KP_DCR1_HEIGHT").getValue(0);
    dcr1_meson_fid_cut->angle  = params->getParameter("KP_DCR1_ANGLE").getValue(0);

    dvz_cut->SetMin(-1 *params->getParameter("KP_DVZ").getValue(0) );
    dvz_cut->SetMax( params->getParameter("KP_DVZ").getValue(0) );

    // Setup pion selectors
    testCut = new TestCut(); 

    positivePionSelector->AddCut(dvz_cut);
    positivePionSelector->AddCut(pp_betap_cut_s1); 
    positivePionSelector->AddCut(pp_betap_cut_s2); 
    positivePionSelector->AddCut(pp_betap_cut_s3); 
    positivePionSelector->AddCut(pp_betap_cut_s4); 
    positivePionSelector->AddCut(pp_betap_cut_s5); 
    positivePionSelector->AddCut(pp_betap_cut_s6); 
    positivePionSelector->AddCut(dcr1_meson_fid_cut);
    positivePionSelector->EnableAll();

    negativePionSelector->AddCut(dvz_cut);
    negativePionSelector->AddCut(pm_betap_cut_s1); 
    negativePionSelector->AddCut(pm_betap_cut_s2); 
    negativePionSelector->AddCut(pm_betap_cut_s3); 
    negativePionSelector->AddCut(pm_betap_cut_s4); 
    negativePionSelector->AddCut(pm_betap_cut_s5); 
    negativePionSelector->AddCut(pm_betap_cut_s6); 
    negativePionSelector->EnableAll();

    positiveKaonSelector->AddCut(dvz_cut);
    positiveKaonSelector->AddCut(kp_tofmass_cut_s1);
    positiveKaonSelector->AddCut(kp_tofmass_cut_s2);
    positiveKaonSelector->AddCut(kp_tofmass_cut_s3);
    positiveKaonSelector->AddCut(kp_tofmass_cut_s4);
    positiveKaonSelector->AddCut(kp_tofmass_cut_s5);
    positiveKaonSelector->AddCut(kp_tofmass_cut_s6);
    /*
    positiveKaonSelector->AddCut(kp_betap_cut_s1);
    positiveKaonSelector->AddCut(kp_betap_cut_s2);
    positiveKaonSelector->AddCut(kp_betap_cut_s3);
    positiveKaonSelector->AddCut(kp_betap_cut_s4);
    positiveKaonSelector->AddCut(kp_betap_cut_s5);
    positiveKaonSelector->AddCut(kp_betap_cut_s6);
    */
    positiveKaonSelector->AddCut(dcr1_meson_fid_cut);
    positiveKaonSelector->EnableAll();
 
    negativeKaonSelector->AddCut(dvz_cut);
    negativeKaonSelector->AddCut(km_tofmass_cut_s1);
    negativeKaonSelector->AddCut(km_tofmass_cut_s2);
    negativeKaonSelector->AddCut(km_tofmass_cut_s3);
    negativeKaonSelector->AddCut(km_tofmass_cut_s4);
    negativeKaonSelector->AddCut(km_tofmass_cut_s5);
    negativeKaonSelector->AddCut(km_tofmass_cut_s6);
    /*
    negativeKaonSelector->AddCut(km_betap_cut_s1);
    negativeKaonSelector->AddCut(km_betap_cut_s2);
    negativeKaonSelector->AddCut(km_betap_cut_s3);
    negativeKaonSelector->AddCut(km_betap_cut_s4);
    negativeKaonSelector->AddCut(km_betap_cut_s5);
    negativeKaonSelector->AddCut(km_betap_cut_s6);
    */
    negativeKaonSelector->EnableAll();
 
}

ParticleFilter::~ParticleFilter(){
    electronSelector->PrintSummary();
}

// setting mc and run number status internally, mostly used for interfacing with corrections class
void ParticleFilter::set_info(bool GSIM, int run){
    runno = run; MC = GSIM;
}

bool ParticleFilter::has_electron(h22Event &event){
    return electronSelector->IsPassedFast(event, 0);
}

DataEventSelector *ParticleFilter::GetSelector(int pid){

    if (pid == 11){
        return electronSelector;
    } else if (pid == 211){
        return positivePionSelector;
    } else if (pid == -211){
        return negativePionSelector;
    } else if (pid == -321){
        return negativeKaonSelector;
    } else if (pid == 321){
        return positiveKaonSelector;
    } else {
        return new DataEventSelector();
    }

}

bool ParticleFilter::isNotAnElectronCandidate(std::vector<int> electrons, int index){
    return (std::find(electrons.begin(),electrons.end(),index) == electrons.end());
}

vector<int> ParticleFilter::getVectorOfParticleIndices(h22Event &event, int pid){

    vector<int> particles;

    if(pid == 11){
        for(int ipart=0; ipart<event.gpart; ++ipart){
	  //            event.vz[ipart] = corr.vz(event, ipart, runno, MC);
            if (electronSelector->IsPassedFast(event, ipart)){ particles.push_back(ipart); }
        }
    }

    else if (pid == 211 || pid == 321){
    //    else if (pid == 321){
      particles.clear(); 

      std::vector<int> electrons = getVectorOfParticleIndices(event, 11);
      if (!electrons.empty()){
	for(int ipart=0; ipart<event.gpart; ipart++){

	  if(event.q[ipart] > 0){
	    // Ugly shite, should do something like add charge cut
	    // to the selector and then -> ->
	    // if(getSelector(pid)->passesFast(event,ipart)){ particles.push_back(ipart); }
	    if(pid == 211){
	      if(positivePionSelector->IsPassedFast(event, ipart)){ particles.push_back(ipart); }
	    } else if (pid == 321){
	      if(positiveKaonSelector->IsPassedFast(event, ipart)){ particles.push_back(ipart); }
	    }
	  }
	}
      }
    }

    else if (pid == -211 || pid == -321){
      //    else if (pid == -321){
      particles.clear(); 

      std::vector<int> electrons = getVectorOfParticleIndices(event, 11);
      if (!electrons.empty()){
	for(int ipart=0; ipart<event.gpart; ipart++){

	  if(isNotAnElectronCandidate(electrons, ipart) && event.q[ipart] < 0){
	    // Ugly shite, should do something like add charge cut
	    // to the selector and then -> ->
	    // if(getSelector(pid)->passesFast(event,ipart)){ particles.push_back(ipart); }
	    if(pid == -211){
	      if(negativePionSelector->IsPassedFast(event, ipart)){ particles.push_back(ipart); }
	    } else if (pid == -321){
	      if(negativeKaonSelector->IsPassedFast(event, ipart)){ particles.push_back(ipart); }
	    }
	  }
	}
      }
    }

    else if (pid == 2212){
      particles.clear(); 

      std::vector<int> electrons = getVectorOfParticleIndices(event, 11);
      if (!electrons.empty()){
	for(int ipart=0; ipart<event.gpart; ipart++){
	  if(event.id[ipart] == 2212){
	    if (dcr1_meson_fid_cut->IsPassed(event, ipart)){ particles.push_back(ipart); }
	  }
	}
      }
    }
    
    // Use the built-in PID
    else{
        for(int ipart=0; ipart<event.gpart; ++ipart){
            if (event.id[ipart] == pid){ particles.push_back(ipart); }
        }
    }

    // Sort the particles by momentum
    if (!particles.empty()){
        particles = event.SortByMomentum(particles);
    }
    
    return particles;
}

vector<TLorentzVector> ParticleFilter::getVectorOfTLorentzVectors(h22Event &event, int pid){
    vector<TLorentzVector> particles;
    vector<int> particleIndices = getVectorOfParticleIndices(event, pid);

    for (int ipart=0; ipart<particleIndices.size(); ipart++){
        particles.push_back(event.GetTLorentzVector(particleIndices[ipart], pid));
    }


    // Pi-0
    if (pid == 111){
        particleIndices.clear();
        vector<int> photons = getVectorOfParticleIndices(event, 22);

        if (photons.size() >= 2){
            for(int iphot=0; iphot<photons.size(); iphot++){
                TLorentzVector firstPhoton = event.GetTLorentzVector(photons[iphot], 22);
                for(int jphot=iphot+1; jphot<photons.size(); jphot++){
                    TLorentzVector secondPhoton = event.GetTLorentzVector(photons[jphot], 22);
                    TLorentzVector pi0 = firstPhoton+secondPhoton;

                    int sector = floor((180.0 + pi0.Phi()*to_degrees)/60.0);

                    if (pi0.Mag() > pi0_invmass_mu[sector]-pi0_invmass_nsigma*pi0_invmass_sigma[sector] &&
                            pi0.Mag() < pi0_invmass_mu[sector]+pi0_invmass_nsigma*pi0_invmass_sigma[sector]){
                        particles.push_back(pi0);
                    }
                }
            }
        }

    }

    return particles;
}

int ParticleFilter::getByPID(h22Event event, int pid){
    int index = -123;

    vector<int> candidates = getVectorOfParticleIndices(event, pid);
    if(!candidates.empty()){
        // Spit back fastest by default.
        // Much more control using the entire vector.
        index = candidates[0];
    }

    return index;   //! Default Case & Nathan Harrison Convention (-123 -> for nothing found)
}

std::map<int, TLorentzVector> ParticleFilter::getMapOfIndexAndTLorentzVector(h22Event event, int pid){
    std::vector<int>              particles = getVectorOfParticleIndices(event, pid);
    std::map<int, TLorentzVector> results;

    for(int ipart=0; ipart<particles.size(); ipart++){
        results[particles[ipart]] = event.GetTLorentzVector(particles[ipart], pid);
    }

    return results;
}

std::map<std::string, bool> ParticleFilter::eid_map(h22Event &event, int index){

    int sector = event.dc_sect[index];
    std::map<std::string, bool> results;
    
    // Probably dirt slow
    string title = Form("CCTheta Cut %d", sector);
    results["CC_THETA"] = (electronSelector->GetCut(title.c_str())->IsPassed(event, index));

    title = Form("Samp Frac Cut %d", sector);
    results["EC_SAMPLING"] = (electronSelector->GetCut(title.c_str())->IsPassed(event, index));

    title = "Z-Vertex Cut";
    results["Z_VERTEX"] = (electronSelector->GetCut(title.c_str())->IsPassed(event, index));

    title = "EC Edep Inner Cut";
    results["EC_IN_OUT"] = (electronSelector->GetCut(title.c_str())->IsPassed(event, index));

    title = "DC Region 1 Fid Cut";
    results["DC_R1_FID"] = (electronSelector->GetCut(title.c_str())->IsPassed(event, index));

    title = "DC Region 3 Fid Cut";
    results["DC_R3_FID"] = (electronSelector->GetCut(title.c_str())->IsPassed(event, index));

    title = "CC Fid Cut";
    results["CC_FID"] = (electronSelector->GetCut(title.c_str())->IsPassed(event, index));

    title = "CCPhiPMT Match Cut";
    results["CC_PHI"] = (electronSelector->GetCut(title.c_str())->IsPassed(event, index));

    results["EC_FID"] = ( electronSelector->GetCut("EC-U Cut")->IsPassed(event, index) &&
                          electronSelector->GetCut("EC-V Cut")->IsPassed(event, index) &&
                          electronSelector->GetCut("EC-W Cut")->IsPassed(event, index) );


    return results;
}

#endif
