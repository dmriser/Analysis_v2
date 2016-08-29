#include <iostream>
#include <vector>
using namespace std;

#include "DBins.h"
#include "TRandom3.h"

int main()
{

  cout << " Bin Example! " << endl; 

  TRandom3 * rand = new TRandom3(); 
  
  DLineBins ebins; 
  ebins.AddBin( DBin(0, 1) );
  ebins.AddBin( DBin(1, 2) );
  ebins.AddBin( DBin(2, 3) );
  
  DLineBins bins0; 
  bins0.AddBin( DBin(0.01, 4.0) );
  bins0.AddBin( DBin(4.0, 8.2) );
  bins0.AddBin( DBin(8.2, 12.5) );
  
  DLineBins bins1; 
  bins1.AddBin( DBin(0.01, 0.05) );
  bins1.AddBin( DBin(0.05, 12.2) );
  bins1.AddBin( DBin(12.2, 12.5) );
  
  DLineBins bins2; 
  bins2.AddBin( DBin(0.01, 0.05) );
  bins2.AddBin( DBin(0.05, 12.2) );
  bins2.AddBin( DBin(12.2, 12.5) );

  DPlaneBins planeBins; 
  planeBins.SetEdgeBins(ebins); 
  planeBins.AddLineBins(bins0);
  planeBins.AddLineBins(bins1);
  planeBins.AddLineBins(bins2); 
  
  for (int i=0; i<1000; i++)
    {
      planeBins.Fill( 0.5, rand->Gaus(6.0, 6.0) );
      planeBins.Fill( 1.5, rand->Gaus(6.0, 6.0) );
      planeBins.Fill( 2.5, rand->Gaus(6.0, 6.0) );
    }

  planeBins.Print(); 
  
  return 0;
}
