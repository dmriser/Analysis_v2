////////////////////////////////////////
/*

 David Riser, University of Connecticut

 
 */
////////////////////////////////////////

#include <iostream>
#include <vector>
#include <map>

#include "CommonTools.h"
#include "Corrections.h"
#include "h22Event.h"
#include "h22Option.h"
#include "GenericAnalysis.h"
#include "MomCorr.h"
#include "MesonHistograms.h"
#include "Parameters.h"
#include "ParticleFilter.h"
#include "PhysicsEvent.h"
#include "PhysicsEventBuilder.h"
#include "StandardHistograms.h"

#include "common/Histograms.h"
#include "common/PIDHistograms.h"
#include "common/Constants.h"
#include "common/Types.h"

#include "TFile.h"
#include "TLorentzVector.h"
#include "TF1.h"
#include "TH1.h"

std::vector<std::string> loadFilesFromList(std::string fileList, int numFiles);
std::vector<std::string> loadFilesFromCommandLine(h22Options * theseOpts, int numFiles);

class Analysis : public GenericAnalysis {

public:
    Analysis(h22Options *opts, Parameters *params) : GenericAnalysis(opts), pars(params) { }
    ~Analysis(){ }

public:
  Parameters          *pars;
  ParticleFilter      *filter;
  Corrections          corr;
  PhysicsEventBuilder *builder;   
  MomCorr_e1f         *momCorr; 
  

  // setup the histogramming package 
  Histos              *histos[constants::NMESON]; 
  MesonHistograms     *mesonHistos[constants::NMESON];
  StandardHistograms  *standardHistos[constants::NMESON]; 
  PidHistos           *pidHistos[constants::NMESON]; 

  void Initialize();
  void ProcessEvent();
  void Save(std::string outfile);
  void InitHistos();
};

void Analysis::InitHistos() {
  histos[Meson::kPionNegative] = new Histos("test", Meson::kPionNegative); 
  histos[Meson::kPionPositive] = new Histos("test", Meson::kPionPositive); 
  histos[Meson::kKaonNegative] = new Histos("test", Meson::kKaonNegative); 
  histos[Meson::kKaonPositive] = new Histos("test", Meson::kKaonPositive); 

  pidHistos[Meson::kPionNegative] = new PidHistos("test", Meson::kPionNegative); 
  pidHistos[Meson::kPionPositive] = new PidHistos("test", Meson::kPionPositive); 
  pidHistos[Meson::kKaonNegative] = new PidHistos("test", Meson::kKaonNegative); 
  pidHistos[Meson::kKaonPositive] = new PidHistos("test", Meson::kKaonPositive); 

  for (int m=0; m<constants::NMESON; m++){
    mesonHistos[m]    = new MesonHistograms(constants::Names::mesons[m], constants::PID::pid[m]); 
    standardHistos[m] = new StandardHistograms(constants::Names::mesons[m], 0);
  }
}

void Analysis::Initialize(){
  InitHistos();
  
  filter = new ParticleFilter(pars);
  filter->set_info(GSIM, GetRunNumber());

  builder = new PhysicsEventBuilder(); 

  // not really safe for use on the farm because I don't set vars 
  // so not sure what to do about this.  maybe passing in the path
  // as a parameter would be ok? 
  std::string path        = Global::Environment::GetAnalysisPath(); 
  std::string momCorrPath = Form("%s/momCorr/",path.c_str());
  momCorr                 = new MomCorr_e1f(momCorrPath);
}

void Analysis::ProcessEvent(){
    // Load up hadrons if we've electron.
  std::vector<int> electronCandidates = filter->getVectorOfParticleIndices(event, 11);

  if ( !electronCandidates.empty()){
    
    // Take the fastest one
    int electronIndex = electronCandidates[0];
    event.SetElectronIndex(electronIndex); 
    corr.correctEvent(&event, GetRunNumber(), GSIM); 

    std::vector<int> kps = filter->getVectorOfParticleIndices(event,  321);
    std::vector<int> kms = filter->getVectorOfParticleIndices(event, -321);
    std::vector<int> pps = filter->getVectorOfParticleIndices(event,  211);
    std::vector<int> pms = filter->getVectorOfParticleIndices(event, -211);
    
    int helicity = -1; 
    if (event.corr_hel > 0)      { helicity = Helicity::kPositive; }
    else if (event.corr_hel < 0) { helicity = Helicity::kNegative; }

    TLorentzVector electron  = event.GetTLorentzVector(electronIndex, 11); 
    electron = momCorr->PcorN(electron, -1, 11);    

    PhysicsEvent electron_ev = builder->getPhysicsEvent(electron); 

    if (!kps.empty() && helicity > -1) {
      
      TLorentzVector meson = event.GetTLorentzVector(kps[0], 321);
      //    pion = momCorr->PcorN(pion, 1, 211);
       
      PhysicsEvent ev = builder->getPhysicsEvent(electron, meson);
      
      //      if (ev.w > 2.00 && ev.qq > 1.00 && ev.mm2 > 1.1 && ev.z > 0.3 && event.p[kps[0]] < 2.05) {
      if (ev.w > 2.00 && ev.qq > 1.00 && ev.mm2 > 1.1 && ev.z > 0.3) {
	histos[Meson::kKaonPositive]        ->Fill(ev, helicity);
	mesonHistos[Meson::kKaonPositive]   ->Fill(event, ev, kps[0]); 
	standardHistos[Meson::kKaonPositive]->Fill(event, electronIndex, kps[0], ev); 
	pidHistos[Meson::kKaonPositive]     ->Fill(event, ev, kps[0]);
	pidHistos[Meson::kPionPositive]     ->Fill(event, ev, kps[0]);
      }
    }

    if (!pms.empty() && helicity > -1) {
      TLorentzVector meson = event.GetTLorentzVector(pms[0], -211);
      //    pion = momCorr->PcorN(pion, 1, 211);
       
      PhysicsEvent ev = builder->getPhysicsEvent(electron, meson);
      
      if (ev.w > 2.00 && ev.qq > 1.00 && ev.mm2 > 1.1 && ev.z > 0.3){
	histos[Meson::kPionNegative]        ->Fill(ev, helicity);
	mesonHistos[Meson::kPionNegative]   ->Fill(event, ev, pms[0]); 
	standardHistos[Meson::kPionNegative]->Fill(event, electronIndex, pms[0], ev); 
	pidHistos[Meson::kPionNegative]     ->Fill(event, ev, pms[0]);
	pidHistos[Meson::kKaonNegative]     ->Fill(event, ev, pms[0]);
      }
    }

    if (!pps.empty() && helicity > -1) {
      TLorentzVector meson = event.GetTLorentzVector(pps[0], 211);
      //    pion = momCorr->PcorN(pion, 1, 211);
      
      PhysicsEvent ev = builder->getPhysicsEvent(electron, meson);
      
      if (ev.w > 2.00 && ev.qq > 1.00 && ev.mm2 > 1.1 && ev.z > 0.3) {
	histos[Meson::kPionPositive]        ->Fill(ev, helicity);
	mesonHistos[Meson::kPionPositive]   ->Fill(event, ev, pps[0]); 
	standardHistos[Meson::kPionPositive]->Fill(event, electronIndex, pps[0], ev); 
	pidHistos[Meson::kPionPositive]     ->Fill(event, ev, pps[0]);
	pidHistos[Meson::kKaonPositive]     ->Fill(event, ev, pps[0]);
      }
    }

    if (!kms.empty() && helicity > -1) {
      TLorentzVector meson = event.GetTLorentzVector(kms[0], -321);
      //    pion = momCorr->PcorN(pion, 1, 211);
       
      PhysicsEvent ev = builder->getPhysicsEvent(electron, meson);
      
      if (ev.w > 2.00 && ev.qq > 1.00 && ev.mm2 > 1.1 && ev.z > 0.3) {
	histos[Meson::kKaonNegative]        ->Fill(ev, helicity);
	mesonHistos[Meson::kKaonNegative]   ->Fill(event, ev, kms[0]); 
	standardHistos[Meson::kKaonNegative]->Fill(event, electronIndex, kms[0], ev); 
	pidHistos[Meson::kKaonNegative]     ->Fill(event, ev, kms[0]);
	pidHistos[Meson::kPionNegative]     ->Fill(event, ev, kms[0]);
      }
    }

  }
}

void Analysis::Save(std::string outfile){
    histos[0]->Save(outfile, "recreate"); 

  for(int m=1; m<constants::NMESON; m++) {
    histos[m]->Save(outfile, "update"); 
  }

  for(int m=0; m<constants::NMESON; m++) {
    pidHistos[m]->Save(outfile, "update"); 
  }

  TFile *out = new TFile(outfile.c_str(), "update"); 
  for(int m=0; m<constants::NMESON; m++) {
    mesonHistos[m]   ->Save(out); 
    standardHistos[m]->Save(out); 
  }

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

  Parameters pars;
  pars.loadParameters(opts.args["PARS"].args);
  
  std::vector<std::string> files; 
  if (opts.args["LIST"].args != "UNSET"){
    files = loadFilesFromList(opts.args["LIST"].args, 10000);
  } else {
    files = loadFilesFromCommandLine(&opts, 10000);
  }

  Analysis *Ana = new Analysis(&opts, &pars); 
  for (int i=0; i<files.size(); i++) {   
    Ana->AddFile(files[i]); 
  }
  Ana->RunAnalysis();
  //  Ana->histos[Meson::kKaonPositive]->CalculateAsymmetry();
  //  Ana->histos[Meson::kPionPositive]->CalculateAsymmetry();
  //  Ana->histos[Meson::kKaonNegative]->CalculateAsymmetry();
  //  Ana->histos[Meson::kPionNegative]->CalculateAsymmetry();
  Ana->Save(opts.args["OUT"].args); 
  
  return 0;
}

std::vector<std::string> loadFilesFromList(std::string fileList, int numFiles){
  std::vector<std::string> theseFiles;
  
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

std::vector<std::string> loadFilesFromCommandLine(h22Options * theseOpts, int numFiles){
  std::vector<std::string> theseFiles;

  for(int ifile = 0; ifile < theseOpts->ifiles.size(); ifile++){
    theseFiles.push_back(theseOpts->ifiles[ifile]);

    if (ifile == numFiles){ break; }
  }

  return theseFiles;
}
