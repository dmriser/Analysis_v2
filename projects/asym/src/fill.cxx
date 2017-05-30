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
  Histos              *exclusiveHistos[constants::NMESON]; 
  MesonHistograms     *mesonHistos[constants::NMESON];
  StandardHistograms  *standardHistos[constants::NMESON]; 
  PidHistos           *pidHistos[constants::NMESON]; 
  PidHistos           *exclusivePidHistos[constants::NMESON]; 

  void Initialize();
  void ProcessEvent();
  void Save(std::string outfile);
  void InitHistos();

  bool IsInclusive(PhysicsEvent &ev, int mesonIndex){
    
    if (mesonIndex == Meson::kPionPositive || mesonIndex == Meson::kPionNegative){

      // heavier than neutron 
      return (ev.w > 2.00 && ev.qq > 1.00 && ev.mm2 > 1.1 && ev.z > 0.1); 
    } else if (mesonIndex == Meson::kKaonPositive || mesonIndex == Meson::kKaonNegative){

      // heavier than sigma 
      return (ev.w > 2.00 && ev.qq > 1.00 && ev.mm2 > 1.45 && ev.z > 0.1);
    }

    return false; 
  }

  bool IsExclusive(PhysicsEvent &ev, int mesonIndex){
    
    if (mesonIndex == Meson::kPionPositive || mesonIndex == Meson::kPionNegative){

      // lighter than neutron 
      return (ev.w > 2.00 && ev.qq > 1.00 && ev.mm2 < 1.1 && ev.z > 0.1); 
    } else if (mesonIndex == Meson::kKaonPositive || mesonIndex == Meson::kKaonNegative){

      // lighter than sigma 
      return (ev.w > 2.00 && ev.qq > 1.00 && ev.mm2 < 1.45 && ev.z > 0.1);
    }

    return false; 
  }

  void FillParticle(int index, int mesonIndex, int helicity, TLorentzVector electron){
    TLorentzVector meson = event.GetTLorentzVector(index, constants::PID::pid[mesonIndex]);
    PhysicsEvent ev      = builder->getPhysicsEvent(electron, meson);
      
    if (IsInclusive(ev, mesonIndex)) { FillInclusive(ev, mesonIndex, index, helicity); }
    if (IsExclusive(ev, mesonIndex)) { FillExclusive(ev, mesonIndex, index, helicity); }
  }


  void FillInclusive(PhysicsEvent &ev, int mesonIndex, int index, int helicity){
	histos[mesonIndex]        ->Fill(ev, helicity);
	mesonHistos[mesonIndex]   ->Fill(event, ev, index); 
	standardHistos[mesonIndex]->Fill(event, event.GetElectronIndex(), index, ev); 

	if (mesonIndex == Meson::kPionNegative || mesonIndex == Meson::kKaonNegative){
	  pidHistos[Meson::kPionNegative]->Fill(event, ev, index);
	  pidHistos[Meson::kKaonNegative]->Fill(event, ev, index);
	} else if (mesonIndex == Meson::kPionPositive || mesonIndex == Meson::kKaonPositive){
	  pidHistos[Meson::kPionPositive]->Fill(event, ev, index);
	  pidHistos[Meson::kKaonPositive]->Fill(event, ev, index);
	}
  }

  void FillExclusive(PhysicsEvent &ev, int mesonIndex, int index, int helicity){
    exclusiveHistos[mesonIndex]        ->Fill(ev, helicity);
    
    if (mesonIndex == Meson::kPionNegative || mesonIndex == Meson::kKaonNegative){
      exclusivePidHistos[Meson::kPionNegative]->Fill(event, ev, index);
      exclusivePidHistos[Meson::kKaonNegative]->Fill(event, ev, index);
    } else if (mesonIndex == Meson::kPionPositive || mesonIndex == Meson::kKaonPositive){
      exclusivePidHistos[Meson::kPionPositive]->Fill(event, ev, index);
      exclusivePidHistos[Meson::kKaonPositive]->Fill(event, ev, index);
    }
  }
  
};

void Analysis::InitHistos() {
  histos[Meson::kPionNegative] = new Histos("test", Meson::kPionNegative); 
  histos[Meson::kPionPositive] = new Histos("test", Meson::kPionPositive); 
  histos[Meson::kKaonNegative] = new Histos("test", Meson::kKaonNegative); 
  histos[Meson::kKaonPositive] = new Histos("test", Meson::kKaonPositive); 

  exclusiveHistos[Meson::kPionNegative] = new Histos("exclusive", Meson::kPionNegative); 
  exclusiveHistos[Meson::kPionPositive] = new Histos("exclusive", Meson::kPionPositive); 
  exclusiveHistos[Meson::kKaonNegative] = new Histos("exclusive", Meson::kKaonNegative); 
  exclusiveHistos[Meson::kKaonPositive] = new Histos("exclusive", Meson::kKaonPositive); 

  pidHistos[Meson::kPionNegative] = new PidHistos("test", Meson::kPionNegative); 
  pidHistos[Meson::kPionPositive] = new PidHistos("test", Meson::kPionPositive); 
  pidHistos[Meson::kKaonNegative] = new PidHistos("test", Meson::kKaonNegative); 
  pidHistos[Meson::kKaonPositive] = new PidHistos("test", Meson::kKaonPositive); 

  exclusivePidHistos[Meson::kPionNegative] = new PidHistos("exclusive", Meson::kPionNegative); 
  exclusivePidHistos[Meson::kPionPositive] = new PidHistos("exclusive", Meson::kPionPositive); 
  exclusivePidHistos[Meson::kKaonNegative] = new PidHistos("exclusive", Meson::kKaonNegative); 
  exclusivePidHistos[Meson::kKaonPositive] = new PidHistos("exclusive", Meson::kKaonPositive); 

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
    electron                 = momCorr->PcorN(electron, -1, 11);    

    if (helicity > -1){
      if (!kps.empty()) { FillParticle(Meson::kKaonPositive, kps[0], helicity, electron); }
      if (!pms.empty()) { FillParticle(Meson::kPionNegative, pms[0], helicity, electron); }
      if (!pps.empty()) { FillParticle(Meson::kPionPositive, pps[0], helicity, electron); }
      if (!kms.empty()) { FillParticle(Meson::kKaonNegative, kms[0], helicity, electron); }
    }

  }
}

void Analysis::Save(std::string outfile){
    histos[0]->Save(outfile, "recreate"); 

  for(int m=1; m<constants::NMESON; m++) {
    histos[m]->Save(outfile, "update"); 
  }

  for(int m=0; m<constants::NMESON; m++) {
    exclusiveHistos[m]    ->Save(outfile, "update"); 
    exclusivePidHistos[m]->Save(outfile, "update"); 
    pidHistos[m]         ->Save(outfile, "update"); 
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
