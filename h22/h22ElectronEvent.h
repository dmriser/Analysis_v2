#ifndef h22_electron_event_h
#define h22_electron_event_h

#include "h22Event.h"

#include "TLorentzVector.h"

class h22ElectronEvent : public h22Event {
 public:

 h22ElectronEvent(const h22Event &event) : h22Event(event) {
    
  }

  ~h22ElectronEvent(){
    
  }

  // Public Data Members 
  TLorentzVector electron; 
  Float_t corr_sc_r[40]; 
  Float_t corr_sc_t[40]; 
  Float_t corr_b[40];
  Float_t corr_vz[40];

  void SetElectronIndex(int index) { 
    fElectronIndex = index; 
  }

  void SetStartTime(float time){
    fStartTime = time; 
  }

  void  SetCorrectedStatus(bool corr){
    fIsCorrected = corr; 
  }

  void SetElectron(const TLorentzVector &e){
    electron = TLorentzVector(e); 
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
