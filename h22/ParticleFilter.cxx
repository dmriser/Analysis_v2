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
#include "pars.h"
#include "ParticleFilter.h"

// root includes
#include "TVector3.h"

ParticleFilter::ParticleFilter()
{
    eparfile = "eid_default_pars.dat";
    hparfile = "hid_default_pars.dat";
    eid.load(eparfile);
    hid.load(hparfile);
    
    MC = false; runno = 0;
}

ParticleFilter::ParticleFilter(std::string efile)
{
    eparfile = efile;
    eid.load(eparfile);
    hparfile = "hid_default_pars.dat";
    hid.load(hparfile);
    
    MC = false; runno = 0;
}

ParticleFilter::ParticleFilter(std::string efile, std::string hfile)
{
    eparfile = efile; hparfile = hfile;
    eid.load(eparfile);
    hid.load(hparfile);
    
    MC = false; runno = 0;
}


ParticleFilter::~ParticleFilter()
{
    // Don't need to save EID pars because it shouldnt be changing them.
}

// setting mc and run number status internally, mostly used for interfacing with corrections class
void ParticleFilter::set_info(bool GSIM, int run)
{
    runno = run; MC = GSIM;
}

bool ParticleFilter::has_electron(h22Event event)
{
    
    if (event.q[0] < 0)                                  //! Particle is Neg.
        if (event.nphe[0] > eid.CCNPHE)                     //! Number of Photoelectrons
            if (event.ec_ei[0] > eid.ECEDEPMIN)
                if (corr.vz(event,0,runno,MC) > eid.VZMIN && corr.vz(event,0,runno,MC) < eid.VZMAX)
                    if (event.theta_cc(0) > eid.cc_min(event.rphi(0)))
                        if (event.ec_sect[0] > 0 && event.dc_sect[0] > 0 && event.cc_sect[0] > 0 && corr.good_sc_paddle(event,0)) //! Particle hit all major detector systems
                            if (event.rot_dc1x(0) > eid.dc_left(event.rot_dc1y(0),1) && event.rot_dc1x(0) > eid.dc_right(event.rot_dc1y(0),1)) //! DCR1 Fid
                                if (event.tl3_x[0] > eid.dc_left(event.tl3_y[0],3) && event.tl3_x[0] > eid.dc_right(event.tl3_y[0],3))//! DCR3 Fid
                                    if (event.etot[0]/event.p[0] > eid.ec_samp_min(event.p[0],event.ec_sect[0]-1) && event.etot[0]/event.p[0] < eid.ec_samp_max(event.p[0],event.ec_sect[0]-1))//! Ec Sampling
                                    {
                                        TVector3 uvw = event.uvw(0); //! Getting U, V, W Coords. for Particle 0
                                        int s=event.ec_sect[0]-1;
                                        if (uvw.X() > eid.ECUMIN[s] && uvw.X() < eid.ECUMAX[s])         //! EC U, V, W Cuts
                                            if (uvw.Y() > eid.ECVMIN[s] && uvw.Y() < eid.ECVMAX[s])
                                                if (uvw.Z() > eid.ECWMIN[s] && uvw.Z() < eid.ECWMAX[s])
                                                    return true;
                                    }
    
    
    
    return false;
}

int ParticleFilter::getByPID(h22Event event, int pid)
{
    int index = -123;
    
    if (pid == 11)
    {
        /*
         Doing electron ID here based on parameters loaded from file. Trying to do cheap and easy cuts first which avoid doing the
         expensive ones.  Only checking 0th particle because it's almost always where reconstruction puts the electron.  One can also
         loop on ipart<gpart and then choose the fastest electron candidate.
         
         */
        
        if (event.q[0] < 0)                                  //! Particle is Neg.
            if (event.nphe[0] > eid.CCNPHE)                     //! Number of Photoelectrons
                if (event.ec_ei[0] > eid.ECEDEPMIN)
                    if (corr.vz(event,0,runno,MC) > eid.VZMIN && corr.vz(event,0,runno,MC) < eid.VZMAX)
                        if (event.theta_cc(0) > eid.cc_min(event.rphi(0)))
                            if (event.ec_sect[0] > 0 && event.dc_sect[0] > 0 && event.cc_sect[0] > 0 && corr.good_sc_paddle(event,0)) //! Particle hit all major detector systems
                                if (event.rot_dc1x(0) > eid.dc_left(event.rot_dc1y(0),1) && event.rot_dc1x(0) > eid.dc_right(event.rot_dc1y(0),1)) //! DCR1 Fid
                                    if (event.tl3_x[0] > eid.dc_left(event.tl3_y[0],3) && event.tl3_x[0] > eid.dc_right(event.tl3_y[0],3))//! DCR3 Fid
                                        if (event.etot[0]/event.p[0] > eid.ec_samp_min(event.p[0],event.ec_sect[0]-1) && event.etot[0]/event.p[0] < eid.ec_samp_max(event.p[0],event.ec_sect[0]-1))//! Ec Sampling
                                        {
                                            TVector3 uvw = event.uvw(0); //! Getting U, V, W Coords. for Particle 0
                                            int s=event.ec_sect[0]-1;
                                            if (uvw.X() > eid.ECUMIN[s] && uvw.X() < eid.ECUMAX[s])         //! EC U, V, W Cuts
                                                if (uvw.Y() > eid.ECVMIN[s] && uvw.Y() < eid.ECVMAX[s])
                                                    if (uvw.Z() > eid.ECWMIN[s] && uvw.Z() < eid.ECWMAX[s])
                                                        index = 0;
                                        }
    }
    
    // Routines Assume that 0 is electron, you should check it before proceeding, can be done with ParticleFilter::has_electron()
    if (pid == 211)
    {
        std::vector<int> particles;
        
        for (int ipart=1; ipart < event.gpart; ipart++)
        {
            int sector = event.dc_sect[ipart]-1;
            double dvz = corr.vz(event,0,runno,MC) - corr.vz(event,ipart,runno,MC);
            double start_time = corr.electron_sct(event,0,runno,MC) - event.sc_r[0]/speed_of_light;
            
            if (event.q[ipart] > 0)
                if (dvz > hid.DVZMIN && dvz < hid.DVZMAX)
                    if (event.rot_dc1x(ipart) > hid.dc_left(event.rot_dc1y(ipart)) && event.rot_dc1x(ipart) > hid.dc_right(event.rot_dc1y(ipart)))
                    {
                        double beta  = (event.sc_r[ipart]/(corr.hadron_sct(event,ipart,runno,MC)-start_time))/speed_of_light;
                        double dbeta = event.p[ipart]/sqrt(event.p[ipart]*event.p[ipart] + pid_to_mass(pid)*pid_to_mass(pid)) - beta;
                        if ( dbeta > hid.PIP_DBMIN[sector] && dbeta < hid.PIP_DBMAX[sector] ) particles.push_back(ipart);
                    }
        }
        
        int candidate = -123;
        
        // Take fastest pi+
        if (particles.size() > 0)
        {
            candidate = particles[0];
            for (int i=1; i<particles.size(); i++)
            {
                if (event.p[particles[i]] > event.p[candidate]) candidate = i;
            }
        }
        
        index = candidate;
    }
    
    if (pid == -211)
    {
        std::vector<int> particles;
        
        for (int ipart=1; ipart < event.gpart; ipart++)
        {
            int sector = event.dc_sect[ipart]-1;
            double dvz = corr.vz(event,0,runno,MC) - corr.vz(event,ipart,runno,MC);
            double start_time = corr.electron_sct(event,0,runno,MC) - event.sc_r[0]/speed_of_light;
            
            if (event.q[ipart] < 0)
                if (dvz > hid.DVZMIN && dvz < hid.DVZMAX)
                    if (event.rot_dc1x(ipart) > hid.dc_left(event.rot_dc1y(ipart)) && event.rot_dc1x(ipart) > hid.dc_right(event.rot_dc1y(ipart)))
                    {
                        double beta  = (event.sc_r[ipart]/(corr.hadron_sct(event,ipart,runno,MC)-start_time))/speed_of_light;
                        double dbeta = event.p[ipart]/sqrt(event.p[ipart]*event.p[ipart] + pid_to_mass(pid)*pid_to_mass(pid)) - beta;
                        if (dbeta > hid.PIM_DBMIN[sector] && dbeta < hid.PIM_DBMAX[sector] ) particles.push_back(ipart);
                    }
        }
        
        int candidate = -123;
        
        // Take fastest pi-
        if (particles.size() > 0)
        {
            candidate = particles[0];
            for (int i=1; i<particles.size(); i++)
            {
                if (event.p[particles[i]] > event.p[candidate]) candidate = i;
            }
        }
        
        index = candidate;
        
    }
    
    if (pid == 2212)
    {
        std::vector<int> particles;
        
        for (int ipart=1; ipart < event.gpart; ipart++)
        {
            int sector = event.dc_sect[ipart]-1;
            double dvz = corr.vz(event,0,runno,MC) - corr.vz(event,ipart,runno,MC);
            double start_time = corr.electron_sct(event,0,runno,MC) - event.sc_r[0]/speed_of_light;
            
            if (event.q[ipart] > 0)
                if (dvz > hid.DVZMIN && dvz < hid.DVZMAX)
                    if (event.rot_dc1x(ipart) > hid.dc_left(event.rot_dc1y(ipart)) && event.rot_dc1x(ipart) > hid.dc_right(event.rot_dc1y(ipart)))
                    {
                        double beta  = (event.sc_r[ipart]/(corr.hadron_sct(event,ipart,runno,MC)-start_time))/speed_of_light;
                        double dbeta = event.p[ipart]/sqrt(event.p[ipart]*event.p[ipart] + pid_to_mass(pid)*pid_to_mass(pid)) - beta;
                        if (dbeta > hid.PROT_DBMIN[sector] && dbeta < hid.PROT_DBMAX[sector] ) particles.push_back(ipart);
                    }
        }
        
        int candidate = -123;
        
        // Take slowest proton
        if (particles.size() > 0)
        {
            candidate = particles[0];
            for (int i=1; i<particles.size(); i++)
            {
                if (event.p[particles[i]] < event.p[candidate]) candidate = i;
            }
        }
        
        index = candidate;
        
    }
    
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
    
    // Start doing Electron Identification Routines
    
    TVector3 uvw = event.uvw(0);
    int s=event.ec_sect[0]-1;
    if (s > -1){
        if (uvw.X() > eid.ECUMIN[s] && uvw.X() < eid.ECUMAX[s])         //! EC U, V, W Cuts
            if (uvw.Y() > eid.ECVMIN[s] && uvw.Y() < eid.ECVMAX[s])
                if (uvw.Z() > eid.ECWMIN[s] && uvw.Z() < eid.ECWMAX[s])
                    ec_geo_pass = true;
    }
    
    if (event.ec_ei[0] > eid.ECEDEPMIN) ec_edep_pass = true;
    if (event.etot[0]/event.p[0] > eid.ec_samp_min(event.p[0],event.ec_sect[0]-1) && event.etot[0]/event.p[0] < eid.ec_samp_max(event.p[0],event.ec_sect[0]-1)) ec_sampling_pass = true;
    if (corr.vz(event,0,runno,MC) > eid.VZMIN && corr.vz(event,0,runno,MC) < eid.VZMAX) vz_pass = true;
    if (event.rot_dc1x(0) > eid.dc_left(event.rot_dc1y(0),1) && event.rot_dc1x(0) > eid.dc_right(event.rot_dc1y(0),1)) dcr1_fid_pass = true;
    if (event.tl3_x[0] > eid.dc_left(event.tl3_y[0],3) && event.tl3_x[0] > eid.dc_right(event.tl3_y[0],3)) dcr3_fid_pass = true;
    if (event.nphe[0] > eid.CCNPHE) cc_nphe_pass = true;
    if (event.theta_cc(0) > eid.cc_min(0)) cc_fid_pass = true;
    
    eid_results["EC_GEO"]      = ec_geo_pass;
    eid_results["EC_EDEP"]     = ec_edep_pass;
    eid_results["EC_SAMPLING"] = ec_sampling_pass;
    eid_results["VZ"]          = vz_pass;
    eid_results["DCR1_FID"]    = dcr1_fid_pass;
    eid_results["DCR3_FID"]    = dcr3_fid_pass;
    eid_results["CC_NPHE"]     = cc_nphe_pass;
    eid_results["CC_FID"]      = cc_fid_pass;
    
    return eid_results;
    
}

std::map<std::string, bool> ParticleFilter::hid_map(h22Event event, int ipart)
{
    bool prot_dvz, prot_dbeta, prot_dcfid, prot_qc;
    bool pip_dvz, pip_dbeta, pip_dcfid, pip_qc;
    bool pim_dvz, pim_dbeta, pim_dcfid, pim_qc;
    
    prot_dvz = false; prot_dbeta = false; prot_dcfid = false;
    pip_dvz  = false; pip_dbeta  = false; pip_dcfid  = false;
    pim_dvz  = false; pim_dbeta  = false; pim_dcfid  = false;
    prot_qc = false; pip_qc = false; pim_qc = false;
    
    double start_time = corr.electron_sct(event,0,runno,MC) - event.sc_r[0]/speed_of_light;
    
    int sector   = event.dc_sect[ipart]-1;
    double dvz   = corr.vz(event,0,runno,MC) - corr.vz(event,ipart,runno,MC);
    double beta  = (event.sc_r[ipart]/(corr.hadron_sct(event,ipart,runno,MC)-start_time))/speed_of_light;
    double dbeta_pim  = event.p[ipart]/sqrt(event.p[ipart]*event.p[ipart] + pid_to_mass(-211)*pid_to_mass(-211)) - beta;
    double dbeta_pip  = event.p[ipart]/sqrt(event.p[ipart]*event.p[ipart] + pid_to_mass(211)*pid_to_mass(211))   - beta;
    double dbeta_prot = event.p[ipart]/sqrt(event.p[ipart]*event.p[ipart] + pid_to_mass(2212)*pid_to_mass(2212)) - beta;
    
    if (event.q[ipart] < 0)
    {
        if (corr.good_sc_paddle(event,ipart)) { pim_qc = true; }
        if (dvz > hid.DVZMIN && dvz < hid.DVZMAX ) { pim_dvz = true; }
        if (event.rot_dc1x(ipart) > hid.dc_left(event.rot_dc1y(ipart)) && event.rot_dc1x(ipart) > hid.dc_right(event.rot_dc1y(ipart))) { pim_dcfid = true; }
        if (dbeta_pim > hid.PIM_DBMIN[sector] && dbeta_pim < hid.PIM_DBMAX[sector]) { pim_dbeta = true; }
    }
    
    if (event.q[ipart] > 0)
    {
        if (corr.good_sc_paddle(event,ipart)) { pip_qc = true; prot_qc = true; }
        if (dvz > hid.DVZMIN && dvz < hid.DVZMAX) { pip_dvz = true; prot_dvz = true;}
        if (event.rot_dc1x(ipart) > hid.dc_left(event.rot_dc1y(ipart)) && event.rot_dc1x(ipart) > hid.dc_right(event.rot_dc1y(ipart))) { pip_dcfid = true; prot_dcfid = true; }
        if (dbeta_pip  > hid.PIP_DBMIN[sector]  && dbeta_pip  < hid.PIP_DBMAX[sector] ) { pip_dbeta  = true; }
        if (dbeta_prot > hid.PROT_DBMIN[sector] && dbeta_prot < hid.PROT_DBMAX[sector]) { prot_dbeta = true; }
    }
    
    std::map<std::string, bool> results;
    
    results["PROT_QC"] = prot_qc; results["PROT_DVZ"] = prot_dvz; results["PROT_DCFID"] = prot_dcfid; results["PROT_DBETA"] = prot_dbeta;
    results["PIP_QC"] = pip_qc; results["PIP_DVZ"] = pip_dvz; results["PIP_DCFID"] = pip_dcfid; results["PIP_DBETA"] = pip_dbeta;
    results["PIM_QC"] = pim_qc; results["PIM_DVZ"] = pim_dvz; results["PIM_DCFID"] = pim_dcfid; results["PIM_DBETA"] = pim_dbeta;
    
    return results;
    
}

#endif