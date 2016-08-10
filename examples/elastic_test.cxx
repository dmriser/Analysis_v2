//////////////////////////////////////////////////////
/*

   David Riser, University of Connecticut 
   August 8, 2016 

   ElasticTest -> Testbed for libaries. 


*/
//////////////////////////////////////////////////////

// c++ includes
#include <iostream>
#include <vector>
using namespace std;

// my includes
#include "ElasticPackage.h"

int main(int argc, char * argv[])
{

  // Setup your binning scheme 
  DBins thetaBins(20, 18, 50);
  DBins phiBins(20, -18, 18);
  DBins pBins(5, 0.5, 5.5);
  ElasticBins bins(thetaBins, phiBins, pBins);

  // Instantiate the analysis class with your bins 
  ElasticAnalysis analysis(bins);

  //  analysis.add_files(0, files);
  //  analysis.init();
  //  analysis.run();
  //  analysis.close();
  
  return 0;
}
