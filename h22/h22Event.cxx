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

// my includes 
#include "h22Event.h"

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
  return acos(cz[ipart])*(180/3.14159);
}

double h22Event::rphi(int ipart)
{
  double rphi = (180/3.14159)*atan(cy[ipart]/cx[ipart]);
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

  Float_t u, v, w, xi, yi, zi;
  Float_t EC_the = 0.4363323;
  Float_t EC_phi;
  Float_t ylow   = -182.974;
  Float_t yhi    = 189.956;
  Float_t tgrho  = 1.95325;
  Float_t sinrho = 0.8901256;
  Float_t cosrho = 0.455715;
  Float_t rot[3][3];

  double phi = (180/3.14159)*atan(cy[ipart]/cx[ipart]);
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

  double x = ech_x[ipart];
  double y = ech_y[ipart];
  double z = ech_z[ipart];

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

double h22Event::mcpx(int ipart)
{
  return mcp[ipart]*sin(mctheta[ipart]*3.14159/180)*cos(mcphi[ipart]*3.14159/180);
}

double h22Event::mcpy(int ipart)
{
  return mcp[ipart]*sin(mctheta[ipart]*3.14159/180)*sin(mcphi[ipart]*3.14159/180);
}

double h22Event::mcpz(int ipart)
{
  return mcp[ipart]*cos(mctheta[ipart]*3.14159/180);
}

#endif
