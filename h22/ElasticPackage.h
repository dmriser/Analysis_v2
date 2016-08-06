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

// my includes 
#include "DBins.h"
#include "DEvent.h"

// root includes
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

#endif
