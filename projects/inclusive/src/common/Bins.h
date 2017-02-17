#ifndef bins_h 
#define bins_h 

#include <vector>
#include <iostream>

class Bins{

 public:
  Bins();
  ~Bins(){ } 

  const int WBINS   = 40;
  const int Q2BINS  = 10; 
  const int PHIBINS = 3; 
  const int SECTORS = 6;

  constexpr static double WMIN   = 1.1; 
  constexpr static double WMAX   = 1.9; 
  constexpr static double Q2MIN  = 1.5; 
  constexpr static double Q2MAX  = 4.5; 
  constexpr static double PHIMIN = -25.0; 
  constexpr static double PHIMAX = 25.0;
  double DW, DQ2, DPHI;

  std::vector<double> wBins, qqBins, phiBins;

  int findWBin(double value);
  int findQQBin(double value);
  int findPhiBin(double value);
};

Bins::Bins(){
  DW   = (WMAX-WMIN)/WBINS;
  DQ2  = (Q2MAX-Q2MIN)/Q2BINS;
  DPHI = (PHIMAX-PHIMIN)/PHIBINS;

  for(int bin=0; bin<=WBINS; bin++)  { wBins  .push_back(WMIN+bin*DW); }
  for(int bin=0; bin<=Q2BINS; bin++) { qqBins .push_back(Q2MIN+bin*DQ2); }
  for(int bin=0; bin<=PHIBINS; bin++){ phiBins.push_back(PHIMIN+bin*DPHI); }
}

int Bins::findWBin(double value){
  for(int bin=0; bin<WBINS; bin++){ 
    if(wBins[bin+1] >= value && wBins[bin] <= value){ \
      return bin;  
    } 
  }

  std::cout << "[Bins::findBin] Passed value = " << value << " is outside of bin range. " << std::endl;  
  return -1;
}

int Bins::findQQBin(double value){
  for(int bin=0; bin<Q2BINS; bin++){ 
    if(qqBins[bin+1] >= value && qqBins[bin] <= value){ \
      return bin;  
    } 
  }

  std::cout << "[Bins::findBin] Passed value = " << value << " is outside of bin range. " << std::endl;  
  return -1;
}

int Bins::findPhiBin(double value){
  for(int bin=0; bin<PHIBINS; bin++){ 
    if(phiBins[bin+1] >= value && phiBins[bin] <= value){ \
      return bin;  
    } 
  }

  std::cout << "[Bins::findBin] Passed value = " << value << " is outside of bin range. " << std::endl;  
  return -1;
}

#endif
