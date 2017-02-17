// ------------------------------------------------
/* 
    
   David Riser, University of Connecticut 
   Feb. 14, 2017 

 */
// ------------------------------------------------

#include <iostream>
#include <vector>
using namespace std; 

// -------------------
// My Libs 
// -------------------
#include "h22Option.h"

// -------------------
// Root Libs 
// -------------------
#include "TAxis.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "THnSparse.h"
#include "TObject.h"
#include "TString.h"

class Analysis{
public:
  Analysis(h22Options *options);
  ~Analysis();

protected:
  string inputFilename, radCorrFilename, outputFilename;  
  TFile      *inputFile;   
  THnSparseF *dataEvents, *recEvents, *genEvents; 
  THnSparseF *acceptance, *crossSection; 
  vector<TObject*> rootObjects; 
  vector<TH1D*> data, rec, gen, acc, sigma; 

public:
  void loadFiles();
  void calculateAcceptance();
  void makeProjections();
  void doCrossSection();
  void resetSparseRange(THnSparseF *s);
  void save();

  vector<TH1D*> projectSparse(THnSparseF* s);
};

Analysis::Analysis(h22Options *options){
  inputFilename   = options->args["INPUT"].args;
  radCorrFilename = options->args["RADCORR_FILE"].args;
  outputFilename  = options->args["OUT"].args;
}

Analysis::~Analysis(){
  if(inputFile->IsOpen()){
    inputFile->Close(); 
  }
}

void Analysis::loadFiles(){
  inputFile = TFile::Open(inputFilename.c_str()); 

  dataEvents = (THnSparseF*) inputFile->Get("dataEvents_events"); 
  recEvents  = (THnSparseF*) inputFile->Get("recEventsRad_events"); 
  genEvents  = (THnSparseF*) inputFile->Get("genEventsRad_events"); 

  int finalBinning[4] = {1,10,20,4};
  dataEvents->Rebin(finalBinning);
  recEvents ->Rebin(finalBinning);
  genEvents ->Rebin(finalBinning);

  cout << "[Analysis::loadFiles] Loaded " << dataEvents->GetTitle() << " w/ Entries = " << dataEvents->GetEntries() << endl; 
  cout << "[Analysis::loadFiles] Loaded " << recEvents ->GetTitle() << " w/ Entries = " << recEvents ->GetEntries() << endl; 
  cout << "[Analysis::loadFiles] Loaded " << genEvents ->GetTitle() << " w/ Entries = " << genEvents ->GetEntries() << endl; 
}

void Analysis::makeProjections(){
  data = projectSparse(dataEvents);
  rec  = projectSparse(recEvents);
  gen  = projectSparse(genEvents);
}

vector<TH1D*> Analysis::projectSparse(THnSparseF *s){
  int SECTS        = s->GetAxis(0)->GetNbins();
  int WBINS        = s->GetAxis(1)->GetNbins();
  int Q2BINS       = s->GetAxis(2)->GetNbins();  
  int PHIBINS      = s->GetAxis(3)->GetNbins();
  string baseName  = s->GetName();

  vector<TH1D*> slices; 

  resetSparseRange(s); 
  for(int sector=1; sector<=SECTS; sector++){
    s->GetAxis(0)->SetRange(sector,sector);

    for(int wBin=1; wBin<=WBINS; wBin++){
      s->GetAxis(3)->SetRange(wBin,wBin);

      for(int q2Bin=1; q2Bin<=Q2BINS; q2Bin++){
	s->GetAxis(2)->SetRange(q2Bin,q2Bin);

	string name  = Form("%s_phi_sect%d_wBin%d_q2Bin%d",baseName.c_str(),sector,wBin,q2Bin);
	string title = Form("Sector %d W = %2.f Q^{2} = %2.f",sector,s->GetAxis(3)->GetBinCenter(wBin),s->GetAxis(2)->GetBinCenter(q2Bin)); 
	TH1D *slice  = s->Projection(1);
	slice->SetName(name.c_str());
	slice->SetTitle(title.c_str());
	slices.push_back(slice);

	cout << "[Analysis::makeProjections] Projecting " << name << endl; 
      }
    }
  }

  /*
  resetSparseRange(s); 
  for(int sector=1; sector<=SECTS; sector++){
    s->GetAxis(0)->SetRange(sector,sector);

    for(int wBin=1; wBin<=WBINS; wBin++){
      s->GetAxis(3)->SetRange(wBin,wBin);

      for(int phiBin=1; phiBin<=PHIBINS; phiBin++){
	s->GetAxis(1)->SetRange(phiBin,phiBin);

	string name  = Form("%s_q2_sect%d_wBin%d_phiBin%d",sector,wBin,phiBin);
	string title = Form("Sector %d W = %.2f #phi_{Rel} = %.2f",sector,s->GetAxis(3)->GetBinCenter(wBin),s->GetAxis(1)->GetBinCenter(phiBin)); 
	TH1D *slice  = s->Projection(2);
	slice->SetName(name.c_str());
	slice->SetTitle(title.c_str());
	slices.push_back(slice);
      }
    }
  }

  resetSparseRange(s); 
  for(int sector=1; sector<=SECTS; sector++){
    s->GetAxis(0)->SetRange(sector,sector);
 
    for(int phiBin=1; phiBin<=PHIBINS; phiBin++){
      s->GetAxis(1)->SetRange(phiBin,phiBin);

      for(int q2Bin=1; q2Bin<=Q2BINS; q2Bin++){
	s->GetAxis(2)->SetRange(q2Bin,q2Bin);

	string name  = Form("%s_w_sect%d_phiBin%d_q2Bin%d",sector,phiBin,q2Bin);
	string title = Form("Sector %d Q^{2} = %.2f #phi_{Rel} = %.2f",sector,s->GetAxis(2)->GetBinCenter(q2Bin),s->GetAxis(1)->GetBinCenter(phiBin)); 
	TH1D *slice  = s->Projection(3);
	slice->SetName(name.c_str());
	slice->SetTitle(title.c_str());
	slices.push_back(slice);
      }
    }
  }
  */

  resetSparseRange(s); 
  return slices; 
}

void Analysis::resetSparseRange(THnSparseF* s){
  int DIM = s->GetNdimensions();
  
  for(int currentAxis=0; currentAxis<DIM; currentAxis++){
    s->GetAxis(currentAxis)->SetRange(1, s->GetAxis(currentAxis)->GetNbins());
  }
}

void Analysis::calculateAcceptance(){
  if(!rec.empty() && !gen.empty() && rec.size() == gen.size()){
    for(int h=0; h<rec.size(); h++){
      TH1D *slice = (TH1D*) rec[h]->Clone();
      slice->Divide(gen[h]);

      // This is going to fail
      TString name = rec[h]->GetName();
      name.Replace(0,2,"acc");
      slice->SetName(name);
      acc.push_back(slice);
    }
    

    cout << "[Analysis::calculateAcceptance] Calculated acceptance w/ Entries = " << acceptance->GetEntries() << endl; 
  } else {
    cout << "[Analysis::calculateAcceptance] Failed to calculate acceptance because the vectors are empty or not the same size. " << endl; 
  }
}

void Analysis::doCrossSection(){
  crossSection = (THnSparseF*) dataEvents->Clone(); 
  crossSection->SetName("crossSection"); 
  crossSection->SetTitle("crossSection"); 
  crossSection->Divide(acceptance);
  crossSection->Scale(1e-11);
  rootObjects.push_back(crossSection); 
}

void Analysis::save(){
  TFile *outputFile = new TFile(outputFilename.c_str(),"recreate"); 

  for(int obj=0; obj<rootObjects.size(); obj++){
    rootObjects[obj]->Write();
    cout << "[Analysis::save] Writing " << rootObjects[obj]->GetName() << " to file " << outputFilename << endl; 
  }
  
  for(int h=0; h<data.size(); h++){
    data[h]->Write(); 
    rec[h]->Write(); 
    gen[h]->Write(); 
  }

  outputFile->Close();
}

int main(int argc, char *argv[]){

  h22Options *opts = new h22Options();
  opts->args["INPUT"].args = "Unset";
  opts->args["INPUT"].type = 1;
  opts->args["INPUT"].name = "Input data file which contains THnSparse for Data/MC";
  opts->args["RADCORR_FILE"].args = "Unset";
  opts->args["RADCORR_FILE"].type = 1;
  opts->args["RADCORR_FILE"].name = "Radiative corrections file";
  opts->set(argc, argv);

  Analysis analysis(opts);
  analysis.loadFiles();
  analysis.makeProjections();
  //  analysis.calculateAcceptance();
  //  analysis.doCrossSection();
  analysis.save();
  return 0;
}
