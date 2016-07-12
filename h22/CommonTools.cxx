//////////////////////////////////////////
/*
 
    David Riser, University of Connecticut 

 
 */
//////////////////////////////////////////

#ifndef COMMON_TOOLS_CXX
#define COMMON_TOOLS_CXX

// C++ headers
#include <iostream>
#include <sstream>

#define pi 3.14159265359
#define to_degrees 57.2957795131
#define to_radians 0.01745329251

//! Masses in MeV
#define  proton_mass     938.27203
#define  neutron_mass    939.56536
#define  p11_mass        1440
#define  delta_mass      1232
#define  electron_mass   0.5109
#define  pi0_mass        134.9766
#define  pi_mass         139.57018
#define  eta_mass        547.853
#define  kaon_mass       493.67
#define  k0_mass         497.614
#define  rho_mass        775.49
#define  omega_mass      782.65
#define  phi_mass        1019.455

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


double pid_to_mass(int pid)
{
    if (pid == proton_id) return proton_mass;
    else if (pid == neutron_id) return neutron_mass;
    else if (pid == delta_id) return delta_mass;
    else if (pid == deltapp_id) return delta_mass;
    else if (pid == electron_id) return electron_mass;
    else if (pid == positron_id) return electron_mass;
    else if (pid == pi0_id) return pi0_mass;
    else if (pid == pip_id) return pi_mass;
    else if (pid == pim_id) return pi_mass;
    else if (pid == eta_id) return eta_mass;
    else if (pid == kp_id) return kaon_mass;
    else if (pid == km_id) return kaon_mass;
    else if (pid == k0_id) return k0_mass;
    else if (pid == rho_id) return rho_mass;
    else if (pid == omega_id) return omega_mass;
    else if (pid == phi_id) return neutron_mass;
    return 0.00;
}


#endif

