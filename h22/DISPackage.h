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
#include "DSelection.h"
#include "ELectronIDPackage.h"
#include "NathanArchive.h"

//  root includes
#include "TH1.h"

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
  
  DBins xBins; DBins qqBins; DBins wBins; 

  // Histograms
  std::vector<std::vector<TH1F*> > h1_gen_x_by_qq;
  std::vector<std::vector<TH1F*> > h1_rec_x_by_qq;
  std::vector<std::vector<TH1F*> > h1_hits_x_by_qq;
  std::vector<std::vector<TH1F*> > h1_acc_x_by_qq;
  std::vector<std::vector<TH1F*> > h1_rxs_x_by_qq;
  std::vector<std::vector<TH1F*> > h1_xs_x_by_qq;
  
  std::vector<std::vector<TH1F*> > h1_gen_x_by_w;
  std::vector<std::vector<TH1F*> > h1_rec_x_by_w;
  std::vector<std::vector<TH1F*> > h1_hits_x_by_w;
  std::vector<std::vector<TH1F*> > h1_acc_x_by_w;
  std::vector<std::vector<TH1F*> > h1_rxs_x_by_w;
  std::vector<std::vector<TH1F*> > h1_xs_x_by_w;

  std::vector<std::vector<TH1F*> > h1_gen_qq_by_x;
  std::vector<std::vector<TH1F*> > h1_rec_qq_by_x;
  std::vector<std::vector<TH1F*> > h1_hits_qq_by_x;
  std::vector<std::vector<TH1F*> > h1_acc_qq_by_x;
  std::vector<std::vector<TH1F*> > h1_rxs_qq_by_x;
  std::vector<std::vector<TH1F*> > h1_xs_qq_by_x;

  std::vector<std::vector<TH1F*> > h1_gen_qq_by_w;
  std::vector<std::vector<TH1F*> > h1_rec_qq_by_w;
  std::vector<std::vector<TH1F*> > h1_hits_qq_by_w;
  std::vector<std::vector<TH1F*> > h1_acc_qq_by_w;
  std::vector<std::vector<TH1F*> > h1_rxs_qq_by_w;
  std::vector<std::vector<TH1F*> > h1_xs_qq_by_w;
  
  // Member Functions
  void fill(DEvent, int); /** Pass in the event and the type data or monte carlo */
  void fill_gen(DEvent); /** Just pass in the generated event */
  void init();
  void set_bins(DBins x, DBins qq, DBins w){ xBins = x; qqBins = qq; wBins = w; }
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
  std::string parfile[2]; 
  std::string outfile;
  
  DBins xBins, qqBins, wBins; 
  DISHistograms histos; 
  DSelection dis_selector; 
  ElectronSelector eid[2];
  h22Reader reader[2];
  NathanEIDWrapper nathan; 
  VirtualityCut * qq_cut;
  WCut * w_cut; 

  // Member Functions
  void add_files(std::vector<std::string>, int);
  void init();
  void set_bins(DBins x, DBins qq, DBins w){ xBins = x; qqBins = qq; wBins = w; }
  void loop(int); 
  
};


#endif
