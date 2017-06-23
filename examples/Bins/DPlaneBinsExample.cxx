#include <iostream>
#include <vector>
using namespace std;

#include "DBins.h"
#include "TRandom3.h"

int main()
{

  TRandom3 * rand = new TRandom3(); 

  // Setup our binning plane  
  DPlaneBins xy_plane;
  DLineBins x_bins(20,-1, 1); 
  DLineBins y_bins(20,-1, 1); 

  // Make symettric bins along the x direction in y, just like a TH2
  xy_plane.SetEdgeBins( x_bins );
  for (int iline=0; iline<xy_plane.GetEdgeBins().GetNumber(); iline++) { xy_plane.AddLineBins( y_bins ); } 

  for (int i=0; i<100000; i++){
    xy_plane.Fill(rand->Gaus(0, 1),rand->Gaus(0, 0.05)); 
  } 
  
  cout << " DPlaneBins " << endl; 
  xy_plane.Print();

  // Doing Statistical Rebin
  cout << " Rebinned DPlaneBins " << endl; 
  DPlaneBins xy_plane_rebin = xy_plane.StatisticalRebin(5,5); 
  xy_plane_rebin.Print(); 
  
  return 0;
}
