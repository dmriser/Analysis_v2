#ifndef binfactory_h
#define binfactory_h

#include <iostream>

#include "Bins.h"
#include "Types.h"

class BinFactory {
 public:
  template <class Bins*>
  Bins* GetInstance(int index){    

    std::cout << "[BinFactory] Building bins for " << index << std::endl; 

    if (index == Meson::kKaonPositive){
      return new KaonPositiveBins(); 
    } else if (index == Meson::kPionPositive){
      return new PionPositiveBins(); 
    } else if (index == Meson::kKaonNegative){
      return new KaonNegativeBins(); 
    } else if (index == Meson::kPionNegative){
      return new PionNegativeBins(); 
    }

    return new NullBins(); 
  } 
};

#endif
