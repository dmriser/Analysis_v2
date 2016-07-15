//////////////////////////////////////////////
/*
 
 pars.h -> holds parameters for electron id
 prints, and loads.
 
 */
//////////////////////////////////////////////

#ifndef EPARS_H
#define EPARS_H

// c++ includes
#include <iostream>
using std::string;

class epars
{
    //
public:
    epars();
    ~epars();
    
    // datatypes
    
    // EC Sampling
    double ECSSA[6], ECSSB[6], ECSSC[6], ECSSD[6]; // sigma
    double ECSMA[6], ECSMB[6], ECSMC[6], ECSMD[6]; // mu
    double ECSNSIGMA[6];                 // (mu +/- NSIGMA*sigma)
    
    // EC Fid. UVW Coords.
    double ECUMIN[6], ECUMAX[6]; // U Limits
    double ECVMIN[6], ECVMAX[6]; // V Limits
    double ECWMIN[6], ECWMAX[6]; // W Limits
    
    // EC Inner Min. Dep. Energy
    double ECEDEPMIN;
    
    // Z-Vertex Center at Target
    double VZMIN, VZMAX;
    
    // Drift Chamber Regions 1 & 3 Fiducial Cuts
    double DCR1FIDH, DCR1FIDA;  //! Height, Angle
    double DCR3FIDH, DCR3FIDA;
    
    // Number of Photoelectrons in Cherenkov Counter PMT
    double CCNPHE;
    
    // Cherenkov Counter Fid. Cut: A - B sqrt(1 - rphi^2/360)
    double CCFIDA, CCFIDB;
    
    // member functions
    void load(string);
    void save(string);
    
    double ec_samp_min(double, int);
    double ec_samp_max(double, int);
    double dc_left(double, int);
    double dc_right(double, int);
    double cc_min(double);
};

class hpars
{
    public:
        hpars();
        ~hpars();
    
    // ---------- All Hadrons -------------
    double DVZMIN[6], DVZMAX[6];    //! Left and Right limit for Electron VZ - Hadron VZ
    double DCR1FIDH, DCR1FIDA;  //! DC Region 1 Fid. Cuts
    
    // ---------- Protons --------------
    double PROT_DBMIN[6], PROT_DBMAX[6];
    
    // ---------- Pi + --------------
    double PIP_DBMIN[6], PIP_DBMAX[6];
    
    // ---------- Pi - --------------
    double PIM_DBMIN[6], PIM_DBMAX[6];
    
    
    // member functions
    void load(string);
    void save(string);
    
    double dc_left(double);   //! ( event.tl1_y[i] )
    double dc_right(double);
};

#endif
