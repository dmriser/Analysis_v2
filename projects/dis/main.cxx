#include <iostream>
#include <vector> 
#include <fstream>
#include <cstdlib>
using namespace std; 

#include "DBins.h"
#include "DISPackage.h"
#include "Dumper.h"
#include "h22Option.h"

int main(int argc, char * argv[]){
  
  // Configuring special options for this code. 
  h22Options options; 
  options.args["MACHINE"].args = "MAC";
  options.args["MACHINE"].type = 1;
  options.args["MACHINE"].name = "Machine being used";
  options.args["N"].arg        = 1;
  options.args["N"].name       = "Number of files"; 
  options.set(argc,argv);
  
  int n_files = options.args["N"].arg;
  
  ifstream mc_list;   vector<string> mc_files;
  ifstream data_list; vector<string> data_files;
  
  if (options.args["MACHINE"].args == "MAC") {
    mc_list.open("mc.dat");      
    data_list.open("data.dat"); 
  } else {
    mc_list.open("mc_farm.dat");      
    data_list.open("data_farm.dat"); 
  }
 
  string line; int ifile = 0; 
  while( getline(mc_list, line) && ifile < n_files)  { mc_files.push_back(line);   ifile++; }  mc_list.close();  ifile = 0;
  while( getline(data_list, line) && ifile < n_files){ data_files.push_back(line); ifile++; }  data_list.close();

  // Microscopic Binning Scheme for 2D Histograms 
  DBins xBins(200, 0.05, 0.65);
  DBins qqBins(200, 1.0, 5.0);
  DBins wBins(200, 1.1, 2.9); 

  DISManager manager(options.args["OUT"].args, true, xBins, qqBins, wBins); 
  manager.add_files(data_files, 0);
  manager.add_files(mc_files,   1);
  manager.infofile   = "runs.info";

  if (options.args["MACHINE"].args == "MAC") { manager.momcorr_path = "/Users/dmriser/Work/analysis/momCorr/"; }
  else                                       { manager.momcorr_path = "/u/home/dmriser/mydoc/analysis/root_scripts/Analysis_v2/momCorr/"; }
  
  //  manager.set_bins(xBins, qqBins, wBins);
  manager.qq_cut->set_min(1.0);
  manager.qq_cut->set_max(50.0);
  manager.w_cut ->set_min(1.1);
  manager.w_cut ->set_max(50.0);  
  manager.y_cut ->set_min(-0.01);
  manager.y_cut ->set_max(0.85);  
  manager.y_cut ->disable();
  manager.init();

  manager.loop(0);
  manager.loop(1);
  manager.dis_selector.summarize(); 
  manager.get_charge( data_files );
  manager.histos.save();

  return 0;
}
