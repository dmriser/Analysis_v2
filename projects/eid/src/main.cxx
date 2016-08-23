///////////////////////////////////////////////////
/*

  David Riser, University of Connecticut 
  August 23, 2016 

  main.cxx -> Electron Identification main code

*/
///////////////////////////////////////////////////

// c++ includes
#include <iostream>
#include <vector>
using std::cout;
using std::endl;
using std::string;
using std::vector; 

// my includes
#include "CommonTools.h"
#include "DEvent.h"
#include "ElectronIDPackage.h"
#include "h22Option.h"

int main(int argc, char * argv[])
{

  // Configure running from command line options 
  h22Options opts;
  opts.set(argc,argv);
  int nfiles     = opts.args["N"].arg;
  string parfile = opts.args["EPARS"].args;
  string outfile = opts.args["OUT"].args; 
  nfiles = smallest(nfiles, opts.ifiles.size());

  // Create an instance of the ElectronIDManager and start loading files
  ElectronIDManager manager;
  for (int ifile=0; ifile<nfiles; ifile++) { manager.AddFile(opts.ifiles[ifile]); } 

  ElectronIDHistograms histos;
  histos.init();
  histos.set_info(manager.runno(), manager.GSIM);
  
  // Customize Cuts
  histos.selector.enable_all();
  //  histos.selector.disable_by_regex("EC");
  //  histos.selector.disable_by_regex("Samp");
  
  manager.init();
  manager.set_parfile(parfile);
  //  manager.calculate_values();

  int runno = 0;
  for (int iev=0; iev<manager.GetEntries(); iev++)
    {
      manager.GetEntry(iev);
      if (runno != manager.runno()) { runno = manager.runno(); histos.set_info(runno,manager.GSIM); } 
      DEvent event( manager.GetEvent() ); 
      histos.fill(event);
    }

  histos.selector.summarize();
  histos.write_and_close();
  
  return 0;
}
