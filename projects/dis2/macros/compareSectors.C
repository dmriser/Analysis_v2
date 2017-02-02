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
  TH1D *ratioAvg[numberSector-1];

  readHisto(data,inputFile,numberSector,numberSlices,"binCorrectedCrossSection_wByQQ");
  readHisto(ratio,inputFile,numberSector,numberSlices,"binCorrectedCrossSectionRatio_wByQQ");
  readHisto(model,inputFile,numberSector,numberSlices,"modelCrossSection_wByQQ");
 
  for(int sector=1; sector<numberSector; sector++){
    ratioAvg[sector-1] = new TH1D(Form("ratioAvgSector%d",sector),"",numberSlices,1,numberSlices); 
    
    for(int slice=0; slice<numberSlices; slice++){
      ratioAvg[sector-1]->SetBinContent(slice+1,ratio[sector][slice]->Integral()/ratio[sector][slice]->GetXaxis()->GetNbins());
    }
  }

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
  compareCan2->Print(Form("%scompareSectorsRatio.png",imagePath.c_str()));

  TCanvas *avgCan = new TCanvas("avgCan","",800,500); 

  for(int s=1; s<7; s++){
    ratioAvg[s-1]->SetLineColor(43 + 8*s); 
    ratioAvg[s-1]->Draw("histsame");
  }

}
