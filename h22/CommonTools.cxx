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

// my includes
#include "CommonTools.h"

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

