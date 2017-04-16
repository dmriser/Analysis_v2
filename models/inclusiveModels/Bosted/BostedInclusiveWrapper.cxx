// -*- C++ -*-

/* class F1F209Wrapper
 * Wrapper class to get f1, f2, and xs with P. Bosted fitting.
 * Unit of xs is ub/MeV-sr.
 * Valid for all W<5 GeV and all Q2<11 GeV2.
 * Use -fno-leading-underscore -fno-second-underscore when compiling F1F209.f
 */

// History:
//   Mar 2013, C. Gu, First public version.
//   May 2014, Comments modified By Jixie Zhang

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>
using std::cout; 
using std::endl; 

#include "BostedInclusiveWrapper.h"

using namespace std;

extern "C" {
void f1f2in09_(double* Z, double* A, double* Q2, double* W2, double* F1, double* F2, double* rc);
void f1f2qe09_(double* Z, double* A, double* Q2, double* W2, double* F1, double* F2);
}

F1F209Wrapper::F1F209Wrapper(){
    // Nothing to do
}

F1F209Wrapper::~F1F209Wrapper(){
    // Nothing to do
}

void F1F209Wrapper::GetF1F2IN09(double Z, double A, double Q2, double W2, double &F1, double &F2, double &rc)
{
    double F1o, F2o, rco;
    f1f2in09_(&Z, &A, &Q2, &W2, &F1o, &F2o, &rco);
    F1 = F1o;
    F2 = F2o;
    rc = rco;
}

void F1F209Wrapper::GetF1F2QE09(double Z, double A, double Q2, double W2, double &F1, double &F2){
    double F1o, F2o, rco;
    f1f2qe09_(&Z, &A, &Q2, &W2, &F1o, &F2o);
    F1 = F1o;
    F2 = F2o;
}

double F1F209Wrapper::GetXS(double Z, double A, double Ei, double Ef, double theta){
    const double M = 0.93825;
    const double alpha = 1/137.0;
    const double pi = 3.141592;


    // Going to rad.
    theta = theta * (3.14159/180.0);
    
    double nu = Ei - Ef;
    double Q2 = 4*Ei*Ef*pow(sin(theta/2),2); 
    double w2 = M*M - Q2 + 2*M*nu;
    
    double F1, F2, r;
    double xs1, xs2;

    GetF1F2IN09(Z, A, Q2, w2, F1, F2, r);
    xs1 = (2. / 137. * Ef / Q2 * cos(theta / 2.))*(2. / 137. * Ef / Q2 * cos(theta / 2.)); // mott
    xs1 = xs1 * (2. / M * F1 * tan(abs(theta) / 2.) * tan(abs(theta) / 2.) + F2 / nu);
    xs1 = xs1 * 389.379; // <---- What is this factor? 

    GetF1F2QE09(Z, A, Q2, w2, F1, F2);
    xs2 = (2. / 137. * Ef / Q2 * cos(theta / 2.))*(2. / 137. * Ef / Q2 * cos(theta / 2.)); // mott
    xs2 = xs2 * (2. / M * F1 * tan(abs(theta) / 2.) * tan(abs(theta) / 2.) + F2 / nu);
    xs2 = xs2 * 389.379;
        
    return (xs1 + xs2); // ub/GeV-sr
}

// Needs to be fixed, working on x-Q2 cross section first. 
double F1F209Wrapper::GetXSByWQQ(double Z, double A, double Ei, double w, double qq){
    const double M = 0.93825;

    double Q2 = qq; 
    double w2 = w*w;
    double x = Q2/(Q2 + w2 - M*M);
    double nu = Q2/(2*M*x);
    double Ef = Ei-nu;
    double theta = 2*asin(sqrt(Q2)/(2*sqrt(Ei*Ef)));
    
    double xs = GetXS(Z, A, Ei, Ef, theta);
    double jacobian = w/(2*Ei*Ef*M)*(2*3.14159);
    //    double jacobian = 1.00; 

    return xs*jacobian; // ub
}


