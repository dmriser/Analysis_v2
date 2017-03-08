// --------------------------------------
//     David Riser
// --------------------------------------

// --------------------------------------
//     c++ 
// --------------------------------------
#include <iostream>
#include <vector>
using namespace std; 

// --------------------------------------
//     my libs 
// --------------------------------------
#include "h22Option.h"
#include "Printer.h"
#include "Tools.h"

// --------------------------------------
//     root 
// --------------------------------------
#include "TFile.h"
#include "TH1.h"

int main(int argc, char *argv[]){

  h22Options opts; 

  opts.args["XTITLE"].args = "x";
  opts.args["XTITLE"].type =  1;
  opts.args["XTITLE"].name = "X axis title";

  opts.args["YTITLE"].args = "y";
  opts.args["YTITLE"].type =  1;
  opts.args["YTITLE"].name = "Y axis title";

  opts.args["TITLE"].args = "title";
  opts.args["TITLE"].type =  1;
  opts.args["TITLE"].name = "title";

  opts.args["STYLE"].args = "default";
  opts.args["STYLE"].type =  1;
  opts.args["STYLE"].name = "Style of histos";

  opts.args["LAYOUT"].args = "default";
  opts.args["LAYOUT"].type =  1;
  opts.args["LAYOUT"].name = "Layout of histos";

  opts.args["HISTO"].args = "none";
  opts.args["HISTO"].type =  1;
  opts.args["HISTO"].name = "Histogram title to get from file";

  opts.args["PATH"].args = "/volatile/clas12/dmriser/plots/";
  opts.args["PATH"].type =  1;
  opts.args["PATH"].name = "Path to output";

  opts.args["PATH"].args = "/volatile/clas12/dmriser/plots/";
  opts.args["PATH"].type =  1;
  opts.args["PATH"].name = "Path to output";

  opts.set(argc, argv);

  if(!opts.ifiles.empty()){
    SimplePlot::Tools tools; 
    SimplePlot::Printer printer;

    printer.setPath(opts.args["PATH"].args);
    printer.setOutputTitle(opts.args["OUT"].args);
    printer.setStyle(opts.args["STYLE"].args);

    if(opts.ifiles.size() > 1){
      cout << "Support not offered yet." << endl; 
    } else {
      
      

      TFile *f = TFile::Open(opts.ifiles[0].c_str());
      if (f->IsOpen()){
	vector<TH1D*> histos = tools.loadVectorOfTH1D(f, opts.args["HISTO"].args);

	if(!histos.empty()){
	  cout << "Loaded " << histos.size() << "histos from file" << endl; 

	  if(opts.args["LAYOUT"].args == "bins"){
	    printer.printBins(histos, opts.args["TITLE"].args, opts.args["XTITLE"].args, opts.args["YTITLE"].args);
	  } else if (opts.args["LAYOUT"].args == "sector" && histos.size() == 6){
	    printer.printSector(histos.data(), opts.args["TITLE"].args, opts.args["XTITLE"].args, opts.args["YTITLE"].args)
	  }

	} else {
	  cout << "Failed to load histos from file." << endl; 
	}

      }

    }


  } else {
    cout << "No input files detected." << endl; 
  }

}
