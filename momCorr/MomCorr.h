#ifndef momcorr_h
#define momcorr_h

#include <stdio.h>
#include <iostream>
#include "TROOT.h"
#include "TLorentzVector.h"

Int_t GetSector(Double_t);

/* CLASS DEFINITION */
class MomCorr_e1f {
public:
  MomCorr_e1f(std::string);
  
  const static int NSect = 6;
  const static int Npar = 4; 
  
  const static int ThetaC_n = 144;

  // C++ 11 Standard
  static constexpr double ThetaC_min = 0;
  static constexpr double ThetaC_max = 144;
  static constexpr double ThetaC_wid = 1.;

  // Older
  //  const static  double ThetaC_min = 0;
  //  const static  double ThetaC_max = 144;
  //  const static  double ThetaC_wid = 1.;
  
  const static int MomC_T_n = 48;

  static constexpr double MomC_T_min = 1;
  static constexpr double MomC_T_max = 145;
  static constexpr double MomC_T_wid = 3.;

  //  const static double MomC_T_min = 1;
  //  const static double MomC_T_max = 145;
  //  const static double MomC_T_wid = 3.;
  
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
