///////////////////////////////////////////
/*

  David Riser, University of Connecticut 
  July 8, 2016 

  Skeleton from h22->MakeClass()

  h22Event.h -> Holds class structure for 
  h22Event type which drives analysis. 

*/
///////////////////////////////////////////

#ifndef h22Event_h
#define h22Event_h


#include <vector>
#include <TROOT.h>
#include "TLorentzVector.h"
#include "TVector3.h"


/**
 * h22Event class is the basis for the h22Reader class as well as ParticleFilter class.
 * It defines the general structure of an event in the h22 ntuple.
 */

class h22Event
{
 public :
 
  // Declaration of leaf types
   UInt_t          evntid;
   UChar_t         ihel;
   Float_t         q_l;
   Float_t         tr_time;
   Int_t           gpart;
   Int_t           q[40];   //[gpart]
   Float_t         p[40];   //[gpart]
   Float_t         b[40];   //[gpart]
   Float_t         cx[40];   //[gpart]
   Float_t         cy[40];   //[gpart]
   Float_t         cz[40];   //[gpart]
   Float_t         vz[40];   //[gpart]
   UChar_t         dc_sect[40];   //[gpart]
   Float_t         tl1_cx[40];   //[gpart]
   Float_t         tl1_cy[40];   //[gpart]
   UChar_t         ec_sect[40];   //[gpart]
   Float_t         ec_r[40];   //[gpart]
   Float_t         ec_t[40];   //[gpart]
   Float_t         ec_ei[40];   //[gpart]
   Float_t         ec_eo[40];   //[gpart]
   Float_t         etot[40];   //[gpart]
   UChar_t         cc_sect[40];   //[gpart]
   Float_t         cc_r[40];   //[gpart]
   Float_t         cc_t[40];   //[gpart]
   UShort_t        nphe[40];   //[gpart]
   Float_t         cc_c2[40];   //[gpart]
   UChar_t         sc_sect[40];   //[gpart]
   Float_t         sc_r[40];   //[gpart]
   Float_t         sc_t[40];   //[gpart]
   Float_t         edep[40];   //[gpart]
   UChar_t         sc_pd[40];   //[gpart]
   UShort_t        cc_segm[40];   //[gpart]
   Float_t         ech_x[40];   //[gpart]
   Float_t         ech_y[40];   //[gpart]
   Float_t         ech_z[40];   //[gpart]
   Float_t         tl1_x[40];   //[gpart]
   Float_t         tl1_y[40];   //[gpart]
   Float_t         tl1_z[40];   //[gpart]
   Float_t         tl3_x[40];   //[gpart]
   Float_t         tl3_y[40];   //[gpart]
   Float_t         tl3_z[40];   //[gpart]
   Float_t         tl3_cx[40];   //[gpart]
   Float_t         tl3_cy[40];   //[gpart]
   Float_t         tl3_cz[40];   //[gpart]
   Int_t           id[40];   //[gpart]
   Float_t         vx[40];   //[gpart]
   Float_t         vy[40];   //[gpart]
   Int_t           nprt;
   Int_t           pidpart[20];   //[nprt]
   Float_t         xpart[20];   //[nprt]
   Float_t         ypart[20];   //[nprt]
   Float_t         zpart[20];   //[nprt]
   Float_t         epart[20];   //[nprt]
   Float_t         pxpart[20];   //[nprt]
   Float_t         pypart[20];   //[nprt]
   Float_t         pzpart[20];   //[nprt]
   Float_t         qpart[20];   //[nprt]
   Int_t           Ipart10[20];   //[nprt]
   Float_t         Rpart11[20];   //[nprt]
   Float_t         Rpart12[20];   //[nprt]
   Int_t           Ipart13[20];   //[nprt]
   Int_t           mcnentr;
   UChar_t         mcnpart;
   Int_t           mcst[40];   //[mcnentr]
   Int_t           mcid[40];   //[mcnentr]
   Int_t           mcpid[40];   //[mcnentr]
   Float_t         mctheta[40];   //[mcnentr]
   Float_t         mcphi[40];   //[mcnentr]
   Float_t         mcp[40];   //[mcnentr]
   Float_t         mcm[40];   //[mcnentr]
   Float_t         mcvx[40];   //[mcnentr]
   Float_t         mcvy[40];   //[mcnentr]
   Float_t         mcvz[40];   //[mcnentr]
   Float_t         mctof[40];   //[mcnentr]

   // A bit of extra information useful in 
   // identification of hadrons and photons. 
   int processingStage; 
   int electronIndex; 
   int hadronIndex; 
   int hadronPID; 

   // Corrected Variables 
   double   correctedStartTime; 

   // Const. & Dest. 
   h22Event();
   ~h22Event();
  
   // Member Functions 
   void printEvent();
   double rphi(int);     //! Azym. angle relative to sector center. 
   double mcrphi(int);   //! MC version of above   
   double theta(int);    //! Polar angle of track
   TVector3 uvw(int);    //! EC Coordinates U, V, W returned as U = vector.X(), V = vector.Y(), W = vector.Z() 
   TLorentzVector gen_particle(int);
   int mcSectorByPID(int index);
   
   double rot_dc1x(int);
   double rot_dc1y(int);
   
   double theta_cc(int); //! Theta as reported at Cherenkov Counter
   
   bool hasParticle(int pid){ for(int i=0;i<gpart; i++){ if(id[i]==pid) { return true; } } return false;}
   
   // MC Momenta 
   double mcpx(int);
   double mcpy(int);
   double mcpz(int);

   // Sorting functions 
   std::vector<int> sortByMomentum(std::vector<int>);
   TLorentzVector getTLorentzVector(int index, int pid);

};

#endif
