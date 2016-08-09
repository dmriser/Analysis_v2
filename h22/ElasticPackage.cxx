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

#ifndef elastic_package_cxx
#define elastic_package_cxx

//c++ includes 
#include <iostream>
using namespace std;

// my includes 
#include "DBins.h"
#include "DEvent.h"
#include "ElasticPackage.h"
#include "h22Event.h"

/////////////////////////////////////////////////////////
/*
  
  ElasticBins

*/
/////////////////////////////////////////////////////////

ElasticBins::ElasticBins()
{
  // Creating Bin Scheme. 
}

ElasticBins::~ElasticBins()
{
  // Destruir todo.
}

ElasticBins::ElasticBins(DBins theta, DBins phi, DBins mom)
{
  thetaBins = theta; phiBins = phi; pBins = mom; 
}

ElasticBins::ElasticBins(int nx, double xmin, double xmax, int ny, double ymin, double ymax, int nz, double zmin, double zmax)
{
  thetaBins = new DBins(nx, xmin, xmax);
  phiBins   = new DBins(ny, ymin, ymax);
  pBins     = new DBins(nz, zmin, zmax);
}

void ElasticBins::set(int nx, double xmin, double xmax, int ny, double ymin, double ymax, int nz, double zmin, double zmax)
{
  thetaBins.set_number(nx); thetaBins.set_min(xmin); thetaBins.set_max(xmax); thetaBins.refresh();
  phiBins.set_number(ny); phiBins.set_min(ymin); phiBins.set_max(ymax); phiBins.refresh();
  pBins.set_number(nz); pBins.set_min(zmin); pBins.set_max(zmax); pBins.refresh();
}

/////////////////////////////////////////////////////////
/*
  
  ElasticEvent

*/
/////////////////////////////////////////////////////////

ElasticEvent::ElasticEvent()
{
  // Not sure what needs to be done here. 
}

ElasticEvent::~ElasticEvent()
{
  // Not sure what needs to be done here. 
}

ElasticEvent::ElasticEvent(TLorentzVector x) : DEvent(x){
  // Nothing to do here 
}

ElasticEvent::ElasticEvent(TLorentzVector kPrime, TLorentzVector pPrime)
{
  // We need to manually calculate the kinematics for the case of final state electron and proton given.
  QQ  = -1*(beam-kPrime).Mag2();
  W   = ((beam-kPrime) + target).Mag();
  
  TLorentzVector missing = (beam+target) - (kPrime+pPrime);
  MM2 = missing.E()*missing.E() - missing.P()*missing.P();
}

bool ElasticEvent::passes()
{
  if (W < W_Max) return true;

  return false;
}

/////////////////////////////////////////////////////////
/*
  
  ElasticHistograms

*/
/////////////////////////////////////////////////////////

ElasticHistograms::ElasticHistograms()
{
  // Nothing to do. 
}

ElasticHistograms::~ElasticHistograms()
{
  // Nothing to do. 
}

void ElasticHistograms::close(string)
{
  // Create, write, and close TFile for root output. 
}

void ElasticHistograms::init()
{
  // Initialize the histograms we defined in the ElasticPackage.h ElasticHistogram section.
  string sect[7] = {"all","s1","s2","s3","s4","s5","s6"};
  string type[2] = {"data","gsim"};

  for (int t=0; t<2; t++)
    for (int s=0; s<7; s++)
      {
	h1_w[t][s]   = new TH1F(Form("h1_w_%s_%s",type[t].c_str(),sect[s].c_str()),Form(" w for %s %s ",type[t].c_str(),sect[s].c_str()),100,0.7,1.2);
	//	h2_rec[t][s] = new TH2F(Form(),Form(),phiBins.number(),phiBins.min(),phiBins.max(),thetaBins.number(),thetaBins.min(),thetaBins.max());
      }

  /*
  // Initialize histograms which depend on external binning parameters 
  for (int b=0; b<pBins.number(); b++)
    {
      TH1F * h = new TH1F();
      h1_w_by_p.push_back(h);
    }
  */
  
}

void ElasticHistograms::fill(int index, h22Event event, ElasticEvent elasticEvent)
{
  // Fill all histograms
  if (1)
    {
      int s = event.dc_sect[index];

      h1_w[index][0]->Fill(elasticEvent.W);
      h1_w[index][s]->Fill(elasticEvent.W);
    
    }
}

void ElasticHistograms::set_bins(ElasticBins this_junk)
{
  bins = this_junk;
}

/////////////////////////////////////////////////////////
/*
  
  ElasticAnalysis

*/
/////////////////////////////////////////////////////////

ElasticAnalysis::ElasticAnalysis()
{
  // To do 
}

ElasticAnalysis::~ElasticAnalysis()
{
  // To do 
}

void ElasticAnalysis::add_files(int index, vector<string> files)
{
  for (int i=0; i<files.size(); i++) fReader[index]->AddFile(files[i]);
}

#endif
