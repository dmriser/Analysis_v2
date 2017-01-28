{

  gROOT->LoadMacro("utils.C");

  //  TFile * inputFile = TFile::Open("../out/crossSectionWithResonancePass1.root");
  TFile * inputFile = TFile::Open("/volatile/clas12/dmriser/rootFiles/inclusive/xs/y7010Bins.root");

  string imagePath       = "/volatile/clas12/dmriser/plots/inclusive/"; 
  const int numberSector = 7;
  const int numberSlices = 10;

  TH1D *data[numberSector][numberSlices];
  TH1D *model[numberSector][numberSlices];
  TH1D *ratio[numberSector][numberSlices];

  readHisto(data,inputFile,numberSector,numberSlices,"binCorrectedCrossSection_wByQQ");
  readHisto(ratio,inputFile,numberSector,numberSlices,"binCorrectedCrossSectionRatio_wByQQ");
  readHisto(model,inputFile,numberSector,numberSlices,"modelCrossSection_wByQQ");
 
  TCanvas *compareCan = new TCanvas("compareCan","",800,1000); 
  compareCan->Divide(2,5); 

  for (int slice=0; slice<numberSlices; slice++){
    for (int sect=1; sect<7; sect++){
      compareCan->cd(slice+1); 
      gPad->SetMargin(0.05, 0.05, 0.05, 0.05); 
      data[sect][slice]->SetLineColor(50+sect*7); 
      data[sect][slice]->SetMarkerColor(50+sect*7); 
      data[sect][slice]->SetMarkerStyle(8); 
      data[sect][slice]->SetMarkerSize(1); 
      data[sect][slice]->Draw("histsame"); 
    }
    model[0][slice]->Draw("lsame");
  }

  TCanvas *compareCan2 = new TCanvas("compareCan2","",800,1000); 
  compareCan2->Divide(2,5); 

  for (int slice=0; slice<numberSlices; slice++){
    for (int sect=1; sect<7; sect++){
      compareCan2->cd(slice+1); 
      gPad->SetMargin(0.05, 0.05, 0.05, 0.05); 
      ratio[sect][slice]->SetLineColor(50+sect*7); 
      ratio[sect][slice]->SetMarkerColor(50+sect*7); 
      ratio[sect][slice]->SetMarkerStyle(8); 
      ratio[sect][slice]->SetMarkerSize(1); 
      ratio[sect][slice]->SetMinimum(0.1); 
      ratio[sect][slice]->SetMaximum(1.5); 
      ratio[sect][slice]->Draw("histsame"); 
    }
  }


}
