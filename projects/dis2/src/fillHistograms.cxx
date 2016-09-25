/* ########################################################################
   #                                                                      #
   #               David Riser, University of Connecticut                 #
   #                      September 24, 2016                              #
   #                                                                      #
   ######################################################################## */

#include <iostream>
using namespace std;

#include "h22Option.h"
#include "DataLoader.h"
#include "MCLoader.h"

int PrintUsage();
void configureCommandLineOptions(h22Options * theseOpts); 

int main(int argc, char * argv[]){

  if (argc < 3){
    return PrintUsage();
  }

  h22Options * options = new h22Options(); 
  configureCommandLineOptions(options); 
  
  return 0;
}

int PrintUsage(){
  cout << " Usage: ./fillHistograms -Flags files" << endl;

  return 0;
} 

void configureCommandLineOptions(h22Options * theseOpts){
  
}
