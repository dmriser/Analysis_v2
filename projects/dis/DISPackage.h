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
#include "DHistogramTools.h"
#include "h22Reader.h"
#include "NathanArchive.h"

//  root includes
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"

////////////////////////////////////////////////////////////////////////
/*

  DISHistograms 

*/
////////////////////////////////////////////////////////////////////////

class DISHistograms
{
 public:
  DISHistograms(std::string outputName, DBins xbins, DBins qqbins, DBins wbins);
  ~DISHistograms();

    // Data Types
  std::string output_name;
  std::string sect[7];
  
  DBins xBins, qqBins, wBins; 

  // Histograms
  std::vector<std::vector<TH1D*> > h1_gen_rad_x_by_qq;
  std::vector<std::vector<TH1D*> > h1_rec_rad_x_by_qq;

  std::vector<std::vector<TH1D*> > h1_gen_norad_x_by_qq;
  std::vector<std::vector<TH1D*> > h1_rec_norad_x_by_qq;

  std::vector<std::vector<TH1D*> > h1_hits_x_by_qq;
  std::vector<std::vector<TH1D*> > h1_acc_x_by_qq;
  std::vector<std::vector<TH1D*> > h1_rc_x_by_qq;
  std::vector<std::vector<TH1D*> > h1_rxs_x_by_qq;
  std::vector<std::vector<TH1D*> > h1_rxs_ratio_x_by_qq;
  std::vector<std::vector<TH1D*> > h1_xs_x_by_qq;
  std::vector<std::vector<TH1D*> > h1_xs_ratio_x_by_qq;
  std::vector<TH1D*>               h1_model_x_by_qq;
  std::vector<TH1D*>               h1_model_avg_x_by_qq;
  std::vector<TH1D*>               h1_bin_corr_x_by_qq;
  std::vector<std::vector<TH1D*> > h1_gen_rad_w_by_qq;
  std::vector<std::vector<TH1D*> > h1_rec_rad_w_by_qq;

  std::vector<std::vector<TH1D*> > h1_gen_norad_w_by_qq;
  std::vector<std::vector<TH1D*> > h1_rec_norad_w_by_qq;

  std::vector<std::vector<TH1D*> > h1_hits_w_by_qq;
  std::vector<std::vector<TH1D*> > h1_rc_w_by_qq;
  std::vector<std::vector<TH1D*> > h1_acc_w_by_qq;
  std::vector<std::vector<TH1D*> > h1_rxs_w_by_qq;
  std::vector<std::vector<TH1D*> > h1_rxs_ratio_w_by_qq;
  std::vector<std::vector<TH1D*> > h1_xs_w_by_qq;
  std::vector<std::vector<TH1D*> > h1_xs_ratio_w_by_qq;
  std::vector<TH1D*>               h1_model_w_by_qq;
  std::vector<TH1D*>               h1_model_avg_w_by_qq;
  std::vector<TH1D*>               h1_bin_corr_w_by_qq;
  
  // Easiest way to hold the charge. 
  TH1D * h1_fcup_charge;
  
  // 2-D Histos Used For Slicing 
  TH2F * h2_hits_x_qq[7];
  TH2F * h2_rec_rad_x_qq[7];
  TH2F * h2_gen_rad_x_qq[7];
  TH2F * h2_rec_norad_x_qq[7];
  TH2F * h2_gen_norad_x_qq[7];
  TH2F * h2_rec_gen_norad_x_qq[7];
  
  TH2F * h2_hits_w_qq[7];
  TH2F * h2_rec_rad_w_qq[7];
  TH2F * h2_gen_rad_w_qq[7];
  TH2F * h2_rec_norad_w_qq[7];
  TH2F * h2_gen_norad_w_qq[7];
  TH2F * h2_rec_gen_norad_w_qq[7];

  TH2F * h2_hits_w_qq_rebin[7];
  TH2F * h2_rec_rad_w_qq_rebin[7];
  TH2F * h2_gen_rad_w_qq_rebin[7];
  TH2F * h2_rec_norad_w_qq_rebin[7];
  TH2F * h2_gen_norad_w_qq_rebin[7];
  TH2F * h2_rec_gen_norad_w_qq_rebin[7];
  
  TH2F * h2_hits_x_qq_rebin[7];
  TH2F * h2_rec_rad_x_qq_rebin[7];
  TH2F * h2_gen_rad_x_qq_rebin[7];
  TH2F * h2_rec_norad_x_qq_rebin[7];
  TH2F * h2_gen_norad_x_qq_rebin[7];
  TH2F * h2_rec_gen_norad_x_qq_rebin[7];
  
  // Tree used for run info
  TTree * runInfoTree; 
  TFile * f;
  
  // Member Functions
  void draw();
  void fill(DEvent, int); /** Pass in the event and the type data or monte carlo */
  void fill_gen(DEvent event, int index);  /** Just pass in the gen_raderated event */
  void init();
  void load();
  void set_bins(DBins x, DBins qq, DBins w){ xBins = x; qqBins = qq; wBins = w; }
  void save();
  void close();  
};

////////////////////////////////////////////////////////////////////////
/*

  DISManager 

*/
////////////////////////////////////////////////////////////////////////

class DISManager
{
 public:
  DISManager(std::string outputFile, bool r, DBins x, DBins qq, DBins w);
  ~DISManager();

  // Data Types
  int eid_version;
  double fcup_charge; 
  bool rec_radalc; 
  std::string outfile;
  std::string infofile; 
  std::string momcorr_path;

  DBins xBins, qqBins, wBins; 
  DInformation info; 
  DISHistograms histos; 
  DSelection dis_selector; 
  h22Reader reader[3];
  NathanEIDWrapper nathan; 
  VirtualityCut * qq_cut;
  YCut          * y_cut;
  WCut          * w_cut; 

  // Member Functions
  void add_files(std::vector<std::string>, int);
  void init();
  void set_bins(DBins x, DBins qq, DBins w){ xBins = x; qqBins = qq; wBins = w; }
  void loop(int); 
  void do_xs();
  void get_charge(std::vector<std::string>);
  void fill_model();  
};

#endif
