#include <iostream>
#include <vector>
using std::cout; 
using std::endl; 
using std::string; 
using std::vector; 

#include "Dimension.h"
#include "PhaseSpace.h"


int main(){

  Dimension *x = new Dimension("x",0.0,3.0); 
  Dimension *y = new Dimension("y",0.0,3.0); 
  Dimension *z = new Dimension("z",0.0,1.0); 
  Dimension *s = new Dimension("s",0.0,1.0); 
  Dimension *v = new Dimension("v",0.0,10.0); 
  Dimension *t = new Dimension("t",0.0,2.0); 

  PhaseSpace xyz; 
  xyz.setName("Toy phase space."); 
  xyz.addDimension(x); 
  xyz.addDimension(y); 
  xyz.addDimension(z); 
  xyz.addDimension(s); 
  xyz.addDimension(v); 
  xyz.addDimension(t); 


  for(int i=0; i<100; i++){
    vector<double> point = xyz.getRandom(); 
    for (int d=0; d<xyz.getRank(); d++){ cout.width(16); cout << point[d]; }
    cout << endl; 
  }


  cout << xyz.getName()             << endl; 
  cout << xyz.getRank()             << endl; 
  cout << xyz.getPhaseSpaceVolume() << endl; 

  return 0;
}
