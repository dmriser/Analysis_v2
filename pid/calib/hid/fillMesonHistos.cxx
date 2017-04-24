////////////////////////////////////////
/*
 David Riser, University of Connecticut
 
 July 13, 2016
 
 Modified:
 March 8, 2017
 April 14, 2017 

 meson.cxx -> fill histograms for mesons 
 
 */
////////////////////////////////////////

#include <iostream>

#include "CommonTools.h"
#include "Corrections.h"
#include "DataEventCut.h"
#include "h22Event.h"
#include "h22Option.h"
#include "h22Reader.h"
#include "GenericAnalysis.h"
#include "MesonHistograms.h"
#include "MomCorr.h"
#include "Parameters.h"
#include "ParameterSet.h"
#include "Pars.h"
#include "ParticleFilter.h"
#include "PhysicsEvent.h"
#include "PhysicsEventBuilder.h"
#include "StandardHistograms.h"

#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraphErrors.h"
#include "TH1.h"
#include "TH2.h"
#include "TLorentzVector.h"


vector<string> loadFilesFromList(string fileList, int numFiles);
vector<string> loadFilesFromCommandLine(h22Options * theseOpts, int numFiles);

class HIDCalibration : public GenericAnalysis {

public:
    HIDCalibration(h22Options *opts, Parameters *params) : GenericAnalysis(opts), pars(params) { }
    ~HIDCalibration(){ }

public:
  Parameters *pars;
  ParticleFilter *filter;
  Corrections corr;
  PhysicsEventBuilder *builder;   
  MomCorr_e1f *momCorr; 

  MesonHistograms *pipHistos; 
  MesonHistograms *pimHistos; 
  MesonHistograms *kpHistos; 
  MesonHistograms *kmHistos; 

  void Initialize();
  void ProcessEvent();
  void Save(string outputFilename);
  void InitHistos();
  bool CurrentParticleIsNotElectronCandidate(std::vector<int> &electronCandidates, int index);

};

void HIDCalibration::InitHistos() {
  pipHistos = new MesonHistograms("pip",  211);
  pimHistos = new MesonHistograms("pim", -211);
  kpHistos = new MesonHistograms("kp",    321);
  kmHistos = new MesonHistograms("km",   -321);
}

void HIDCalibration::Initialize(){
  InitHistos();
  
  filter = new ParticleFilter(pars);
  filter->set_info(GSIM, GetRunNumber());

  // not really safe for use on the farm because I don't set vars 
  // so not sure what to do about this.  maybe passing in the path
  // as a parameter would be ok? 
  std::string path = Global::Environment::GetAnalysisPath(); 
  std::string momCorrPath = Form("%s/momCorr/",path.c_str());
  momCorr = new MomCorr_e1f("/u/home/dmriser/Analysis_v2/momCorr/");
}

void HIDCalibration::ProcessEvent(){

    // Load up hadrons if we've electron.
    vector<int> electronCandidates = filter->getVectorOfParticleIndices(event, 11);
    if ( !electronCandidates.empty() ){
      
      // Take the fastest one
      int electronIndex = electronCandidates[0];
      event.SetElectronIndex(electronIndex); 
      corr.correctEvent(&event, GetRunNumber(), GSIM); 

      for (int ipart=0; ipart<event.gpart; ipart++){
	
	// This important line stops other electrons from
	// getting added to the plots.
 	if (CurrentParticleIsNotElectronCandidate(electronCandidates, ipart)) {
	  double dvz = fabs(event.corr_vz[electronIndex] - event.corr_vz[ipart]);
	  
	  TLorentzVector electron = event.GetTLorentzVector(electronIndex, 11);
	  TLorentzVector candidate = event.GetTLorentzVector(ipart, event.id[ipart]);

	  // correct the momenta 
	  electron = momCorr->PcorN(electron, -1, 11);
	  candidate = momCorr->PcorN(candidate, event.q[ipart], event.id[ipart]);

	  PhysicsEvent physEv = builder->getPhysicsEvent(electron, candidate); 

	  //	  if (dvz < 4.0  && event.etot[ipart] > 0.01){
	  if (dvz < 4.0){
	    if (event.q[ipart] == 1){ 
	      pipHistos->Fill(event, physEv, ipart); 
	      kpHistos->Fill(event, physEv, ipart);
	    }
	    else if (event.q[ipart] == -1){
	      pimHistos->Fill(event, physEv, ipart); 
	      kmHistos->Fill(event, physEv, ipart);
	    }
	  }

	}
      }
    }

}

bool HIDCalibration::CurrentParticleIsNotElectronCandidate(std::vector<int> &electronCandidates,int index){
    return (std::find(electronCandidates.begin(), electronCandidates.end(), index) == electronCandidates.end());
}

void HIDCalibration::Save(string outputFilename){
    TFile *outputFile = new TFile(outputFilename.c_str(), "recreate");

    pipHistos->Save(outputFile);
    kpHistos->Save(outputFile);
    pimHistos->Save(outputFile);
    kmHistos->Save(outputFile);

    outputFile->Write();
    outputFile->Close();
}

int main(int argc, char * argv[]){

    // Setup Options
    h22Options opts;
    opts.args["PARS"].args = "/u/home/dmriser/Analysis_v2/lists/data.pars";
    opts.args["PARS"].type = 1;
    opts.args["PARS"].name = "Parameter file";
    opts.args["LIST"].args = "UNSET";
    opts.args["LIST"].type = 1;
    opts.args["LIST"].name = "List of files";
    opts.set(argc,argv);

    int GSIM        = opts.args["MC"].arg;
    long int nev    = opts.args["N"].arg;
    string parfile  = opts.args["PARS"].args;

    Parameters pars;
    pars.loadParameters(opts.args["PARS"].args);

    std::vector<std::string> files; 
    if (opts.args["LIST"].args != "UNSET"){
      files = loadFilesFromList(opts.args["LIST"].args, 10000);
    } else {
      files = loadFilesFromCommandLine(&opts, 10000);
    }

    HIDCalibration Analysis(&opts, &pars);
    for (std::vector<std::string>::iterator it=files.begin(); it<files.end(); it++) { Analysis.AddFile(*it); }
    Analysis.RunAnalysis();
    Analysis.Save(opts.args["OUT"].args);

    return 0;
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
