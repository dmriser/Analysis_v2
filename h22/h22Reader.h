/////////////////////////////////////////////
/*

  David Riser, University of Connecticut 
  Standard Introductory Text Goes Here. 

*/
/////////////////////////////////////////////


#ifndef h22Reader_h
#define h22Reader_h

// c++ includes 
#include <iostream>
#include <fstream>

// my includes 
#include "h22Event.h"

// root includes 
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

/**
 * h22Reader is a class which relies on the h22Event class, it constructs a chain of events from file.
 * h22Reader can be initialized for data or gsim, which contains extra mc banks.
 */

class h22Reader{
public :
   TChain         *fchain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; // current number in chain
   Int_t           GSIM; // -1 (uninitialized) 0 (data) 1 (gsim)

   h22Event event;

   // List of branches
   TBranch        *b_evntid;   //!
   TBranch        *b_ihel;   //!
   TBranch        *b_q_l;   //!
   TBranch        *b_tr_time;   //!
   TBranch        *b_gpart;   //!
   TBranch        *b_q;   //!
   TBranch        *b_p;   //!
   TBranch        *b_b;   //!
   TBranch        *b_cx;   //!
   TBranch        *b_cy;   //!
   TBranch        *b_cz;   //!
   TBranch        *b_vz;   //!
   TBranch        *b_dc_sect;   //!
   TBranch        *b_tl1_cx;   //!
   TBranch        *b_tl1_cy;   //!
   TBranch        *b_ec_sect;   //!
   TBranch        *b_ec_r;   //!
   TBranch        *b_ec_t;   //!
   TBranch        *b_ec_ei;   //!
   TBranch        *b_ec_eo;   //!
   TBranch        *b_etot;   //!
   TBranch        *b_cc_sect;   //!
   TBranch        *b_cc_r;   //!
   TBranch        *b_cc_t;   //!
   TBranch        *b_nphe;   //!
   TBranch        *b_cc_c2;   //!
   TBranch        *b_sc_sect;   //!
   TBranch        *b_sc_r;   //!
   TBranch        *b_sc_t;   //!
   TBranch        *b_edep;   //!
   TBranch        *b_sc_pd;   //!
   TBranch        *b_cc_segm;   //!
   TBranch        *b_ech_x;   //!
   TBranch        *b_ech_y;   //!
   TBranch        *b_ech_z;   //!
   TBranch        *b_tl1_x;   //!
   TBranch        *b_tl1_y;   //!
   TBranch        *b_tl1_z;   //!
   TBranch        *b_tl3_x;   //!
   TBranch        *b_tl3_y;   //!
   TBranch        *b_tl3_z;   //!
   TBranch        *b_tl3_cx;   //!
   TBranch        *b_tl3_cy;   //!
   TBranch        *b_tl3_cz;   //!
   TBranch        *b_id;   //!
   TBranch        *b_vx;   //!
   TBranch        *b_vy;   //!
   TBranch        *b_nprt;   //!
   TBranch        *b_pidpart;   //!
   TBranch        *b_xpart;   //!
   TBranch        *b_ypart;   //!
   TBranch        *b_zpart;   //!
   TBranch        *b_epart;   //!
   TBranch        *b_pxpart;   //!
   TBranch        *b_pypart;   //!
   TBranch        *b_pzpart;   //!
   TBranch        *b_qpart;   //!
   TBranch        *b_Ipart10;   //!
   TBranch        *b_Rpart11;   //!
   TBranch        *b_Rpart12;   //!
   TBranch        *b_Ipart13;   //!
   TBranch        *b_mcnentr;   //!
   TBranch        *b_mcnpart;   //!
   TBranch        *b_mcst;   //!
   TBranch        *b_mcid;   //!
   TBranch        *b_mcpid;   //!
   TBranch        *b_mctheta;   //!
   TBranch        *b_mcphi;   //!
   TBranch        *b_mcp;   //!
   TBranch        *b_mcm;   //!
   TBranch        *b_mcvx;   //!
   TBranch        *b_mcvy;   //!
   TBranch        *b_mcvz;   //!
   TBranch        *b_mctof;   //!

 public:
   h22Reader(int);
   ~h22Reader();
   void Init();
   void AddFile(TString);
   int GetEntries(){return fchain->GetEntries(); }
   h22Event GetEvent(){return event;}
   std::string GetFilename(){return fchain->GetCurrentFile()->GetName();}
   std::string GetFilenameChunk(int,int);
   void AddList(std::string, int);
   void AddList(std::string, int, int);
   void GetEntry(int ien){fchain->GetEntry(ien);}
   int runno();
   
};
#endif
