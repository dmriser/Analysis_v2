#include <iostream>
using namespace std;

#include "EventMonitor.h"
#include "EventMonitor.cxx"

#include "h22Option.h"

int main(int argc, char * argv[]){


  if (argc < 2) { return 0; }
  
  // Setup command line options.
  h22Options opts; 
  opts.args["TYPE"].args = "rec";
  opts.args["TYPE"].type = 1;
  opts.args["TYPE"].name = "Data type";
  opts.set(argc, argv);

  EventMonitor *Analyzer;
  bool isGen = false; 

  if (opts.args["TYPE"].args == "gen") { isGen = true; }
  Analyzer = new EventMonitor(isGen);

  for (int ifile=1; ifile<opts.ifiles.size(); ifile++){ Analyzer->AddFile(opts.ifiles[ifile]); } 
  Analyzer->Init(); 
  Analyzer->Loop(); 
  Analyzer->Save(opts.args["OUT"].args);

  delete Analyzer;
  return 0; 
	
}
