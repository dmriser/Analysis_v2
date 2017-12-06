#ifndef nathan_archive_h
#define nathan_archive_h 

// root includes 
#include "TROOT.h" //! Needed for all the special types Type_t 

// my includes 
#include "h22Event.h"

class NathanEIDWrapper
{
 public: 
  NathanEIDWrapper();
  ~NathanEIDWrapper();

  int runno, vz_strict, ec_sf_strict, ec_edep_strict, cc_theta_strict, ec_uvw_strict, dc_r1_strict, dc_r3_strict, cc_phi_strict, cc_fid_strict; 
  bool GSIM; 
  
  void set_info(int, bool); /** Pass in run number and monte carlo status */
  int get_electron(h22Event &event); /** You supply the event, we supply the electron */

  // getters and setters for strictnesses 
  int getRunno(){ return runno; }
  int getStrictZVertex(){ return vz_strict; }
  int getStrictECSamplingFraction(){ return ec_sf_strict; }
  int getStrictECEDep(){ return ec_edep_strict; }
  int getStrictCCTheta(){ return cc_theta_strict; }
  int getStrictCCPhi(){ return cc_phi_strict; }
  int getStrictCCFiducial(){ return cc_fid_strict; }
  int getStrictECUVW(){ return ec_uvw_strict; }
  int getStrictDCR1Fiducial(){ return dc_r1_strict; }
  int getStrictDCR3Fiducial(){ return dc_r3_strict; }

  void setStrictZVertex(int s){ vz_strict = s; }
  void setStrictECSamplingFraction(int s){ ec_sf_strict = s; }
  void setStrictECEDep(int s){ ec_edep_strict = s; }
  void setStrictCCTheta(int s){ cc_theta_strict = s; }
  void setStrictCCPhi(int s){ cc_phi_strict = s; }
  void setStrictCCFiducial(int s){ cc_fid_strict = s; }
  void setStrictECUVW(int s){ ec_uvw_strict = s; }
  void setStrictDCR1Fiducial(int s){ dc_r1_strict = s; }
  void setStrictDCR3Fiducial(int s){ dc_r3_strict = s; }



};

// Nathan's Functions 
int eID(Int_t gpart, Int_t q[], Float_t p[], UChar_t cc_sect[], UChar_t sc_sect[], UChar_t ec_sect[], UChar_t dc_sect[], Float_t cx[], Float_t cy[], Float_t cz[], Float_t tl1_x[], Float_t tl1_y[], Float_t tl3_x[], Float_t tl3_y[], Float_t tl3_z[], Float_t tl3_cx[], Float_t tl3_cy[], Float_t tl3_cz[], int e_zvertex_strict, Float_t vz[], Float_t vy[], Float_t vx[], int e_ECsampling_strict, int ExpOrSim, Float_t etot[], int e_ECoutVin_strict, Float_t ec_ei[], Float_t ech_x[], Float_t ech_y[], Float_t ech_z[], int e_CCthetaMatching_strict, UShort_t cc_segm[], int e_ECgeometric_strict, int e_R1fid_strict, int e_R3fid_strict, int e_CCphiMatching_strict, UChar_t sc_pd[], int e_CCfiducial_strict);

Int_t ccphimatching(Int_t cc_segm, Float_t phi);
float getCorrZ(int ExpOrSim, float vx, float vy, float vz, float px, float py, float pz, int s);
float e_sctimeCorr(int ExpOrSim, float sctime, int sector, int paddle, int runno);
float h_sctimeCorr(int ExpOrSim, float sctime, int sector, int paddle, int runno);
Float_t get_thetaCC(Float_t dc_xsc, Float_t dc_ysc, Float_t dc_zsc, Float_t dc_cxsc, Float_t dc_cysc, Float_t dc_czsc);
Float_t shift180180to30330(Float_t phi);
Float_t get_rel_phi(Float_t phi);
Float_t get_rel_phi2(Float_t phi, Int_t sector); // phi should be in the -30 to 330 range convention
Float_t atan3(Float_t y, Float_t x); // same as atan2 but returns a value between 0 and 2pi instead of between -pi and pi
bool goodORbadSCpaddle(int sector, int paddle);
Bool_t e_zvertex_pass(int strict, int ExpOrSim, Float_t corrvz);
Bool_t e_ECsampling_pass(int strict, int ExpOrSim, int sector, Float_t etot, Float_t p);
Bool_t e_ECoutVin_pass(int strict, Float_t ec_ei);
Bool_t e_ECgeometric_pass(int strict, Float_t x, Float_t y, Float_t z);
Bool_t e_CCthetaMatching_pass(int strict, int ExpOrSim, int sector, Float_t thetaCC, Int_t ccseg);
Bool_t e_CCfiducial_pass(int strict, Float_t thetaCC, Float_t relphi);
Bool_t e_R1fid_pass(int strict, Int_t sector, Float_t tl1_x, Float_t tl1_y);
Bool_t e_R3fid_pass(int strict, Int_t sector, Float_t tl3_x, Float_t tl3_y);
Bool_t e_CCphiMatching_pass(int strict, Int_t CCphiM);


#endif

