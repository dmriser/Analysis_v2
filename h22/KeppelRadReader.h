#ifndef KeppelRadReader_h
#define KeppelRadReader_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include "TString.h"

class KeppelRadReader {
public :
   TChain         *fChain;   
   Int_t           fCurrent; 

   Float_t         es;
   Float_t         ep;
   Float_t         thete;
   Float_t         w;
   Float_t         wreal;
   Float_t         eg;
   Float_t         csthk;
   Float_t         phik;
   Float_t         qx;
   Float_t         qz;
   Float_t         q0;
   Float_t         csthe;
   Float_t         egx;
   Float_t         egy;
   Float_t         egz;

   TBranch        *b_es;   
   TBranch        *b_ep;   
   TBranch        *b_thete;
   TBranch        *b_w;   
   TBranch        *b_wreal;
   TBranch        *b_eg;   
   TBranch        *b_csthk;
   TBranch        *b_phik; 
   TBranch        *b_qx;  
   TBranch        *b_qz;  
   TBranch        *b_q0;  
   TBranch        *b_csthe;
   TBranch        *b_egx;  
   TBranch        *b_egy;  
   TBranch        *b_egz;  

   KeppelRadReader();
   virtual ~KeppelRadReader();
   virtual Int_t    GetEntry(Long64_t entry);
   virtual void     Init();
   virtual void     Loop(int numberOfEvents);
   virtual void     ProcessEvent();
   virtual void     Execute(int numberOfEvents);

   void AddFile(TString file);
   void AddFile(std::string file);
   Long64_t GetEntries();
};

#endif
