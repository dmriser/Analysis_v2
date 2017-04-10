/* 

   Writing out Electron ID 
   histograms 
   
   March 22, 2016

   Updated: Dec 16, 2016 
   Updating to Analysis_v2 classes

 */

// C++ Libraries
#include <iostream>
#include <cstdlib>
#include <map>
using namespace std;

// My Libraries
#include "h22Event.h"
#include "h22Reader.h"
#include "h22Option.h"
#include "Parameters.h"
#include "ParticleFilter.h"

// CERN Root Libraries
#include "TFile.h"
#include "TGraph.h"
#include "TH1.h"
#include "TH2.h"
#include "TMVA/Reader.h"

class PIDHistograms{
public:
  PIDHistograms();
 ~PIDHistograms(){}
  
  // 1-D 
  TH1D *h1_nphe[12][7];
  TH1F *h1_ec_edep_inner[12][7];
  TH1F *h1_ec_edep_outer[12][7];
  TH1F *h1_p[12][7];
  TH1F *h1_z_vertex[12][7];

  // 2-D 
  TH2F *h2_cc_theta[12][7];
  TH2F *h2_etot_p[12][7];
  TH2F *h2_ang_fid[12][7];
  TH2F *h2_ec_edep[12][7];
  TH2F *h2_dcr1_fid[12][7];
  TH2F *h2_dcr3_fid[12][7];
  TH2F *h2_ec_fid[12][7];
  
  // TMVA Shit 
  TMVA::Reader *TMVAReader;
  float input1, input2, input3; 

  void Fill(h22Event event, int ipart, int cutType);
  void Save(string outputFilename);
};

PIDHistograms::PIDHistograms(){

  TMVAReader = new TMVA::Reader("");
  //  TMVAReader->AddVariable("ec_ei/p",&input1);
  //  TMVAReader->AddVariable("ec_eo/p",&input2);
  //  TMVAReader->AddVariable("nphe",   &input3);  
  //  TMVAReader->BookMVA("SVM","/u/home/dmriser/mydoc/analysis/root_scripts/Analysis_v2/pid/calib/eid/v2/weights/TMVAClassification_SVM.weights.xml");

  string type[12]   = {"allNegatives", "cuts","Z_VERTEX","CC_FID","CC_PHI","CC_THETA","DC_R1_FID","DC_R3_FID","EC_FID","EC_IN_OUT","EC_SAMPLING","SVM"};
    string sect[7]  = {"all", "s1", "s2", "s3", "s4", "s5", "s6"};
    // initialize 
    for (int itype = 0; itype < 12; itype++)
      for(int isect = 0; isect < 7; isect++)
	{
	  // 1d
	  h1_nphe[itype][isect]          = new TH1D(Form("h1_nphe_%s_%s",type[itype].c_str(),sect[isect].c_str()),Form("h1_nphe_%s_%s",type[itype].c_str(),sect[isect].c_str()),200,0,100);
	  h1_ec_edep_inner[itype][isect] = new TH1F(Form("h1_ec_edep_inner_%s_%s",type[itype].c_str(),sect[isect].c_str()),Form("h1_ec_edep_inner_%s_%s",type[itype].c_str(),sect[isect].c_str()),200,0,4);
	  h1_ec_edep_outer[itype][isect] = new TH1F(Form("h1_ec_edep_outer_%s_%s",type[itype].c_str(),sect[isect].c_str()),Form("h1_ec_edep_outer_%s_%s",type[itype].c_str(),sect[isect].c_str()),200,0,4);
	  h1_p[itype][isect]             = new TH1F(Form("h1_p_%s_%s",type[itype].c_str(),sect[isect].c_str()),Form("h1_p_%s_%s",type[itype].c_str(),sect[isect].c_str()),200,0,5);
	  h1_z_vertex[itype][isect]      = new TH1F(Form("h1_z_vertex_%s_%s",type[itype].c_str(),sect[isect].c_str()),Form("h1_z_vertex_%s_%s",type[itype].c_str(),sect[isect].c_str()),200,-35,-15);
	  
	  // 2d
	  h2_cc_theta[itype][isect] = new TH2F(Form("h_cc_theta_%s_%s",type[itype].c_str(),sect[isect].c_str()),Form("h_cc_theta_%s_%s",type[itype].c_str(),sect[isect].c_str()),18,0,17,200,0,60);
	  h2_etot_p[itype][isect]   = new TH2F(Form("h_etot_p_%s_%s",type[itype].c_str(),sect[isect].c_str()),Form("h_etot_p_%s_%s",type[itype].c_str(),sect[isect].c_str()),200,0,5,200,0.05,0.5);
	  h2_ang_fid[itype][isect]  = new TH2F(Form("h_ang_fid_%s_%s",type[itype].c_str(),sect[isect].c_str()),Form("h_ang_fid_%s_%s",type[itype].c_str(),sect[isect].c_str()),200,-30,30,200,5,65);
	  h2_ec_edep[itype][isect]  = new TH2F(Form("h_ec_edep_%s_%s",type[itype].c_str(),sect[isect].c_str()),Form("h_ec_edep_%s_%s",type[itype].c_str(),sect[isect].c_str()),200,0.01,0.5,200,0.01,0.5);
	  h2_dcr1_fid[itype][isect] = new TH2F(Form("h_dcr1_fid_%s_%s",type[itype].c_str(),sect[isect].c_str()),Form("h_dcr1_fid_%s_%s",type[itype].c_str(),sect[isect].c_str()),200,-100,100,200,-100,100);
	  h2_dcr3_fid[itype][isect] = new TH2F(Form("h_dcr3_fid_%s_%s",type[itype].c_str(),sect[isect].c_str()),Form("h_dcr3_fid_%s_%s",type[itype].c_str(),sect[isect].c_str()),200,50,400,200,-200,200);
	  h2_ec_fid[itype][isect]   = new TH2F(Form("h_ec_fid_%s_%s",type[itype].c_str(),sect[isect].c_str()),Form("h_ec_fid_%s_%s",type[itype].c_str(),sect[isect].c_str()),200,-500,500,200,-500,500);
	}
  }

void PIDHistograms::Fill(h22Event event, int ipart, int cutType){
  
  int sector = event.dc_sect[ipart];
  
  // filling histograms for all negatives hN_abc[0][x]
  // 1-D 
  h1_nphe[cutType][0]          ->Fill(event.nphe[ipart]/10);
  h1_ec_edep_inner[cutType][0] ->Fill(event.ec_ei[ipart]);
  h1_ec_edep_outer[cutType][0] ->Fill(event.ec_eo[ipart]);
  h1_p[cutType][0]             ->Fill(event.p[ipart]);
  h1_z_vertex[cutType][0]      ->Fill(event.vz[ipart]);
  
  // 2-D 
  h2_cc_theta[cutType][0] ->Fill((event.cc_segm[ipart]%1000)/10, event.GetThetaCC(ipart));
  h2_etot_p[cutType][0]   ->Fill(event.p[ipart], event.etot[ipart]/event.p[ipart]);
  h2_ang_fid[cutType][0]  ->Fill(event.GetRelativePhi(ipart), event.GetTheta(ipart));
  h2_ec_edep[cutType][0]  ->Fill(event.ec_ei[ipart], event.ec_eo[ipart]);
  h2_dcr1_fid[cutType][0] ->Fill(event.tl1_x[ipart], event.tl1_y[ipart]);
  h2_dcr3_fid[cutType][0] ->Fill(event.tl3_x[ipart], event.tl3_y[ipart]);
  h2_ec_fid[cutType][0]   ->Fill(event.ech_x[ipart], event.ech_y[ipart]);
  
  if(sector > 0)
    {
      h1_nphe[cutType][sector]          ->Fill(event.nphe[ipart]/10);
      h1_ec_edep_inner[cutType][sector] ->Fill(event.ec_ei[ipart]);
      h1_ec_edep_outer[cutType][sector] ->Fill(event.ec_eo[ipart]);
      h1_p[cutType][sector]             ->Fill(event.p[ipart]);
      h1_z_vertex[cutType][sector]      ->Fill(event.vz[ipart]);
      
      h2_cc_theta[cutType][sector] ->Fill((event.cc_segm[ipart]%1000)/10, event.GetThetaCC(ipart));
      h2_etot_p[cutType][sector]   ->Fill(event.p[ipart], event.etot[ipart]/event.p[ipart]);
      h2_ang_fid[cutType][sector]  ->Fill(event.GetRelativePhi(ipart), event.GetTheta(ipart));
      h2_ec_edep[cutType][sector]  ->Fill(event.ec_ei[ipart], event.ec_eo[ipart]);
      h2_dcr1_fid[cutType][sector] ->Fill(event.tl1_x[ipart], event.tl1_y[ipart]);
      h2_dcr3_fid[cutType][sector] ->Fill(event.tl3_x[ipart], event.tl3_y[ipart]);
      h2_ec_fid[cutType][sector]   ->Fill(event.ech_x[ipart], event.ech_y[ipart]);
    }
  
}

void PIDHistograms::Save(string outputFilename){

  TFile outfile(outputFilename.c_str(),"recreate");  

  for (int itype = 0; itype < 12; itype++){
    for(int isect = 0; isect < 7; isect++){
      // 1d
      h1_nphe[itype][isect]->Write();          
      h1_ec_edep_inner[itype][isect]->Write();
      h1_ec_edep_outer[itype][isect]->Write();
      h1_p[itype][isect]->Write();             
      h1_z_vertex[itype][isect]->Write();      
      
      // 2d
      h2_cc_theta[itype][isect]->Write(); 
      h2_etot_p[itype][isect]->Write();   
      h2_ang_fid[itype][isect]->Write();  
      h2_ec_edep[itype][isect]->Write();  
      h2_dcr1_fid[itype][isect]->Write(); 
      h2_dcr3_fid[itype][isect]->Write(); 
      h2_ec_fid[itype][isect]->Write();   
    }
  }
  
  outfile.Write(); 
  outfile.Close(); 
}

vector<string> loadFilesFromList(string fileList, int numFiles);

int main (int argc, char * argv[]){
  
  // get number of files from command line 
  //  if (argc < 4){ return 0; }

  h22Options opts; 
  opts.args["OUT"].args = "PID.root";

  opts.args["PARS"].args = "/u/home/dmriser/mydoc/analysis/root_scripts/Analysis_v2/pid/calib/eid/v2/data.pars";
  opts.args["PARS"].type = 1;
  opts.args["PARS"].name = "Parameter file";

  opts.args["LIST"].type = 1; 
  opts.args["LIST"].args = "/u/home/dmriser/mydoc/analysis/root_scripts/Analysis_v2/projects/dis2/allGoodEvents.dat"; 
  opts.args["LIST"].name = "File list"; 

  opts.set(argc, argv);
  
  int nFiles        = opts.args["N"].arg;
  string files      = opts.args["LIST"].args;
  string outputFile = opts.args["OUT"].args;

  if (nFiles == 0){ cout<< " >>> No files?" << endl; return 0; }
  vector<string> fileVector = loadFilesFromList(files, nFiles);

  // setup file reader and add files
  h22Reader reader;
  for(int ifile=0; ifile<fileVector.size(); ifile++){ reader.AddFile(fileVector[ifile]); }
  reader.Init(); // set branch addresses

  int nEvents = reader.GetEntries();

  Parameters *pars = new Parameters(); 
  pars->loadParameters(opts.args["PARS"].args);
  
  ParticleFilter filter(pars);
  filter.set_info(reader.GSIM, reader.GetRunNumber()); 
  //  filter.getCut("Track Quality Cut")->disable();

  PIDHistograms histos;
  
  // loop over events
  for (int iEvent = 0; iEvent < nEvents; iEvent++){
      reader.GetEntry(iEvent);
      h22Event event = reader.GetEvent();

      // loop over all negatives in the event 
      for(int ipart = 0; ipart < event.gpart; ipart++){     
	  if (event.q[ipart] < 0){

	    histos.input1 = event.ec_ei[ipart]/event.p[ipart];
	    histos.input2 = event.ec_eo[ipart]/event.p[ipart];
	    histos.input3 = event.nphe[ipart];

	    //  holds the result of all cuts, intensive 
	    map<string, bool> eID_Status = filter.eid_map(event, ipart);
	    
	      histos.Fill(event, ipart, 0);
	      if(eID_Status["Z_VERTEX"]){    histos.Fill(event, ipart, 2); }
	      if(eID_Status["CC_FID"]){      histos.Fill(event, ipart, 3); }
	      if(eID_Status["CC_PHI"]){      histos.Fill(event, ipart, 4); }
	      if(eID_Status["CC_THETA"]){    histos.Fill(event, ipart, 5); }
	      if(eID_Status["DC_R1_FID"]){   histos.Fill(event, ipart, 6); }
	      if(eID_Status["DC_R3_FID"]){   histos.Fill(event, ipart, 7); }
	      if(eID_Status["EC_FID"]){      histos.Fill(event, ipart, 8); }
	      if(eID_Status["EC_IN_OUT"]){   histos.Fill(event, ipart, 9); }
	      if(eID_Status["EC_SAMPLING"]){ histos.Fill(event, ipart, 10); }
	      /*
	      if(eID_Status["CC_FID"] && 
		 eID_Status["EC_FID"] && 
		 eID_Status["DC_R1_FID"] && 
		 histos.TMVAReader->EvaluateMVA("SVM") > 0.75){ histos.Fill(event, ipart, 11); }
	      */
	    }
	} // end ipart loop 
      

      // look for electron in event
      int e_index = filter.getByPID(event, 11);
      if (e_index > -123){ histos.Fill(event, e_index, 1); }

      if (iEvent%1000 == 0) { cout << "\r done " << iEvent << " of " << nEvents << flush; }
    }  // end loop over events

  histos.Save(outputFile); 


  return 0;
}

// Support xs
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


