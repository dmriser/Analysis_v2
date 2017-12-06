#ifndef simple_ntuple_writer_cxx 
#define simple_ntuple_writer_cxx 

#include "SimpleNTupleWriter.h"

// c++ standard 
#include <iostream>
#include <map>

// root 
#include "TBranch.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TTree.h"

SimpleNTupleWriter::SimpleNTupleWriter(){
  fTree = new TTree("events", "events"); 
}

SimpleNTupleWriter::SimpleNTupleWriter(std::string treeName){
  fTree = new TTree(treeName.c_str(), treeName.c_str());
}

SimpleNTupleWriter::~SimpleNTupleWriter(){
  
}

void SimpleNTupleWriter::addInt(std::string key){
  fInts[key]     = 0; 
  fBranches[key] = fTree->Branch(key.c_str(), &fInts[key]);
}

void SimpleNTupleWriter::addFloat(std::string key){
  fFloats[key]     = 0.0; 
  fBranches[key] = fTree->Branch(key.c_str(), &fFloats[key]);
}

void SimpleNTupleWriter::addDouble(std::string key){
  fDoubles[key]  = 0.0; 
  fBranches[key] = fTree->Branch(key.c_str(), &fDoubles[key]);
}

void SimpleNTupleWriter::addTLorentzVector(std::string key){
  fTLorentzVectors[key] = TLorentzVector(0, 0, 0, 0); 
  fBranches[key]       = fTree->Branch(key.c_str(), &fTLorentzVectors[key]);
}

void SimpleNTupleWriter::setInt(std::string key, int value){
  fInts[key] = value; 
}

void SimpleNTupleWriter::setFloat(std::string key, float value){
  fFloats[key] = value; 
}

void SimpleNTupleWriter::setDouble(std::string key, double value){
  fDoubles[key] = value; 
}

void SimpleNTupleWriter::setTLorentzVector(std::string key, TLorentzVector & value){
  fTLorentzVectors[key] = value; 
}

void SimpleNTupleWriter::writeEvent(){
  fTree->Fill(); 
}
 
void SimpleNTupleWriter::save(std::string file){
  TFile *out = new TFile(file.c_str(), "recreate");
  fTree->Write(); 
  out->Close();
}

void SimpleNTupleWriter::save(TFile *out){
  if(out && out->IsOpen()){
    fTree->Write(); 
  }
}

#endif
