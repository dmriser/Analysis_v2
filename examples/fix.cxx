#include <iostream>
using namespace std; 

#include "ElectronIDPackage.h"

int main(int argc, char * argv[])
{

  if ( argc < 2 ) { exit(0); } 

  ElectronIDManager eid; 
  
  for (int iarg=1; iarg<argc; iarg++) { eid.AddFile(argv[iarg]); }
  eid.set_parfile("test_pars.dat");
  eid.init();
  eid.calculate_values();
  
  return 0;
}
