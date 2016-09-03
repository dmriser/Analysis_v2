#include <iostream>
#include <vector> 
#include <fstream>
#include <cstdlib>
using namespace std; 

#include "DBins.h"
#include "DISPackage.h"
#include "Dumper.h"
#include "h22Option.h"

int main(int argc, char * argv[])
{
  // Configuring special options for this code! 
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

  // Binning Scheme 
  DBins xBins(40, 0.05, 0.65);
  DBins qqBins(10, 1.0, 5.0); 

  DISManager manager; 
  manager.add_files(data_files, 0);
  manager.add_files(mc_files,   1);
  manager.parfile[0] = "epars.dat";
  manager.parfile[1] = "epars.dat"; 
  manager.outfile    = "dis_devel";
  manager.infofile   = "runs.info";
  if (options.args["MACHINE"].args == "MAC") { manager.momcorr_path = "/Users/dmriser/Work/analysis/momCorr/"; }
  else { manager.momcorr_path = "/u/home/dmriser/mydoc/analysis/root_scripts/Analysis_v2/momCorr/"; }
  manager.eid_version = 0;
  
  manager.set_bins(xBins, qqBins);
  manager.qq_cut->set_min(1.0);
  manager.qq_cut->set_max(50.0);
  manager.w_cut ->set_min(2.05);
  manager.w_cut ->set_max(50.0);  
  manager.y_cut ->set_min(-0.01);
  manager.y_cut ->set_max(0.85);  
  manager.init();
  
  cout << " Length of lists: data=" << manager.reader[0].GetEntries() << " mc=" << manager.reader[1].GetEntries() << endl; 
  cout << " Detected information from readers : " << endl;
  cout << " Reader 0 - Type: " << manager.reader[0].GSIM << " Run Number " << manager.reader[0].runno() << endl;
  cout << " Reader 1 - Type: " << manager.reader[1].GSIM << " Run Number " << manager.reader[1].runno() << endl; 

  manager.loop(0);
  manager.loop(1);
  manager.dis_selector.summarize(); 
  manager.get_charge(data_files); 
  manager.fill_model();
  manager.do_xs();
  manager.histos.draw();
  manager.histos.save();

  if (manager.eid_version == 1) {
    manager.eid[0].summarize();
    manager.eid[1].summarize(); 
  }

  cout << " Total charge in (uC): " << manager.fcup_charge << endl; 

  manager.print_table(); 
  
  return 0;
}
