#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

// Mine
#include "CommonTools.h"
#include "DBins.h"
#include "h22Option.h"
#include "GenericAnalysis.h"
#include "Parameters.h"
#include "ParticleFilter.h"
#include "PhysicsEvent.h"
#include "PhysicsEventBuilder.h"

// Root
#include "TString.h"
#include "TLorentzVector.h"
#include "TH1.h"
#include "TH2.h"
#include "THnSparse.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "TPad.h"
#include "TStyle.h"

// ----------------------------------------------
/* 
   Run file-by-file and remove events that 
   fall in bad events window, save file. 

 */
// ----------------------------------------------
vector<string> loadFilesFromList(string fileList, int numFiles);

class MyAnalysis : public GenericAnalysis {
public:
  MyAnalysis(Parameters *pars){ filter = new ParticleFilter(pars); } 
  ~MyAnalysis(){ }
  

protected:
  ParticleFilter  *filter;
  TFile           *outputFile; 
  TChain          *clonedChain; 
  TTree           *clonedTree; 

  int iBadWindow; 
  vector<int> badEventStart;
  vector<int> badEventEnd;

public:
  void ProcessEvent();
  void Initialize();
  void LoadBadEventsFile(); 
  void Save();
  bool SkimCriteriaPasses();
};

// Example override of Init using 
// the methods of h22Reader runno()
// and GSIM const.
void MyAnalysis::Initialize(){
  filter->set_info(runno(), GSIM);

  outputFile  = new TFile(Form("/volatile/clas12/dmriser/analysis/e1f_analysis/skim/allGoodEventsV2/%d.root",runno()),"RECREATE");
  clonedChain = (TChain*) fchain->CloneTree(0);
  clonedTree  = clonedChain->GetTree();

  cout << "[MyAnalysis::Init] Initialization done" << endl;
}

void MyAnalysis::LoadBadEventsFile(){
  iBadWindow  = 0;
  ifstream eventQCFile(Form("/volatile/clas12/dmriser/analysis/e1f_analysis/badEvents/%d.txt",runno()));
    
  string line, buffer;
  while(getline(eventQCFile,line)){
      istringstream iss(line);
      int istring = 0;

      while(iss){
	  iss >> buffer;
	  if(istring == 0){
	      int s = atoi(buffer.c_str());
	      badEventStart.push_back(s);
	    }
	  else if (istring == 1){
	      int e = atoi(buffer.c_str());
	      badEventEnd.push_back(e);
	    }
	  istring++;
      }
  }
  eventQCFile.close();

  cout << "[MyAnalysis::LoadBadEventsFile] Found " << badEventStart.size() << " bad event windows for run " << runno() << endl;
}

void MyAnalysis::ProcessEvent(){  
  if(SkimCriteriaPasses()){
    clonedTree->Fill();
  }
}

 bool MyAnalysis::SkimCriteriaPasses(){
   if (iBadWindow < badEventStart.size()){
       // Catch times when we passed the current window
       if (event.evntid > badEventEnd[iBadWindow]) iBadWindow++;

       // We're in a bad event region, dQ = 0
       if ((event.evntid > badEventStart[iBadWindow]) && (event.evntid < badEventEnd[iBadWindow])){
	 return false;
       } else {
	 return true;
       }
     }
   
   return true;
}

void MyAnalysis::Save(){
  TString fileName = fchain->GetCurrentFile()->GetName();
  cout << "[MyAnalysis::Save] Saving " << fileName << " Old Events = " << fchain->GetEntries() << " New Events = " << clonedTree->GetEntries() << endl; 


  outputFile->Write(); 
  outputFile->Close(); 
}


int main(int argc, char *argv[]){

  h22Options *options = new h22Options();
  options->args["PARS"].args = "/u/home/dmriser/mydoc/analysis/root_scripts/Analysis_v2/lists/data.pars";
  options->args["PARS"].type = 1;
  options->args["PARS"].name = "Parameter file";

  options->args["LIST"].args = "/u/home/dmriser/mydoc/analysis/root_scripts/Analysis_v2/projects/dis2/allGoodEvents.dat";
  options->args["LIST"].type = 1;
  options->args["LIST"].name = "File list";
  options->set(argc, argv);

  Parameters *pars = new Parameters(); 
  pars->loadParameters(options->args["PARS"].args); 

  vector<string> filesToProcess = loadFilesFromList(options->args["LIST"].args, options->args["N"].arg);;
 
  for (int i=0; i<filesToProcess.size(); i++) { 
    MyAnalysis *analysis = new MyAnalysis(pars);
    analysis->AddFile(filesToProcess[i]); 

    if (analysis->RunAnalysis(1e9)){
      //      analysis->Save();
    } else {
      cout << "[SkimOutBadEvents] Warning: File " << filesToProcess[i] << " failed to be skimmed. " << endl;
    }

    delete analysis; 
  } 

  return 0;
}

vector<string> loadFilesFromList(string fileList, int numFiles){
  vector<string> theseFiles;

  std::ifstream inputFile;
  inputFile.open(fileList.c_str());

  int ifile = 0; string line;
  while (getline(inputFile, line) && ifile < numFiles){
    theseFiles.push_back(line);
    ifile++;
  }

  inputFile.close();
  return theseFiles;
}
