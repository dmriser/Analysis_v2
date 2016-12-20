/* ########################################################################
   #                                                                      #
   #               David Riser, University of Connecticut                 #
   #                      October 14, 201  6                              #
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

  // ---------------------------- Setup Physics Options -------------------------------

  PhysicsEventSelector * eventSelector = new PhysicsEventSelector();
  PhysicsEventCut_w * w_cut = new PhysicsEventCut_w();
  PhysicsEventCut_y * y_cut = new PhysicsEventCut_y();
  PhysicsEventCut_qq * qq_cut = new PhysicsEventCut_qq();

  w_cut->set_min( 2.05 );
  w_cut->set_max( 99.9 ); 
  y_cut->set_min( 0.00 );
  y_cut->set_max( 0.85 ); 
  qq_cut->set_min( 1.00 );
  qq_cut->set_max( 99.9 ); 

  eventSelector->add_cut(w_cut);
  eventSelector->add_cut(y_cut);
  eventSelector->add_cut(qq_cut); 
  eventSelector->enable_all();

  // ----------------------------------------------------------------------------------
  //  
  // ------------------------ Boring Bits that make shit work -------------------------
  //
  // Check for improper usage of this code. 
  if (argc < 3){
    return PrintUsage();
  }

  // Collect command line options, configure ours and compare.
  h22Options * options = new h22Options(); 
  configureCommandLineOptions(options); 
  options->set(argc, argv);

  // Set local variables to command line flags. 
  string fileList = options->args["LIST"].args; 
  string outputFilename = options->args["OUT"].args;
  int numberOfFilesToProcess = options->args["N"].arg; 

  Parameters *pars = new Parameters();
  pars->loadParameters(options->args["PARS"].args); 

  // Check for no files, then setup the list of files to process correctly. 
  if (numberOfFilesToProcess == 0){ cout << " No files/list detected. " << endl; return 0; }
  vector<string> files;

  if (fileList != "UNSET") {
    files = loadFilesFromList(fileList, numberOfFilesToProcess);
  } else {
    files = loadFilesFromCommandLine(options, numberOfFilesToProcess);
  }

  // Start running the correct type
  MCLoader loader(eventSelector, pars, outputFilename, "UPDATE", "200Bins");
  for (int ifile = 0; ifile < files.size(); ifile++) { loader.AddFile(files[ifile]); }
  loader.Execute();

  MCLoader loader2(eventSelector, pars, outputFilename, "UPDATE", "100Bins");
  for (int ifile = 0; ifile < files.size(); ifile++) { loader2.AddFile(files[ifile]); }
  loader2.Initialize();
  loader2.Rebin(2,2);
  loader2.Loop();
  loader2.Save();

  MCLoader loader3(eventSelector, pars, outputFilename, "UPDATE", "50Bins");
  for (int ifile = 0; ifile < files.size(); ifile++) { loader3.AddFile(files[ifile]); }
  loader3.Initialize();
  loader3.Rebin(4,4);
  loader3.Loop();
  loader3.Save();

  MCLoader loader4(eventSelector, pars, outputFilename, "UPDATE", "40Bins");
  for (int ifile = 0; ifile < files.size(); ifile++) { loader4.AddFile(files[ifile]); }
  loader4.Initialize();
  loader4.Rebin(5,5);
  loader4.Loop();
  loader4.Save();

  MCLoader loader5(eventSelector, pars, outputFilename, "UPDATE", "34Bins");
  for (int ifile = 0; ifile < files.size(); ifile++) { loader5.AddFile(files[ifile]); }
  loader5.Initialize();
  loader5.Rebin(6,6);
  loader5.Loop();
  loader5.Save();

  MCLoader loader6(eventSelector, pars, outputFilename, "UPDATE", "20Bins");
  for (int ifile = 0; ifile < files.size(); ifile++) { loader6.AddFile(files[ifile]); }
  loader6.Initialize();
  loader6.Rebin(10,10);
  loader6.Loop();
  loader6.Save();

  MCLoader loader7(eventSelector, pars, outputFilename, "UPDATE", "66Bins");
  for (int ifile = 0; ifile < files.size(); ifile++) { loader7.AddFile(files[ifile]); }
  loader7.Initialize();
  loader7.Rebin(3,3);
  loader7.Loop();
  loader7.Save();

  MCLoader loader8(eventSelector, pars, outputFilename, "UPDATE", "10Bins");
  for (int ifile = 0; ifile < files.size(); ifile++) { loader8.AddFile(files[ifile]); }
  loader8.Initialize();
  loader8.Rebin(20,20);
  loader8.Loop();
  loader8.Save();


  return 0;
}

void configureCommandLineOptions(h22Options * theseOpts){

  theseOpts->args["N"].arg = 0;
  theseOpts->args["N"].type = 0;
  theseOpts->args["N"].name = "Number of files to process";
  
  theseOpts->args["LIST"].args = "UNSET";
  theseOpts->args["LIST"].type = 1;
  theseOpts->args["LIST"].name = "Process list of files";
  
  theseOpts->args["PARS"].args = "/u/home/dmriser/mydoc/analysis/root_scripts/Analysis_v2/lists/keppelRad.pars";
  theseOpts->args["PARS"].type = 1;
  theseOpts->args["PARS"].name = "Parameter file";
  
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

vector<string> loadFilesFromCommandLine(h22Options * theseOpts, int numFiles){
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
  cout << "#    fillPurityHistograms -> fills purity histograms #" << endl;
  cout << "#                                                    #" << endl;
  cout << "#  This code requires a set of files specified by    #" << endl;
  cout << "#  a list using -LIST=files.dat, or by wildcard      #" << endl;
  cout << "#  after the option flags.                           #" << endl;
  cout << "#                                                    #" << endl;
  cout << "#                                                    #" << endl;
  cout << "#  Set the number of files using the -N= flag        #" << endl;
  cout << "#                                                    #" << endl;
  cout << "#  Set the output file using the -OUT= flag.         #" << endl;
  cout << "#                                                    #" << endl;
  cout << "#  Sample usage:                                     #" << endl;
  cout << "#  ./fillPurityHistograms -N=1000                    #" << endl;
  cout << "#   -LIST=goodFiles.dat                              #" << endl;
  cout << "#   -OUT=pass1.root                                  #" << endl;
  cout << "#                                                    #" << endl;
  cout << "######################################################" << endl;
  cout << endl;
  cout << endl;
  
  return 0;
} 
