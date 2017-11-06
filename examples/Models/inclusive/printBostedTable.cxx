#include <iostream>
#include <cmath>
#include <stdio.h>

#include "BostedInclusiveWrapper.h"
#include "KeppelInclusiveWrapper.h"

class BinnedDimension {
public:
  BinnedDimension(int n, double min, double max) : fNumberBins(n), fMin(min), fMax(max) {
    fWidth = (fMax-fMin)/fNumberBins; 
  }

  ~BinnedDimension(){ 
  }

  int GetNumberBins() const {
    return fNumberBins; 
  }

  double GetMinumum() const {
    return fMin; 
  }

  double GetMaximum() const {
    return fMax; 
  }

  double GetWidth() const {
    return fWidth; 
  }

  int FindBin(double value) const { 
    if (value > fMax || value < fMin){ return -1; }
    return floor((value-fMin)/fWidth); 
  }

  double GetBinCenter(int index){
    return fMin+index*fWidth + fWidth/2.0; 
  }

protected:
  int    fNumberBins; 
  double fMin;
  double fMax; 
  double fWidth; 

private:
  BinnedDimension(){ }

};

class Utils {
public:
  static double ToDegrees(){
    return (180/3.14159);
  }

  static double ToRadians(){
    return (3.14159/180);
  }

  static double ConvertXQ2toW2(double x, double q2){
    return (pow(0.938,2)-q2+q2/x);
  }
  /* 
  static double Jacobian(double ebeam, double eprime, double theta){
    double q2 = 4*ebeam*eprime*pow(sin(theta/2*ToRadians()),2);
    double nu = ebeam - eprime; 

    double a = 2*ebeam*sin(theta*ToRadians())*q2/(2*0.938*nu); 
    double b = 2*0.938*eprime*(1+nu); 

    return a * (b/nu -1); 
  }
  */

  static double Jacobian(double ebeam, double eprime, double theta){
    double q2 = 2*ebeam*eprime*(1-cos(theta*ToRadians()));
    double nu = ebeam-eprime; 
    double x  = q2/(2*0.938*nu); 
    double w  = sqrt(ConvertXQ2toW2(x,q2));
    return w/(2*ebeam*eprime*0.938); 
  }

  static double ConvertXQ2toEPrime(double beam, double x, double q2){
    return (beam - q2/(2*0.938*x));
  }

  static double ConvertWQ2toEPrime(double beam, double w, double q2){
    return (beam - (pow(w,2)-pow(0.938,2)+q2)/(2*0.938));
  }

  static double ConvertEEPrimeQ2toTheta(double beam, double eprime, double q2){
    return ToDegrees()*2*asin( sqrt(q2/(4 *beam *eprime)) );
  }

  static double CalculatePhotonFlux(double beamEnergy, double x, double qq){
    double w = sqrt(ConvertXQ2toW2(x, qq));
    double nu = (pow(w,2) - pow(0.938, 2) + qq)/(2*0.938);
    double finalEnergy = beamEnergy - nu;
    double theta = ConvertEEPrimeQ2toTheta(beamEnergy, finalEnergy, qq)*ToRadians();
    double epsilon = 1/(1+2*(1+pow(nu,2)/qq)*pow(tan(theta/2),2));

    printf("[CalculatePhotonFlux] %.3e %.3e %.3e %.3e %.3e \n", w,  nu, finalEnergy, theta, epsilon); 

    return ((1/137)/pow(3.14159,2) * (finalEnergy/beamEnergy) * (pow(w,2) - pow(0.938,2))/(2*0.938)/qq * (1/(1-epsilon)));
  }

};

int main(int argc, char *argv[]){

  F1F209Wrapper          model; 
  KeppelInclusiveWrapper modelKeppel; 

  BinnedDimension w(100, 1.15, 3.00), q2(10, 1.7, 4.2);
  double beamEnergy = 5.5;   

  std::cout << "w_bin,q2_bin,w,q2,xs_bosted,xs_keppel" << std::endl; 

  for (int i=0; i<q2.GetNumberBins(); i++){
    for(int j=0; j<w.GetNumberBins(); j++){
      double eprime = Utils::ConvertWQ2toEPrime(beamEnergy, w.GetBinCenter(j), q2.GetBinCenter(i));
      double theta  = Utils::ConvertEEPrimeQ2toTheta(beamEnergy, eprime, q2.GetBinCenter(i));

      //      double xs = model.GetXS(1, 1, beamEnergy, eprime, theta) /Utils::Jacobian(beamEnergy, eprime, theta) *Utils::CalculatePhotonFlux(beamEnergy, x.GetBinCenter(i), q2.GetBinCenter(j));

      double xs    = model.GetXS(1, 1, beamEnergy, eprime, theta);
      double jacob = Utils::Jacobian(beamEnergy, eprime, theta); 
      //      double flux  = Utils::CalculatePhotonFlux(beamEnergy, w.GetBinCenter(), q2.GetBinCenter()); 

      double xsKeppel = modelKeppel.GetXS(beamEnergy, q2.GetBinCenter(i), w.GetBinCenter(j));

      if (std::isnan(xs)){
	xs = 0.00; 
      }
	
      printf("%d,%d,%.5e,%.5e,%.5e,%.5e\n", j+1, i+1, w.GetBinCenter(j), q2.GetBinCenter(i), xs, xsKeppel);
    }
  }


  return 0;
}
