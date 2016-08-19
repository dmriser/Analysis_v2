/////////////////////////////////////////////////////////
/*

  David Riser, University of Connecticut 
  August 17, 2016 

  ElectronIDPackage.h -> Defining class structure for 
  electron identification package.  

  This serves the following purposes: 
  --> Determine electron ID cut values and write them to 
  the parameter files. 
  --> Fill histograms for electron ID validation and
  information, draw these histograms. 
  --> Defines the ElectronSelector : DSelection object
  which holds the standard electron identification cuts. 
  
  Objects Defined Below: 
  --> ElectronIDManager 
  --> ElectronIDHistograms 
  --> ElectronSelector 
  

*/
/////////////////////////////////////////////////////////

#ifndef electronid_package_h
#define electronid_package_h

// c++ includes
#include <iostream>

// my includes
#include "CommonTools.h"
#include "Corrections.h"
#include "DBins.h"
#include "DCut.h"
#include "DEvent.h"
#include "DSelection.h"
#include "h22Event.h"
#include "h22Reader.h"
#include "Pars.h"

// root includes
#include "TH1.h"
#include "TH2.h"

/////////////////////////////////////////////////////////
/*

  ElectronIDManager 
  
*/
/////////////////////////////////////////////////////////

class ElectronIDManager : public h22Reader
{
 public:
  ElectronIDManager();
  ~ElectronIDManager();

  ElectronIDManager(int);

  // Data Members
  Corrections corr; 
  epars pars; 
  
};

/////////////////////////////////////////////////////////
/*

  ElectronIDHistograms 
  
*/
/////////////////////////////////////////////////////////

class ElectronIDHistograms
{
 public:
  ElectronIDHistograms();
  ~ElectronIDHistograms();

  // Data Members
  std::string cut[3]; /** holds names of cuts used throughout */
  std::string output_name; /** name for writing out histograms to file */

  // <----> Histograms <----> 
  // [3][7] = [0:Raw, 1:Passed This, 2:Passed All][0:All Sectors, 1-6:Sector Number]
  // 1-D
  TH1F * h1_nphe[3][7];
  TH1F * h1_vz[3][7];
  TH1F * h1_ecu[3][7];
  TH1F * h1_ecv[3][7];
  TH1F * h1_ecw[3][7];
  TH1F * h1_ec_in[3][7];

  // 2-D
  TH2F * h2_ec_sampling[3][7];
  TH2F * h2_ec_uvw[3][7];
  TH2F * h2_dcr1[3][7];
  TH2F * h2_dcr3[3][7];
  TH2F * h2_cc[3][7];

  // Member Functions 
 public:
  void draw();
  void fill(h22Event);
  void init(); /** initialize histograms, must be done */
  void set_name(std::string n) { output_name = n; };
  void write_and_close(); /** writes root file named by output_name */
  
};

/////////////////////////////////////////////////////////
/*

  ElectronSelector : DSelection 
  
*/
/////////////////////////////////////////////////////////

class ElectronSelector : public DSelection
{
 public:
  ElectronSelector();
  ~ElectronSelector();

  ElectronSelector(std::string); /** Pass the parameter filename for electron id */
  
  // Data Members
  // Parameters
  int runno; 
  bool mc_status; 
  std::string parameter_file; 
  
  Corrections corr; 
  epars pars;
  
  // Cuts
  ChargeCut * negativity_cut; /** defined in DCut.h, generic cut used to pick negative, positive, neutral particles */
  CCFiducialCut * cc_fid_cut;
  DCR1FiducialCut * dcr1_fid_cut;
  DCR3FiducialCut * dcr3_fid_cut;
  ECEdepInnerCut * edep_cut; 
  ECUCut * ecu_cut;
  ECVCut * ecv_cut;
  ECWCut * ecw_cut; 
  SampFracCut * sf_s1_cut;
  SampFracCut * sf_s2_cut;
  SampFracCut * sf_s3_cut;
  SampFracCut * sf_s4_cut;
  SampFracCut * sf_s5_cut;
  SampFracCut * sf_s6_cut;
  ZVertexCut * vz_cut;

  // Member Functions 
  bool passes(DEvent, int);
  void set_parfile(std::string s){ parameter_file = s; init(); }
  void set_info(int r, bool m){ runno = r; mc_status = m; }
  void set_runno(int r){ runno = r; }
  void set_mc_status(bool m){ mc_status = m; }
  void init();
  
};

#endif 
