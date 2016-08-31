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
#include "NathanArchive.h"
#include "MomCorr.h"

//  root includes
#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include "TLatex.h"
#include "TLorentzVector.h"
#include "TRegexp.h"
#include "TStyle.h"
#include "TString.h"

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
}


DISHistograms::~DISHistograms()
{
  // Destroy things. 
}

void DISHistograms::init()
{
  // Initialize Histograms

  // Generated Events for x-by-qq
  for (int s=0; s<7; s++) {
    vector<TH1F*> v;
    for (int b=0; b<(qqBins.number()+1); b++)
      {
	string name  = Form("h1_gen_x_by_qq_%d_%s",b,sect[s].c_str());
	string title = Form("gen x-by-qq for bin %d of %s",b,sect[s].c_str());
	v.push_back( new TH1F(name.c_str(), title.c_str(), xBins.number(), xBins.min(), xBins.max()) );
      }
    h1_gen_x_by_qq.push_back(v);
  }

  // Reconstructed Events for x-by-qq
  for (int s=0; s<7; s++) {
    vector<TH1F*> v;
    for (int b=0; b<(qqBins.number()+1); b++)
      {
	string name  = Form("h1_rec_x_by_qq_%d_%s",b,sect[s].c_str());
	string title = Form("rec x-by-qq for bin %d of %s",b,sect[s].c_str());
	v.push_back( new TH1F(name.c_str(), title.c_str(), xBins.number(), xBins.min(), xBins.max()) );
      }
    h1_rec_x_by_qq.push_back(v);
  }

  // Data Events for x-by-qq
  for (int s=0; s<7; s++) {
    vector<TH1F*> v;
    for (int b=0; b<(qqBins.number()+1); b++)
      {
	string name  = Form("h1_hits_x_by_qq_%d_%s",b,sect[s].c_str());
	string title = Form("hits x-by-qq for bin %d of %s",b,sect[s].c_str());
	v.push_back( new TH1F(name.c_str(), title.c_str(), xBins.number(), xBins.min(), xBins.max()) );
      }
    h1_hits_x_by_qq.push_back(v);
  }

  // Acceptance for x-by-qq
  for (int s=0; s<7; s++) {
    vector<TH1F*> v;
    for (int b=0; b<(qqBins.number()+1); b++)
      {
	string name  = Form("h1_acc_x_by_qq_%d_%s",b,sect[s].c_str());
	string title = Form("Acc. x-by-qq for bin %d of %s",b,sect[s].c_str());
	v.push_back( new TH1F(name.c_str(), title.c_str(), xBins.number(), xBins.min(), xBins.max()) );
      }
    h1_acc_x_by_qq.push_back(v);
  }

  // rxs for x-by-qq
  for (int s=0; s<7; s++) {
    vector<TH1F*> v;
    for (int b=0; b<(qqBins.number()+1); b++)
      {
	string name  = Form("h1_rxs_x_by_qq_%d_%s",b,sect[s].c_str());
	string title = Form("rxs x-by-qq for bin %d of %s",b,sect[s].c_str());
	v.push_back( new TH1F(name.c_str(), title.c_str(), xBins.number(), xBins.min(), xBins.max()) );
      }
    h1_rxs_x_by_qq.push_back(v);
  }

  // xs for x-by-qq
  for (int s=0; s<7; s++) {
    vector<TH1F*> v;
    for (int b=0; b<(qqBins.number()+1); b++)
      {
	string name  = Form("h1_xs_x_by_qq_%d_%s",b,sect[s].c_str());
	string title = Form("xs x-by-qq for bin %d of %s",b,sect[s].c_str());
	v.push_back( new TH1F(name.c_str(), title.c_str(), xBins.number(), xBins.min(), xBins.max()) );
      }
    h1_xs_x_by_qq.push_back(v);
  }
}

void DISHistograms::fill(DEvent event, int index)
{
  if (event.e_index == -123){ cout << " You must set DEvent::e_index before filling histograms! " << endl; return; }
  
  int s = event.tracks.dc_sect[event.e_index];
  int xbin  = 1+xBins .find_bin( event.x );
  int qqbin = 1+qqBins.find_bin( event.qq);

  // Protect agaisnt Seg fault 
  if (xbin < 0 || qqbin < 0 ) return;
  if (xbin >= xBins.number() || qqbin >= qqBins.number()) return; 

  // Monte Carlo 
  if (index == 1) {
    h1_rec_x_by_qq[0][0]     ->Fill(event.x);
    h1_rec_x_by_qq[0][qqbin] ->Fill(event.x);
    h1_rec_x_by_qq[s][0]     ->Fill(event.x);
    h1_rec_x_by_qq[s][qqbin] ->Fill(event.x);
  }

  // Data Events
  else if (index == 0) {
    h1_hits_x_by_qq[0][0]     ->Fill(event.x);
    h1_hits_x_by_qq[0][qqbin]     ->Fill(event.x);
    h1_hits_x_by_qq[s][0]     ->Fill(event.x);
    h1_hits_x_by_qq[s][qqbin] ->Fill(event.x);
  }

}

void DISHistograms::fill_gen(DEvent event)
{
  int e_index = 0;
  for (int ipart=0; ipart<event.tracks.gpart; ipart++)
    {
      if (event.tracks.mcpid[ipart] == 11)
	{
	  e_index = ipart;
	  break;
	} 
    }

  TLorentzVector gen_electron(event.tracks.mcpx(e_index), event.tracks.mcpy(e_index), event.tracks.mcpz(e_index), event.tracks.mcp[e_index]);
  event.set_electron(gen_electron); 

  // Filling gen histograms
  int s = floor(event.tracks.mcphi[e_index]/60) +1; 
  int xbin  = 1+xBins .find_bin( event.x );
  int qqbin = 1+qqBins.find_bin( event.qq);

  // Protect agaisnt Seg fault 
  if (xbin < 0 || qqbin < 0 ) return;
  if (xbin >= xBins.number() || qqbin >= qqBins.number() ) return;

  h1_gen_x_by_qq[0][0]     ->Fill(event.x);
  h1_gen_x_by_qq[0][qqbin] ->Fill(event.x);
  h1_gen_x_by_qq[s][0]     ->Fill(event.x);
  h1_gen_x_by_qq[s][qqbin] ->Fill(event.x);
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

  canvas->Print( Form("%s.pdf]",output_name.c_str()) );
}

void DISHistograms::save()
{
  TFile out(Form("%s.root",output_name.c_str()),"recreate");

  for (int s=0; s<7; s++)
    {
      for (int b=0; b<=qqBins.number(); b++)
	{
	  h1_hits_x_by_qq[s][b] ->Write();
	  h1_rec_x_by_qq[s][b]  ->Write();
	  h1_gen_x_by_qq[s][b]  ->Write();
	  h1_acc_x_by_qq[s][b] ->Write();
	  h1_rxs_x_by_qq[s][b] ->Write();
	  h1_xs_x_by_qq[s][b] ->Write();
	}
    }

  out.Write();
  out.Close();
}


////////////////////////////////////////////////////////////////////////
/*

  DISManager

*/
////////////////////////////////////////////////////////////////////////

DISManager::DISManager()
{
  outfile     = "unset";
  parfile[0]  = "unset";
  parfile[1]  = "unset";
  momcorr_path = "unset";
  
  eid_version = 0;
  fcup_charge = 0.00; 
  
  qq_cut = new VirtualityCut();
  w_cut = new WCut();
  y_cut = new YCut(); 

  xBins  = DBins(100,0,1);
  qqBins = DBins(100,0,6);

  infofile = "unset";

  // Really don't want to do this.  
  xLims[0] = 0.1;
  xLims[1] = 0.2;
  xLims[2] = 0.3;
  xLims[3] = 0.4;
  xLims[4] = 0.5; 
  xLims[5] = 0.6; 

  // For x - 0.1-0.2
  qqLims[0][0] = 1.0;
  qqLims[0][1] = 1.3;
  qqLims[0][2] = 5.0; 

  // For x = 0.2-0.3 
  qqLims[1][0] = 1.0;
  qqLims[1][1] = 1.7;
  qqLims[1][2] = 5.0; 

  // For x = 0.3-0.4
  qqLims[2][0] = 1.0;
  qqLims[2][1] = 2.2;
  qqLims[2][2] = 5.0; 

  // For x = 0.4-0.5
  qqLims[3][0] = 1.0;
  qqLims[3][1] = 2.9;
  qqLims[3][2] = 5.0; 

  // For x = 0.5-0.6
  qqLims[4][0] = 1.0;
  qqLims[4][1] = 5.0;
  qqLims[4][2] = 99.0; 

  for (int s=0; s<7; s++) 
    for (int ixbin=0; ixbin<n_x_bins; ixbin++)
      for (int iqqbin=0; iqqbin<n_qq_bins; iqqbin++)
	{
	  hits[s][ixbin][iqqbin] = 0;
	  rec[s][ixbin][iqqbin]  = 0;
	  gen[s][ixbin][iqqbin]  = 0;
	  acc[s][ixbin][iqqbin]  = 0.0;
	  xs[s][ixbin][iqqbin]   = 0.0;
	  corr_hits[s][ixbin][iqqbin]      = 0.0;
	  data_mc_ratio[s][ixbin][iqqbin]  = 0.0;
      }
  
}

DISManager::~DISManager()
{
  // Destruir todo. 
}

void DISManager::get_charge(vector<string> files)
{
  cout << " Getting charge from database file. " << endl;

  if ( !info.has_information() ){
    cout << " Error opening database for load! " << endl;
    return;
  } 
  
  TRegexp regex("[1-9][0-9][0-9][0-9][0-9]");

  cout.width(12); cout << " runno: ";
  cout.width(12); cout << " charge: ";
  cout.width(12); cout << " tot charge: ";
  cout.width(12); cout << " events: " << endl;
  
  for (int f=0; f<files.size(); f++)
    {
      TString file(files[f]);
      TString srunno = file(regex);
      int runno      = srunno.Atoi();
      double charge  = (double) info.info[runno].dQ/info.info[runno].daq_scale;
      fcup_charge    += charge;
      cout.width(12); cout << runno;
      cout.width(12); cout << charge;
      cout.width(12); cout << fcup_charge;
      cout.width(12); cout << info.info[runno].dN << endl; 
    }
}

void DISManager::init()
{
  // Warning for not setting parameters.  
  if (parfile[0] == "unset" || parfile[1] == "unset" || outfile == "unset" || infofile == "unset")
    {
      cout << " Warning: Not all parameters have been set before initializing! " << endl;
      cout << " parfile[0] = " << parfile[0] << " parfile[1] = " << parfile[1] << " outfile = " << outfile << " infofile = " << infofile << endl;  
    }

  reader[0].Init();
  reader[1].Init();

  if ( reader[0].GetEntries() == 0 || reader[1].GetEntries() == 0 ) { cout << " Fatal Error: Trying to initialize with no files in one of the readers. " << endl; exit(0); }  
  
  // Setup Histograms
  histos.output_name = outfile; 
  histos.set_bins(xBins, qqBins);
  histos.init();

  // Setting up ElectronSelector 
  eid[0].set_parfile( parfile[0] );
  eid[1].set_parfile( parfile[1] );
  eid[0].set_info(reader[0].runno(), reader[0].GSIM);
  eid[1].set_info(reader[1].runno(), reader[1].GSIM);
 
  // Setting up Electron ID from Nathan, needs to be updated while running. 
  nathan.set_info(reader[0].runno(), reader[0].GSIM);

  // Adding Cuts 
  dis_selector.add_cut( qq_cut );
  dis_selector.add_cut( w_cut );
  dis_selector.add_cut( y_cut );

  // Loading information for runs
  info.load(infofile);
}

void DISManager::add_files(vector<string> files, int index)
{
  for (int ifile=0; ifile<files.size(); ifile++) reader[index].AddFile(files[ifile]);
}

void DISManager::do_xs()
{
  string sect[7] = {"all","s1","s2","s3","s4","s5","s6"};

  // Doing Cross Section
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

	  name  = Form("h1_xs_x_by_qq_%d_%s",b,sect[s].c_str());
	  title = Form("xs for x-by-qq Bin %d Sector %s",b,sect[s].c_str());
	  histos.h1_xs_x_by_qq[s][b] = (TH1F*) histos.h1_rxs_x_by_qq[s][b]->Clone();
	  histos.h1_xs_x_by_qq[s][b]->SetTitle(title.c_str());
	  histos.h1_xs_x_by_qq[s][b]->SetName(name.c_str());
	}
    }
  
  
}

void DISManager::loop(int index)
{
  // General Setup 
  int nev = reader[index].GetEntries(); 
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

	  event.set_electron( TLorentzVector(event.tracks.mcpx(0), event.tracks.mcpy(0), event.tracks.mcpz(0), event.tracks.mcp[0])); 
	  int xb     = find_x_bin(event.x);
	  int qqb    = find_qq_bin(event.x, event.qq); 
	  int mcsect = floor(event.tracks.mcphi[0]/60.0) +1; 
	  if (xb > -1 && qqb > -1 && mcsect > 0) { gen[0][xb][qqb]++; gen[mcsect][xb][qqb]++; }
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

	    // Assigning this event to a bin. 
	    if (index == 0) {
	      int xb  = find_x_bin(event.x);
	      int qqb = find_qq_bin(event.x, event.qq); 
	      int sect = event.tracks.dc_sect[e_index]; 
	      if (xb > -1 && qqb > -1 && sect > 0) { hits[0][xb][qqb]++; hits[sect][xb][qqb]++; }
	    }
	    
	    else {
	      int xb  = find_x_bin(event.x);
	      int qqb = find_qq_bin(event.x, event.qq); 
	      int sect = event.tracks.dc_sect[e_index]; 
	      if (xb > -1 && qqb > -1 && sect > 0) { rec[0][xb][qqb]++; rec[sect][xb][qqb]++; }
	    }
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
	    int xb     = find_x_bin(event.x);
	    int qqb    = find_qq_bin(event.x, event.qq); 
	    int mcsect = floor(event.tracks.mcphi[0]/60.0) +1; 
	    if (xb > -1 && qqb > -1 && mcsect > 0) { gen[0][xb][qqb]++; gen[mcsect][xb][qqb]++; }
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

		// Assigning this event to a bin. 
		if (index == 0) {
		  int xb  = find_x_bin(event.x);
		  int qqb = find_qq_bin(event.x, event.qq); 
		  int sect = event.tracks.dc_sect[0]; 
		  if (xb > -1 && qqb > -1 && sect > 0) { hits[0][xb][qqb]++; hits[sect][xb][qqb]++; }
		}
		
		else {
		  int xb  = find_x_bin(event.x);
		  int qqb = find_qq_bin(event.x, event.qq); 
		  int sect = event.tracks.dc_sect[0]; 
		  if (xb > -1 && qqb > -1 && sect > 0) { rec[0][xb][qqb]++; rec[sect][xb][qqb]++; }
		}
	      }
	    }
	}
    }
}

void DISManager::print_table()
{
  // Calculating things.
  int data_max[7] = {0,0,0,0,0,0,0};
  int mc_max[7]   = {0,0,0,0,0,0,0}; 

  // Getting max of each.
  for (int s=0; s<7; s++)
    for (int i=0; i<n_x_bins; i++)
      {
	for (int j=0; j<n_qq_bins; j++)
	  {
	    if (hits[s][i][j] > data_max[s]) data_max[s] = hits[s][i][j];
	    if (rec[s][i][j]  > mc_max[s])     mc_max[s] = rec[s][i][j]; 
	  }
      }
  
  // Calculating Acceptance. 
  for (int s=0; s<7; s++)
    for (int i=0; i<n_x_bins; i++)
      {
	for (int j=0; j<n_qq_bins; j++)
	  {
	    acc[s][i][j] = (double) rec[s][i][j]/gen[s][i][j];
	    data_mc_ratio[s][i][j] = (double) ( (double) hits[s][i][j]/data_max[s])/( (double) rec[s][i][j]/mc_max[s]);
	    corr_hits[s][i][j]     = (double) hits[s][i][j]/acc[s][i][j];
	    xs[s][i][j] = cm_to_outhouse*(electron_c*hydrogen_molar_weight/(avogadro*hydrogen_density))*corr_hits[s][i][j]/((xLims[i+1]-xLims[i])*(qqLims[i][j+1]-qqLims[i][j]));
	  }
      }
  
  cout << "----->  Results: <-----" << endl; 
  cout.width(8); cout << "sect";
  cout.width(8); cout << "x left";
  cout.width(8); cout << "x right";
  cout.width(8); cout << "qq left";
  cout.width(8); cout << "qq right"; 
  cout.width(8); cout << "raw hits";
  cout.width(8); cout << "rec";
  cout.width(8); cout << "gen";
  cout.width(12); cout << "acc";
  cout.width(12); cout << "corr hit";
  cout.width(12); cout << " xs ";
  cout.width(12); cout << "data/mc" << endl;

  for (int s=0; s<7; s++) 
    for (int i=0; i<n_x_bins; i++)
      {
	for (int j=0; j<n_qq_bins; j++)
	  {
	    cout.width(8); cout << s;
	    cout.width(8); cout << xLims[i];
	    cout.width(8); cout << xLims[i+1];
	    cout.width(8); cout << qqLims[i][j];
	    cout.width(8); cout << qqLims[i][j+1]; 
	    cout.width(8); cout << hits[s][i][j];
	    cout.width(8); cout << rec[s][i][j];
	    cout.width(8); cout << gen[s][i][j];
	    cout.width(12); cout << acc[s][i][j];
	    cout.width(12); cout << corr_hits[s][i][j];
	    cout.width(12); cout << xs[s][i][j];
	    cout.width(12); cout << data_mc_ratio[s][i][j] << endl;
	  }
      }
}

int DISManager::find_x_bin(double x)
{
  for (int i=0; i<n_x_bins; i++) { if ( x > xLims[i] && x < xLims[i+1] ) return i; }
  return -123;
}

int DISManager::find_qq_bin(double x, double qq)
{
  int xb = find_x_bin(x);
  if (xb < 0) return -123;
  
  for (int i=0; i<n_qq_bins; i++) { if ( qq > qqLims[xb][i] && qq < qqLims[xb][i+1] ) return i; }
  return -123;
}

#endif
