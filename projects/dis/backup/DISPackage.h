////////////////////////////////////////////////////////////////////
/*

  David Riser, University of Connecticut 
  August 24, 2016 

  DISPackage.h 

*/
////////////////////////////////////////////////////////////////////

#ifndef dis_h
#define dis_h

// c++ includes 
#include <iostream>
#include <vector>

// my includes
#include "DBins.h"
#include "DCut.h"
#include "DEvent.h"
#include "DInformation.h"
#include "DSelection.h"
#include "ElectronIDPackage.h"
#include "NathanArchive.h"

//  root includes
#include "TH1.h"
#include "TH2.h"

////////////////////////////////////////////////////////////////////////
/*

  DISHistograms 

*/
////////////////////////////////////////////////////////////////////////

class DISHistograms
{
 public:
  DISHistograms();
  ~DISHistograms();

  // Data Types
  std::string output_name;
  std::string sect[7];
  
  DBins xBins, qqBins; 

  // Histograms
  std::vector<std::vector<TH1F*> > h1_gen_x_by_qq;
  std::vector<std::vector<TH1F*> > h1_rec_x_by_qq;
  std::vector<std::vector<TH1F*> > h1_hits_x_by_qq;
  std::vector<std::vector<TH1F*> > h1_acc_x_by_qq;
  std::vector<std::vector<TH1F*> > h1_rxs_x_by_qq;
  std::vector<std::vector<TH1F*> > h1_rxs_ratio_x_by_qq;
  std::vector<std::vector<TH1F*> > h1_xs_x_by_qq;
  std::vector<std::vector<TH1F*> > h1_xs_ratio_x_by_qq;
  std::vector<TH1F*> h1_model_x_by_qq;
   
  TH2F * h2_hits_x_qq[7];
  TH2F * h2_rec_x_qq[7];
  TH2F * h2_gen_x_qq[7];
  
  // Member Functions
  void draw();
  void fill(DEvent, int); /** Pass in the event and the type data or monte carlo */
  void fill_gen(DEvent); /** Just pass in the generated event */
  void init();
  void load();
  void set_bins(DBins x, DBins qq){ xBins = x; qqBins = qq; }
  void save();
  
};

////////////////////////////////////////////////////////////////////////
/*

  DISManager 

*/
////////////////////////////////////////////////////////////////////////

class DISManager
{
 public:
  DISManager();
  ~DISManager();

  // Data Types
  int eid_version;
  double fcup_charge; 
  std::string parfile[2]; 
  std::string outfile;
  std::string infofile; 
  std::string momcorr_path;

  // Don't really want to do this but DPlaneBins isn't working yet 
  const static int n_x_bins = 5, n_qq_bins = 2; 
  int hits[7][n_x_bins][n_qq_bins], rec[7][n_x_bins][n_qq_bins], gen[7][n_x_bins][n_qq_bins];
  double acc[7][n_x_bins][n_qq_bins], data_mc_ratio[7][n_x_bins][n_qq_bins], corr_hits[7][n_x_bins][n_qq_bins], xs[7][n_x_bins][n_qq_bins]; 
  
  double xLims[n_x_bins+1], qqLims[n_x_bins][n_qq_bins+1]; 
  
  DBins xBins, qqBins; 
  DPlaneBins x_qq_bins; 
  DInformation info; 
  DISHistograms histos; 
  DSelection dis_selector; 
  ElectronSelector eid[2];
  h22Reader reader[2];
  NathanEIDWrapper nathan; 
  VirtualityCut * qq_cut;
  YCut * y_cut;
  WCut * w_cut; 

  // Member Functions
  void add_files(std::vector<std::string>, int);
  void init();
  void set_bins(DBins x, DBins qq){ xBins = x; qqBins = qq; }
  void loop(int); 
  void do_xs();
  void get_charge(std::vector<std::string>);
  void print_table(); 
  void fill_model();
  
  int find_qq_bin(double,double);
  int find_x_bin(double);
};


#endif