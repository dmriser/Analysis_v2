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
//#include <TROOT.h>
#include "TLorentzVector.h"
#include "TVector3.h"


/**
 * h22Event class is the basis for the h22Reader class as well as ParticleFilter class.
 * It defines the general structure of an event in the h22 ntuple.
 */

class h22Event {
 public :
 
  // Declaration of leaf types
   UInt_t          evntid;
   UChar_t         ihel;
   Int_t           corr_hel;
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
   
   // Extra Stuffs 
   //   Float_t         corr_sc_r[40];
   Float_t         corr_sc_t[40]; // Added 
   Float_t         corr_b[40];
   Float_t         corr_vz[40];

   // Const. & Dest. 
   h22Event();
   h22Event(const h22Event &event);
   ~h22Event();
   // Member Functions 
   void PrintEvent() const;
   int GetSector(int index);
   double GetRelativePhi(int);     //! Azym. angle relative to sector center.
   double GetRelativePhiMC(int);   //! MC version of above
   double GetTheta(int);    //! Polar angle of track
   double GetPhi(int index) const; 
   TVector3 GetUVWVector(int);    //! EC Coordinates U, V, W returned as U = vector.X(), V = vector.Y(), W = vector.Z()
   TLorentzVector GetGeneratedParticle(int);
   int GetGeneratedParticleIndex(int pid);
   int GetMCSectorByPID(int index);
   
   double GetRotatedDCR1PosX(int);
   double GetRotatedDCR1PosY(int);
   
   double GetThetaCC(int); //! Theta as reported at Cherenkov Counter
   
   bool HasParticle(int pid);

   // returns user ana pid vector 
   std::vector<int> GetVectorOfUserAnaParticleIndices(int pid);

   // returns the index of the reconstructed particle that 
   // was generated with coded pid, and a status code 
   std::pair<bool, int> GetGeneratedToReconstructedMapping(int pid); 
   
   // MC Momenta 
   double GetMCPx(int);
   double GetMCPy(int);
   double GetMCPz(int);

   // Sorting functions 
   std::vector<int> SortByMomentum(std::vector<int>);
   TLorentzVector GetTLorentzVector(int index, int pid);

   void SetElectronIndex(int index) {
     fElectronIndex = index;
   }

   void SetStartTime(float time){
     fStartTime = time;
   }

   void  SetCorrectedStatus(bool corr){
     fIsCorrected = corr;
   }

   int GetElectronIndex() const {
     return fElectronIndex;
   }

   float GetStartTime() const {
     return fStartTime;
   }

   bool IsCorrected() const {
     return fIsCorrected;
   }

 protected:
   int   fElectronIndex;
   float fStartTime;
   bool  fIsCorrected;  

};

#endif
