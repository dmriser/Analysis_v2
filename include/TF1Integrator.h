#ifndef tf1_integrator_h
#define tf1_integrator_h

#include "TF1.h"
#include "TRandom3.h"

class TF1Integrator {
 public:
  TF1Integrator(){
  }

  ~TF1Integrator(){
  }

  void SetFunction(TF1 *f){
    fFunction = f; 
  }

  double GetLowerLimit() const {
    return fLowerLimit; 
  }

  double GetUpperLimit() const {
    return fUpperLimit; 
  }

  int GetNumberSteps() const {
    return fNumberSteps; 
  }

  void SetLowerLimit(double low){
    fLowerLimit = low; 
  }

  void SetUpperLimit(double up){
    fUpperLimit = up; 
  }

  void SetNumberSteps(int n){
    fNumberSteps = n; 
  }
  
  virtual double Integrate(); 

 protected:
  TF1 *fFunction; 
  double fLowerLimit, fUpperLimit; 
  double fValue; 
  int fNumberSteps; 
};

class TF1Integrator_Trapozoidal : public TF1Integrator {
 public:
  double Integrate();
};

class TF1Integrator_Simpsons : public TF1Integrator {
 public:
  double Integrate();
};

class TF1Integrator_MC : public TF1Integrator {
 public:
  double Integrate(); 

 private:
  TRandom3 rand; 
};

#endif
