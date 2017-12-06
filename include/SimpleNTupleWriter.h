#ifndef simple_ntuple_writer_h 
#define simple_ntuple_writer_h 

// c++ standard libraries 
#include <iostream>
#include <map>

// root 
#include "TBranch.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TTree.h"

class SimpleNTupleWriter {

 public:
  SimpleNTupleWriter();
  SimpleNTupleWriter(std::string treeName);
  ~SimpleNTupleWriter();

  // tree building functions 
  void addInt(std::string key);
  void addFloat(std::string key);
  void addDouble(std::string key);
  void addTLorentzVector(std::string key);

  // set values
  void setInt(std::string key, int value);
  void setFloat(std::string key, float value);
  void setDouble(std::string key, double value);
  void setTLorentzVector(std::string key, TLorentzVector &value);

  void save(std::string file);
  void save(TFile * file);
  void writeEvent();

 protected:
  // tree for the ntuple 
  TTree *fTree; 
  
  // container for the simple types 
  // and branches
  std::map<std::string, TBranch*>       fBranches; 

  // types 
  std::map<std::string, int>            fInts; 
  std::map<std::string, float>          fFloats; 
  std::map<std::string, double>         fDoubles; 
  std::map<std::string, TLorentzVector> fTLorentzVectors; 
};

#endif
