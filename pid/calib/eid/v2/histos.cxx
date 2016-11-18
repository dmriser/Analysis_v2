#ifndef histo_cxx
#define histo_cxx

#include "TCanvas.h"
#include "TF1.h"
#include "TGraphErrors.h"
#include "TH1.h"
#include "TH2.h"
#include "TLine.h"
#include "TVector3.h"

#include "h22Event.h"
#include "histos.h"

#include <iostream>

void Histograms::Init(){ 
  for (int sect=0; sect<7; sect++){
    h2_ecSampling[sect] = new TH2I(Form("h2_ecSamplingSector%d",sect),"",100,0.5,4.0,100,0,0.5); 
    h2_ccThetaSegment[sect] = new TH2I(Form("h2_ccThetaSegmentSector%d",sect),"",100,0,18,100,0,60);

    h1_ecu[sect] = new TH1I(Form("h1_ecuSector%d",sect),"",100,0,450);
    h1_ecv[sect] = new TH1I(Form("h1_ecvSector%d",sect),"",100,0,450);
    h1_ecw[sect] = new TH1I(Form("h1_ecwSector%d",sect),"",100,0,450);
  }
}

void Histograms::Fill(h22Event event, int ipart){ 

  int sector = event.dc_sect[ipart];

  if (sector > 0 && sector < 7){
    if (event.etot[ipart] > 0.001) { 
      h2_ecSampling[0]     ->Fill(event.p[ipart], event.etot[ipart]/event.p[ipart]); 
      h2_ecSampling[sector]->Fill(event.p[ipart], event.etot[ipart]/event.p[ipart]); 

      h2_ccThetaSegment[0]->Fill((event.cc_segm[ipart]%1000)/10 -1, event.theta_cc(ipart));
      h2_ccThetaSegment[sector]->Fill((event.cc_segm[ipart]%1000)/10 -1, event.theta_cc(ipart));

      TVector3 uvw = event.uvw(ipart);
      h1_ecu[0]->Fill(uvw.X());
      h1_ecu[sector]->Fill(uvw.X());
      h1_ecv[0]->Fill(uvw.Y());
      h1_ecv[sector]->Fill(uvw.Y());
      h1_ecw[0]->Fill(uvw.Z());
      h1_ecw[sector]->Fill(uvw.Z());
    }
  }

}

void Histograms::Draw(){
  TCanvas *c1 = new TCanvas("c1","",1200,800);
  c1->Divide(3,2);
  c1->cd(1);
  h2_ecSampling[1]->Draw("colz");
  c1->cd(2);
  h2_ecSampling[2]->Draw("colz");
  c1->cd(3);
  h2_ecSampling[3]->Draw("colz");
  c1->cd(4);
  h2_ecSampling[4]->Draw("colz");
  c1->cd(5);
  h2_ecSampling[5]->Draw("colz");
  c1->cd(6);
  h2_ecSampling[6]->Draw("colz");

  TCanvas *c2 = new TCanvas("c2","",1200,800); 
  c2->Divide(3,2); 
  c2->cd(1);
  h2_ccThetaSegment[1]->Draw("colz"); 
  c2->cd(2);
  h2_ccThetaSegment[2]->Draw("colz");   
  c2->cd(3);
  h2_ccThetaSegment[3]->Draw("colz");   
  c2->cd(4);
  h2_ccThetaSegment[4]->Draw("colz");   
  c2->cd(5);
  h2_ccThetaSegment[5]->Draw("colz");   
  c2->cd(6);
  h2_ccThetaSegment[6]->Draw("colz"); 
  
  TCanvas *c3 = new TCanvas("c2","",1200,400); 
  c3->Divide(3,1); 
  c3->cd(1);
  h1_ecu[0]->Draw();
  c3->cd(2);
  h1_ecv[0]->Draw();
  c3->cd(3);
  h1_ecw[0]->Draw();

}

#endif
