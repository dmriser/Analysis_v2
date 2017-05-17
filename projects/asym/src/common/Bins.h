#ifndef bins_h
#define bins_h

#include <iostream>

#include "Types.h"

#include "DBins.h"

class Bins {
 public:
  Bins(){
  }

  void SetName(std::string n){
    fName = n; 
  }

  void SetXBins(DLineBins *b){
    xbins = b;
  }

  void SetZBins(DLineBins *b){
    zbins = b;
  }

  void SetPtBins(DLineBins *b){
    ptbins = b;
  }

  void SetPhiBins(DLineBins *b){
    phibins = b;
  }
  
  std::string GetName() const {
    return fName; 
  }

  DLineBins *GetXBins() const {
    return xbins; 
  }
  
  DLineBins *GetZBins() const {
    return zbins; 
  }
  
  DLineBins *GetPtBins() const {
    return ptbins; 
  }
  
  DLineBins *GetPhiBins() const {
    return phibins; 
  }

  static Bins *GetInstance(int index);

 protected:

  std::string fName; 
  DLineBins *xbins; 
  DLineBins *zbins; 
  DLineBins *ptbins; 
  DLineBins *phibins; 

};

class KaonPositiveBins : public Bins {
 public:
  KaonPositiveBins(){
    xbins   = new DLineBins(5,     0.1,   0.6); 
    zbins   = new DLineBins(5,     0.3,   0.7); 
    ptbins  = new DLineBins(5,     0.0,   1.0); 
    phibins = new DLineBins(18, -180.0, 180.0); 

    std::cout << "KaonPositiveBins have been created." << std::endl; 
  }
};

class KaonNegativeBins : public Bins {
 public:
  KaonNegativeBins(){
    xbins   = new DLineBins(5,     0.1,   0.6); 
    zbins   = new DLineBins(5,     0.3,   0.7); 
    ptbins  = new DLineBins(5,     0.0,   1.0); 
    phibins = new DLineBins(18, -180.0, 180.0); 

    std::cout << "KaonNegativeBins have been created." << std::endl; 
  }
};

class PionPositiveBins : public Bins {
 public:
  PionPositiveBins(){
    xbins   = new DLineBins(5,     0.1,   0.6); 
    zbins   = new DLineBins(5,     0.3,   0.7); 
    ptbins  = new DLineBins(5,     0.0,   1.0); 
    phibins = new DLineBins(18, -180.0, 180.0); 

    std::cout << "PionPositiveBins have been created." << std::endl; 
  }
};

class PionNegativeBins : public Bins {
 public:
  PionNegativeBins(){
    xbins   = new DLineBins(5,     0.1,   0.6); 
    zbins   = new DLineBins(5,     0.3,   0.7); 
    ptbins  = new DLineBins(5,     0.0,   1.0); 
    phibins = new DLineBins(18, -180.0, 180.0); 

    std::cout << "PionNegativeBins have been created." << std::endl; 
  }
};

class NullBins : public Bins {
 public:
  NullBins(){
    xbins   = new DLineBins(2, 0.0, 1.0); 
    zbins   = new DLineBins(2, 0.0, 1.0); 
    ptbins  = new DLineBins(2, 0.0, 1.0); 
    phibins = new DLineBins(2, 0.0, 1.0); 

    std::cout << "NullBins have been created." << std::endl; 
  }
};

Bins* Bins::GetInstance(int index){

  if (index == Meson::kKaonPositive){
    return new KaonPositiveBins;
  } else if (index == Meson::kPionPositive){
    return new PionPositiveBins;
  } else if (index == Meson::kKaonNegative){
    return new KaonNegativeBins;
  } else if (index == Meson::kPionNegative){
    return new PionNegativeBins;
  }
  

  return new NullBins; 
}

#endif
