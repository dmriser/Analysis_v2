#include <iostream>
#include <vector> 
#include <fstream>
using namespace std; 

#include "DBins.h"
#include "DISPackage.h"
#include "Dumper.h"

int main(int argc, char * argv[])
{

  ifstream mc_list("mc.dat");      vector<string> mc_files;
  ifstream data_list("data.dat");  vector<string> data_files;

  string line; 
  while( getline(mc_list, line) )  { mc_files.push_back(line);   }  mc_list.close();
  while( getline(data_list, line) ){ data_files.push_back(line); }  data_list.close();

  // Binning Scheme 
  DBins xBins(10, 0.05, 1.05);
  DBins qqBins(10, 0.5, 6.0); 
  DBins wBins(10, 0.5, 6.0); 

  DISManager manager; 
  manager.add_files(data_files, 0);
  manager.add_files(mc_files,   1);
  manager.parfile[0] = "epars.dat";
  manager.parfile[1] = "epars.dat"; 
  manager.outfile = "fix.root";
  manager.eid_version = 0; 
  
  manager.set_bins(xBins, qqBins, wBins);
  manager.qq_cut->set_min(1.0);
  manager.qq_cut->set_max(50.0);
  manager.w_cut ->set_min(1.0);
  manager.w_cut ->set_max(50.0);  
  manager.init();
  
  cout << " Length of lists: data=" << manager.reader[0].GetEntries() << " mc=" << manager.reader[1].GetEntries() << endl; 
  cout << " Detected information from readers : " << endl;
  cout << " Reader 0 - Type: " << manager.reader[0].GSIM << " Run Number " << manager.reader[0].runno() << endl;
  cout << " Reader 1 - Type: " << manager.reader[1].GSIM << " Run Number " << manager.reader[1].runno() << endl; 

  manager.loop(0);
  manager.loop(1);
  manager.dis_selector.summarize(); 

  if (manager.eid_version == 1) {
    manager.eid[0].summarize();
    manager.eid[1].summarize(); 
  }
  
  return 0;
}
