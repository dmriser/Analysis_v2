#include <iostream>
#include <cmath>
#include <stdio.h>

#include "BostedInclusiveWrapper.h"

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
    return fMin+index*fWidth; 
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

  static double Jacobian(double ebeam, double eprime, double theta){
    double q2 = 4*ebeam*eprime*pow(sin(theta/2*ToRadians()),2);
    double nu = ebeam - eprime; 

    double a = 2*ebeam*sin(theta*ToRadians())*q2/(2*0.938*nu); 
    double b = 2*0.938*eprime*(1+nu); 

    return a * (b/nu -1); 
  }

  static double ConvertXQ2toEPrime(double beam, double x, double q2){
    return (beam - q2/(2*0.938*x));
  }

  static double ConvertEEPrimeQ2toTheta(double beam, double eprime, double q2){
    return ToDegrees()*2*asin( sqrt(q2/(4 *beam *eprime)) );
  }

  static double CalculatePhotonFlux(double beamEnergy, double x, double qq){
    double w = sqrt(ConvertXQ2toW2(x, qq));
    double nu = (pow(w,2) - pow(0.938, 2) + qq)/(2*0.938);
    double finalEnergy = beamEnergy - nu;
    double theta = 2*asin( sqrt(qq/(4 * beamEnergy * finalEnergy)) );
    double epsilon = 1/(1+2*(1+pow(nu,2)/qq)*pow(tan(theta/2),2));

    return ((1/137)/pow(3.14159,2) * (finalEnergy/beamEnergy) * (pow(w,2) - pow(0.938,2))/(2*0.938)/qq * (1/(1-epsilon)));
  }

};

int main(int argc, char *argv[]){

  F1F209Wrapper model; 
  BinnedDimension x(48, 0.05, 0.99), q2(100, 1.0, 20.8);
  double beamEnergy = 11.00;   

  for (int i=0; i<q2.GetNumberBins(); i++){
    for(int j=0; j<x.GetNumberBins(); j++){
      double eprime = Utils::ConvertXQ2toEPrime(beamEnergy, x.GetBinCenter(j), q2.GetBinCenter(i));
      double theta  = Utils::ConvertEEPrimeQ2toTheta(beamEnergy, eprime, q2.GetBinCenter(i));

      double xs = model.GetXS(1, 1, beamEnergy, eprime, theta) /Utils::Jacobian(beamEnergy, eprime, theta) *Utils::CalculatePhotonFlux(beamEnergy, x.GetBinCenter(i), q2.GetBinCenter(j));
      if (std::isnan(xs)){
	xs = 0.00; 
      }
	
      printf("%d %d %.5f \n", i+1, j+1, xs);
    }
  }


  return 0;
}
