////////////////////////////////////////////////////////////////////
/*

  David Riser, University of Connecticut 
  August 24, 2016 

  DISPackage.cxx 

*/
////////////////////////////////////////////////////////////////////

#ifndef dis_cxx
#define dis_cxx

// c++ includes 
#include <iostream>
#include <vector>
using namespace std; 

// my includes
#include "CommonTools.h"
#include "DBins.h"
#include "DCut.h"
#include "DEvent.h"
#include "DInformation.h"
#include "DISPackage.h"
#include "DSelection.h"
#include "ElectronIDPackage.h"
#include "F1F209Wrapper.hh"
#include "NathanArchive.h"
#include "MomCorr.h"

//  root includes
#include "TBranch.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TLatex.h"
#include "TLorentzVector.h"
#include "TRegexp.h"
#include "TStyle.h"
#include "TString.h"
#include "TTree.h"

////////////////////////////////////////////////////////////////////////
/*

  DISHistograms 

*/
////////////////////////////////////////////////////////////////////////

DISHistograms::DISHistograms()
{
  sect[0] = "all";
  sect[1] = "s1";
  sect[2] = "s2";
  sect[3] = "s3";
  sect[4] = "s4";
  sect[5] = "s5";
  sect[6] = "s6";
  output_name = "unset";

  runInfoTree = new TTree("runInfoTree","Holds run information.");
}


DISHistograms::~DISHistograms()
{
  // Destroy things. 
}

void DISHistograms::init()
{
  // Initialize Histograms
  for (int s=0; s<7; s++) {
    h2_hits_x_qq[s] = new TH2F(Form("h2_hits_x_qq_%d",s),Form("Hits QQ vs. X Sector %d",s),xBins.number(),xBins.min(),xBins.max(),qqBins.number(),qqBins.min(),qqBins.max());
    h2_rec_x_qq[s]  = new TH2F(Form("h2_rec_x_qq_%d",s), Form("Rec QQ vs. X Sector %d",s), xBins.number(),xBins.min(),xBins.max(),qqBins.number(),qqBins.min(),qqBins.max());
    h2_gen_x_qq[s]  = new TH2F(Form("h2_gen_x_qq_%d",s), Form("Gen QQ vs. X Sector %d",s), xBins.number(),xBins.min(),xBins.max(),qqBins.number(),qqBins.min(),qqBins.max()); 
  }
}

void DISHistograms::fill(DEvent event, int index)
{
  if (event.e_index == -123){ cout << " You must set DEvent::e_index before filling histograms! " << endl; return; }
  
  int s = event.tracks.dc_sect[event.e_index];

  // Monte Carlo 
  if (index == 1) {
    h2_rec_x_qq[s]->Fill(event.x, event.qq);
    h2_rec_x_qq[0]->Fill(event.x, event.qq);
  }

  // Data Events
  else if (index == 0) {
    h2_hits_x_qq[s]->Fill(event.x, event.qq);
    h2_hits_x_qq[0]->Fill(event.x, event.qq);
  }

}

void DISHistograms::fill_gen(DEvent event)
{
  int e_index = 0;
  if ( event.tracks.mcid[0] != 11) {
    for (int ipart=0; ipart<event.tracks.gpart; ipart++)
      {
	if (event.tracks.mcid[ipart] == 11)
	  {
	    e_index = ipart;
	    break;
	  } 
      }
  }
  
  TLorentzVector gen_electron(event.tracks.mcpx(e_index), event.tracks.mcpy(e_index), event.tracks.mcpz(e_index), event.tracks.mcp[e_index]);
  event.set_electron(gen_electron); 

  int s = 1+floor(event.tracks.mcphi[e_index]/60.0);

  // Do the fill. 
  h2_gen_x_qq[0]->Fill(event.x, event.qq);
  h2_gen_x_qq[s]->Fill(event.x, event.qq);
}

void DISHistograms::load(){

  //  string sect[7] = {"all","s1","s2","s3","s4","s5","s6"};
  TFile * f = TFile::Open( output_name.c_str() );

  if ( !f->IsOpen() ) {
    cout << " Fatal Error in: DISHistograms::load(). File not opened! " << output_name << endl;
    exit(0);
  }

  for (int s=0; s<7; s++){
    h2_hits_x_qq[s] = (TH2F*) f->Get( Form("h2_hits_x_qq_%d",s) );
    h2_rec_x_qq[s]  = (TH2F*) f->Get( Form("h2_rec_x_qq_%d", s) );
    h2_gen_x_qq[s]  = (TH2F*) f->Get( Form("h2_gen_x_qq_%d", s) );
  }

  cout << h2_hits_x_qq[0]->GetEntries() << endl;
  f->Close();
}

void DISHistograms::draw()
{
  // Draw Histograms 
  TCanvas * canvas = new TCanvas("canvas","",1200,800);

  // Standard
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  gStyle->SetOptTitle(0);

  // Latex Writer
  TLatex lab;
  lab.SetNDC();
  lab.SetTextFont(22);
  
  canvas->Print( Form("%s.pdf[",output_name.c_str()) );
  canvas->Clear();

  int can_size = floor(sqrt(h1_hits_x_by_qq[0].size()));
  /*
  // Hits x-by-qq
  for (int s=0; s<7; s++)
    {
      canvas->Divide(can_size+1, can_size);
      for (int b=0; b<h1_hits_x_by_qq[s].size()-1; b++)
	{
	  canvas->cd(b+1);

	  double qq_start = qqBins.min() + (b-1)*qqBins.width();
	  double qq_end   = qqBins.min() + b*qqBins.width();
	  if (b == 0) { qq_start = qqBins.min(); qq_end = qqBins.max(); }

	  h1_hits_x_by_qq[s][b]->SetMarkerStyle(8);
	  h1_hits_x_by_qq[s][b]->Draw("PE");
	  lab.DrawLatex(0.3, 0.02, Form(" Hits (x): Sector %d Bin %d ",s,b));
	  lab.DrawLatex(0.25, 0.925, Form(" Q^{2} (%.2f #rightarrow %.2f) GeV^{2}/c^{2} ",qq_start, qq_end));
	}

      canvas->Print( Form("%s.pdf",output_name.c_str()) );
      canvas->Clear();
    }

  // Rec. x-by-qq
  for (int s=0; s<7; s++)
    {
      canvas->Divide(can_size+1, can_size);
      for (int b=0; b<h1_rec_x_by_qq[s].size()-1; b++)
	{
	  canvas->cd(b+1);

	  double qq_start = qqBins.min() + (b-1)*qqBins.width();
	  double qq_end   = qqBins.min() + b*qqBins.width();
	  if (b == 0) { qq_start = qqBins.min(); qq_end = qqBins.max(); }

	  h1_rec_x_by_qq[s][b]->SetMarkerStyle(8);
	  h1_rec_x_by_qq[s][b]->Draw("PE");
	  lab.DrawLatex(0.3, 0.02, Form(" Rec (x): Sector %d Bin %d ",s,b));
	  lab.DrawLatex(0.25, 0.925, Form(" Q^{2} (%.2f #rightarrow %.2f) GeV^{2}/c^{2} ",qq_start, qq_end));
	}

      canvas->Print( Form("%s.pdf",output_name.c_str()) );
      canvas->Clear();
    }

  // Gen x-by-qq
  for (int s=0; s<7; s++)
    {
      canvas->Divide(can_size+1, can_size);
      for (int b=0; b<h1_gen_x_by_qq[s].size()-1; b++)
	{
	  canvas->cd(b+1);

	  double qq_start = qqBins.min() + (b-1)*qqBins.width();
	  double qq_end   = qqBins.min() + b*qqBins.width();
	  if (b == 0) { qq_start = qqBins.min(); qq_end = qqBins.max(); }

	  h1_gen_x_by_qq[s][b]->SetMarkerStyle(8);
	  h1_gen_x_by_qq[s][b]->Draw("PE");
	  lab.DrawLatex(0.3, 0.02, Form(" Gen (x): Sector %d Bin %d ",s,b));
	  lab.DrawLatex(0.25, 0.925, Form(" Q^{2} (%.2f #rightarrow %.2f) GeV^{2}/c^{2} ",qq_start, qq_end));
	}

      canvas->Print( Form("%s.pdf",output_name.c_str()) );
      canvas->Clear();
    }

  // Acc x-by-qq
  for (int s=0; s<7; s++)
    {
      canvas->Divide(can_size+1, can_size);
      for (int b=0; b<h1_acc_x_by_qq[s].size()-1; b++)
	{
	  canvas->cd(b+1);

	  double qq_start = qqBins.min() + (b-1)*qqBins.width();
	  double qq_end   = qqBins.min() + b*qqBins.width();
	  if (b == 0) { qq_start = qqBins.min(); qq_end = qqBins.max(); }

	  h1_acc_x_by_qq[s][b]->SetMarkerStyle(8);
	  h1_acc_x_by_qq[s][b]->Draw("PE");
	  lab.DrawLatex(0.3, 0.02, Form(" Acc (x): Sector %d Bin %d ",s,b));
	  lab.DrawLatex(0.25, 0.925, Form(" Q^{2} (%.2f #rightarrow %.2f) GeV^{2}/c^{2} ",qq_start, qq_end));
	}

      canvas->Print( Form("%s.pdf",output_name.c_str()) );
      canvas->Clear();
    }

  // Raw XS x-by-qq
  for (int s=0; s<7; s++)
    {
      canvas->Divide(can_size+1, can_size);
      for (int b=0; b<h1_rxs_x_by_qq[s].size()-1; b++)
	{
	  canvas->cd(b+1);

	  double qq_start = qqBins.min() + (b-1)*qqBins.width();
	  double qq_end   = qqBins.min() + b*qqBins.width();
	  if (b == 0) { qq_start = qqBins.min(); qq_end = qqBins.max(); }

	  h1_rxs_x_by_qq[s][b]->SetMarkerStyle(8);
	  h1_rxs_x_by_qq[s][b]->Draw("PE");
	  lab.DrawLatex(0.3, 0.02, Form(" #sigma_{raw} (x): Sector %d Bin %d ",s,b));
	  lab.DrawLatex(0.25, 0.925, Form(" Q^{2} (%.2f #rightarrow %.2f) GeV^{2}/c^{2} ",qq_start, qq_end));
	}

      canvas->Print( Form("%s.pdf",output_name.c_str()) );
      canvas->Clear();
    }

  // xs x-by-qq
  for (int s=0; s<7; s++)
    {
      canvas->Divide(can_size+1, can_size);
      for (int b=0; b<h1_xs_x_by_qq[s].size()-1; b++)
	{
	  canvas->cd(b+1);

	  double qq_start = qqBins.min() + (b-1)*qqBins.width();
	  double qq_end   = qqBins.min() + b*qqBins.width();
	  if (b == 0) { qq_start = qqBins.min(); qq_end = qqBins.max(); }

	  h1_xs_x_by_qq[s][b]->SetMarkerStyle(8);
	  h1_xs_x_by_qq[s][b]->Draw("PE");
	  lab.DrawLatex(0.3, 0.02, Form(" #sigma (x): Sector %d Bin %d ",s,b));
	  lab.DrawLatex(0.25, 0.925, Form(" Q^{2} (%.2f #rightarrow %.2f) GeV^{2}/c^{2} ",qq_start, qq_end));
	}

      canvas->Print( Form("%s.pdf",output_name.c_str()) );
      canvas->Clear();
    }

  // xs ratio x-by-qq
  for (int s=0; s<7; s++)
    {
      canvas->Divide(can_size+1, can_size);
      for (int b=0; b<h1_xs_x_by_qq[s].size()-1; b++)
	{
	  canvas->cd(b+1);

	  double qq_start = qqBins.min() + (b-1)*qqBins.width();
	  double qq_end   = qqBins.min() + b*qqBins.width();
	  if (b == 0) { qq_start = qqBins.min(); qq_end = qqBins.max(); }

	  h1_xs_ratio_x_by_qq[s][b]->SetMarkerStyle(8);
	  h1_xs_ratio_x_by_qq[s][b]->Draw("PE");
	  lab.DrawLatex(0.3, 0.02, Form(" #sigma ratio(x): Sector %d Bin %d ",s,b));
	  lab.DrawLatex(0.25, 0.925, Form(" Q^{2} (%.2f #rightarrow %.2f) GeV^{2}/c^{2} ",qq_start, qq_end));
	}

      canvas->Print( Form("%s.pdf",output_name.c_str()) );
      canvas->Clear();
    }
  
  // rxs ratio x-by-qq
  for (int s=0; s<7; s++)
    {
      canvas->Divide(can_size+1, can_size);
      for (int b=0; b<h1_rxs_ratio_x_by_qq[s].size()-1; b++)
	{
	  canvas->cd(b+1);

	  double qq_start = qqBins.min() + (b-1)*qqBins.width();
	  double qq_end   = qqBins.min() + b*qqBins.width();
	  if (b == 0) { qq_start = qqBins.min(); qq_end = qqBins.max(); }

	  h1_rxs_ratio_x_by_qq[s][b]->SetMarkerStyle(8);
	  h1_rxs_ratio_x_by_qq[s][b]->Draw("PE");
	  lab.DrawLatex(0.3, 0.02, Form(" raw #sigma ratio(x): Sector %d Bin %d ",s,b));
	  lab.DrawLatex(0.25, 0.925, Form(" Q^{2} (%.2f #rightarrow %.2f) GeV^{2}/c^{2} ",qq_start, qq_end));
	}

      canvas->Print( Form("%s.pdf",output_name.c_str()) );
      canvas->Clear();
    }

  // Model xs x-by-qq
  canvas->Divide(can_size+1, can_size);
  for (int b=0; b<h1_model_x_by_qq.size()-1; b++)
    {
      canvas->cd(b+1);
      
      double qq_start = qqBins.min() + (b-1)*qqBins.width();
      double qq_end   = qqBins.min() + b*qqBins.width();
      if (b == 0) { qq_start = qqBins.min(); qq_end = qqBins.max(); }
      
      h1_model_x_by_qq[b]->SetMarkerStyle(8);
      h1_model_x_by_qq[b]->Draw("P");
      lab.DrawLatex(0.3, 0.02, Form(" Model (x): Bin %d ",b));
      lab.DrawLatex(0.25, 0.925, Form(" Q^{2} (%.2f #rightarrow %.2f) GeV^{2}/c^{2} ",qq_start, qq_end));

      canvas->Update(); 
    }
  
  canvas->Print( Form("%s.pdf",output_name.c_str()) );
  canvas->Clear();
  

  // rec & hits x-by-qq
  for (int s=0; s<7; s++)
    {
      canvas->Divide(can_size+1, can_size);
      for (int b=0; b<h1_rec_x_by_qq[s].size()-1; b++)
	{
	  canvas->cd(b+1);

	  double qq_start = qqBins.min() + (b-1)*qqBins.width();
	  double qq_end   = qqBins.min() + b*qqBins.width();
	  if (b == 0) { qq_start = qqBins.min(); qq_end = qqBins.max(); }

	  h1_rec_x_by_qq[s][b]->SetFillStyle(3004);
	  h1_rec_x_by_qq[s][b]->SetFillColorAlpha(kRed,1.0);
	  h1_rec_x_by_qq[s][b]->SetLineColor(kRed);
	  h1_hits_x_by_qq[s][b]->Scale( 1/h1_hits_x_by_qq[s][b]->Integral() );
	  h1_rec_x_by_qq[s][b]->Scale( 1/h1_rec_x_by_qq[s][b]->Integral() );
	  h1_hits_x_by_qq[s][b]->Draw();
	  h1_rec_x_by_qq[s][b]->Draw("HISTsame");
	  lab.DrawLatex(0.3, 0.02, Form(" Hits & Rec Sector %d Bin %d ",s,b));
	  lab.DrawLatex(0.25, 0.925, Form(" Q^{2} (%.2f #rightarrow %.2f) GeV^{2}/c^{2} ",qq_start, qq_end));
	}

      canvas->Print( Form("%s.pdf",output_name.c_str()) );
      canvas->Clear();
    }


  */
  // 2d
  canvas->Divide(3, 2);
  for (int s=1; s<7; s++)
    {
      canvas->cd(s);
      h2_hits_x_qq[s]->Draw("colz");
    }
  canvas->Print( Form("%s.pdf",output_name.c_str()) );
  canvas->Clear();

  canvas->Divide(3, 2);
  for (int s=1; s<7; s++)
    {
      canvas->cd(s);
      h2_rec_x_qq[s]->Draw("colz");
    }
  canvas->Print( Form("%s.pdf",output_name.c_str()) );
  canvas->Clear();

  canvas->Divide(3, 2);
  for (int s=1; s<7; s++)
    {
      canvas->cd(s);
      h2_gen_x_qq[s]->Draw("colz");
    }
  canvas->Print( Form("%s.pdf",output_name.c_str()) );
  canvas->Clear();

  canvas->Print( Form("%s.pdf]",output_name.c_str()) );
}

void DISHistograms::save()
{
  TFile out(Form("%s.root",output_name.c_str()),"recreate");

  for (int s=0; s<7; s++)
    {
      h2_hits_x_qq[s] ->Write();
      h2_rec_x_qq[s]  ->Write();
      h2_gen_x_qq[s]  ->Write();
    }
  
  runInfoTree->Write();
  out.Write();
  out.Close();
}


////////////////////////////////////////////////////////////////////////
/*

  DISManager

*/
////////////////////////////////////////////////////////////////////////

DISManager::DISManager(string outputFile, bool r)
{
  outfile      = outputFile;
  parfile[0]   = "unset";
  parfile[1]   = "unset";
  momcorr_path = "unset";
  recalc       = r; 
  
  eid_version = 0;
  fcup_charge = 0.00; 
  
  qq_cut = new VirtualityCut();
  w_cut = new WCut();
  y_cut = new YCut(); 

  xBins  = DBins(100,0,1);
  qqBins = DBins(100,0,6);

  infofile = "unset";

  histos.output_name = outputFile; 
}

DISManager::~DISManager()
{
  // Destruir todo. 
}

void DISManager::fill_model(){

  // Get model 
  F1F209Wrapper model; 
  double beam_energy = 5.498; 
  
  for (int ibin=1; ibin<qqBins.number(); ibin++){
    for (int jbin=1; jbin<xBins.number(); jbin++) {
      double model_value = model.GetXS(1, 1, beam_energy, xBins.bin_center(jbin), qqBins.bin_center(ibin));

      if (model_value > 1e-20 && model_value < 1e20) {
	histos.h1_model_x_by_qq[ibin]->SetBinContent(jbin,model_value/mev_to_gev);
	histos.h1_model_x_by_qq[ibin]->SetBinError(jbin,0.00);
      }
      
      cout.width(12); cout << model.GetXS(1,1,beam_energy,xBins.bin_center(jbin),qqBins.bin_center(ibin));
      cout.width(12); cout << xBins.bin_center(jbin);
      cout.width(12); cout << qqBins.bin_center(ibin) << endl; 
    }
  }
}

void DISManager::get_charge(vector<string> files)
{
  cout << " Getting charge from database file. " << endl;

  if ( !info.has_information() ){
    cout << " Error opening database for load! " << endl;
    return;
  } 
  
  TRegexp regex("[1-9][0-9][0-9][0-9][0-9]");

  int    runno  = -1;
  double charge = -1.0;
  
  // Setting up information tree.
  TBranch * runnoBranch      = histos.runInfoTree->Branch("runno",      &runno);
  TBranch * fcupChargeBranch = histos.runInfoTree->Branch("fcupCharge", &charge);
  
  cout.width(12); cout << " runno: ";
  cout.width(12); cout << " charge: ";
  cout.width(12); cout << " tot charge: ";
  cout.width(12); cout << " events: " << endl;
  
  for (int f=0; f<files.size(); f++)
    {
      TString file(files[f]);
      TString srunno = file(regex);
      runno        = srunno.Atoi();
      charge       = (double) info.info[runno].dQ/info.info[runno].daq_scale;
      fcup_charge += charge;
      cout.width(12); cout << runno;
      cout.width(12); cout << charge;
      cout.width(12); cout << fcup_charge;
      cout.width(12); cout << info.info[runno].dN << endl;

      histos.runInfoTree->Fill();
    }

}

void DISManager::init()
{
  // Warning for not setting parameters.  
  if (recalc){
    if (parfile[0] == "unset" || parfile[1] == "unset" || outfile == "unset" || infofile == "unset")
      {
	cout << " Warning: Not all parameters have been set before initializing! " << endl;
	cout << " parfile[0] = " << parfile[0] << " parfile[1] = " << parfile[1] << " outfile = " << outfile << " infofile = " << infofile << endl;  
      }
  }
  
  // Setup Histograms
  histos.set_bins(xBins, qqBins);
  
  if (recalc) {
    reader[0].Init();
    reader[1].Init();
    histos.init();
  
    // Setting up ElectronSelector 
    eid[0].set_parfile( parfile[0] );
    eid[1].set_parfile( parfile[1] );
    eid[0].set_info(reader[0].runno(), reader[0].GSIM);
    eid[1].set_info(reader[1].runno(), reader[1].GSIM);
    
    // Setting up Electron ID from Nathan, needs to be updated while running. 
    nathan.set_info(reader[0].runno(), reader[0].GSIM);
    
    // Adding Cuts to DIS Event Selector 
    dis_selector.add_cut( qq_cut );
    dis_selector.add_cut( w_cut );
    dis_selector.add_cut( y_cut );

    info.load(infofile);
    if ( reader[0].GetEntries() == 0 || reader[1].GetEntries() == 0 ) { cout << " Fatal Error: Trying to initialize with no files in one of the readers. " << endl; exit(0); }  
  }

  // Doing just loading, slicing, drawing. 
  else {
    cout << " Loading histograms from... " << outfile << endl; 
    histos.load();     
  }

}

void DISManager::add_files(vector<string> files, int index)
{
  for (int ifile=0; ifile<files.size(); ifile++) reader[index].AddFile(files[ifile]);
}

void DISManager::do_xs()
{
  cout << " Inside of DISManager::do_xs() with " << histos.h2_hits_x_qq[0]->GetEntries() << " entries. " << endl;

  // Exists in histo.sect <-- Should be used instead of redeclaration. 
  string sect[7] = {"all","s1","s2","s3","s4","s5","s6"};

  F1F209Wrapper model; 

  // Creating 1-Dim histograms from 2-Dim ones
  // We need to use ProjectionX() to collapse the 2-Dim
  // histograms into 1-D, but first we need to discover the binning scheme used
  // for the 2-D histos so that we can collapse the corretly into the new bin
  // scheme.

  int n_micro_bins_x  = histos.h2_hits_x_qq[0]->GetXaxis()->GetNbins();
  double micro_x_max  = histos.h2_hits_x_qq[0]->GetXaxis()->GetXmax();
  double micro_x_min  = histos.h2_hits_x_qq[0]->GetXaxis()->GetXmin(); 

  int n_micro_bins_qq = histos.h2_hits_x_qq[0]->GetYaxis()->GetNbins();
  double micro_qq_max = histos.h2_hits_x_qq[0]->GetYaxis()->GetXmax();
  double micro_qq_min = histos.h2_hits_x_qq[0]->GetYaxis()->GetXmin(); 
  
  cout << " Discovered binning scheme: " << endl;
  cout << "      x bins: " << endl;
  cout << "      " << n_micro_bins_x << " " << micro_x_min << " " << micro_x_max << endl;
  cout << "      qq bins: " << endl;
  cout << "      " << n_micro_bins_qq << " " << micro_qq_min << " " << micro_qq_max << endl;
  
  // Doing Cross Section AFTER creating histograms from 2D histograms.  

  /*
  for (int s=0; s<7; s++)
    {
      for (int b=0; b<qqBins.number(); b++)
	{
	  histos.h1_hits_x_by_qq[s][b] ->Sumw2();
	  histos.h1_rec_x_by_qq[s][b]  ->Sumw2();
	  histos.h1_gen_x_by_qq[s][b]  ->Sumw2();

	  string name  = Form("h1_acc_x_by_qq_%d_%s",b,sect[s].c_str());
	  string title = Form("Acc. for x-by-qq Bin %d Sector %s",b,sect[s].c_str());
	  histos.h1_acc_x_by_qq[s][b] = (TH1F*) histos.h1_rec_x_by_qq[s][b]->Clone();
	  histos.h1_acc_x_by_qq[s][b]->Divide( histos.h1_gen_x_by_qq[s][b] ); 
	  histos.h1_acc_x_by_qq[s][b]->SetTitle(title.c_str());
	  histos.h1_acc_x_by_qq[s][b]->SetName(name.c_str());

	  name  = Form("h1_rxs_x_by_qq_%d_%s",b,sect[s].c_str());
	  title = Form("rxs for x-by-qq Bin %d Sector %s",b,sect[s].c_str());
	  histos.h1_rxs_x_by_qq[s][b] = (TH1F*) histos.h1_hits_x_by_qq[s][b]->Clone();
	  histos.h1_rxs_x_by_qq[s][b]->Divide( histos.h1_acc_x_by_qq[s][b] ); 
	  histos.h1_rxs_x_by_qq[s][b]->SetTitle(title.c_str());
	  histos.h1_rxs_x_by_qq[s][b]->SetName(name.c_str());
	  histos.h1_rxs_x_by_qq[s][b]->Scale(1/(xBins.width()*qqBins.width())); 
	  histos.h1_rxs_x_by_qq[s][b]->Scale(cm_to_outhouse*(electron_c*hydrogen_molar_weight/(avogadro*hydrogen_density)));

	  name  = Form("h1_xs_x_by_qq_%d_%s",b,sect[s].c_str());
	  title = Form("xs for x-by-qq Bin %d Sector %s",b,sect[s].c_str());
	  histos.h1_xs_x_by_qq[s][b] = (TH1F*) histos.h1_rxs_x_by_qq[s][b]->Clone();
	  histos.h1_xs_x_by_qq[s][b]->SetTitle(title.c_str());
	  histos.h1_xs_x_by_qq[s][b]->SetName(name.c_str());

	  name  = Form("h1_rxs_ratio_x_by_qq_%d_%s",b,sect[s].c_str());
	  title = Form("rxs ratio for x-by-qq Bin %d Sector %s",b,sect[s].c_str());
	  histos.h1_rxs_ratio_x_by_qq[s][b] = (TH1F*) histos.h1_rxs_x_by_qq[s][b]->Clone();
	  histos.h1_rxs_ratio_x_by_qq[s][b]->Divide( histos.h1_model_x_by_qq[b] );
	  histos.h1_rxs_ratio_x_by_qq[s][b]->SetTitle(title.c_str());
	  histos.h1_rxs_ratio_x_by_qq[s][b]->SetName(name.c_str());

	  name  = Form("h1_xs_ratio_x_by_qq_%d_%s",b,sect[s].c_str());
	  title = Form("xs ratio for x-by-qq Bin %d Sector %s",b,sect[s].c_str());
	  histos.h1_xs_ratio_x_by_qq[s][b] = (TH1F*) histos.h1_xs_x_by_qq[s][b]->Clone();
	  histos.h1_xs_ratio_x_by_qq[s][b]->Divide( histos.h1_model_x_by_qq[b] );
	  histos.h1_xs_ratio_x_by_qq[s][b]->SetTitle(title.c_str());
	  histos.h1_xs_ratio_x_by_qq[s][b]->SetName(name.c_str());	  
	}
    }
  */  
}

void DISManager::loop(int index)
{
  // General Setup 
  int nev   = reader[index].GetEntries(); 
  int runno = reader[index].runno(); 

  MomCorr_e1f momcorr(momcorr_path);
  
  cout << " Starting DISManager::loop() for index " << index << " with " << nev << " entries, using eid version " << eid_version << endl;  

  if (eid_version == 0) { 
    for (int iev=0; iev<nev; iev++)
      {
	if (iev%1000 == 0){ cout << "\r done " << iev << " of " << nev << flush; } 
	reader[index].GetEntry(iev); 
	DEvent event( reader[index].GetEvent() );
	if (runno != reader[index].runno()) { runno = reader[index].runno(); nathan.set_info(runno, reader[index].GSIM); }

	// Do generated for MC 
	if (index == 1) {
	  histos.fill_gen(event);
	}
	
	int e_index = nathan.get_electron(event.tracks); 
	if (e_index > -123) {
	  
 	  TLorentzVector electron(event.tracks.p[e_index]*event.tracks.cx[e_index],
				  event.tracks.p[e_index]*event.tracks.cy[e_index],
				  event.tracks.p[e_index]*event.tracks.cz[e_index],
				  event.tracks.p[e_index]);

	  if (index == 0) { electron = momcorr.PcorN(electron, -1, 11); }
	  event.set_e_index(e_index);
	  event.set_electron(electron);
	  
	  if (dis_selector.passes(event, e_index)) {
	    histos.fill(event, index);
	  }
	}
      }
  }

  else
    {
      for (int iev=0; iev<nev; iev++)
	{
	  if (iev%1000 == 0){ cout << "\r done " << iev << " of " << nev << flush; } 
	  
	  reader[index].GetEntry(iev); 
	  DEvent event( reader[index].GetEvent() );
	  if (runno != reader[index].runno()) { runno = reader[index].runno(); eid[index].set_info(runno, reader[index].GSIM); }

	  // Do generated for MC 
	  if (index == 1) {
	    histos.fill_gen(event);

	    event.set_electron( TLorentzVector(event.tracks.mcpx(0), event.tracks.mcpy(0), event.tracks.mcpz(0), event.tracks.mcp[0])); 
	  }
	  
	  if (eid[index].passes(event, 0))
	    {
	      // Doing Mom Corrections 
	      TLorentzVector electron(event.tracks.p[0]*event.tracks.cx[0],
				      event.tracks.p[0]*event.tracks.cy[0],
				      event.tracks.p[0]*event.tracks.cz[0],
				      event.tracks.p[0]);
	      
	      if (index == 0) { electron = momcorr.PcorN(electron, -1, 11); }
	      event.set_e_index(0);
	      event.set_electron(electron);

	      if (dis_selector.passes(event, 0)){
		histos.fill(event, index);
	      }
	    }
	}
    }

  cout << endl;
}

#endif
