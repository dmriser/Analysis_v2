#ifndef momcorr_h
#define momcorr_h

#include <stdio.h>
#include <iostream>
#include "TROOT.h"
#include "TLorentzVector.h"

#define NSect 6

/* Theta Binning for Theta correction */
#define ThetaC_n 144
Double_t ThetaC_min = 0;
Double_t ThetaC_max = 144;
Double_t ThetaC_wid = 1.;

/* Theta Binning for Momentum correction */
#define MomC_T_n 48
Double_t MomC_T_min = 1;
Double_t MomC_T_max = 145;
Double_t MomC_T_wid = 3.;

/* Size of Vectors with parameters for phi dependence of Momentum Correction
   phi between -30 and 30 deg */
#define Npar 4

Int_t GetSector(Double_t);

/* CLASS DEFINITION */
class MomCorr_e1f {
public:
  MomCorr_e1f(std::string);


private:
  std::string path;
  
  /* Vectors with parameters for phi dependence of Theta Correction 
     phi between -30 and 30 deg */
  Double_t c0_theta[ThetaC_n][NSect];
  Double_t c1_theta[ThetaC_n][NSect];
  Double_t c2_theta[ThetaC_n][NSect];

  /* Vectors with parameters for phi dependence of Electron Momentum Correction
     phi between -30 and 30 deg */
  Double_t c0_mom[MomC_T_n][NSect][Npar];
  Double_t c1_mom[MomC_T_n][NSect][Npar];


  /* Vectors with parameters for phi dependence of pi+ Momentum Correction
     phi between -30 and 30 deg */
  Double_t d0_mom[MomC_T_n][NSect][Npar];
  Double_t d1_mom[MomC_T_n][NSect][Npar];


  /* REad angle correction parameters */
  void read_theta_par();
  /* Read momentum correction parameters for electrons */
  void read_mom_par();
  /* Read momentum correction parameters for pi+ */
  void read_mom_pip_par();

  /* Angle correction */
  Double_t theta_corr(Double_t , Double_t , Int_t );

  /* momentum correction for electrons */
  Double_t mom_corr(Double_t , Double_t , Double_t , Int_t );

  /* momentum correction for hadrons */
  Double_t mom_corr_pip(Double_t , Double_t , Double_t , Int_t);

public:
  TLorentzVector PcorN(TLorentzVector Pin, Int_t charge, Int_t ipart);
  void set_path(std::string);
};

#endif
