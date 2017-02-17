/* ########################################################################
   #                                                                      #
   #          Inclusive Histogram filling code.                           #
   #                                                                      #
   #               David Riser, University of Connecticut                 #
   #                      February 13, 2017                               #
   #                                                                      #
   ######################################################################## */

#include <iostream>
#include <fstream>
using namespace std;

#include "h22Option.h"
#include "DInformation.h"
#include "MomCorr.h"
#include "Parameters.h"
#include "PhysicsEventCut.h"
#include "PhysicsEventSelector.h"

#include "common/DataLoader.h"
#include "common/FaradayCupLoader.h"
#include "common/MCLoader.h"

int PrintUsage();
void configureCommandLineOptions(h22Options * theseOpts); 
vector<string> loadFilesFromList(string fileList, int numFiles);
vector<string> loadFilesFromCommandLine(h22Options * theseOpts, int numFiles);

int main(int argc, char * argv[]){

  if (argc < 3){
    return PrintUsage();
  }

  // Collect command line options, configure ours and compare.
  h22Options *options = new h22Options(); 
  configureCommandLineOptions(options); 
  options->set(argc, argv);

  // ------------------------------------------------------------
  //       Physics Options 
  // ------------------------------------------------------------
  PhysicsEventSelector * eventSelector = new PhysicsEventSelector();
  PhysicsEventCut_w *w_cut             = new PhysicsEventCut_w();
  PhysicsEventCut_p *p_cut             = new PhysicsEventCut_p();
  PhysicsEventCut_x *x_cut             = new PhysicsEventCut_x();
  PhysicsEventCut_y *y_cut             = new PhysicsEventCut_y();
  PhysicsEventCut_qq *qq_cut           = new PhysicsEventCut_qq();

  p_cut->set_min( options->args["PMIN"].arg );
  p_cut->set_max( 5.498 ); 
  w_cut->set_min( 1.1 );
  w_cut->set_max( 99.9 ); 
  y_cut->set_min( 0.00 );
  y_cut->set_max( options->args["YCUT"].arg ); 
  qq_cut->set_min( 1.00 );
  qq_cut->set_max( 99.9 ); 

  eventSelector->add_cut(p_cut);
  eventSelector->add_cut(w_cut);
  eventSelector->add_cut(y_cut);
  eventSelector->add_cut(qq_cut); 
  eventSelector->enable_all();


  // ------------------------------------------------------------
  //       Configure local vars from command line options 
  // ------------------------------------------------------------
  string runMode             = options->args["TYPE"].args; 
  string fileList            = options->args["LIST"].args; 
  string outputFilename      = options->args["OUT"].args;
  int numberOfFilesToProcess = options->args["N"].arg; 

  Parameters *pars = new Parameters();
  pars->loadParameters(options->args["PARS"].args);


  //  Load list of files from appropriate source 
  if (numberOfFilesToProcess == 0){ cout << " No files/list detected. " << endl; return 0; }
  vector<string> files;

  if (fileList != "UNSET") {
    files = loadFilesFromList(fileList, numberOfFilesToProcess);
  } else {
    files = loadFilesFromCommandLine(options, numberOfFilesToProcess);
  }

  // Start running the correct type
  if (runMode == "data"){
    MomCorr_e1f * momentumCorrection = new MomCorr_e1f("/u/home/dmriser/mydoc/analysis/root_scripts/Analysis_v2/momCorr/"); 
    DataLoader loader(eventSelector, momentumCorrection, pars, outputFilename, "RECREATE");
    for (int ifile = 0; ifile < files.size(); ifile++) { loader.AddFile(files[ifile]); }
    loader.Execute();

    DInformation *runInformation = new DInformation();
    runInformation->load("/u/home/dmriser/mydoc/analysis/root_scripts/Analysis_v2/lists/runsNew.info"); 

    FaradayCupAppender chargeAppender(runInformation);
    chargeAppender.AddFiles(files);
    chargeAppender.AppendToFile(outputFilename);
  }

  else if (runMode == "mcrad"){
    MCLoader loader(eventSelector, pars, outputFilename, "UPDATE", "Rad");
    for (int ifile = 0; ifile < files.size(); ifile++) { loader.AddFile(files[ifile]); }
    loader.Execute();
  }

  else if (runMode == "mcnorad"){
    MCLoader loader(eventSelector, pars, outputFilename, "UPDATE", "NoRad");
    for (int ifile = 0; ifile < files.size(); ifile++) { loader.AddFile(files[ifile]); }
    loader.Execute();
  }
  
  // Elastic subtraction elastic events
  else if (runMode == "es_elast"){
    MCLoader loader(eventSelector, pars, outputFilename, "UPDATE", "Elastic");
    for (int ifile = 0; ifile < files.size(); ifile++) { loader.AddFile(files[ifile]); }
    loader.Execute();
  }

  // Elastic subtraction inelastic events
  else if (runMode == "es_inelast"){
    MCLoader loader(eventSelector, pars, outputFilename, "UPDATE", "Inelastic");
    for (int ifile = 0; ifile < files.size(); ifile++) { loader.AddFile(files[ifile]); }
    loader.Execute();
  }
  
  else {
    cout << "[FillHistograms] TYPE not recognized: " << runMode << endl;
    return 0; 
  }


  return 0;
}

void configureCommandLineOptions(h22Options *theseOpts){
  theseOpts->args["N"].arg = 0;
  theseOpts->args["N"].type = 0;
  theseOpts->args["N"].name = "Number of files to process";
 
  theseOpts->args["YCUT"].arg = 1.00;
  theseOpts->args["YCUT"].type = 0;
  theseOpts->args["YCUT"].name = "Y-Cut";
 
  theseOpts->args["PMIN"].arg = 0.50;
  theseOpts->args["PMIN"].type = 0;
  theseOpts->args["PMIN"].name = "Minimum track p ";
  
  theseOpts->args["TYPE"].args = "UNSET";
  theseOpts->args["TYPE"].type = 1;
  theseOpts->args["TYPE"].name = "Processing mode";
    
  theseOpts->args["LIST"].args = "UNSET";
  theseOpts->args["LIST"].type = 1;
  theseOpts->args["LIST"].name = "Process list of files";

  theseOpts->args["PARS"].args = "/u/home/dmriser/mydoc/analysis/root_scripts/Analysis_v2/pid/calib/eid/v2/data.pars";
  theseOpts->args["PARS"].type = 1;
  theseOpts->args["PARS"].name = "Particle ID parameters file";
}

vector<string> loadFilesFromList(string fileList, int numFiles){
  vector<string> theseFiles; 

  ifstream inputFile; 
  inputFile.open(fileList.c_str());

  int ifile = 0; string line;
  while (getline(inputFile, line) && ifile < numFiles){
    theseFiles.push_back(line);
    ifile++;
  }
  
  inputFile.close();
  return theseFiles; 
}

vector<string> loadFilesFromCommandLine(h22Options *theseOpts, int numFiles){
  vector<string> theseFiles; 

  for(int ifile = 0; ifile < theseOpts->ifiles.size(); ifile++){
    theseFiles.push_back(theseOpts->ifiles[ifile]);
    ifile++;

    if (ifile == numFiles){ break; }
  }

  return theseFiles; 
}

int PrintUsage(){

  cout << endl;
  cout << endl; 
  cout << "######################################################" << endl;
  cout << "#                                                    #" << endl;
  cout << "#    fillHistograms -> Used to fill DIS histograms   #" << endl;
  cout << "#                                                    #" << endl;
  cout << "#  This code requires a set of files specified by    #" << endl;
  cout << "#  a list using -LIST=files.dat, or by wildcard      #" << endl;
  cout << "#  after the option flags.                           #" << endl;
  cout << "#                                                    #" << endl;
  cout << "#                                                    #" << endl;
  cout << "#  Set the number of files using the -N= flag, and   #" << endl;
  cout << "#  set the run type using the -TYPE= flag.           #" << endl; 
  cout << "#                                                    #" << endl;
  cout << "#  Valid types are data, mcnorad, mcrad.             #" << endl;
  cout << "#                                                    #" << endl;
  cout << "#  Set the output file using the -OUT= flag.         #" << endl;
  cout << "#                                                    #" << endl;
  cout << "#  Sample usage:                                     #" << endl;
  cout << "#  ./fillHistograms -N=1000 -LIST=goodFiles.dat      #" << endl;
  cout << "#   -OUT=pass1.root                                  #" << endl;
  cout << "#                                                    #" << endl;
  cout << "######################################################" << endl;
  cout << endl;
  cout << endl;
  
  return 0;
} 
