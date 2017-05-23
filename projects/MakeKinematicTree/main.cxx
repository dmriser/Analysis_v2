#include <iostream>
using namespace std;

#include "KinematicTree.h"
#include "KinematicTree.cxx"

#include "h22Option.h"
#include "Parameters.h"

int main(int argc, char *argv[]){

  h22Options opts; 
  Parameters pars; 
  pars.loadParameters("/u/home/dmriser/Analysis_v2/lists/data.pars"); 
  
  opts.set(argc, argv);

  if (argc < 2){ return 0; }

  KinematicTree treeWriter(&opts, &pars); 
  for (int i=0; i<opts.ifiles.size(); ++i) { treeWriter.AddFile( opts.ifiles[i] ); }
  //  treeWriter.isMonteCarlo = true; 
  treeWriter.SetupTree();
  treeWriter.RunAnalysis();
  treeWriter.Save(opts.args["OUT"].args); 

  return 0;
}
