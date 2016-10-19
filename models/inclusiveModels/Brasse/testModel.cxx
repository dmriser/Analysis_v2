#include <iostream>
using namespace std; 

extern"C"{
  float brasse_(float *beamEnergy, float *qq, float *w);
}

int main (){

  int numberQQBins = 10;
  int numberWBins = 10; 

  float qqMin = 1.0; float qqMax = 4.8; 
  float wMin = 1.2; float wMax = 2.7; 

  float beamEnergy = 5.498;
  
  for (int qqBin=0; qqBin< numberQQBins; qqBin++){
    float qq = qqMin + qqBin*(qqMax-qqMin)/numberQQBins;
    for (int wBin=0; wBin< numberWBins; wBin++){
      float w = wMin + wBin*(wMax-wMin)/numberWBins;
      cout.width(12); cout << qq;
      cout.width(12); cout << w;
      cout.width(12); cout << brasse_(&beamEnergy, &qq, &w) << endl;
    }
  }

  return 0;
}


