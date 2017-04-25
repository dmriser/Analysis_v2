#include <iostream>

#include "TF1Integrator.h"

#include "TF1.h"

int main(int argc, char *argv[]){

  double xmin = 0.0; 
  double xmax = 10.0; 
  int nsteps = 10000; 


  TF1 *lin1 = new TF1("lin1","[0]+[1]*x", xmin, xmax); 
  TF1 *pol3 = new TF1("pol3","[0]+[1]*x +[2]*x^2 + [3]*x^3", xmin, xmax); 
  
  lin1->SetParameter(0, 0.0); 
  lin1->SetParameter(1, 1.0); 

  pol3->SetParameter(0, 0.0);
  pol3->SetParameter(1, 1.0);
  pol3->SetParameter(2, -4.0);
  pol3->SetParameter(3, -1.0);

  TF1Integrator_Trapozoidal trap; 
  TF1Integrator_Simpsons simps; 
  TF1Integrator_MC mc; 

  trap.SetNumberSteps(nsteps); 
  trap.SetLowerLimit(xmin); 
  trap.SetUpperLimit(xmax); 

  simps.SetNumberSteps(nsteps); 
  simps.SetLowerLimit(xmin); 
  simps.SetUpperLimit(xmax); 

  mc.SetNumberSteps(1000000); 
  mc.SetLowerLimit(xmin); 
  mc.SetUpperLimit(xmax); 

  trap.SetFunction(lin1);
  simps.SetFunction(lin1);
  mc.SetFunction(lin1);
  std::cout << "trapozoidal lin1 = " << trap.Integrate() << std::endl; 
  std::cout << "simpsons lin1 = " << simps.Integrate() << std::endl; 
  std::cout << "mc lin1 = " << mc.Integrate() << std::endl; 

  trap.SetFunction(pol3);
  simps.SetFunction(pol3);
  mc.SetFunction(pol3);
  std::cout << "trapozoidal pol3 = " << trap.Integrate() << std::endl; 
  std::cout << "simpsons pol3 = " << simps.Integrate() << std::endl; 
  std::cout << "mc pol3 = " << mc.Integrate() << std::endl; 

  return 0; 
}
