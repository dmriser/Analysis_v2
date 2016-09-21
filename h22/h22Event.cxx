//////////////////////////////////////////
/*
 
 David Riser, University of Connecticut
 
 Standard Introductory Text.
 
 */
//////////////////////////////////////////

#include <TROOT.h>

#ifndef h22Event_cxx
#define h22Event_cxx

// c++ includes
#include <cmath>
#include <iostream>
using namespace std;

// root includes
#include "TVector3.h"
#include "TLorentzVector.h"

// my includes
#include "h22Event.h"
#include "CommonTools.h"

h22Event::h22Event() 
{
    // Should probably find something to do here.
}


h22Event::~h22Event()
{
    // Nothing to destroy.
}

double h22Event::theta(int ipart)
{
    return acos(cz[ipart])*to_degrees;
}

double h22Event::rphi(int ipart)
{
    double rphi = to_degrees*atan(cy[ipart]/cx[ipart]);
    if (rphi > 330.00) rphi -= 360.00;
    rphi = rphi - 60*floor((rphi+30)/60);
    return rphi;
}

double h22Event::mcrphi(int ipart)
{
    double rphi = mcphi[ipart];
    if (rphi > 330.00) rphi -= 360.00;
    rphi = rphi - 60*floor((rphi+30)/60);
    return rphi;
}

TLorentzVector h22Event::gen_particle(int pid)
{
  for (int ipart=0; ipart<gpart; ipart++)
    {
      if (mcid[ipart] == pid) {
	return TLorentzVector(mcpx(ipart), mcpy(ipart), mcpz(ipart), mcp[ipart]);
      }
    }
  
  cout << " WARNING: GEN PARTICLE NOT FOUND! " << endl; 
  return TLorentzVector(0,0,1000,1000); 
}

void h22Event::printEvent()
{
    for (int ipart=0; ipart<gpart; ipart++)
    {
        cout.width(6); cout << ipart;
        cout.width(6); cout << q[ipart];
        cout.width(6); cout << "1";
        cout.width(6); cout << "-123";
        cout.width(4); cout << "0";
        cout.width(4); cout << "0";
        cout.width(12); cout << cx[ipart]*p[ipart];
        cout.width(12); cout << cy[ipart]*p[ipart];
        cout.width(12); cout << cz[ipart]*p[ipart];
        cout.width(12); cout << p[ipart];
        cout.width(4); cout << "0";
        cout.width(12); cout << vx[ipart];
        cout.width(12); cout << vy[ipart];
        cout.width(12); cout << vz[ipart] << endl;
        
    }
}

TVector3 h22Event::uvw(int ipart)
{
    //! Routine hijacked from Nathan
    double u, v, w, xi, yi, zi;
    double EC_the = 0.4363323;
    double EC_phi;
    double ylow   = -182.974;
    double yhi    = 189.956;
    double tgrho  = 1.95325;
    double sinrho = 0.8901256;
    double cosrho = 0.455715;
    double rot[3][3];
    
    double x = ech_x[ipart];
    double y = ech_y[ipart];
    double z = ech_z[ipart];
    
    double at = atan2(y, x);
    if (at < 0) at += 2*pi;
    
    
    double phi = to_degrees*at;
    phi=phi+30.;
    if (phi>=360.) phi=phi-360.;
    
    EC_phi = (int)(phi/60.) * 1.0471975;
    
    rot[0][0] = cos(EC_the)*cos(EC_phi);
    rot[0][1] = -sin(EC_phi);
    rot[0][2] = sin(EC_the)*cos(EC_phi);
    rot[1][0] = cos(EC_the)*sin(EC_phi);
    rot[1][1] = cos(EC_phi);
    rot[1][2] = sin(EC_the)*sin(EC_phi);
    rot[2][0] = -sin(EC_the);
    rot[2][1] = 0.;
    rot[2][2] = cos(EC_the);
    
    yi=x*rot[0][0]+y*rot[1][0]+z*rot[2][0];
    xi=x*rot[0][1]+y*rot[1][1]+z*rot[2][1];
    zi=x*rot[0][2]+y*rot[1][2]+z*rot[2][2];
    
    zi=zi-510.32;
    
    u = (yi-ylow)/sinrho;
    v = (yhi-ylow)/tgrho-xi+(yhi-yi)/tgrho;
    w = ((yhi-ylow)/tgrho+xi+(yhi-yi)/tgrho)/2./cosrho;
    
    TVector3 uvw(u,v,w);
    return uvw;
    
}

double h22Event::theta_cc(int ipart)
{
    //! Routine Hijacked from Nathan who Hijacked it from Maurizio. 
    
    double cc_pln[3] = {-0.0007840784063, 0.0, -0.001681461571};
    double d = 1.0;
    
    double dir[3] = {tl3_cx[ipart], tl3_cy[ipart], tl3_cz[ipart]};
    double P1[3] = {tl3_x[ipart], tl3_y[ipart], tl3_z[ipart]};
    double t = (cc_pln[0]*P1[0] + cc_pln[1]*P1[1] + cc_pln[2]*P1[2] + d)/(cc_pln[0]*dir[0] + cc_pln[1]*dir[1] + cc_pln[2]*dir[2]);
    
    double CCx = (P1[0] + dir[0]*t)*10;
    double CCy = (P1[1] + dir[1]*t)*10;
    double CCz = (P1[2] + dir[2]*t)*10;
    
    double thetaCC = atan2(sqrt(CCx*CCx + CCy*CCy), CCz);
    return thetaCC*to_degrees;
}

double h22Event::mcpx(int ipart)
{
    return mcp[ipart]*sin(mctheta[ipart]*to_radians)*cos(mcphi[ipart]*to_radians);
}

double h22Event::mcpy(int ipart)
{
    return mcp[ipart]*sin(mctheta[ipart]*to_radians)*sin(mcphi[ipart]*to_radians);
}

double h22Event::mcpz(int ipart)
{
    return mcp[ipart]*cos(mctheta[ipart]*to_radians);
}

double h22Event::rot_dc1x(int ipart)
{
    int sm1 = dc_sect[ipart]-1;
    return tl1_y[ipart]*sin(sm1*to_radians*60.0)+tl1_x[ipart]*cos(sm1*to_radians*60.0);
}

double h22Event::rot_dc1y(int ipart)
{
    int sm1 = dc_sect[ipart]-1;
    return tl1_y[ipart]*cos(sm1*to_radians*60.0)-tl1_x[ipart]*sin(sm1*to_radians*60.0);
}


#endif
