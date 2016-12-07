/* ########################################################################
   #                                                                      #
   #               David Riser, University of Connecticut                 #
   #                         October 27, 2016                             #
   #                                                                      #
   ######################################################################## */
#include <cmath>
#include <iostream>
#include <fstream>
using namespace std;

#include "CommonTools.h"
#include "h22Option.h"

#include "common/BaseDISHistograms.h"
#include "common/DIS1DHistograms.h"
#include "common/RadiativeCorrectionLoader.h"

int PrintUsage();
void configureCommandLineOptions(h22Options *theseOpts); 

int main(int argc, char * argv[]){

  // ------------- Physics Options -------------
  int numberOfXBins = 80; // x and W 
  int numberOfYBins = 40; // Q^{2}

  // Check for improper usage of this code. 
  if (argc < 3){ return PrintUsage(); }

  // Collect command line options, configure ours and compare.
  h22Options *options = new h22Options(); 
  configureCommandLineOptions(options); 
  options->set(argc, argv);

  int numberOfEvents        = options->args["N"].arg;
  string inputRadFilename   = options->args["LUND_RAD"].args; 
  string inputNoRadFilename = options->args["LUND_NORAD"].args; 
  string outputFilename     = options->args["OUT"].args; 

  if (inputRadFilename != "UNSET" && inputNoRadFilename != "UNSET"){

    // Generate 2-D Histograms for both radiaited and unradiated events and 
    // save them in the output file. 
    RadiativeCorrectionLoader radLoader("radEvents","Radiated Gen. Events from LUND",outputFilename,"recreate");
    radLoader.LoadEvents(inputRadFilename);
    radLoader.Execute(numberOfEvents);

    RadiativeCorrectionLoader noRadLoader("noRadEvents","Gen. Events from LUND",outputFilename,"update");
    noRadLoader.LoadEvents(inputNoRadFilename);
    noRadLoader.Execute(numberOfEvents);

    //    BaseDISHistograms *radEvents2D = new BaseDISHistograms();
    //    radEvents2D->Load(inputRadFilename.c_str(),"radEvents");

    //    BaseDISHistograms *noRadEvents2D = new BaseDISHistograms();
    //    noRadEvents2D->Load(inputNoRadFilename.c_str(),"noRadEvents");
     
    //    int xRebinFactor = floor(radEvents2D->numberOfXBins/numberOfXBins); 
    //    int yRebinFactor = floor(radEvents2D->numberOfQQBins/numberOfYBins); 
    //    radEvents2D->Rebin2D(xRebinFactor, yRebinFactor); 
    //    noRadEvents2D->Rebin2D(xRebinFactor, yRebinFactor); 

    //    DIS1DHistograms * dataEvents = new DIS1DHistograms();
    //    dataEvents->Create(dataEvents2D);
    //    dataEvents->SetErrors();
    //    dataEvents->Save(outputFilename.c_str(),"recreate");

  } else {
    return PrintUsage();
  }

  return 0;
}

void configureCommandLineOptions(h22Options * theseOpts){
  
  theseOpts->args["LUND_RAD"].args = "UNSET";
  theseOpts->args["LUND_RAD"].type = 1;
  theseOpts->args["LUND_RAD"].name = "Radiaited lund events";

  theseOpts->args["LUND_NORAD"].args = "UNSET";
  theseOpts->args["LUND_NORAD"].type = 1;
  theseOpts->args["LUND_NORAD"].name = "Unradiated lund events";
}

int PrintUsage(){

  cout << endl;
  cout << endl; 
  cout << "######################################################" << endl;
  cout << "#                                                    #" << endl;
  cout << "#    doRadiativeCorrection                           #" << endl;
  cout << "#                                                    #" << endl;
  cout << "#   This code requires an input/output flagged       #" << endl;
  cout << "#   -LUND_RAD=rad.lund -LUND_NORAD=norad.lund        #" << endl;
  cout << "#       -OUT=out/radCorr.root                        #" << endl;
  cout << "#                                                    #" << endl;
  cout << "######################################################" << endl;
  cout << endl;
  cout << endl;
  
  return 0;
} 
