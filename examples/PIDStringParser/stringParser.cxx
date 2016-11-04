#include <iostream>
#include <vector>
using std::cout; 
using std::endl; 
using std::string; 
using std::vector; 

#include "PIDStringParser.h"

int main(){
  
  string particlesIWant("11+22+22+2212");
  PIDStringParser pidParser; 
  vector<int> targets = pidParser.getPIDTargets( particlesIWant );
  
  cout << "Particles in string: " << particlesIWant << endl;
  cout << "Number of targets found: " << targets.size() << endl; 
  
  for (int i=0; i<targets.size(); i++){
    cout << targets[i] << " "; 
  }
  cout << endl;

  return 0;
}
