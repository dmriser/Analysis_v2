#include <iostream>
using namespace std;

extern"C"{
  double brasse_(double *beamEnergy, double *qq, double *w);
  float tkeppel_(float *beamEnergy, float *qq, float *w);
  double bosted_(double *Z, double *A, double *beamEnergy, double *qq, double *w);
  void f1f2in09_(double *Z, double *A, double *qq, double *ww, double *f1, double *f2, double *r);
  void f1f2qe09_(double *Z, double *A, double *qq, double *ww, double *f1, double *f2);
}

int main(){


  double w = 1.21;
  double ww = w*w;
  double beamEnergy = 5.75;
  double A = 1; 
  double Z = 1; 
  double f1, f2, r;

  float wFloat = 1.21; 
  float beamEnergyFloat = 5.75;

  cout.width(12); cout << " Q^2 (GeV^2/c^2)";
  cout.width(12); cout << " W (GeV/c^2)";
  cout.width(16); cout << " Brasse (uB) ";
  cout.width(16); cout << " Bosted (uB) ";
  cout.width(16); cout << " Keppel (uB) " << endl;

  for (int i=0; i<100; i++){
    double qq = 1.0 + 0.05*i; 
    float qqFloat = 1.0 + 0.05*i; 
    cout.width(12); cout << qq; 
    cout.width(12); cout << w;
    cout.width(16); cout << brasse_(&beamEnergy, &qq, &w);
    cout.width(16); cout << bosted_(&Z, &A, &beamEnergy, &qq, &w);
    cout.width(16); cout << tkeppel_(&beamEnergyFloat, &qqFloat, &wFloat) << endl;
  }

  return 0;
}
