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
  thetaBins = DBins(nx, xmin, xmax);
  phiBins   = DBins(ny, ymin, ymax);
  pBins     = DBins(nz, zmin, zmax);
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
  w_histo_bins  = DBins(100,0.8,1.3);
  qq_histo_bins = DBins(100,0.0,5.5); 
}

ElasticHistograms::~ElasticHistograms()
{
  // Nothing to do. 
}

void ElasticHistograms::close(string)
{
  // Create, write, and close TFile for root output. 
}

void ElasticHistograms::set_histo_bins(DBins w, DBins qq)
{
  w_histo_bins  = w;
  qq_histo_bins = qq;
}

void ElasticHistograms::init()
{
  // Initialize the histograms we defined in the ElasticPackage.h ElasticHistogram section.
  string sect[7] = {"all","s1","s2","s3","s4","s5","s6"};

  // w for pBins
  for (int s=0; s<7; s++) {
    vector<TH1F*> v;
    for (int b=0; b<(bins.pBins.number()+1); b++)
      {
	string name  = Form("h1_w_by_p_%d_%s",b,sect[s].c_str());
	string title = Form("W for bin %d of %s",b,sect[s].c_str());
	v.push_back( new TH1F(name.c_str(), title.c_str(), w_histo_bins.number(), w_histo_bins.min(), w_histo_bins.max()) );
      }
    h1_w_by_p.push_back(v);
  }

  // Q^2 for pBins
  for (int s=0; s<7; s++) {
    vector<TH1F*> v;
    for (int b=0; b<(bins.pBins.number()+1); b++)
      {
	string name  = Form("h1_qq_by_p_%d_%s",b,sect[s].c_str());
	string title = Form("Q^2 for bin %d of %s",b,sect[s].c_str());
	v.push_back( new TH1F(name.c_str(), title.c_str(), qq_histo_bins.number(), qq_histo_bins.min(), qq_histo_bins.max()) );
      }
    h1_qq_by_p.push_back(v);
  }

  // hits for phi bins 
  for (int s=0; s<7; s++) {
    vector<TH1F*> v;
    for (int b=0; b<(bins.pBins.number()+1); b++)
      {
	string name  = Form("h1_hits_by_phi_%d_%s",b,sect[s].c_str());
	string title = Form("Hits for bin %d of %s",b,sect[s].c_str());
	v.push_back( new TH1F(name.c_str(), title.c_str(), bins.thetaBins.number(), bins.thetaBins.min(), bins.thetaBins.max()) );
      }
    h1_hits_by_phi.push_back(v);
  }

  // reconstructed hits for phi bins 
  for (int s=0; s<7; s++) {
    vector<TH1F*> v;
    for (int b=0; b<(bins.pBins.number()+1); b++)
      {
	string name  = Form("h1_rec_by_phi_%d_%s",b,sect[s].c_str());
	string title = Form("Reconstructed hits for bin %d of %s",b,sect[s].c_str());
	v.push_back( new TH1F(name.c_str(), title.c_str(), bins.thetaBins.number(), bins.thetaBins.min(), bins.thetaBins.max()) );
      }
    h1_rec_by_phi.push_back(v);
  }

  // gen for phi bins 
  for (int s=0; s<7; s++) {
    vector<TH1F*> v;
    for (int b=0; b<(bins.pBins.number()+1); b++)
      {
	string name  = Form("h1_gen_by_phi_%d_%s",b,sect[s].c_str());
	string title = Form("Generated for bin %d of %s",b,sect[s].c_str());
	v.push_back( new TH1F(name.c_str(), title.c_str(), bins.thetaBins.number(), bins.thetaBins.min(), bins.thetaBins.max()) );
      }
    h1_gen_by_phi.push_back(v);
  }


  // hits for p bins 
  for (int s=0; s<7; s++) {
    vector<TH1F*> v;
    for (int b=0; b<(bins.pBins.number()+1); b++)
      {
	string name  = Form("h1_hits_by_p_%d_%s",b,sect[s].c_str());
	string title = Form("Hits for bin %d of %s",b,sect[s].c_str());
	v.push_back( new TH1F(name.c_str(), title.c_str(), bins.thetaBins.number(), bins.thetaBins.min(), bins.thetaBins.max()) );
      }
    h1_hits_by_p.push_back(v);
  }

  // reconstructed hits for phi bins 
  for (int s=0; s<7; s++) {
    vector<TH1F*> v;
    for (int b=0; b<(bins.pBins.number()+1); b++)
      {
	string name  = Form("h1_rec_by_p_%d_%s",b,sect[s].c_str());
	string title = Form("Reconstructed hits for bin %d of %s",b,sect[s].c_str());
	v.push_back( new TH1F(name.c_str(), title.c_str(), bins.thetaBins.number(), bins.thetaBins.min(), bins.thetaBins.max()) );
      }
    h1_rec_by_p.push_back(v);
  }

  // gen for phi bins 
  for (int s=0; s<7; s++) {
    vector<TH1F*> v;
    for (int b=0; b<(bins.pBins.number()+1); b++)
      {
	string name  = Form("h1_gen_by_p_%d_%s",b,sect[s].c_str());
	string title = Form("Generated for bin %d of %s",b,sect[s].c_str());
	v.push_back( new TH1F(name.c_str(), title.c_str(), bins.thetaBins.number(), bins.thetaBins.min(), bins.thetaBins.max()) );
      }
    h1_gen_by_p.push_back(v);
  }
  
}

void ElasticHistograms::fill(int index, double w_cut, h22Event event, ElasticEvent elasticEvent)
{
  // Fill all histograms
  if (index == 0 && elasticEvent.W < w_cut)
    {
      int s = event.dc_sect[index];
    
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

ElasticAnalysis::ElasticAnalysis(ElasticBins b)
{
  bins = b;
}

void ElasticAnalysis::set_bins(ElasticBins b)
{
  bins = b;
}

void ElasticAnalysis::add_files(int index, vector<string> files)
{
  for (int i=0; i<files.size(); i++) fReader[index]->AddFile(files[i]);
}


/////////////////////////////////////////////////////////
/*
  
  ElasticModel

*/
/////////////////////////////////////////////////////////

/*
extern"C"{
  float elas_(float*,float*);
  float elasrad_(float*,float*,float*,float*);
}

ElasticModel::ElasticModel()
{
  // Nothing to do here.
}

ElasticModel::~ElasticModel()
{
  // Nothing to do here.
}

void ElasticModel::set_beam_energy(double temp_energy)
{
  beam_energy = (float) temp_energy;
}

void ElasticModel::set_target_frac_length(double temp_length)
{
  target_frac_length = (float) temp_length;
}

void ElasticModel::set_w_cut(double temp_cut)
{
  w_cut = (float) temp_cut;
}

double ElasticModel::xs(double theta)
{
  float fTheta  = (float) theta;
  double xs = elas_(&beam_energy, &fTheta);
  return xs;
}

double ElasticModel::rad_xs(double theta)
{
  float fTheta  = (float) theta;
  double xs = elasrad_(&beam_energy, &fTheta, &target_frac_length, &w_cut);
  return xs;
}
*/

#endif
