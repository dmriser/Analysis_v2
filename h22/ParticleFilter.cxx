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
    electronSelector = new DataEventSelector();

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

    // Set limits on cuts from parameters 
    negativity_cut->set_min(-1.1);
    negativity_cut->set_max(-0.9);

    cc_fid_cut->a = params->getParameter("EL_CC_FIDA").getValue(0);
    cc_fid_cut->b = params->getParameter("EL_CC_FIDB").getValue(0);
    cc_fid_cut->c = params->getParameter("EL_CC_FIDC").getValue(0);

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

    vz_cut->set_min( params->getParameter("EL_VZ_MIN").getValue(0) );
    vz_cut->set_max( params->getParameter("EL_VZ_MAX").getValue(0) );

    nphe_cut->set_min(25);
    nphe_cut->set_max(1000);

    p_cut->set_min(0.5);
    p_cut->set_max(100.0);

    // Push back the cuts 
    electronSelector->add_cut( negativity_cut );
    electronSelector->add_cut( nphe_cut );
    electronSelector->add_cut( p_cut );
    electronSelector->add_cut( cc_fid_cut );
    electronSelector->add_cut( cc_phi_match_cut );
    electronSelector->add_cut( cc_theta_s1_cut );
    electronSelector->add_cut( cc_theta_s2_cut );
    electronSelector->add_cut( cc_theta_s3_cut );
    electronSelector->add_cut( cc_theta_s4_cut );
    electronSelector->add_cut( cc_theta_s5_cut );
    electronSelector->add_cut( cc_theta_s6_cut );
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
    //    electronSelector->add_cut( qc_cut );
    electronSelector->add_cut( vz_cut );

    // This can be overridden by the user by just 
    // turning off the ones you don't want. This 
    // happens at the time of construction so your
    // call will be after.
    electronSelector->enable_all();
    electronSelector->disable_by_name("NPhe Cut");

    // Neutral Pion 
    for(int s=0; s<6; s++){
      pi0_invmass_mu[s]    = params->getParameter("PI0_INVMASS_MU")   .getValue(s);
      pi0_invmass_sigma[s] = params->getParameter("PI0_INVMASS_SIGMA").getValue(s);
    }
      pi0_invmass_nsigma   = params->getParameter("PI0_INVMASS_NSIGMA").getValue(0);

      // Charged Pion 
      for(int s=0; s<6; s++){
	pim_tofmass_mu[s]     = params->getParameter("PIM_TOFMASS_MU").getValue(s); 
	pim_tofmass_sigma[s]  = params->getParameter("PIM_TOFMASS_SIGMA").getValue(s); 
	pim_tofmass_nsigma[s] = params->getParameter("PIM_TOFMASS_NSIGMA").getValue(0); 
	pip_tofmass_mu[s]     = params->getParameter("PIP_TOFMASS_MU").getValue(s); 
	pip_tofmass_sigma[s]  = params->getParameter("PIP_TOFMASS_SIGMA").getValue(s); 
	pip_tofmass_nsigma[s] = params->getParameter("PIP_TOFMASS_NSIGMA").getValue(0); 
      }

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

DataEventSelector * ParticleFilter::getSelector(int pid){

  if (pid == 11){
    return electronSelector; 
  }
  else {
    return new DataEventSelector();
  }

}

vector<int> ParticleFilter::getVectorOfParticleIndices(h22Event event, int pid){
  
  vector<int> particles;
  
  if(pid == 11){
    for(int ipart=0; ipart<event.gpart; ++ipart){
      if (electronSelector->passesFast(event, ipart)){ particles.push_back(ipart); }
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
    particles = event.sortByMomentum(particles);
  }
  

  return particles;
}

vector<TLorentzVector> ParticleFilter::getVectorOfTLorentzVectors(h22Event event, int pid){
  vector<TLorentzVector> particles; 
  vector<int> particleIndices = getVectorOfParticleIndices(event, pid);

  for (int ipart=0; ipart<particleIndices.size(); ipart++){
    particles.push_back(event.getTLorentzVector(particleIndices[ipart], pid));
  }
  

  // Pi-0 
  if (pid == 111){
    particleIndices.clear();
    vector<int> photons = getVectorOfParticleIndices(event, 22);

    if (photons.size() >= 2){
      for(int iphot=0; iphot<photons.size(); iphot++){
	TLorentzVector firstPhoton = event.getTLorentzVector(photons[iphot], 22); 
	for(int jphot=iphot+1; jphot<photons.size(); jphot++){
	  TLorentzVector secondPhoton = event.getTLorentzVector(photons[jphot], 22); 
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

  // Need to move to above routine.
  /*
  else if (pid == 211){
    particleIndices.clear();
    
    vector<int> electrons = getVectorOfParticleIndices(event, 11);
    if( !electrons.empty() ){
      double start_time = corr.electron_sct(event,electrons[0],runno,GSIM) - event.sc_r(electrons[0])/speed_of_light;


      for(int ipart=0; ipart<event.gpart; ipart++){
	if(ipart == electrons[0]){ ipart++; }
	
	
      }
    }
  }
  */
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

    /*
    // check for electrons
    if(pid == 11){
      vector<int> candidates;
      for (int ipart=0; ipart<event.gpart; ++ipart){
	if (electronSelector->passes(event, ipart)) candidates.push_back(ipart); 
      }

      // Spit back the fastest track 
      if (candidates.size() == 1){ index = candidates[0]; }

      else if (candidates.size() > 1){
	int fast = candidates[0]; 
	for (int i=1; i<candidates.size(); ++i){ if (event.p[candidates[i]] > event.p[fast]){ fast = candidates[i]; } }

	index = fast; 
      }
    
    } // End of electron
    */

    return index;   //! Default Case & Nathan Harrison Convention (-123 -> for nothing found)
}

std::map<std::string, bool> ParticleFilter::eid_map(h22Event event, int index){

  int sector = event.dc_sect[index];
  std::map<std::string, bool> results;
    
  // Probably dirt slow
  string title = Form("CCTheta Cut %d", sector); 
  results["CC_THETA"] = (electronSelector->getCut(title.c_str())->passes(event, index));
  
  title = Form("Samp Frac Cut %d", sector); 
  results["EC_SAMPLING"] = (electronSelector->getCut(title.c_str())->passes(event, index));

  title = "Z-Vertex Cut";
  results["Z_VERTEX"] = (electronSelector->getCut(title.c_str())->passes(event, index));

  title = "EC Edep Inner Cut";
  results["EC_IN_OUT"] = (electronSelector->getCut(title.c_str())->passes(event, index));

  title = "DC Region 1 Fid Cut";
  results["DC_R1_FID"] = (electronSelector->getCut(title.c_str())->passes(event, index));

  title = "DC Region 3 Fid Cut";
  results["DC_R3_FID"] = (electronSelector->getCut(title.c_str())->passes(event, index));

  title = "CC Fid Cut";
  results["CC_FID"] = (electronSelector->getCut(title.c_str())->passes(event, index));

  title = "CCPhiPMT Match Cut";
  results["CC_PHI"] = (electronSelector->getCut(title.c_str())->passes(event, index));

  results["EC_FID"] = ( electronSelector->getCut("EC-U Cut")->passes(event, index) && 
			electronSelector->getCut("EC-V Cut")->passes(event, index) &&
			electronSelector->getCut("EC-W Cut")->passes(event, index) );


  return results;    
}

#endif
