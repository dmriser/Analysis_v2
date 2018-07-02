/////////////////////////////////////////////
/*

  David Riser, University of Connecticut 
  Standard Introductory Text Goes Here. 

*/
/////////////////////////////////////////////


#ifndef h22Reader_cxx
#define h22Reader_cxx

// c++ includes 
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdio.h>

// my includes 
#include "Corrections.h"
#include "h22Event.h"
#include "h22Reader.h"

// root includes 
#include <TROOT.h>
#include <TList.h>
#include <TObjArray.h>
#include <TString.h>
#include <TChain.h>
#include <TFile.h>
#include <TRegexp.h>

h22Reader::h22Reader(){
  GSIM           = -1;
  fchain         = NULL; 
  fInitStage     = init_stages::fUninitialized; 
}

h22Reader::h22Reader(int mc) {
  GSIM           = mc;
  fchain         = NULL;
  fInitStage     = init_stages::fUninitialized; 
}

h22Reader::~h22Reader(){
  fchain->Delete();
}

void h22Reader::AddFile(std::string _fname){
    
  if(!fchain){
    // we're trying to figure out if the file has 
    // a tree called h10 or h22 or neither 
    TFile *testFile = TFile::Open(_fname.c_str()); 
    TList *keys     = testFile->GetListOfKeys(); 
    TObject *obj    = keys->First(); 

    std::string name(obj->GetName()); 

    if(name == "h10"){
      fTreeType = file_types::h10; 
      fchain = new TChain("h10"); 
    } else if (name == "h22"){
      fTreeType = file_types::h22; 
      fchain = new TChain("h22"); 
    } else {
      std::cerr << "[h22Reader::AddFile] Trouble getting the tree structure from file, dying on " << _fname << std::endl; 
      exit(0); 
    }
    
    testFile->Close(); 



    std::cout << "[h22Reader::AddFile] Setting up for first file, type is " << fTreeType << std::endl; 

    fInitStage = init_stages::fHasChain; 
  }

  if(fchain){
    fchain->AddFile(_fname.c_str());
  } 

}


void h22Reader::SetupMC(){
  if (fInitStage >= init_stages::fHasChain){

    GSIM = false; 
    
    TObjArray *branches = fchain->GetListOfBranches(); 
    for(int i=0; i<branches->GetEntries(); i++){
      std::string branchName(branches->At(i)->GetName()); 

      std::cout << "[h22Reader::SetupMC] Searching branch... " << branchName << std::endl; 

      if (branchName == "mcid"){
	std::cout << "[h22Reader::SetupMC] Found mcid branch! " << std::endl; 
	GSIM = true; 
      }
    }
    
    std::cout << "[h22Reader::SetupMC] GSIM status = " << GSIM << std::endl;
    fInitStage = init_stages::fHasMCStatus; 

  } else {
    std::cerr << "[h22Reader::SetupMC] Initialization of class not sufficient to setup MC, stage = " << fInitStage << std::endl; 
    exit(0); 
  }
}

/**< Init() must be run once to link the branches of the TChain to the h22Event class members*/
void h22Reader::Init(){

  if ( GetEntries() > 0 && fInitStage >= init_stages::fHasChain) {
    if ( GetRunNumber() > 10000 && GetRunNumber() < 39000 )      GSIM = 0;
    else if ( GetRunNumber() > 50000 && GetRunNumber() < 60000 ) GSIM = 0;
    else                                                         GSIM = 1;
  } else { 
    std::cerr << "[h22Reader::Init] Dying for not having files before calling Init(), Entries =" << GetEntries() << std::endl; 
    exit(0);
  }

  SetupMC(); 
  
  // Set branch addresses and branch pointers
   fchain->SetBranchAddress("evntid", &event.evntid, &b_evntid);
   fchain->SetBranchAddress("q_l", &event.q_l, &b_q_l);
   fchain->SetBranchAddress("gpart", &event.gpart, &b_gpart);
   fchain->SetBranchAddress("q", event.q, &b_q);
   fchain->SetBranchAddress("p", event.p, &b_p);
   fchain->SetBranchAddress("b", event.b, &b_b);
   fchain->SetBranchAddress("id", event.id, &b_id);
   fchain->SetBranchAddress("cx", event.cx, &b_cx);
   fchain->SetBranchAddress("cy", event.cy, &b_cy);
   fchain->SetBranchAddress("cz", event.cz, &b_cz);
   fchain->SetBranchAddress("vz", event.vz, &b_vz);
   fchain->SetBranchAddress("dc_sect", event.dc_sect, &b_dc_sect);
   fchain->SetBranchAddress("tl1_cx", event.tl1_cx, &b_tl1_cx);
   fchain->SetBranchAddress("tl1_cy", event.tl1_cy, &b_tl1_cy);
   fchain->SetBranchAddress("ec_sect", event.ec_sect, &b_ec_sect);
   fchain->SetBranchAddress("ec_r", event.ec_r, &b_ec_r);
   fchain->SetBranchAddress("ec_t", event.ec_t, &b_ec_t);
   fchain->SetBranchAddress("ec_ei", event.ec_ei, &b_ec_ei);
   fchain->SetBranchAddress("ec_eo", event.ec_eo, &b_ec_eo);
   fchain->SetBranchAddress("etot", event.etot, &b_etot);
   fchain->SetBranchAddress("cc_sect", event.cc_sect, &b_cc_sect);
   fchain->SetBranchAddress("cc_r", event.cc_r, &b_cc_r);
   fchain->SetBranchAddress("cc_t", event.cc_t, &b_cc_t);
   fchain->SetBranchAddress("nphe", event.nphe, &b_nphe);
   fchain->SetBranchAddress("cc_c2", event.cc_c2, &b_cc_c2);
   fchain->SetBranchAddress("sc_sect", event.sc_sect, &b_sc_sect);
   fchain->SetBranchAddress("sc_r", event.sc_r, &b_sc_r);
   fchain->SetBranchAddress("sc_t", event.sc_t, &b_sc_t);
   fchain->SetBranchAddress("edep", event.edep, &b_edep);
   fchain->SetBranchAddress("sc_pd", event.sc_pd, &b_sc_pd);
   fchain->SetBranchAddress("cc_segm", event.cc_segm, &b_cc_segm);
   fchain->SetBranchAddress("ech_x", event.ech_x, &b_ech_x);
   fchain->SetBranchAddress("ech_y", event.ech_y, &b_ech_y);
   fchain->SetBranchAddress("ech_z", event.ech_z, &b_ech_z);
   fchain->SetBranchAddress("tl1_x", event.tl1_x, &b_tl1_x);
   fchain->SetBranchAddress("tl1_y", event.tl1_y, &b_tl1_y);
   fchain->SetBranchAddress("tl1_z", event.tl1_z, &b_tl1_z);

   if(fTreeType == file_types::h22 && GSIM == false){
     fchain->SetBranchAddress("ihel",      &event.ihel,     &b_ihel); 
     fchain->SetBranchAddress("corr_hel",  &event.corr_hel, &b_corr_hel); 
     fchain->SetBranchAddress("tl3_x", event.tl3_x, &b_tl3_x);
     fchain->SetBranchAddress("tl3_y", event.tl3_y, &b_tl3_y);
     fchain->SetBranchAddress("tl3_z", event.tl3_z, &b_tl3_z);
     fchain->SetBranchAddress("tl3_cx", event.tl3_cx, &b_tl3_cx);
     fchain->SetBranchAddress("tl3_cy", event.tl3_cy, &b_tl3_cy);
     fchain->SetBranchAddress("tl3_cz", event.tl3_cz, &b_tl3_cz);
   }
   else if(fTreeType == file_types::h22 && GSIM == true){
     fchain->SetBranchAddress("ihel",      &event.ihel,     &b_ihel); 
     fchain->SetBranchAddress("tl3_x", event.tl3_x, &b_tl3_x);
     fchain->SetBranchAddress("tl3_y", event.tl3_y, &b_tl3_y);
     fchain->SetBranchAddress("tl3_z", event.tl3_z, &b_tl3_z);
     fchain->SetBranchAddress("tl3_cx", event.tl3_cx, &b_tl3_cx);
     fchain->SetBranchAddress("tl3_cy", event.tl3_cy, &b_tl3_cy);
     fchain->SetBranchAddress("tl3_cz", event.tl3_cz, &b_tl3_cz);
   }
   else if(fTreeType == file_types::h10){
     fchain->SetBranchAddress("evntclas", &event.ihel,     &b_ihel); 
     fchain->SetBranchAddress("evntclas", &event.corr_hel, &b_corr_hel); 
     fchain->SetBranchAddress("dc_xsc", event.tl3_x, &b_tl3_x);
     fchain->SetBranchAddress("dc_ysc", event.tl3_y, &b_tl3_y);
     fchain->SetBranchAddress("dc_zsc", event.tl3_z, &b_tl3_z);
     fchain->SetBranchAddress("dc_cxsc", event.tl3_cx, &b_tl3_cx);
     fchain->SetBranchAddress("dc_cysc", event.tl3_cy, &b_tl3_cy);
     fchain->SetBranchAddress("dc_czsc", event.tl3_cz, &b_tl3_cz);
   }

   fchain->SetBranchAddress("vx", event.vx, &b_vx);
   fchain->SetBranchAddress("vy", event.vy, &b_vy);

   if (GSIM){
       fchain->SetBranchAddress("mcnentr", &event.mcnentr, &b_mcnentr);
       fchain->SetBranchAddress("mcnpart", &event.mcnpart, &b_mcnpart);
       fchain->SetBranchAddress("mcst", event.mcst, &b_mcst);
       fchain->SetBranchAddress("mcid", event.mcid, &b_mcid);
       fchain->SetBranchAddress("mcpid", event.mcpid, &b_mcpid);
       fchain->SetBranchAddress("mctheta", event.mctheta, &b_mctheta);
       fchain->SetBranchAddress("mcphi", event.mcphi, &b_mcphi);
       fchain->SetBranchAddress("mcp", event.mcp, &b_mcp);
       fchain->SetBranchAddress("mcm", event.mcm, &b_mcm);
       fchain->SetBranchAddress("mcvx", event.mcvx, &b_mcvx);
       fchain->SetBranchAddress("mcvy", event.mcvy, &b_mcvy);
       fchain->SetBranchAddress("mcvz", event.mcvz, &b_mcvz);
       fchain->SetBranchAddress("mctof", event.mctof, &b_mctof);
     }

   fInitStage = init_stages::fReady; 
}

std::string h22Reader::GetFilenameChunk(int stringStart, int stringLen){
  if (fchain->GetEntries() == 0){ 
    std::cerr << "trying to get chunk when fchain empty" << std::endl; 
  }

  std::string fname = fchain->GetCurrentFile()->GetName();
  return fname.substr(stringStart,stringLen); ;
}

int h22Reader::GetRunNumber(){
  int runno = 0;

  TString filename = fchain->GetFile()->GetName(); 
  TRegexp runno_regex("[1-9][0-9][0-9][0-9][0-9]");
  TString srunno = filename( runno_regex );
  runno = srunno.Atoi();

  return runno; 
}

void h22Reader::GetEntry(int ientry){
  fchain->GetEntry(ientry);

  for(int ipart=0; ipart<event.gpart; ipart++){
    event.corr_b[ipart]    = event.b[ipart]; 
    event.corr_sc_t[ipart] = event.sc_t[ipart]; 


    if(event.q[ipart] != 0 && !GSIM){
      event.corr_vz[ipart] = Corrections::vz(event, ipart, GSIM);
    }
    else{
      event.corr_vz[ipart] = event.vz[ipart];     
    }
  }

  if(fTreeType == file_types::h10){
    event.corr_hel = 0;
    
    if (event.ihel > 0){
      event.ihel     = 1;
      event.corr_hel = 1;
    }
    else if (event.ihel < 0){
      event.ihel     = -1;
      event.corr_hel = -1;
    }
    else {
      event.ihel = 0;
    }
  }
}

#endif
