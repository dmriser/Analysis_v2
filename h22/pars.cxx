/////////////////////////////////////////////
/*
 
 pars.cc -> EID Parameters
 
 */
/////////////////////////////////////////////
#ifndef epars_CXX
#define epars_CXX

// c++ includes
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <time.h>
using namespace std;

// my includes
#include "CommonTools.h"
#include "pars.h"


epars::epars()
{
    // Initialize Default Values
    for (int s=0; s<6; s++)
    {
        ECSSA[s] = 0.00;   ECSSB[s] = 0.00;   ECSSC[s] = 0.00;  ECSSD[s] = 0.00;
        ECSMA[s] = 0.00;   ECSMB[s] = 0.00;   ECSMC[s] = 0.00;  ECSMD[s] = 0.00;
        ECUMIN[s] = 70;   ECVMIN[s] = 0;    ECWMIN[s] = 0;
        ECUMAX[s] = 400;  ECVMAX[s] = 362;  ECWMAX[s] = 395;
        ECSNSIGMA[s] = 3.00;
    }
    
    ECEDEPMIN = 0.05; //!  50 MeV Minimum EDEP in Inner Electromagnetic Calorimeter
    VZMIN     = -27.7302;//!  Centimeters
    VZMAX     = -22.6864;
    CCNPHE    = 25;   //   This really means 2.5
    DCR1FIDH  = 22.0; DCR1FIDA = 60.0;
    DCR3FIDH  = 80.0; DCR3FIDA = 49.0;
    CCFIDA    = 46.0; CCFIDB = 35.0;
    
}

epars::~epars()
{
    // Nothing to destroy.
}

void epars::load(string fname)
{
    ifstream parfile(fname.c_str());
    
    string line;
    double value;
    
    // Get information from parameters file.
    while(getline(parfile,line))
    {
        istringstream cuts(line);
        
        while(!cuts.eof())
        {
            string buffer;
            cuts >> buffer;
            
            if (buffer == "ECSSA:")     { for (int s=0; s<6; s++) cuts >> ECSSA[s]; }
            else if (buffer == "ECSSB:"){ for (int s=0; s<6; s++) cuts >> ECSSB[s]; }
            else if (buffer == "ECSSC:"){ for (int s=0; s<6; s++) cuts >> ECSSC[s]; }
            else if (buffer == "ECSSD:"){ for (int s=0; s<6; s++) cuts >> ECSSD[s]; }
            else if (buffer == "ECSMA:"){ for (int s=0; s<6; s++) cuts >> ECSMA[s]; }
            else if (buffer == "ECSMB:"){ for (int s=0; s<6; s++) cuts >> ECSMB[s]; }
            else if (buffer == "ECSMC:"){ for (int s=0; s<6; s++) cuts >> ECSMC[s]; }
            else if (buffer == "ECSMD:"){ for (int s=0; s<6; s++) cuts >> ECSMD[s]; }
            else if (buffer == "ECSNSIGMA:"){ for (int s=0; s<6; s++) cuts >> ECSNSIGMA[s]; }
            
            else if (buffer == "ECUMIN:"){ for (int s=0; s<6; s++) cuts >> ECUMIN[s]; }
            else if (buffer == "ECVMIN:"){ for (int s=0; s<6; s++) cuts >> ECVMIN[s]; }
            else if (buffer == "ECWMIN:"){ for (int s=0; s<6; s++) cuts >> ECWMIN[s]; }
            else if (buffer == "ECUMAX:"){ for (int s=0; s<6; s++) cuts >> ECUMAX[s]; }
            else if (buffer == "ECVMAX:"){ for (int s=0; s<6; s++) cuts >> ECVMAX[s]; }
            else if (buffer == "ECWMAX:"){ for (int s=0; s<6; s++) cuts >> ECWMAX[s]; }
            
            else if (buffer == "ECEDEPMIN:"){ cuts >> ECEDEPMIN; }
            else if (buffer == "VZMIN:"){ cuts >> VZMIN; }
            else if (buffer == "VZMAX:"){ cuts >> VZMAX; }
            else if (buffer == "CCNPHE:"){ cuts >> CCNPHE; }
            else if (buffer == "DCR1FIDH:"){ cuts >> DCR1FIDH; }
            else if (buffer == "DCR1FIDA:"){ cuts >> DCR1FIDA; }
            else if (buffer == "DCR3FIDH:"){ cuts >> DCR3FIDH; }
            else if (buffer == "DCR3FIDA:"){ cuts >> DCR3FIDA; }
            else if (buffer == "CCFIDA:"){ cuts >> CCFIDA; }
            else if (buffer == "CCFIDB:"){ cuts >> CCFIDB; }

        }
    }
    
    parfile.close();
}

void epars::save(string fname)
{
    // Writing time and date into file
    time_t rawtime;
    struct tm * timeinfo;
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    
    ofstream parfile;
    parfile.open(fname.c_str());
    
    parfile << "\n # Parameter File " << fname << " Updated at " << asctime(timeinfo) << endl;
    parfile << "\n # EC Sampling Fraction Cuts Ap^3 + Bp^2 + Cp + D " << endl;
    parfile.width(12); parfile << "ECSSA:"; for (int s=0; s<6; s++) { parfile.width(12); parfile<<ECSSA[s]; } parfile << endl;
    parfile.width(12); parfile << "ECSSB:"; for (int s=0; s<6; s++) { parfile.width(12); parfile<<ECSSB[s]; } parfile << endl;
    parfile.width(12); parfile << "ECSSC:"; for (int s=0; s<6; s++) { parfile.width(12); parfile<<ECSSC[s]; } parfile << endl;
    parfile.width(12); parfile << "ECSSD:"; for (int s=0; s<6; s++) { parfile.width(12); parfile<<ECSSD[s]; } parfile << endl;
    parfile.width(12); parfile << "ECSMA:"; for (int s=0; s<6; s++) { parfile.width(12); parfile<<ECSMA[s]; } parfile << endl;
    parfile.width(12); parfile << "ECSMB:"; for (int s=0; s<6; s++) { parfile.width(12); parfile<<ECSMB[s]; } parfile << endl;
    parfile.width(12); parfile << "ECSMC:"; for (int s=0; s<6; s++) { parfile.width(12); parfile<<ECSMC[s]; } parfile << endl;
    parfile.width(12); parfile << "ECSMD:"; for (int s=0; s<6; s++) { parfile.width(12); parfile<<ECSMD[s]; } parfile << endl;
    parfile.width(12); parfile << "ECSNSIGMA:"; for (int s=0; s<6; s++) { parfile.width(12); parfile<<ECSNSIGMA[s]; } parfile << endl;
    
    parfile << "\n # EC Geometric Cuts U, V, W Coordinates " << endl;
    parfile.width(12); parfile << "ECUMIN:"; for (int s=0; s<6; s++) { parfile.width(12); parfile<<ECUMIN[s]; } parfile << endl;
    parfile.width(12); parfile << "ECVMIN:"; for (int s=0; s<6; s++) { parfile.width(12); parfile<<ECVMIN[s]; } parfile << endl;
    parfile.width(12); parfile << "ECWMIN:"; for (int s=0; s<6; s++) { parfile.width(12); parfile<<ECWMIN[s]; } parfile << endl;
    parfile.width(12); parfile << "ECUMAX:"; for (int s=0; s<6; s++) { parfile.width(12); parfile<<ECUMAX[s]; } parfile << endl;
    parfile.width(12); parfile << "ECVMAX:"; for (int s=0; s<6; s++) { parfile.width(12); parfile<<ECVMAX[s]; } parfile << endl;
    parfile.width(12); parfile << "ECWMAX:"; for (int s=0; s<6; s++) { parfile.width(12); parfile<<ECWMAX[s]; } parfile << endl;
    
    parfile << "\n # EC Inner - Minimum Energy Deposited " << endl;
    parfile.width(12); parfile << "ECEDEPMIN:"; parfile.width(12); parfile << ECEDEPMIN << endl;
    
    parfile << "\n # Z-Vertex of Track Bounds  " << endl;
    parfile.width(12); parfile << "VZMIN:"; parfile.width(12); parfile << VZMIN << endl;
    parfile.width(12); parfile << "VZMAX:"; parfile.width(12); parfile << VZMAX << endl;
    
    parfile << "\n # Number of Photoelectrons in Cherenkov Counter (x10)" << endl;
    parfile.width(12); parfile << "CCNPHE:"; parfile.width(12); parfile << CCNPHE << endl;

    parfile << "\n # Cherenkov Counter Fid. Cuts A - B sqrt(1-rphi^2/360)" << endl;
    parfile.width(12); parfile << "CCFIDA:"; parfile.width(12); parfile << CCFIDA << endl;
    parfile.width(12); parfile << "CCFIDB:"; parfile.width(12); parfile << CCFIDB << endl;
    
    
    parfile << "\n # Drift Chamber Region 1/3 Fiducial Height and Angle " << endl;
    parfile.width(12); parfile << "DCR1FIDH:"; parfile.width(12); parfile << DCR1FIDH << endl;
    parfile.width(12); parfile << "DCR1FIDA:"; parfile.width(12); parfile << DCR1FIDA << endl;
    parfile.width(12); parfile << "DCR3FIDH:"; parfile.width(12); parfile << DCR3FIDH << endl;
    parfile.width(12); parfile << "DCR3FIDA:"; parfile.width(12); parfile << DCR3FIDA << endl;
    
    parfile.close();
}

double epars::ec_samp_min(double p, int s)
{
    return ( (ECSMA[s]-ECSNSIGMA[s]*ECSSA[s])*p*p*p +  (ECSMB[s]-ECSNSIGMA[s]*ECSSB[s])*p*p +  (ECSMC[s]-ECSNSIGMA[s]*ECSSC[s])*p +  (ECSMD[s]-ECSNSIGMA[s]*ECSSD[s]) );
}

double epars::ec_samp_max(double p, int s)
{
    return ( (ECSMA[s]+ECSNSIGMA[s]*ECSSA[s])*p*p*p +  (ECSMB[s]+ECSNSIGMA[s]*ECSSB[s])*p*p +  (ECSMC[s]+ECSNSIGMA[s]*ECSSC[s])*p +  (ECSMD[s]+ECSNSIGMA[s]*ECSSD[s]) );
}

//! region passed in
double epars::dc_left(double y, int r)
{
    if (r == 1)
    {
        double slope = 1/tan(0.5*to_radians*DCR1FIDA);
        return (DCR1FIDH - slope*y);
    }
    
    else if (r == 3)
    {
        double slope = 1/tan(0.5*to_radians*DCR3FIDA);
        return (DCR3FIDH - slope*y);
        
    }
    return 0.0;
}

//! region passed in
double epars::dc_right(double y, int r)
{
    if (r == 1)
    {
        double slope = 1/tan(0.5*to_radians*DCR1FIDA);
        return (DCR1FIDH + slope*y);
    }
    
    else if (r == 3)
    {
        double slope = 1/tan(0.5*to_radians*DCR3FIDA);
        return (DCR3FIDH + slope*y);
        
    }
    return 0.0;
}

double epars::cc_min(double rphi)
{
    return CCFIDA-CCFIDB*sqrt(1-rphi*rphi/360);
}

// ------------------- Hadrons ------------------


hpars::hpars()
{
    // Initialize Default Values
    for (int s=0; s<6; s++)
    {
        PROT_DBMIN[s] = -0.05;  PROT_DBMAX[s] = 0.05;
        PIP_DBMIN[s]  = -0.05;  PIP_DBMAX[s]  = 0.05;
        PIM_DBMIN[s]  = -0.05;  PIM_DBMAX[s]  = 0.05;
    }

    DVZMIN = -3;      DVZMAX = 3;
    DCR1FIDH  = 22.0; DCR1FIDA = 60.0;

}

hpars::~hpars()
{
    // Nothing to destroy.
}

void hpars::load(string fname)
{
    ifstream parfile(fname.c_str());
    
    string line;
    double value;
    
    // Get information from parameters file.
    while(getline(parfile,line))
    {
        istringstream cuts(line);
        
        while(!cuts.eof())
        {
            string buffer;
            cuts >> buffer;
            
            if (buffer == "DVZMIN:")    { cuts >> DVZMIN; }
            else if (buffer == "DVZMAX:") { cuts >> DVZMAX; }
            else if (buffer == "DCR1FIDA:"){ cuts >> DCR1FIDA; }
            else if (buffer == "DCR1FIDH:"){ cuts >> DCR1FIDH; }
            else if (buffer == "PROT_DBMIN:"){ for (int s=0; s<6; s++) cuts >> PROT_DBMIN[s]; }
            else if (buffer == "PROT_DBMAX:"){ for (int s=0; s<6; s++) cuts >> PROT_DBMAX[s]; }
            else if (buffer == "PIP_DBMIN:"){ for (int s=0; s<6; s++) cuts >> PIP_DBMIN[s]; }
            else if (buffer == "PIP_DBMAX:"){ for (int s=0; s<6; s++) cuts >> PIP_DBMAX[s]; }
            else if (buffer == "PIM_DBMIN:"){ for (int s=0; s<6; s++) cuts >> PIM_DBMIN[s]; }
            else if (buffer == "PIM_DBMAX:"){ for (int s=0; s<6; s++) cuts >> PIM_DBMAX[s]; }

            }
    }
    
    parfile.close();
}

void hpars::save(string fname)
{
    // Writing time and date into file
    time_t rawtime;
    struct tm * timeinfo;
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    
    ofstream parfile;
    parfile.open(fname.c_str());
    
    parfile << "\n # Parameter File " << fname << " Updated at " << asctime(timeinfo) << endl;
    parfile << "\n # All Hadrons " << endl;
    parfile << "\n # Z-Vertex Difference Between Electron and Hadron Limits " << endl;
    parfile.width(12); parfile << "DVZMIN:"; parfile.width(12); parfile<<DVZMIN; parfile << endl;
    parfile.width(12); parfile << "DVZMAX:"; parfile.width(12); parfile<<DVZMAX; parfile << endl;

    
    parfile << "\n # Drift Chamber Region 1 Fiducial Height and Angle " << endl;
    parfile.width(12); parfile << "DCR1FIDH:"; parfile.width(12); parfile << DCR1FIDH << endl;
    parfile.width(12); parfile << "DCR1FIDA:"; parfile.width(12); parfile << DCR1FIDA << endl;
    
    parfile << "\n # Protons " << endl;
    parfile << "\n # Delta Beta Assuming Proton Min/Max" << endl;
    parfile.width(12); parfile << "PROT_DBMIN:"; for (int s=0; s<6; s++) { parfile.width(12); parfile<<PROT_DBMIN[s]; } parfile << endl;
    parfile.width(12); parfile << "PROT_DBMAX:"; for (int s=0; s<6; s++) { parfile.width(12); parfile<<PROT_DBMAX[s]; } parfile << endl;

    
    parfile << "\n # Pi + " << endl;
    parfile << "\n # Delta Beta Assuming Pi + Min/Max" << endl;
    parfile.width(12); parfile << "PIP_DBMIN:"; for (int s=0; s<6; s++) { parfile.width(12); parfile<<PIP_DBMIN[s]; } parfile << endl;
    parfile.width(12); parfile << "PIP_DBMAX:"; for (int s=0; s<6; s++) { parfile.width(12); parfile<<PIP_DBMAX[s]; } parfile << endl;

    parfile << "\n # Pi - " << endl;
    parfile << "\n # Delta Beta Assuming Pi - Min/Max" << endl;
    parfile.width(12); parfile << "PIM_DBMIN:"; for (int s=0; s<6; s++) { parfile.width(12); parfile<<PIM_DBMIN[s]; } parfile << endl;
    parfile.width(12); parfile << "PIM_DBMAX:"; for (int s=0; s<6; s++) { parfile.width(12); parfile<<PIM_DBMAX[s]; } parfile << endl;

    
    parfile.close();
}

double hpars::dc_left(double y)
{

        double slope = 1/tan(0.5*to_radians*DCR1FIDA);
        return (DCR1FIDH - slope*y);

}

double hpars::dc_right(double y)
{
    double slope = 1/tan(0.5*to_radians*DCR1FIDA);
    return (DCR1FIDH + slope*y);
    
}

#endif
