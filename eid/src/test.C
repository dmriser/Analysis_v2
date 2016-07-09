
// c++ includes 
#include <iostream>

// my includes 
#include "pars.h"
#include "pars.cxx"

int main(int argc, char * argv[])
{

  epars params;
  params.load("test_modded.dat");

    for (int i=0; i<10; i++){
        double p=0.5 +i*0.4;
        cout << "min: " << params.ec_samp_min(p, 3) << " max: " << params.ec_samp_max(p, 3) << endl;
    }
    
    params.save("test.dat");
  
  return 0;
}
