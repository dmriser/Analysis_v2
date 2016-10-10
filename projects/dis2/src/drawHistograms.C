#include "TFile.h"
#include "TList.h"
#include "TObject.h"
#include "TString.h"
#include "TStyle.h"

void drawHistograms(){

  gStyle->SetOptTitle(1); 

  TFile * inputFile = new TFile("../out/crossSectionTest.root");

  const Int_t markerStyle = 7; 

  TCanvas * can = new TCanvas("can","",800,800); 
  can->Divide(2,2); 

  can->cd(1);
  crossSectionRatio_xByQQ_s0_slice1->SetFillColorAlpha(kRed,0.25);
  crossSectionRatio_xByQQ_s0_slice1->Draw();

  can->cd(2);
  crossSectionRatio_xByQQ_s0_slice10->SetFillColorAlpha(kRed,0.25);
  crossSectionRatio_xByQQ_s0_slice10->Draw();

  can->cd(3);
  crossSectionRatio_xByQQ_s0_slice30->SetFillColorAlpha(kRed,0.25);
  crossSectionRatio_xByQQ_s0_slice30->Draw();

  can->cd(4);
  crossSectionRatio_xByQQ_s0_slice50->SetFillColorAlpha(kRed,0.25);
  crossSectionRatio_xByQQ_s0_slice50->Draw();


}
