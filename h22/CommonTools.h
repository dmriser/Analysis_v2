//////////////////////////////////////////
/*
 
    David Riser, University of Connecticut 
 
    CommonTools.h -> Place to store consts. 
    and handy functions for analysis libs.
 
    Heavily lifted from Maurizio.
 
        July 12, 2016
 
 */
//////////////////////////////////////////

#ifndef COMMON_TOOLS_H
#define COMMON_TOOLS_H

// C++ headers
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>

#define pi 3.14159265359
#define to_degrees 57.2957795131
#define to_radians 0.01745329251
#define cm_to_barn 1e24
#define cm_to_outhouse 1e30
#define mev_to_gev 1e-3
#define gev_to_mev 1e3

//! Masses in GeV
#define  proton_mass     0.93827203
#define  neutron_mass    0.93956536
#define  p11_mass        1.440
#define  delta_mass      1.232
#define  electron_mass   0.0005109
#define  pi0_mass        0.1349766
#define  pi_mass         0.13957018
#define  eta_mass        0.547853
#define  kaon_mass       0.49367
#define  k0_mass         0.497614
#define  rho_mass        0.77549
#define  omega_mass      0.78265
#define  phi_mass        1.019455

//! PDG Particle Identifiers
#define  proton_id     2212
#define  neutron_id    2112
#define  delta_id      2214
#define  deltapp_id    2224
#define  electron_id   11
#define  positron_id   -11
#define  photon_id     22
#define  pi0_id        111
#define  pip_id        211
#define  pim_id       -211
#define  eta_id        221
#define  kp_id         321
#define  km_id        -321
#define  k0_id         311
#define  rho_id        113
#define  omega_id      223
#define  phi_id        333

//! Stuff
#define  ALPHA                   0.007299270    // Fine structure constant
#define  electron_c              1.602E-19      // Charge of electron, Coulomb
#define  hydrogen_molar_weight   1.00794        // Molar weight of Hydrogen
#define  hydrogen_density        0.0708         // density of Hydrogen at 20 degrees, g/cm^3
#define  avogadro                6.022E23       // Avogadro Number
#define  speed_of_light          29.9792458     // cm / ns


double pid_to_mass(int);

inline std::string stringify(double x)
{
    std::ostringstream o;
    o << x;
    return o.str();
}

inline std::string stringify(int x)
{
    std::ostringstream o;
    o << x;
    return o.str();
}

inline int biggest(int a, int b)
{
    if (a > b) return a;
    else return b;
}

inline int smallest(int a, int b)
{
    if (a < b) return a;
    else return b;
}

inline double convert_x_qq_to_w(double x, double qq)
{
  return sqrt(pow(proton_mass,2) + (qq/x)*(1-x));
}

inline double convert_w_qq_to_x(double w, double qq)
{
  return qq/(qq+ pow(w,2)-pow(proton_mass,2));
}

inline double calculatePhotonFlux(double beamEnergy, double w, double qq){

  double nu = (pow(w,2) - pow(proton_mass, 2) + qq)/(2*proton_mass);
  double finalEnergy = beamEnergy - nu; 
  double theta = 2*asin( sqrt(qq/(4 * beamEnergy * finalEnergy)) ); 
  double epsilon = 1/(1+2*(1+pow(nu,2)/qq)*pow(tan(theta/2),2));

  return (ALPHA/pow(pi,2)) * (finalEnergy/beamEnergy) * (pow(w,2) - pow(proton_mass,2))/(2*proton_mass)/qq * (1/(1-epsilon));
}

inline void split(const std::string &s, char delim, std::vector<std::string> &elems) {
  std::stringstream ss;
  ss.str(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    elems.push_back(item);
  }
}


inline std::vector<std::string> split(const std::string &s, char delim) {
  std::vector<std::string> elems;
  split(s, delim, elems);
  return elems;
}

#endif

