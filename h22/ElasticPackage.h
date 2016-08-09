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

  ElasticHistograms(ElasticBins);
  
  // Datatypes 
 public:
  ElasticBins bins;

  // 1-D 
  TH1F * h1_w[2][7]; /** Final State Invariant Mass (7 Spaces for all hits and for Sectors 1-6) */
  TH1F * h1_xs[7];
  TH1F * h1_xs_ratio[7];
  TH1F * h1_xs_model[2];

  // Binned By Outside Variables
  std::vector<TH1F*> h1_w_by_p;
  std::vector<TH1F*> h1_xs_by_phi;
  std::vector<TH1F*> h1_xs_by_p; 
  std::vector<TH1F*> h1_xs_ratio_by_phi;
  std::vector<TH1F*> h1_xs_ratio_by_p; 
  
  // 2-D
  TH2F * h2_acc[7];
  TH2F * h2_rec[2][7];
  TH2F * h2_gen[7];
  
  // Methods
  void close(std::string); /** Save histograms to file */
  void draw();
  void dump(std::string); /** Dumping all histograms into .pdf file specified by name */
  void fill(int, h22Event, ElasticEvent); /** Filling histograms requires electron index, the event, and the elastic event */
  void init();
  void set_errors();
  void set_bins(ElasticBins);

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

  // Datatypes
  ElasticBins bins; 
  ElasticHistograms histos; 
  h22Reader * fReader[2]; /** Data & Monte Carlo */

  // Member Functions
  void add_files(int, std::vector<std::string>);
  void close(std::string);
  void run();
  void init();
  
};

#endif
