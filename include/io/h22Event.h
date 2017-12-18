///////////////////////////////////////////
/*

  David Riser, University of Connecticut 
  July 8, BUFFER_SIZE16 

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
 
  const static int BUFFER_SIZE = 24; 

  // Declaration of leaf types
   UInt_t          evntid;
   UChar_t         ihel;
   Int_t           corr_hel;
   Float_t         q_l;
   Float_t         tr_time;
   Int_t           gpart;
   Int_t           q[BUFFER_SIZE];   //[gpart]
   Float_t         p[BUFFER_SIZE];   //[gpart]
   Float_t         b[BUFFER_SIZE];   //[gpart]
   Float_t         cx[BUFFER_SIZE];   //[gpart]
   Float_t         cy[BUFFER_SIZE];   //[gpart]
   Float_t         cz[BUFFER_SIZE];   //[gpart]
   Float_t         vz[BUFFER_SIZE];   //[gpart]
   UChar_t         dc_sect[BUFFER_SIZE];   //[gpart]
   Float_t         tl1_cx[BUFFER_SIZE];   //[gpart]
   Float_t         tl1_cy[BUFFER_SIZE];   //[gpart]
   UChar_t         ec_sect[BUFFER_SIZE];   //[gpart]
   Float_t         ec_r[BUFFER_SIZE];   //[gpart]
   Float_t         ec_t[BUFFER_SIZE];   //[gpart]
   Float_t         ec_ei[BUFFER_SIZE];   //[gpart]
   Float_t         ec_eo[BUFFER_SIZE];   //[gpart]
   Float_t         etot[BUFFER_SIZE];   //[gpart]
   UChar_t         cc_sect[BUFFER_SIZE];   //[gpart]
   Float_t         cc_r[BUFFER_SIZE];   //[gpart]
   Float_t         cc_t[BUFFER_SIZE];   //[gpart]
   UShort_t        nphe[BUFFER_SIZE];   //[gpart]
   Float_t         cc_c2[BUFFER_SIZE];   //[gpart]
   UChar_t         sc_sect[BUFFER_SIZE];   //[gpart]
   Float_t         sc_r[BUFFER_SIZE];   //[gpart]
   Float_t         sc_t[BUFFER_SIZE];   //[gpart]
   Float_t         edep[BUFFER_SIZE];   //[gpart]
   UChar_t         sc_pd[BUFFER_SIZE];   //[gpart]
   UShort_t        cc_segm[BUFFER_SIZE];   //[gpart]
   Float_t         ech_x[BUFFER_SIZE];   //[gpart]
   Float_t         ech_y[BUFFER_SIZE];   //[gpart]
   Float_t         ech_z[BUFFER_SIZE];   //[gpart]
   Float_t         tl1_x[BUFFER_SIZE];   //[gpart]
   Float_t         tl1_y[BUFFER_SIZE];   //[gpart]
   Float_t         tl1_z[BUFFER_SIZE];   //[gpart]
   Float_t         tl3_x[BUFFER_SIZE];   //[gpart]
   Float_t         tl3_y[BUFFER_SIZE];   //[gpart]
   Float_t         tl3_z[BUFFER_SIZE];   //[gpart]
   Float_t         tl3_cx[BUFFER_SIZE];   //[gpart]
   Float_t         tl3_cy[BUFFER_SIZE];   //[gpart]
   Float_t         tl3_cz[BUFFER_SIZE];   //[gpart]
   Int_t           id[BUFFER_SIZE];   //[gpart]
   Float_t         vx[BUFFER_SIZE];   //[gpart]
   Float_t         vy[BUFFER_SIZE];   //[gpart]
   Int_t           nprt;
   Int_t           pidpart[BUFFER_SIZE];   //[nprt]
   Float_t         xpart[BUFFER_SIZE];   //[nprt]
   Float_t         ypart[BUFFER_SIZE];   //[nprt]
   Float_t         zpart[BUFFER_SIZE];   //[nprt]
   Float_t         epart[BUFFER_SIZE];   //[nprt]
   Float_t         pxpart[BUFFER_SIZE];   //[nprt]
   Float_t         pypart[BUFFER_SIZE];   //[nprt]
   Float_t         pzpart[BUFFER_SIZE];   //[nprt]
   Float_t         qpart[BUFFER_SIZE];   //[nprt]
   Int_t           Ipart10[BUFFER_SIZE];   //[nprt]
   Float_t         Rpart11[BUFFER_SIZE];   //[nprt]
   Float_t         Rpart12[BUFFER_SIZE];   //[nprt]
   Int_t           Ipart13[BUFFER_SIZE];   //[nprt]
   Int_t           mcnentr;
   UChar_t         mcnpart;
   Int_t           mcst[BUFFER_SIZE];   //[mcnentr]
   Int_t           mcid[BUFFER_SIZE];   //[mcnentr]
   Int_t           mcpid[BUFFER_SIZE];   //[mcnentr]
   Float_t         mctheta[BUFFER_SIZE];   //[mcnentr]
   Float_t         mcphi[BUFFER_SIZE];   //[mcnentr]
   Float_t         mcp[BUFFER_SIZE];   //[mcnentr]
   Float_t         mcm[BUFFER_SIZE];   //[mcnentr]
   Float_t         mcvx[BUFFER_SIZE];   //[mcnentr]
   Float_t         mcvy[BUFFER_SIZE];   //[mcnentr]
   Float_t         mcvz[BUFFER_SIZE];   //[mcnentr]
   Float_t         mctof[BUFFER_SIZE];   //[mcnentr]
   
   // Extra Stuffs 
   //   Float_t         corr_sc_r[BUFFER_SIZE];
   Float_t         corr_sc_t[BUFFER_SIZE]; // Added 
   Float_t         corr_b[BUFFER_SIZE];
   Float_t         corr_vz[BUFFER_SIZE];

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
