#include <iostream>
using namespace std; 

#include "h22Reader.h"

int main(int argc, char * argv[])
{

  h22Reader fReader(0);
  fReader.AddFile( argv[1] ); 
  fReader.Init();
  cout << fReader.runno() << endl; 

  
  return 0;
}
