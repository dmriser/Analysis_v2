//////////////////////////////////////////////
/*

  David Riser, University of Connecticut 
  August 6, 2016 

  ElasticPackage.h -> Define Class Structure 
  for: 
  
  > ElasticAnalysis 
  > ElasticBins  
  > ElasticEvent 
  > ElasticHistograms 
  > ElasticModel 


*/
//////////////////////////////////////////////

#ifndef elastic_package_h
#define elastic_package_h

//c++ includes 
#include <iostream>
#include <vector>

// my includes 
#include "DBins.h"
#include "DEvent.h"
#include "h22Event.h"
#include "h22Reader.h"

// root includes
#include "TH1.h"
#include "TH2.h"
#include "TLorentzVector.h"

/////////////////////////////////////////////
/*
  
  ElasticBins 

*/
/////////////////////////////////////////////

class ElasticBins
{
  // Default Constr. & Destr. 
 public:
  ElasticBins();
  ~ElasticBins();

  ElasticBins(DBins, DBins, DBins); /** Constructor that accepts your binning scheme  */
  ElasticBins(int, double, double,int, double, double,int, double, double); /** Constructor that acceps your binning parameters and then creates the binning schemes */
  
  // Datatypes 
 public:
  DBins thetaBins;
  DBins phiBins;
  DBins pBins;

  // Member Functions
  void set_theta_bins(DBins b){ thetaBins = b; }
  void set_phi_bins(DBins b){ phiBins = b; }
  void set_p_bins(DBins b){ pBins = b; }
  void set_all_bins(DBins t, DBins ph, DBins p){ thetaBins = t; phiBins = ph; pBins = p; } /** Setting bins by passing in bin structures (theta, phi, momentum)*/
  void set(int, double, double, int, double, double, int, double, double);
};

/////////////////////////////////////////////
/*
  
  ElasticEvent 

*/
/////////////////////////////////////////////

class ElasticEvent : public DEvent 
{
 public:
 ElasticEvent();
  ~ElasticEvent();

 ElasticEvent(TLorentzVector);
 ElasticEvent(TLorentzVector, TLorentzVector);  
  
  // Datatypes
 public:
 double W_Max;

 // Member Functions 
 bool passes();
 void set_w_max(double W){ W_Max = W;}
  
};

/////////////////////////////////////////////
/*
  
  ElasticHistograms 

*/
/////////////////////////////////////////////

class ElasticHistograms
{
 public:
  ElasticHistograms();
  ~ElasticHistograms();

  ElasticHistograms(ElasticBins); /** Pass in elastic binning scheme */
  
  // Datatypes 
 public:
  ElasticBins bins;
  DBins w_histo_bins;
  DBins qq_histo_bins;
  
  // Binned By Outside Variables
  /** The binning is done by ElasticBins, first vector is sectors and second is the binning scheme */
  std::vector<std::vector<TH1F*> > h1_hits_by_phi;
  std::vector<std::vector<TH1F*> > h1_rec_by_phi;
  std::vector<std::vector<TH1F*> > h1_gen_by_phi;
  std::vector<std::vector<TH1F*> > h1_hits_by_p;
  std::vector<std::vector<TH1F*> > h1_rec_by_p;
  std::vector<std::vector<TH1F*> > h1_gen_by_p;
  std::vector<std::vector<TH1F*> > h1_acc_by_phi;
  std::vector<std::vector<TH1F*> > h1_acc_by_p;
  std::vector<std::vector<TH1F*> > h1_xs_by_phi;
  std::vector<std::vector<TH1F*> > h1_xs_by_p; 
  std::vector<std::vector<TH1F*> > h1_xs_ratio_by_phi;
  std::vector<std::vector<TH1F*> > h1_xs_ratio_by_p; 
  std::vector<std::vector<TH1F*> > h1_w_by_p;
  std::vector<std::vector<TH1F*> > h1_qq_by_p;
    
  // Methods
  void close(std::string); /** Save histograms to file */
  void draw();
  void dump(std::string); /** Dumping all histograms into .pdf file specified by name */
  void fill(int, double, h22Event, ElasticEvent); /** Filling histograms requires electron index, the event, and the elastic event */
  void init();
  void set_errors();
  void set_bins(ElasticBins);
  void set_histo_bins(DBins, DBins); /** Pass in w histo bins and qq histo bins */
};

/////////////////////////////////////////////
/*
  
  ElasticAnalysis 

*/
/////////////////////////////////////////////

class ElasticAnalysis
{
 public:
  ElasticAnalysis();
  ~ElasticAnalysis();

  ElasticAnalysis(ElasticBins);
  
  // Datatypes
  ElasticBins bins; 
  ElasticHistograms histos; 
  h22Reader * fReader[2]; /** Data & Monte Carlo */

  // Member Functions
  void add_files(int, std::vector<std::string>);
  void close(std::string);
  void run();
  void init();
  void set_bins(ElasticBins);
};

#endif
