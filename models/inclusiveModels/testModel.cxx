#include <iostream>
using namespace std;

extern"C"{
  float brasse_(float *beamEnergy, float *qq, float *w);
  float tkeppel_(float *beamEnergy, float *qq, float *w);
}

int main(){


  float w = 1.21;
  float beamEnergy = 5.75;

  cout.width(12); cout << " Q^2 (GeV^2/c^2)";
  cout.width(12); cout << " W (GeV/c^2)";
  cout.width(16); cout << " Brasse (uB) ";
  cout.width(16); cout << " Keppel (uB) " << endl;

  for (int i=0; i<100; i++){
    float qq = 1.0 + 0.05*i; 
    cout.width(12); cout << qq; 
    cout.width(12); cout << w;
    cout.width(16); cout << brasse_(&beamEnergy, &qq, &w);
    cout.width(16); cout << tkeppel_(&beamEnergy, &qq, &w) << endl;
  }

  return 0;
}
