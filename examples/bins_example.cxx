#include <iostream>
using namespace std;

#include "DBins.h"

int main()
{

  DBinsAsymmetric b;
  b.add_bin(0.00);
  b.add_bin(1.00);
  b.add_bin(2.05);
  b.add_bin(4.50);
  b.add_bin(4.607);
  b.add_bin(5.0);
  b.add_bin(6.40);
  b.add_bin(9.60);
  b.add_bin(123.2340);

  cout << " Found " << b.number() << " bins. " << endl;
  cout.width(12); cout << " Min ";
  cout.width(12); cout << " Max ";
  cout.width(12); cout << " Center ";
  cout.width(12); cout << " Width " << endl;
  

  for (int ibin=0; ibin<b.number(); ibin++) 
    {
      cout.width(12); cout << b.min(ibin); 
      cout.width(12); cout << b.max(ibin);
      cout.width(12); cout << b.center(ibin);
      cout.width(12); cout << b.width(ibin) << endl;
    }


  return 0;
}
