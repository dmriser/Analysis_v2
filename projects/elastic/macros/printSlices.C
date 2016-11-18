{
  const int numberSectors = 6;
  const int numberSlices = 100;

  TFile *file = TFile::Open("../test.root");
  THnSparseI *grid = (THnSparseI*) file->Get("dataEvents");

  TH1D *thetaByPhi[numberSectors][numberSlices];

  for(int sect=0; sect<numberSectors; sect++){
    for(int slice=0; slice<numberSlices; slice++){
      grid->GetAxis(0)->SetRange(sect,sect+1);
      grid->GetAxis(3)->SetRange(slice,slice+1);
      thetaByPhi[sect][slice] = grid->Projection(2);
      thetaByPhi[sect][slice]->SetName(Form("dataEvents_thetaByPhi_s%d_slice%d",sect,slice));
      thetaByPhi[sect][slice]->Sumw2();
    }
  }

  //  TCanvas *sliceCanvas = new TCanvas("sliceCanvas","",800,800);
  //  thetaByPhi[1][50]->Draw("pe");
  TBrowser *b = new TBrowser();


}
