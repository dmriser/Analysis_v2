#include <iostream>
using namespace std; 

#include "DBins.h"

int main(int argc, char * argv[])
{

  DBins firstBins(100, 34, 53);
  DBins secondBins; 
  secondBins = firstBins;

  cout << " First Bins: " << firstBins.number() << " " << firstBins.min() << " " << firstBins.max() << endl;
  cout << " Second Bins: " << secondBins.number() << " " << secondBins.min() << " " << secondBins.max() << endl; 

  secondBins.set_number(20);
  secondBins.set_min(10);
  secondBins.set_max(11);
  
  cout << " First Bins: " << firstBins.number() << " " << firstBins.min() << " " << firstBins.max() << endl;
  cout << " Second Bins: " << secondBins.number() << " " << secondBins.min() << " " << secondBins.max() << " " << secondBins.width() << endl; 

  secondBins.refresh();
  
  cout << " First Bins: " << firstBins.number() << " " << firstBins.min() << " " << firstBins.max() << endl;
  cout << " Second Bins: " << secondBins.number() << " " << secondBins.min() << " " << secondBins.max() << " " << secondBins.width() << endl; 
  
  return 0;
}
