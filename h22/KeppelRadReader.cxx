#ifndef KeppelRadReader_cxx
#define KeppelRadReader_cxx

#include "KeppelRadReader.h"

#include <iostream>
using std::cout; 
using std::endl;
using std::flush;
 
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include "TString.h"
#include "TStopwatch.h"

KeppelRadReader::KeppelRadReader() {
  fChain = new TChain("h10");
}

KeppelRadReader::~KeppelRadReader(){
}

Long64_t KeppelRadReader::GetEntries(){
  return fChain->GetEntries();
}

Int_t KeppelRadReader::GetEntry(Long64_t entry){
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}

void KeppelRadReader::AddFile(TString file){
  fChain->AddFile(file);
  cout << "[KeppelRadReader] Added file " << file << " to the chain. " << endl;
}

void KeppelRadReader::Init(){
   fChain->SetBranchAddress("es", &es, &b_es);
   fChain->SetBranchAddress("ep", &ep, &b_ep);
   fChain->SetBranchAddress("thete", &thete, &b_thete);
   fChain->SetBranchAddress("w", &w, &b_w);
   fChain->SetBranchAddress("wreal", &wreal, &b_wreal);
   fChain->SetBranchAddress("eg", &eg, &b_eg);
   fChain->SetBranchAddress("csthk", &csthk, &b_csthk);
   fChain->SetBranchAddress("phik", &phik, &b_phik);
   fChain->SetBranchAddress("qx", &qx, &b_qx);
   fChain->SetBranchAddress("qz", &qz, &b_qz);
   fChain->SetBranchAddress("q0", &q0, &b_q0);
   fChain->SetBranchAddress("csthe", &csthe, &b_csthe);
   fChain->SetBranchAddress("egx", &egx, &b_egx);
   fChain->SetBranchAddress("egy", &egy, &b_egy);
   fChain->SetBranchAddress("egz", &egz, &b_egz);
}

void KeppelRadReader::Loop(int numberOfEvents){
  if(GetEntries() < numberOfEvents){ numberOfEvents = GetEntries(); }
  cout << "[KeppelRadReader::Loop] Starting event loop for " << numberOfEvents << " events. " << endl; 

  TStopwatch *timer = new TStopwatch();
  timer->Reset();
  timer->Start();

  for(int ievent=0; ievent<numberOfEvents; ++ievent){
    GetEntry(ievent);
    ProcessEvent();
    if (ievent%1000 == 0){ cout << "\r[KeppelRadReader::Loop] Done " << ievent << flush; }
  }

  double loopTime  = timer->RealTime();
  double eventRate = numberOfEvents/loopTime;
  cout << endl;
  cout << "[KeppelRadReader::Loop] Finished in " << loopTime << " seconds with rate " << eventRate << " events/sec. " << endl;

}

void KeppelRadReader::ProcessEvent(){
  cout << "[KeppelRadReader] Processing event number: "; 
  cout.width(12); cout << fCurrent << endl;
}

void KeppelRadReader::Execute(int numberOfEvents){
  Init();
  Loop(numberOfEvents);
}

#endif 
