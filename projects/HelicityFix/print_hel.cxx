#include <iostream>

#include "TChain.h"
#include "TTree.h"
#include "TFile.h"
#include "TBranch.h"

class h10Reader {
public:
  h10Reader() {
    chain = new TChain("h10"); 
    SetupBranches(); 
  }

  ~h10Reader(){
    chain->Delete(); 
  }

  TChain *chain; 
  Int_t  event; 
  Char_t  hel; 
  TBranch *b_event; 
  TBranch *b_hel; 

  void SetupBranches() {
    chain->SetBranchAddress("evntid", &event, &b_event);
    chain->SetBranchAddress("evthel", &hel, &b_hel);
 }

  void AddFile(std::string f){
    chain->AddFile(f.c_str()); 
  }

  int GetEntries() const {
    return chain->GetEntries(); 
  }

  void GetEntry(int i) {
    chain->GetEntry(i);
  }

};

class h22Reader {
public:
  h22Reader() {
    chain = new TChain("h22"); 
    SetupBranches(); 
  }

  ~h22Reader(){
    chain->Delete(); 
  }

  TChain *chain; 
  Int_t  event; 
  Char_t  hel; 

  TBranch *b_event; 
  TBranch *b_hel; 

  void SetupBranches() {
    chain->SetBranchAddress("evntid", &event, &b_event);
    chain->SetBranchAddress("ihel", &hel, &b_hel);
 }

  void AddFile(std::string f){
    chain->AddFile(f.c_str()); 
  }

  int GetEntries() const {
    return chain->GetEntries(); 
  }

  void GetEntry(int i) {
    chain->GetEntry(i);
  }

};

int main(int argc, char *argv[]){

  if(argc < 4){
    std::cout << "./print FILE1(h22) FILE2(h10) OUTFILE" << std::endl; 
    return 0;
  }

  std::string file_h22(argv[1]);
  std::string file_h10(argv[2]);
  std::string outfile(argv[3]); 

  std::cout << " Running for " << file_h22 << " and " << file_h10 << std::endl; 

  h10Reader reader10;  
  reader10.AddFile(file_h10); 

  h22Reader reader22;  
  reader22.AddFile(file_h22); 

  // new stuff 
  TFile * newNtuple = new TFile(outfile.c_str(),"recreate");
  TChain * newChain = (TChain*) reader22.chain->CloneTree(0);
  TTree * newTree   = newChain->GetTree();
  
  Char_t corrHel;
  TBranch *b_corrHel; 

  b_corrHel = newTree->Branch("corrHel", &corrHel);

  int currentEvent10 = 0; 
  int currentEvent22 = 0; 
  int skippedEvent10 = 0; 
  int skippedEvent22 = 0; 
  while (currentEvent10 < reader10.GetEntries() && currentEvent22 < reader22.GetEntries()){
    reader10.GetEntry(currentEvent10); 
    reader22.GetEntry(currentEvent22); 

    if (reader10.event == reader22.event){
      std::cout.width(10); std::cout << reader10.event; 
      std::cout.width(10); std::cout << (int) reader10.hel; 
      std::cout.width(10); std::cout << reader22.event; 
      std::cout.width(10); std::cout << (int) reader22.hel << std::endl; 

      corrHel = reader10.hel; 
      newTree->Fill(); 

      currentEvent10++; 
      currentEvent22++; 
    } else if (reader10.event > reader22.event){
      currentEvent22++; 
      skippedEvent22++; 
    } 
    else if (reader10.event < reader22.event){
      currentEvent10++; 
      skippedEvent10++; 
    }
  }

  std::cout << "Skipped events 10: " << skippedEvent10 << std::endl; 
  std::cout << "Skipped events 22: " << skippedEvent22 << std::endl; 
  std::cout << "Events 10: " << reader10.GetEntries() << std::endl; 
  std::cout << "Events 22: " << reader22.GetEntries() << std::endl; 

  newTree->Write(); 
  newNtuple->Close(); 

  return 0;
}
