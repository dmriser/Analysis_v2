
#ifndef base_dis_histograms_h
#define base_dis_histograms_h

// c++ 
#include <iostream>
using namespace std; 

// me 
#include "CommonTools.h"
#include "PhysicsEvent.h"

// root 
#include "TFile.h"
#include "THnSparse.h"

#define NDIM 4
 
class BaseDISHistograms{
 public:
  BaseDISHistograms();
  ~BaseDISHistograms();

  string baseName; 
  
  TFile      *inputFile; 
  THnSparseF *events; 

  void Init(std::string name, std::string title);
  void InitWithNewNumberOfBins(std::string name, std::string title, int xBins, int yBins);
  void Load(std::string filename, std::string name); 
  void Save(std::string filename, std::string option);
  void Fill(PhysicsEvent event, int sector); 
};

BaseDISHistograms::BaseDISHistograms(){
  inputFile = new TFile();
}

BaseDISHistograms::~BaseDISHistograms(){
  if (inputFile->IsOpen()){
    inputFile->Close();
  }
}

void BaseDISHistograms::Init(string name, string title){
  baseName = name; 

  // Sparse that holds everything 
  // For THnSparseI Array 
  // 4-Dimensional Binning 
  // 0:sect 
  // 1:phi_rel 
  // 2:q2 
  // 3:w
  int numberBins[NDIM] = {  6,   5,  10,  40};
  double min[NDIM]     = {1.0, -20, 0.5, 1.1};
  double max[NDIM]     = {7.0,  20, 4.8, 3.0};
  events               = new THnSparseF(Form("%s_events",name.c_str()),Form("THnSparseI Events for %s",title.c_str()),NDIM,numberBins,min,max);
 
  // Unused but has to be here for the event that 
  // class is instantiated in loading mode. 
  inputFile = new TFile(); 
}

void BaseDISHistograms::InitWithNewNumberOfBins(string name, string title, int xBins, int yBins){
  baseName = name; 

  // Unused but has to be here for the event that 
  // class is instantiated in loading mode. 
  inputFile = new TFile(); 
}

void BaseDISHistograms::Save(string filename, string option){

  TFile * file = TFile::Open(filename.c_str(),option.c_str());

  if (file->IsOpen()){
    events->Write();
  } else {
    cout << "[BaseDISHistograms::Save] Error opening file... " << filename << endl; 
  }

  file->Write();
  file->Close();
}

void BaseDISHistograms::Load(string filename, string name){

  inputFile = TFile::Open(filename.c_str());
  baseName = name; 

  if (inputFile->IsOpen()){
    events = (THnSparseF*) inputFile->Get(Form("%s_events",name.c_str()));
  } else {
    cout << "[BaseDISHistograms::Load] Error opening file... " << filename << endl; 
  }

  //  file->Close();
   
}

void BaseDISHistograms::Fill(PhysicsEvent event, int sector){
  // We assume that nobody sends us sector > 6 or sector < 1
  double relativePhi     = getRelativePhi(to_degrees*event.detectedElectron.Phi());
  double dataPoint[NDIM] = {(double)sector, relativePhi, event.qq, event.w};
  events->Fill(dataPoint);
}

#endif
