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
#include "h22ElectronEvent.h"
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
  Parameters          *pars;
  ParticleFilter      *filter;
  Corrections          corr;
  PhysicsEventBuilder *builder;   
  MomCorr_e1f         *momCorr; 

  MesonHistograms *posHistos; 
  MesonHistograms *negHistos; 
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
  posHistos = new MesonHistograms("pos",  321);
  negHistos = new MesonHistograms("neg", -321);
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

  vector<int> electronCandidates = filter->getVectorOfParticleIndices(event, 11);
  if ( !electronCandidates.empty() ){
    int electronIndex = electronCandidates[0];
    h22ElectronEvent electronEvent(event); 
    electronEvent.SetElectronIndex(electronIndex); 
    corr.CorrectElectronEvent(&electronEvent, GetRunNumber(), GSIM); 
    
    std::vector<int> pipIndices = filter->getVectorOfParticleIndices(electronEvent, 211); 
    std::vector<int> pimIndices = filter->getVectorOfParticleIndices(electronEvent, -211); 
    std::vector<int> kpIndices = filter->getVectorOfParticleIndices(electronEvent, 321); 
    std::vector<int> kmIndices = filter->getVectorOfParticleIndices(electronEvent, -321); 
    
    TLorentzVector electron = event.GetTLorentzVector(electronIndex, 11); 
    electron = momCorr->PcorN(electron, -1, 11);

    if (!pipIndices.empty()){
      TLorentzVector meson = event.GetTLorentzVector(pipIndices[0], 211);
      meson = momCorr->PcorN(meson, 1, 211);

      PhysicsEvent physicsEvent = builder->getPhysicsEvent(electron, meson); 
      pipHistos->Fill(electronEvent, physicsEvent, pipIndices[0]);
    }

    if (!pimIndices.empty()){
      TLorentzVector meson = event.GetTLorentzVector(pimIndices[0], -211);
      meson = momCorr->PcorN(meson, -1, -211);

      PhysicsEvent physicsEvent = builder->getPhysicsEvent(electron, meson); 
      pimHistos->Fill(electronEvent, physicsEvent, pimIndices[0]);
    }

    if (!kpIndices.empty()){
      TLorentzVector meson = event.GetTLorentzVector(kpIndices[0], 321);
      meson = momCorr->PcorN(meson, 1, 321);

      PhysicsEvent physicsEvent = builder->getPhysicsEvent(electron, meson); 
      kpHistos->Fill(electronEvent, physicsEvent, kpIndices[0]);
    }

    if (!kmIndices.empty()){
      TLorentzVector meson = event.GetTLorentzVector(kmIndices[0], -321);
      meson = momCorr->PcorN(meson, -1, -321);

      PhysicsEvent physicsEvent = builder->getPhysicsEvent(electron, meson); 
      kmHistos->Fill(electronEvent, physicsEvent, kmIndices[0]);
    }
    
  }
}

bool HIDCalibration::CurrentParticleIsNotElectronCandidate(std::vector<int> &electronCandidates,int index){
    return (std::find(electronCandidates.begin(), electronCandidates.end(), index) == electronCandidates.end());
}

void HIDCalibration::Save(string outputFilename){
    TFile *outputFile = new TFile(outputFilename.c_str(), "recreate");

    posHistos->Save(outputFile);
    negHistos->Save(outputFile);
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
