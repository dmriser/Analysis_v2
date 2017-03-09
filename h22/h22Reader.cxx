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
using namespace std;

// my includes 
#include "h22Event.h"
#include "h22Reader.h"

// root includes 
#include <TROOT.h>
#include <TString.h>
#include <TChain.h>
#include <TFile.h>
#include <TRegexp.h>

h22Reader::h22Reader(){
  GSIM   = -1;
  fchain = new TChain("h22");
}

h22Reader::h22Reader(int mc) {
  GSIM   = mc;
  fchain = new TChain("h22");
}

h22Reader::~h22Reader(){
  fchain->Delete();
}

void h22Reader::AddList(string _file, int nfiles){
  string type[2] = {"data", "GSIM"};

  ifstream file;
  file.open(_file.c_str());
  
  string line = "";
  int ifile   = 0;

  while ( !file.eof() && (ifile < nfiles) ){
      file >> line;
      fchain->AddFile( line.c_str() );
      cout << "[h22Reader::AddList] Added " << line.c_str() << " to the TChain as type " << type[GSIM] << endl;
      ifile++;
    }

  file.close();

}

void h22Reader::AddList(string _file, int nfiles, int startfile){
  string type[2] = {"data", "GSIM"};

  ifstream file;
  file.open(_file.c_str());
  
  string line = "";
  int ifile   = 0;
  int jfile   = 0;

  while ( !file.eof() && (jfile < nfiles) )
    {
      file >> line;

      if (ifile >= startfile)
	{
	  fchain->AddFile( line.c_str() );
	  cout << " > Added " << line.c_str() << " to the TChain as type " << type[GSIM] << endl;
	  jfile++;
	}
      ifile++;
    }

  file.close();

}

void h22Reader::AddFile(TString _fname){
  fchain->AddFile(_fname);
  return;
}


/**< Init() must be run once to link the branches of the TChain to the h22Event class members*/
void h22Reader::Init(){

  if ( GetEntries() > 0 ) {
    if ( runno() > 37000 && runno() < 39000 )      GSIM = 0;
    else if ( runno() > 50000 && runno() < 60000 ) GSIM = 0; 
    else                                           GSIM = 1;
  } 

  else { cout << "[h22Reader::Init] Defaulting to GSIM = 0, Entries =" << GetEntries() << endl; }
  
  // Set branch addresses and branch pointers
   fchain->SetBranchAddress("evntid", &event.evntid, &b_evntid);
   fchain->SetBranchAddress("ihel", &event.ihel, &b_ihel);
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
   fchain->SetBranchAddress("tl3_x", event.tl3_x, &b_tl3_x);
   fchain->SetBranchAddress("tl3_y", event.tl3_y, &b_tl3_y);
   fchain->SetBranchAddress("tl3_z", event.tl3_z, &b_tl3_z);
   fchain->SetBranchAddress("tl3_cx", event.tl3_cx, &b_tl3_cx);
   fchain->SetBranchAddress("tl3_cy", event.tl3_cy, &b_tl3_cy);
   fchain->SetBranchAddress("tl3_cz", event.tl3_cz, &b_tl3_cz);
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

}

string h22Reader::GetFilenameChunk(int stringStart, int stringLen)
{
  if (fchain->GetEntries() == 0){ cerr << "trying to get chunk when fchain empty" << endl; }
  string fname = fchain->GetCurrentFile()->GetName();
  return fname.substr(stringStart,stringLen); ;
}

int h22Reader::runno(){
  int runno = 0;

  TString filename = fchain->GetFile()->GetName(); 
  TRegexp runno_regex("[1-9][0-9][0-9][0-9][0-9]");
  TString srunno = filename( runno_regex );
  runno = srunno.Atoi();

  return runno; 
}

#endif
