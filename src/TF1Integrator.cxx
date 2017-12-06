#ifndef tf1_integrator_cxx
#define tf1_integrator_cxx

#include <iostream>
#include <vector>

#include "TF1Integrator.h"

#include "TF1.h"
#include "TRandom3.h"

double TF1Integrator::Integrate(){
  return 0.0; 
}

double TF1Integrator_Trapozoidal::Integrate(){
  double sum = 0.0; 
  double dx = (fUpperLimit-fLowerLimit)/(fNumberSteps+1);
  
  if (dx < 0.0){
    std::cout << "[TF1Integrator] Bounds are incorrect, dx is negative! " << std::endl;
    return 0.0; 
  }

  std::vector<double> x; 
  std::vector<double> f; 

  for(int i=0; i<fNumberSteps+1; i++){
    x.push_back(fLowerLimit +dx*i); 
    f.push_back(fFunction->Eval(x[i])); 
  }

  for(int i=0; i<x.size()-1; i++){
    sum += 0.5*(f[i]+f[i+1]); 
  }
    
  return sum *dx; 
}


double TF1Integrator_Simpsons::Integrate(){
  double sum = 0.0; 
  double dx = (fUpperLimit-fLowerLimit)/(fNumberSteps+2);
  
  if (dx < 0.0){
    std::cout << "[TF1Integrator] Bounds are incorrect, dx is negative! " << std::endl;
    return 0.0; 
  }

  std::vector<double> x; 
  std::vector<double> f; 

  for(int i=0; i<fNumberSteps+2; i++){
    x.push_back(fLowerLimit +dx*i); 
    f.push_back(fFunction->Eval(x[i])); 
  }

  for(int i=0; i<x.size()-2; i++){
    sum += (f[i]+4*f[i+1]+f[i+2])/3; 
  }
    
  return sum *dx/2; 
}

double TF1Integrator_MC::Integrate(){
  double sum = 0.0; 
  double range = fUpperLimit-fLowerLimit;   
  
  for(int i=0; i<fNumberSteps; i++){
    double x = fLowerLimit + range*rand.Uniform(); 
    sum += fFunction->Eval(x); 
  }

  return sum*(range)/fNumberSteps; 
}

#endif
