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
#include "TChain.h"
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

}

void DISHistograms::init()
{
  // Initialize Histograms
  for (int s=0; s<7; s++) {
    h2_hits_x_qq[s] = new TH2F(Form("h2_hits_x_qq_%d",s),Form("Hits QQ vs. X Sector %d",s),xBins.number(),xBins.min(),xBins.max(),qqBins.number(),qqBins.min(),qqBins.max());
    h2_rec_x_qq[s]  = new TH2F(Form("h2_rec_x_qq_%d",s), Form("Rec QQ vs. X Sector %d",s), xBins.number(),xBins.min(),xBins.max(),qqBins.number(),qqBins.min(),qqBins.max());
    h2_gen_x_qq[s]  = new TH2F(Form("h2_gen_x_qq_%d",s), Form("Gen QQ vs. X Sector %d",s), xBins.number(),xBins.min(),xBins.max(),qqBins.number(),qqBins.min(),qqBins.max()); 
    h2_hits_w_qq[s] = new TH2F(Form("h2_hits_w_qq_%d",s),Form("Hits QQ vs. W Sector %d",s),wBins.number(),wBins.min(),wBins.max(),qqBins.number(),qqBins.min(),qqBins.max());
    h2_rec_w_qq[s]  = new TH2F(Form("h2_rec_w_qq_%d",s), Form("Rec QQ vs. W Sector %d",s), wBins.number(),wBins.min(),wBins.max(),qqBins.number(),qqBins.min(),qqBins.max());
    h2_gen_w_qq[s]  = new TH2F(Form("h2_gen_w_qq_%d",s), Form("Gen QQ vs. W Sector %d",s), wBins.number(),wBins.min(),wBins.max(),qqBins.number(),qqBins.min(),qqBins.max()); 
  }

  h1_fcup_charge = new TH1D("h1_fcup_charge","",xBins.number(),xBins.min(),xBins.max());
}

void DISHistograms::fill(DEvent event, int index)
{
  if (event.e_index == -123){ cout << " You must set DEvent::e_index before filling histograms! " << endl; return; }
  
  int s = event.tracks.dc_sect[event.e_index];

  // Monte Carlo 
  if (index == 1) {
    h2_rec_x_qq[s]->Fill(event.x, event.qq);
    h2_rec_x_qq[0]->Fill(event.x, event.qq);
    h2_rec_w_qq[s]->Fill(event.w, event.qq);
    h2_rec_w_qq[0]->Fill(event.w, event.qq);
  }

  // Data Events
  else if (index == 0) {
    h2_hits_x_qq[s]->Fill(event.x, event.qq);
    h2_hits_x_qq[0]->Fill(event.x, event.qq);
    h2_hits_w_qq[s]->Fill(event.w, event.qq);
    h2_hits_w_qq[0]->Fill(event.w, event.qq);
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

  if (s > 0 && s < 7){ 
    // Do the fill. 
    h2_gen_x_qq[0]->Fill(event.x, event.qq);
    h2_gen_x_qq[s]->Fill(event.x, event.qq);
    h2_gen_w_qq[0]->Fill(event.w, event.qq);
    h2_gen_w_qq[s]->Fill(event.w, event.qq);
  }
}

void DISHistograms::load(){

  //  string sect[7] = {"all","s1","s2","s3","s4","s5","s6"};
  f = TFile::Open( output_name.c_str() );
  runInfoTree = (TTree*) f->Get("runInfoTree"); 
  
  if ( !f->IsOpen() ) {
    cout << " Fatal Error in: DISHistograms::load(). File not opened! " << output_name << endl;
    exit(0);
  }

  for (int s=0; s<7; s++){
    h2_hits_x_qq[s] = (TH2F*) f->Get( Form("h2_hits_x_qq_%d",s) );
    h2_rec_x_qq[s]  = (TH2F*) f->Get( Form("h2_rec_x_qq_%d", s) );
    h2_gen_x_qq[s]  = (TH2F*) f->Get( Form("h2_gen_x_qq_%d", s) );

    h2_hits_w_qq[s] = (TH2F*) f->Get( Form("h2_hits_w_qq_%d",s) );
    h2_rec_w_qq[s]  = (TH2F*) f->Get( Form("h2_rec_w_qq_%d", s) );
    h2_gen_w_qq[s]  = (TH2F*) f->Get( Form("h2_gen_w_qq_%d", s) );

    h2_hits_x_qq_rebin[s] = (TH2F*) h2_hits_x_qq[s]->Clone();
    h2_hits_x_qq_rebin[s]->SetTitle(Form("h2_hits_x_qq_rebin_%d",s));

    h2_rec_x_qq_rebin[s]  = (TH2F*) h2_rec_x_qq[s]->Clone();
    h2_rec_x_qq_rebin[s]->SetTitle(Form("h2_rec_x_qq_rebin_%d",s));

    h2_gen_x_qq_rebin[s]  = (TH2F*) h2_gen_x_qq[s]->Clone();
    h2_gen_x_qq_rebin[s]->SetTitle(Form("h2_gen_x_qq_rebin_%d",s)); 

    h2_hits_w_qq_rebin[s] = (TH2F*) h2_hits_w_qq[s]->Clone();
    h2_hits_w_qq_rebin[s]->SetTitle(Form("h2_hits_w_qq_rebin_%d",s));

    h2_rec_w_qq_rebin[s]  = (TH2F*) h2_rec_w_qq[s]->Clone();
    h2_rec_w_qq_rebin[s]->SetTitle(Form("h2_rec_w_qq_rebin_%d",s));

    h2_gen_w_qq_rebin[s]  = (TH2F*) h2_gen_w_qq[s]->Clone();
    h2_gen_w_qq_rebin[s]->SetTitle(Form("h2_gen_w_qq_rebin_%d",s)); 
  }

  h1_fcup_charge = (TH1D*) f->Get("h1_fcup_charge");
  cout << " Total Charge (uC) From File " << output_name << " = " << h1_fcup_charge->GetBinContent(1) << endl;
  
}

void DISHistograms::draw()
{
  // Draw Histograms 
  TCanvas * canvas = new TCanvas("canvas","",1200,800);

  // Standard
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  gStyle->SetOptTitle(0);
  gStyle->SetPalette(112);
  
  // Latex Writer
  TLatex lab;
  lab.SetNDC();
  lab.SetTextFont(22);
  
  canvas->Print( Form("%s.pdf[",output_name.c_str()) );
  canvas->Clear();

  int can_size = floor(sqrt(h1_hits_x_by_qq[0].size()));
  double integ_lumi = ((avogadro*5.00*hydrogen_density*1e-6*h1_fcup_charge->GetBinContent(1))/(hydrogen_molar_weight*electron_c));
  
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
	  lab.DrawLatex(0.15,0.85,Form("Entries: %0.f",h1_hits_x_by_qq[s][b]->GetEntries()));
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
	  lab.DrawLatex(0.15,0.85,Form("Entries: %0.f",h1_rec_x_by_qq[s][b]->GetEntries()));
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
	  lab.DrawLatex(0.15,0.85,Form("Entries: %0.f",h1_gen_x_by_qq[s][b]->GetEntries()));
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
	  lab.DrawLatex(0.15,0.85,Form("Entries: %0.f",h1_hits_x_by_qq[s][b]->GetEntries()));
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
	  lab.DrawLatex(0.15,0.85,Form("Entries: %0.f",h1_hits_x_by_qq[s][b]->GetEntries()));
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
  for (int b=0; b<h1_model_x_by_qq.size(); b++)
    {
      canvas->cd(b+1);
      
      double qq_start = qqBins.min() + (b-1)*qqBins.width();
      double qq_end   = qqBins.min() + b*qqBins.width();
      if (b == 0) { qq_start = qqBins.min(); qq_end = qqBins.max(); }
      
      h1_model_x_by_qq[b]->SetMarkerStyle(7);
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

  // 2d
  canvas->Divide(3, 2);
  for (int s=1; s<7; s++)
    {
      canvas->cd(s);
      gPad->SetMargin(0.15,0.1,0.1,0.1); 
      gPad->SetLogz();
      h2_hits_x_qq[s]->Draw("colz");
      lab.DrawLatex(0.2,0.85,Form("Entries: %0.f",h2_hits_x_qq[s]->GetEntries()));
      lab.DrawLatex(0.325,0.925,Form("Hits Q^{2} vs. x #rightarrow Sector %d",s)); 
      lab.DrawLatex(0.45,0.025,"x_{Bjorken}  "); 
      lab.SetTextAngle(90);
      lab.DrawLatex(0.05, 0.35,"Q^{2} [GeV^{2}/c^{2}]"); 
      lab.SetTextAngle(0);
    }
  canvas->Print( Form("%s.pdf",output_name.c_str()) );
  canvas->Clear();

  canvas->Divide(3, 2);
  for (int s=1; s<7; s++)
    {
      canvas->cd(s);
      gPad->SetMargin(0.15,0.1,0.1,0.1); 
      gPad->SetLogz();
      h2_rec_x_qq[s]->Draw("colz");
      lab.DrawLatex(0.2,0.85,Form("Entries: %0.f",h2_rec_x_qq[s]->GetEntries())); 
      lab.DrawLatex(0.3,0.925,Form("Rec. MC Q^{2} vs. x #rightarrow Sector %d",s)); 
      lab.DrawLatex(0.45,0.025,"x_{Bjorken}  "); 
      lab.SetTextAngle(90);
      lab.DrawLatex(0.05, 0.35,"Q^{2} [GeV^{2}/c^{2}]"); 
      lab.SetTextAngle(0);
    }
  canvas->Print( Form("%s.pdf",output_name.c_str()) );
  canvas->Clear();

  canvas->Divide(3, 2);
  for (int s=1; s<7; s++)
    {
      canvas->cd(s);
      gPad->SetMargin(0.15,0.1,0.1,0.1); 
      gPad->SetLogz();
      h2_gen_x_qq[s]->Draw("colz");
      lab.DrawLatex(0.2,0.85,Form("Entries: %0.f",h2_gen_x_qq[s]->GetEntries())); 
      lab.DrawLatex(0.3,0.925,Form("Gen MC Q^{2} vs. x #rightarrow Sector %d",s)); 
      lab.DrawLatex(0.45,0.025,"x_{Bjorken}  "); 
      lab.SetTextAngle(90);
      lab.DrawLatex(0.05, 0.35,"Q^{2} [GeV^{2}/c^{2}]"); 
      lab.SetTextAngle(0);
    }
  canvas->Print( Form("%s.pdf",output_name.c_str()) );
  canvas->Clear();

  canvas->Divide(3, 2);
  for (int s=1; s<7; s++)
    {
      canvas->cd(s);
      gPad->SetMargin(0.15,0.1,0.1,0.1); 
      gPad->SetLogz();
      h2_hits_x_qq_rebin[s]->Draw("colz");
      lab.DrawLatex(0.2,0.85,Form("Entries: %0.f",h2_hits_x_qq[s]->GetEntries())); 
      lab.DrawLatex(0.325,0.925,Form("Hits Q^{2} vs. x #rightarrow Sector %d",s)); 
      lab.DrawLatex(0.45,0.025,"x_{Bjorken}  "); 
      lab.SetTextAngle(90);
      lab.DrawLatex(0.05, 0.35,"Q^{2} [GeV^{2}/c^{2}]"); 
      lab.SetTextAngle(0);
    }
  canvas->Print( Form("%s.pdf",output_name.c_str()) );
  canvas->Clear();

  canvas->Divide(3, 2);
  for (int s=1; s<7; s++)
    {
      canvas->cd(s);
      gPad->SetMargin(0.15,0.1,0.1,0.1); 
      gPad->SetLogz();
      h2_rec_x_qq_rebin[s]->Draw("colz");
      lab.DrawLatex(0.2,0.85,Form("Entries: %0.f",h2_rec_x_qq[s]->GetEntries())); 
      lab.DrawLatex(0.3,0.925,Form("Rec MC Q^{2} vs. x #rightarrow Sector %d",s)); 
      lab.DrawLatex(0.45,0.025,"x_{Bjorken}  "); 
      lab.SetTextAngle(90);
      lab.DrawLatex(0.05, 0.35,"Q^{2} [GeV^{2}/c^{2}]"); 
      lab.SetTextAngle(0);
    }
  canvas->Print( Form("%s.pdf",output_name.c_str()) );
  canvas->Clear();

  canvas->Divide(3, 2);
  for (int s=1; s<7; s++)
    {
      canvas->cd(s);
      gPad->SetMargin(0.15,0.1,0.1,0.1); 
      gPad->SetLogz();
      h2_gen_x_qq_rebin[s]->Draw("colz");
      lab.DrawLatex(0.2,0.85,Form("Entries: %0.f",h2_gen_x_qq[s]->GetEntries())); 
      lab.DrawLatex(0.3,0.925,Form("Gen MC Q^{2} vs. x #rightarrow Sector %d",s)); 
      lab.DrawLatex(0.45,0.025,"x_{Bjorken}  "); 
      lab.SetTextAngle(90);
      lab.DrawLatex(0.05, 0.35,"Q^{2} [GeV^{2}/c^{2}]"); 
      lab.SetTextAngle(0);
    }
  canvas->Print( Form("%s.pdf",output_name.c_str()) );
  canvas->Clear();

  canvas->Divide(3, 2);
  for (int s=1; s<7; s++)
    {
      canvas->cd(s);
      gPad->SetMargin(0.15,0.1,0.1,0.1); 
      gPad->SetLogz();
      h2_hits_w_qq[s]->Draw("colz");
      lab.DrawLatex(0.2,0.85,Form("Entries: %0.f",h2_hits_w_qq[s]->GetEntries())); 
      lab.DrawLatex(0.3,0.925,Form("Hits Q^{2} vs. W #rightarrow Sector %d",s)); 
      lab.DrawLatex(0.45,0.025," W [GeV/c^{2}] "); 
      lab.SetTextAngle(90);
      lab.DrawLatex(0.05, 0.35,"Q^{2} [GeV^{2}/c^{2}]"); 
      lab.SetTextAngle(0);
    }
  canvas->Print( Form("%s.pdf",output_name.c_str()) );
  canvas->Clear();

  canvas->Divide(3, 2);
  for (int s=1; s<7; s++)
    {
      canvas->cd(s);
      gPad->SetMargin(0.15,0.1,0.1,0.1); 
      gPad->SetLogz();
      h2_rec_w_qq[s]->Draw("colz");
      lab.DrawLatex(0.2,0.85,Form("Entries: %0.f",h2_rec_w_qq[s]->GetEntries())); 
      lab.DrawLatex(0.3,0.925,Form("Rec MC Q^{2} vs. W #rightarrow Sector %d",s)); 
      lab.DrawLatex(0.45,0.025," W [GeV/c^{2}] "); 
      lab.SetTextAngle(90);
      lab.DrawLatex(0.05, 0.35,"Q^{2} [GeV^{2}/c^{2}]"); 
      lab.SetTextAngle(0);
    }
  canvas->Print( Form("%s.pdf",output_name.c_str()) );
  canvas->Clear();


  canvas->Divide(3, 2);
  for (int s=1; s<7; s++)
    {
      canvas->cd(s);
      gPad->SetMargin(0.15,0.1,0.1,0.1); 
      gPad->SetLogz();
      h2_gen_w_qq[s]->Draw("colz");
      lab.DrawLatex(0.2,0.85,Form("Entries: %0.f",h2_gen_w_qq[s]->GetEntries())); 
      lab.DrawLatex(0.3,0.925,Form("Gen MC Q^{2} vs. W #rightarrow Sector %d",s)); 
      lab.DrawLatex(0.45,0.025," W [GeV/c^{2}] "); 
      lab.SetTextAngle(90);
      lab.DrawLatex(0.05, 0.35,"Q^{2} [GeV^{2}/c^{2}]"); 
      lab.SetTextAngle(0);
    }
  canvas->Print( Form("%s.pdf",output_name.c_str()) );
  canvas->Clear();
  canvas->Divide(3, 2);
  for (int s=1; s<7; s++)
    {
      canvas->cd(s);
      gPad->SetMargin(0.15,0.1,0.1,0.1); 
      gPad->SetLogz();
      h2_hits_w_qq[s]->Draw("colz");
      lab.DrawLatex(0.2,0.85,Form("Entries: %0.f",h2_hits_w_qq[s]->GetEntries())); 
      lab.DrawLatex(0.3,0.925,Form("Hits Q^{2} vs. W #rightarrow Sector %d",s)); 
      lab.DrawLatex(0.45,0.025," W [GeV/c^{2}] "); 
      lab.SetTextAngle(90);
      lab.DrawLatex(0.05, 0.35,"Q^{2} [GeV^{2}/c^{2}]"); 
      lab.SetTextAngle(0);
    }
  canvas->Print( Form("%s.pdf",output_name.c_str()) );
  canvas->Clear();

  canvas->Divide(3, 2);
  for (int s=1; s<7; s++)
    {
      canvas->cd(s);
      gPad->SetMargin(0.15,0.1,0.1,0.1); 
      gPad->SetLogz();
      h2_rec_w_qq[s]->Draw("colz");
      lab.DrawLatex(0.2,0.85,Form("Entries: %0.f",h2_rec_w_qq[s]->GetEntries())); 
      lab.DrawLatex(0.3,0.925,Form("Rec MC Q^{2} vs. W #rightarrow Sector %d",s)); 
      lab.DrawLatex(0.45,0.025," W [GeV/c^{2}] "); 
      lab.SetTextAngle(90);
      lab.DrawLatex(0.05, 0.35,"Q^{2} [GeV^{2}/c^{2}]"); 
      lab.SetTextAngle(0);
    }
  canvas->Print( Form("%s.pdf",output_name.c_str()) );
  canvas->Clear();

  canvas->Divide(3, 2);
  for (int s=1; s<7; s++)
    {
      canvas->cd(s);
      gPad->SetMargin(0.15,0.1,0.1,0.1); 
      gPad->SetLogz();
      h2_gen_w_qq_rebin[s]->Draw("colz");
      lab.DrawLatex(0.2,0.85,Form("Entries: %0.f",h2_gen_w_qq[s]->GetEntries())); 
      lab.DrawLatex(0.3,0.925,Form("Gen MC Q^{2} vs. W #rightarrow Sector %d",s)); 
      lab.DrawLatex(0.45,0.025," W [GeV/c^{2}] "); 
      lab.SetTextAngle(90);
      lab.DrawLatex(0.05, 0.35,"Q^{2} [GeV^{2}/c^{2}]"); 
      lab.SetTextAngle(0);
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

      h2_hits_w_qq[s] ->Write();
      h2_rec_w_qq[s]  ->Write();
      h2_gen_w_qq[s]  ->Write();
    }

  h1_fcup_charge->Write();
  
  runInfoTree->Write();
  out.Write();
  out.Close();

  if ( !out.IsOpen() ){
    cout << " DISHistograms::save() finished with success. " << endl;
  } 
}

void DISHistograms::close(){
  if (f->IsOpen()){
    f->Write();
    f->Close();
  } 
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
  wBins  = DBins(100,0,6);

  infofile = "unset";

  histos.output_name = outputFile; 
}

DISManager::~DISManager()
{

}

void DISManager::fill_model(){

  // Get model 
  F1F209Wrapper model; 
  double beam_energy = 5.498; 

  // Initialize the model histograms before loading them. 
  for (int b=0; b<qqBins.number(); b++){
    TH1D * hist = new TH1D(Form("h1_model_x_by_qq_bin%d",b),"",xBins.number(),xBins.min(),xBins.max()); 
    histos.h1_model_x_by_qq.push_back(hist); 
  } 
  
  
  for (int ibin=0; ibin<qqBins.number(); ibin++){
    for (int jbin=0; jbin<xBins.number(); jbin++) {
      double model_value = model.GetXS(1, 1, beam_energy, xBins.bin_center(jbin), qqBins.bin_center(ibin));

      if (model_value > 1e-20 && model_value < 1e20) {
	histos.h1_model_x_by_qq[ibin]->SetBinContent(jbin+1,model_value/mev_to_gev);
	histos.h1_model_x_by_qq[ibin]->SetBinError(jbin+1,0.00);
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

  // Filling the histogram which holds the number. 
  for (int b=0; b<xBins.number(); b++) {
    histos.h1_fcup_charge->SetBinContent(1+b,fcup_charge); 
    histos.h1_fcup_charge->SetBinError(1+b,0.0); 
  }

  cout << " Finished getting charge! " << endl;
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
  histos.set_bins(xBins, qqBins, wBins);
  
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

  /*

    Normalization for the cross section. 

                      M * e                1                          1 
    xs = N_hits ------------------  ---------------  ---------------------------------
                dQ * A * l * rho     Acc  RaddCorr     xBins.width() * qqBins.width()
   

		M   = Molar Mass of Hydrogen : hydrogen_molar_mass 
		e   = Electron Charge        : electron_c  
		dQ  = Faraday Cup Charge     : fcup_charge
		A   = Avogadro's Number      : avogadro 
		l   = target length          : 5 cm 
		rho = hydrogen density       : hydrogen_density

  */

  // Getting charge for normalization. 
  fcup_charge = histos.h1_fcup_charge->GetBinContent(1);
  
  double abs_norm = cm_to_outhouse*(hydrogen_molar_weight*electron_c*1e6)/(fcup_charge*5.00*avogadro*hydrogen_density); 

  cout << " Abs. Norm Scale: " << abs_norm << " outhouses. " << endl; 
  
  int n_micro_bins_x  = histos.h2_hits_x_qq[0]->GetXaxis()->GetNbins();
  double micro_x_max  = histos.h2_hits_x_qq[0]->GetXaxis()->GetXmax();
  double micro_x_min  = histos.h2_hits_x_qq[0]->GetXaxis()->GetXmin(); 

  int n_micro_bins_qq = histos.h2_hits_x_qq[0]->GetYaxis()->GetNbins();
  double micro_qq_max = histos.h2_hits_x_qq[0]->GetYaxis()->GetXmax();
  double micro_qq_min = histos.h2_hits_x_qq[0]->GetYaxis()->GetXmin(); 

  int n_micro_bins_w = histos.h2_hits_w_qq[0]->GetYaxis()->GetNbins();
  double micro_w_max = histos.h2_hits_w_qq[0]->GetYaxis()->GetXmax();
  double micro_w_min = histos.h2_hits_w_qq[0]->GetYaxis()->GetXmin(); 
  
  DBins microXBins(n_micro_bins_x, micro_x_min, micro_x_max);
  DBins microWBins(n_micro_bins_w, micro_w_min, micro_w_max);
  DBins microQQBins(n_micro_bins_qq,micro_qq_min,micro_qq_max);

  // Dying for bad binning scheme. 
  if ( xBins.number() > microXBins.number() || qqBins.number() > microQQBins.number() || wBins.number() > microWBins.number()) {
    cout << "Fatal Error: Trying to bin finer than possible. " << endl;
    exit(0);
  }

  int xConversion  = microXBins.number() /xBins.number();
  int qqConversion = microQQBins.number()/qqBins.number();
  int wConversion  = microWBins.number() /wBins.number();
  
  // Do rebinning
  for (int s=0; s<7; s++) {
    histos.h2_hits_x_qq_rebin[s] ->Rebin2D(xConversion, qqConversion);
    histos.h2_rec_x_qq_rebin[s]  ->Rebin2D(xConversion, qqConversion);
    histos.h2_gen_x_qq_rebin[s]  ->Rebin2D(xConversion, qqConversion);

    histos.h2_hits_w_qq_rebin[s] ->Rebin2D(wConversion, qqConversion);
    histos.h2_rec_w_qq_rebin[s]  ->Rebin2D(wConversion, qqConversion);
    histos.h2_gen_w_qq_rebin[s]  ->Rebin2D(wConversion, qqConversion);
  }
  
  // Projecting into 1-D Histograms, could be combined into above loop. More readable like this. 
  for (int s=0; s<7; s++) {
    vector<TH1D*> v;

    for (int b=0; b<=qqBins.number(); b++) {
      string name = Form("h1_hits_x_by_qq_bin%d_%d",b,s);
      TH1D * hist = new TH1D(name.c_str(),"",xBins.number(),xBins.min(),xBins.max()); 
      if (b == 0) { histos.h2_hits_x_qq_rebin[s]->ProjectionX(name.c_str(),1,qqBins.number()); }
      else        { histos.h2_hits_x_qq_rebin[s]->ProjectionX(name.c_str(),b+1,b+2); }
      hist->Sumw2();
      v.push_back(hist);
    }
    
    histos.h1_hits_x_by_qq.push_back(v); 
  }

  // reconstructed 
  for (int s=0; s<7; s++) {
    vector<TH1D*> v;

    for (int b=0; b<=qqBins.number(); b++) {
      string name = Form("h1_rec_x_by_qq_bin%d_%d",b,s);
      TH1D * hist = new TH1D(name.c_str(),"",xBins.number(),xBins.min(),xBins.max()); 
      if (b == 0) { histos.h2_rec_x_qq_rebin[s]->ProjectionX(name.c_str(),1,qqBins.number()); }
      else        { histos.h2_rec_x_qq_rebin[s]->ProjectionX(name.c_str(),b+1,b+2); }
      hist->Sumw2();
      v.push_back(hist);
    }
    
    histos.h1_rec_x_by_qq.push_back(v); 
  }
 
  // Generated 
  for (int s=0; s<7; s++) {
    vector<TH1D*> v;

    for (int b=0; b<=qqBins.number(); b++) {
      string name = Form("h1_gen_x_by_qq_bin%d_%d",b,s);
      TH1D * hist = new TH1D(name.c_str(),"",xBins.number(),xBins.min(),xBins.max()); 
      if (b == 0) { histos.h2_gen_x_qq_rebin[s]->ProjectionX(name.c_str(),1,qqBins.number()); }
      else        { histos.h2_gen_x_qq_rebin[s]->ProjectionX(name.c_str(),b+1,b+2); }
      hist->Sumw2();
      v.push_back(hist);
    }
    
    histos.h1_gen_x_by_qq.push_back(v); 
  }

  // Acceptance 
  for (int s=0; s<7; s++) {
    vector<TH1D*> v;

    for (int b=0; b<=qqBins.number(); b++) {
      string name = Form("h1_acc_x_by_qq_bin%d_%d",b,s);
      TH1D * hist = new TH1D(name.c_str(),"",xBins.number(),xBins.min(),xBins.max()); 
      v.push_back(hist);
    }
    
    histos.h1_acc_x_by_qq.push_back(v); 
  }

  // Raw Cross Section 
  for (int s=0; s<7; s++) {
    vector<TH1D*> v;

    for (int b=0; b<=qqBins.number(); b++) {
      string name = Form("h1_rxs_x_by_qq_bin%d_%d",b,s);
      TH1D * hist = new TH1D(name.c_str(),"",xBins.number(),xBins.min(),xBins.max()); 
      v.push_back(hist);
    }
    
    histos.h1_rxs_x_by_qq.push_back(v); 
  }

  //  Cross Section 
  for (int s=0; s<7; s++) {
    vector<TH1D*> v;

    for (int b=0; b<=qqBins.number(); b++) {
      string name = Form("h1_xs_x_by_qq_bin%d_%d",b,s);
      TH1D * hist = new TH1D(name.c_str(),"",xBins.number(),xBins.min(),xBins.max()); 
      v.push_back(hist);
    }
    
    histos.h1_xs_x_by_qq.push_back(v); 
  }
    // Ratio Raw 
  for (int s=0; s<7; s++) {
    vector<TH1D*> v;

    for (int b=0; b<=qqBins.number(); b++) {
      string name = Form("h1_rxs_ratio_x_by_qq_bin%d_%d",b,s);
      TH1D * hist = new TH1D(name.c_str(),"",xBins.number(),xBins.min(),xBins.max()); 
      v.push_back(hist);
    }
    
    histos.h1_rxs_ratio_x_by_qq.push_back(v); 
  }
  
  // Ratio 
  for (int s=0; s<7; s++) {
    vector<TH1D*> v;

    for (int b=0; b<=qqBins.number(); b++) {
      string name = Form("h1_xs_ratio_x_by_qq_bin%d_%d",b,s);
      TH1D * hist = new TH1D(name.c_str(),"",xBins.number(),xBins.min(),xBins.max()); 
      v.push_back(hist);
    }
    
    histos.h1_xs_ratio_x_by_qq.push_back(v); 
  }
  
  
  // Doing Cross Section AFTER creating histograms from 2D histograms.    
  for (int s=0; s<7; s++)
    {
      for (int b=0; b<qqBins.number(); b++)
	{

	  cout << Form(" Doing Acceptance for Sect. %d Bin %d",s,b) << endl;
	  string name  = Form("h1_acc_x_by_qq_bin%d_%s",b,sect[s].c_str());
	  string title = Form("Acc. for x-by-qq Bin %d Sector %s",b,sect[s].c_str());
	  histos.h1_acc_x_by_qq[s][b] = (TH1D*) histos.h1_rec_x_by_qq[s][b]->Clone();
	  histos.h1_acc_x_by_qq[s][b]->Divide( histos.h1_gen_x_by_qq[s][b] ); 
	  histos.h1_acc_x_by_qq[s][b]->SetTitle( title.c_str() );
	  histos.h1_acc_x_by_qq[s][b]->SetName(  name.c_str()  );

	  cout << Form(" Doing Raw XS for Sect. %d Bin %d",s,b) << endl;
	  name  = Form("h1_rxs_x_by_qq_%d_%s",b,sect[s].c_str());
	  title = Form("rxs for x-by-qq Bin %d Sector %s",b,sect[s].c_str());
	  histos.h1_rxs_x_by_qq[s][b] = (TH1D*) histos.h1_hits_x_by_qq[s][b]->Clone();
	  histos.h1_rxs_x_by_qq[s][b]->Divide( histos.h1_acc_x_by_qq[s][b] ); 
	  histos.h1_rxs_x_by_qq[s][b]->SetTitle(title.c_str());
	  histos.h1_rxs_x_by_qq[s][b]->SetName(name.c_str());
	  histos.h1_rxs_x_by_qq[s][b]->Scale(1/(xBins.width()*qqBins.width())); 
	  histos.h1_rxs_x_by_qq[s][b]->Scale( abs_norm );
	  //    Note sure why this just gives blank histograms. 
	  //	  if (s == 0) { histos.h1_rxs_x_by_qq[s][b]->Scale(1/6);               }
	  //	  if (b == 0) { histos.h1_rxs_x_by_qq[s][b]->Scale(1/qqBins.number()); }

	  cout << Form(" Doing XS for Sect. %d Bin %d",s,b) << endl;
	  name  = Form("h1_xs_x_by_qq_%d_%s",b,sect[s].c_str());
	  title = Form("xs for x-by-qq Bin %d Sector %s",b,sect[s].c_str());
	  histos.h1_xs_x_by_qq[s][b] = (TH1D*) histos.h1_rxs_x_by_qq[s][b]->Clone();
	  histos.h1_xs_x_by_qq[s][b]->SetTitle(title.c_str());
	  histos.h1_xs_x_by_qq[s][b]->SetName(name.c_str());

	  cout << Form(" Doing Raw XS Ratio for Sect. %d Bin %d",s,b) << endl;
	  name  = Form("h1_rxs_ratio_x_by_qq_%d_%s",b,sect[s].c_str());
	  title = Form("rxs ratio for x-by-qq Bin %d Sector %s",b,sect[s].c_str());
	  histos.h1_rxs_ratio_x_by_qq[s][b] = (TH1D*) histos.h1_rxs_x_by_qq[s][b]->Clone();
	  histos.h1_rxs_ratio_x_by_qq[s][b]->Divide( histos.h1_model_x_by_qq[b] );
	  histos.h1_rxs_ratio_x_by_qq[s][b]->SetTitle(title.c_str());
	  histos.h1_rxs_ratio_x_by_qq[s][b]->SetName(name.c_str());

	  cout << Form(" Doing XS Ratio for Sect. %d Bin %d",s,b) << endl;
	  name  = Form("h1_xs_ratio_x_by_qq_%d_%s",b,sect[s].c_str());
	  title = Form("xs ratio for x-by-qq Bin %d Sector %s",b,sect[s].c_str());
	  histos.h1_xs_ratio_x_by_qq[s][b] = (TH1D*) histos.h1_xs_x_by_qq[s][b]->Clone();
	  histos.h1_xs_ratio_x_by_qq[s][b]->Divide( histos.h1_model_x_by_qq[b] );
	  histos.h1_xs_ratio_x_by_qq[s][b]->SetTitle(title.c_str());
	  histos.h1_xs_ratio_x_by_qq[s][b]->SetName(name.c_str());	  	  
  
	  }
    }

  cout << " Number of bins summary for debugging: " << endl;
  cout << " hits " << histos.h1_hits_x_by_qq[0][0]->GetXaxis()->GetNbins() << endl;
  cout << " rec " << histos.h1_rec_x_by_qq[0][0]->GetXaxis()->GetNbins() << endl;
  cout << " gen " << histos.h1_gen_x_by_qq[0][0]->GetXaxis()->GetNbins() << endl;
  cout << " model " << histos.h1_model_x_by_qq[0]->GetXaxis()->GetNbins() << endl;

  
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
