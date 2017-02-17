{
  const int numberSectors = 7;
  const int numberSlices = 100;

  TFile *file      = TFile::Open("../out/sectorFix.root");
  THnSparseI *grid = (THnSparseI*) file->Get("dataEvents");

  TH1D *thetaByPhi[numberSectors][numberSlices];

  for(int sect=1; sect<numberSectors; sect++){
    for(int slice=0; slice<numberSlices; slice++){
      //      grid->GetAxis(0)->SetRange(sect,sect+1);
      //      grid->GetAxis(3)->SetRange(slice,slice+1);
      grid->GetAxis(0)->SetRange(sect,sect);
      grid->GetAxis(3)->SetRange(slice,slice);
      thetaByPhi[sect][slice] = grid->Projection(2);
      thetaByPhi[sect][slice]->SetName(Form("dataEvents_thetaByPhi_s%d_slice%d",sect,slice));
      thetaByPhi[sect][slice]->Sumw2();
    }
  }

  //  TCanvas *sliceCanvas = new TCanvas("sliceCanvas","",800,800);
  //  thetaByPhi[1][50]->Draw("pe");

  TStyle *st1 = new TStyle("st1","my style");
  st1->SetOptStat(0);
  st1->SetPalette(1, 0);
  st1->SetOptTitle(0);
  st1->SetDrawBorder(0);
  st1->SetAxisColor(1, "x");
  st1->SetAxisColor(5, "y");
  st1->SetBarOffset(2);
  st1->SetBarWidth(2);
  st1->SetCanvasColor(25);
  st1->SetPadColor(25);
  st1->SetFrameBorderMode(0);
  st1->SetFrameFillColor(23);

  st1->SetGridColor(1);
  st1->SetLabelFont(11, "x");
  st1->SetLabelFont(11, "y");
  st1->SetLabelColor(4, "y");
  st1->SetLabelSize(0.06, "y");
  st1->SetLabelSize(0.06, "x");
  st1->SetNdivisions(505, "xyz");
  st1->cd();
  gROOT->ForceStyle();  

  TBrowser *b = new TBrowser();
}
