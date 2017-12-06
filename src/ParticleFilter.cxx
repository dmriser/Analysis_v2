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
    protonSelector       = new DataEventSelector(); 
    positiveMesonCandidateSelector = new DataEventSelector(); 

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

    // fiducial cuts on r1 triangle shape 
    dcr1_meson_fid_cut = new DCR1FiducialCut();

    // delta beta cuts 
    kp_dbeta_cut     = new DataEventCut_DeltaBetaCut(321); 
    pp_dbeta_cut     = new DataEventCut_DeltaBetaCut(211); 
    proton_dbeta_cut = new DataEventCut_DeltaBetaCut(2212); 

    // time of flight cuts 
    pip_tofmass_cut = new DataEventCut_TOFMassCut(); 
    pim_tofmass_cut = new DataEventCut_TOFMassCut(); 
    kp_tofmass_cut  = new DataEventCut_TOFMassCut(); 
    km_tofmass_cut  = new DataEventCut_TOFMassCut(); 

    pip_tofmass_cut->SetMin(params->getParameter("PIP_TOFMASS_MIN").getValue(0));
    pip_tofmass_cut->SetMax(params->getParameter("PIP_TOFMASS_MAX").getValue(0));
    pim_tofmass_cut->SetMin(params->getParameter("PIM_TOFMASS_MIN").getValue(0));
    pim_tofmass_cut->SetMax(params->getParameter("PIM_TOFMASS_MAX").getValue(0));
    kp_tofmass_cut->SetMin(params->getParameter("KP_TOFMASS_MIN").getValue(0));
    kp_tofmass_cut->SetMax(params->getParameter("KP_TOFMASS_MAX").getValue(0));
    km_tofmass_cut->SetMin(params->getParameter("KM_TOFMASS_MIN").getValue(0));
    km_tofmass_cut->SetMax(params->getParameter("KM_TOFMASS_MAX").getValue(0));

    // new cut from Dan Carman 
    dbeta_minimizer_211      = new DataEventCut_DBetaMinimizerCut(); 
    dbeta_minimizer_321      = new DataEventCut_DBetaMinimizerCut(); 
    dbeta_minimizer_2212     = new DataEventCut_DBetaMinimizerCut(); 
    dbeta_minimizer_neg_211  = new DataEventCut_DBetaMinimizerCut(); 
    dbeta_minimizer_neg_321  = new DataEventCut_DBetaMinimizerCut(); 

    betap_likelihood_211  = new DataEventCut_BetaPLikelihood(211); 
    betap_likelihood_321  = new DataEventCut_BetaPLikelihood(321); 
    betap_likelihood_2212 = new DataEventCut_BetaPLikelihood(2212); 

    betap_likelihood_211 ->Configure(params); 
    betap_likelihood_321 ->Configure(params);     
    betap_likelihood_2212->Configure(params); 

    dbeta_minimizer_211->AddPossibleParticle(211); 
    dbeta_minimizer_211->AddPossibleParticle(321); 
    dbeta_minimizer_211->AddPossibleParticle(2212); 
    dbeta_minimizer_211->SetTarget(211); 

    dbeta_minimizer_321->AddPossibleParticle(211); 
    dbeta_minimizer_321->AddPossibleParticle(321); 
    dbeta_minimizer_321->AddPossibleParticle(2212); 
    dbeta_minimizer_321->SetTarget(321); 

    dbeta_minimizer_neg_321->AddPossibleParticle(-211); 
    dbeta_minimizer_neg_321->AddPossibleParticle(-321); 
    dbeta_minimizer_neg_321->SetTarget(-321); 

    dbeta_minimizer_neg_211->AddPossibleParticle(-211); 
    dbeta_minimizer_neg_211->AddPossibleParticle(-321); 
    dbeta_minimizer_neg_211->SetTarget(-211); 

    dbeta_minimizer_2212->AddPossibleParticle(211); 
    dbeta_minimizer_2212->AddPossibleParticle(321); 
    dbeta_minimizer_2212->AddPossibleParticle(2212); 
    dbeta_minimizer_2212->SetTarget(2212); 
    
    // -1 just let's any sector be applied 
    //    mesonCandidate_tofmass_cut = new DataEventCut_TOFMassCut(); 
    //    mesonCandidate_tofmass_cut->SetMin(params->getParameter("PIP_TOFMASS_MIN").getValue(0)); 
    //    mesonCandidate_tofmass_cut->SetMax(params->getParameter("KP_TOFMASS_MAX").getValue(0)); 

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
    edep_cut->SetMax(1.20); // estimated from looking at data max value

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

    vz_cut->SetMin(params->getParameter("EL_VZ_MIN").getValue(0) );
    vz_cut->SetMax( params->getParameter("EL_VZ_MAX").getValue(0) );

    nphe_cut->SetMin(20);
    nphe_cut->SetMax(1000);

    p_cut->SetMin(0.5);
    p_cut->SetMax(6.0);

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

    // fiducial cuts 
    dcr1_meson_fid_cut->height = params->getParameter("KP_DCR1_HEIGHT").getValue(0);
    dcr1_meson_fid_cut->angle  = params->getParameter("KP_DCR1_ANGLE").getValue(0);

    dvz_cut->SetMin(-1 *params->getParameter("KP_DVZ").getValue(0) );
    dvz_cut->SetMax( params->getParameter("KP_DVZ").getValue(0) );

    // strict hard coded cuts on protons
    proton_dbeta_cut->SetMin(-0.045); 
    proton_dbeta_cut->SetMax( 0.045);

    // Setup pion selectors
    testCut = new TestCut(); 

    //    positivePionSelector->AddCut(dbeta_minimizer_211);
    positivePionSelector->AddCut(betap_likelihood_211); 
    //    positivePionSelector->AddCut(pip_tofmass_cut); 
    positivePionSelector->EnableAll();

    negativePionSelector->AddCut(dvz_cut);
    negativePionSelector->AddCut(pim_tofmass_cut); 
    negativePionSelector->AddCut(dbeta_minimizer_neg_211);
    negativePionSelector->EnableAll();

    //    positiveKaonSelector->AddCut(dbeta_minimizer_321);
    positiveKaonSelector->AddCut(betap_likelihood_321);
    //    positiveKaonSelector->AddCut(kp_tofmass_cut); 
    positiveKaonSelector->EnableAll();
 
    negativeKaonSelector->AddCut(dvz_cut);
    negativeKaonSelector->AddCut(km_tofmass_cut);
    negativeKaonSelector->AddCut(dbeta_minimizer_neg_321);
    negativeKaonSelector->EnableAll();
 
    positiveMesonCandidateSelector->AddCut(dcr1_meson_fid_cut); 
    positiveMesonCandidateSelector->AddCut(dvz_cut); 
    positiveMesonCandidateSelector->EnableAll(); 

    //    protonSelector->AddCut(dbeta_minimizer_2212);
    protonSelector->AddCut(betap_likelihood_2212); 
    protonSelector->AddCut(dcr1_meson_fid_cut);
    protonSelector->EnableAll(); 
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
    } else if (pid == 2212){
        return protonSelector;
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
	    if (positiveMesonCandidateSelector->IsPassedFast(event, ipart)){
	      if(pid == 211){
		if(positivePionSelector->IsPassedFast(event, ipart)){ particles.push_back(ipart); }
	      } else if (pid == 321){
		if(positiveKaonSelector->IsPassedFast(event, ipart)){ particles.push_back(ipart); }
	      }
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
	  if (event.q[ipart]>0){
	    if(protonSelector->IsPassedFast(event, ipart)){ particles.push_back(ipart); }
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

std::map<std::string, float> ParticleFilter::eid_distance_map(h22Event &event, int index){
  int sector = event.dc_sect[index];
  std::map<std::string, float> results; 

  string title = Form("Samp Frac Cut %d", sector);
  results["EC_SAMPLING"] = (electronSelector->GetCut(title.c_str())->GetFractionalDistance(event, index));

  title = "Z-Vertex Cut";
  results["Z_VERTEX"] = (electronSelector->GetCut(title.c_str())->GetFractionalDistance(event, index));

  title = "EC Edep Inner Cut";
  results["EC_IN_OUT"] = (electronSelector->GetCut(title.c_str())->GetFractionalDistance(event, index));

  title = Form("CCTheta Cut %d", sector);
  results["CC_THETA"] = (electronSelector->GetCut(title.c_str())->GetFractionalDistance(event, index));

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
