#include <iostream>
using namespace std;

#include "TApplication.h"
#include "h22Option.h"
#include "findElectronIDParameters.h"
#include "findElectronIDParameters.cxx"


int main(int argc, char * argv[]){

        findElectronIDParameters Analyzer;
        if (argc < 2) { return 0; }

	h22Options opts; 
	opts.args["GUI"].args = "false";
	opts.args["GUI"].type = 1;
	opts.args["GUI"].name = "Use GUI";
	opts.set(argc, argv); 

        for (int ifile=1; ifile<opts.ifiles.size(); ifile++){ Analyzer.AddFile(opts.ifiles[ifile]); } 
        Analyzer.Init(); 
        Analyzer.Loop(); 
	Analyzer.WriteHardCodedParameters(); 
	Analyzer.eidParameters.saveParameters("eidPars.dat"); 

	if (opts.args["GUI"].args == "true") {
	  TApplication app("app",&argc, argv); 
	  Analyzer.histos.Draw(); 
	  app.Run(); 
	}

        return 0; 
}
