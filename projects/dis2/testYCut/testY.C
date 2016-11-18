#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include "TLatex.h"

void testY(){

  TFile *file[5];
  file[0] = TFile::Open("0.7.root");
  file[1] = TFile::Open("0.75.root");
  file[2] = TFile::Open("0.8.root");
  file[3] = TFile::Open("0.85.root");
  file[4] = TFile::Open("0.9.root");
  
  TH2I *wByQQ[5];
  TH1D *projection[5]; 
  TH1D *model;
  
  const int numberFiles = 5;
  Double_t max = 0.0;

  for (int f=0; f<numberFiles; f++){
    wByQQ[f] = (TH2I*) file[f]->Get("dataEvents_wByQQ_s0");
    projection[f] = wByQQ[f]->ProjectionX();
    projection[f]->SetName(Form("projection%d",f));
    projection[f]->SetTitle(Form("projection%d",f));

    if (projection[f]->GetMaximum() > max){ max = projection[f]->GetMaximum(); }
  }


  TCanvas *c1 = new TCanvas("c1","",1000,500);
  TLatex lab, caption;
  lab.SetNDC();
  lab.SetTextSize(0.025);

  caption.SetNDC();
  caption.SetTextSize(0.045);
  caption.SetTextColor(kBlack);

  Int_t lineBase = 50;
  Int_t lineStep = 3; 
  for (int f=0; f<numberFiles; f++){
    projection[f]->SetMaximum(max); 
    projection[f]->SetLineColor(lineBase+f*lineStep);
  }

  projection[0]->Draw();
  projection[1]->Draw("same");
  projection[2]->Draw("same");
  projection[3]->Draw("same");
  projection[4]->Draw("same");

  for (int f=0; f<numberFiles; f++){
    lab.SetTextColor(lineBase+f*lineStep);
    lab.DrawLatex(0.18, 0.675+0.03*f,Form("y < %.3f",0.05*f+0.7));
  }
  
  caption.DrawLatex(0.78,0.88,"y = 1 - #frac{E'}{E}");
  caption.DrawLatex(0.78,0.07,"W [GeV/c^{2}]");
  caption.SetTextAngle(90.0);
  caption.DrawLatex(0.10,0.7,"Counts");

  c1->Print("yCuts.png");

}
