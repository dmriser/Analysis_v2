void readIntegratedHisto(TH1D *histo[numberSector], TFile *file, const int numberSector, string title){
  for (int s=0; s< numberSector; s++){
    string fullTitle = Form("%s_s%d",title.c_str(),s);
    histo[s] = (TH1D*) file->Get(fullTitle.c_str());
    cout << " >>> Getting "  << fullTitle << " with " << histo[s]->GetEntries() << " entries. " << endl; 
  }
}

void readHisto(TH1D *histo[numberSector][numberSlices], TFile *file, const int numberSector, const int numberSlices, string title){

  for (int s=0; s< numberSector; s++){
    for (int b=0; b< numberSlices; b++) {
      string fullTitle = Form("%s_s%d_slice%d",title.c_str(),s,b);
      histo[s][b] = (TH1D*) file->Get(fullTitle.c_str());
      cout << " >>> Getting "  << fullTitle << " with " << histo[s][b]->GetEntries() << " entries. " << endl; 
    }
  }
}

void readEvents(TH2D *histo[numberSector], TFile *file, const int numberSector, string title){
  for (int s=0; s< numberSector; s++){
    string fullTitle = Form("%s_s%d",title.c_str(),s);
    histo[s] = (TH2D*) file->Get(fullTitle.c_str());
    cout << " >>> Getting "  << fullTitle << " with " << histo[s]->GetEntries() << " entries. " << endl; 
  }
}

void showIntegratedHisto(TH1D *histo[numberSector], const int numberSector){
  TCanvas * can = new TCanvas("can","",1200,800);
  can->Divide(3,2);
  
  TLatex sectorCaption;
  sectorCaption.SetNDC();
  sectorCaption.SetTextFont(12);

  for (int s=1; s<numberSector; s++){
    can->cd(s);
    histo[s]->SetMarkerColor(kBlack);
    histo[s]->SetMarkerStyle(7);
    histo[s]->Draw();
    sectorCaption.DrawLatex(0.4, 0.95, Form("Sector %d",s));
  }

}

void showEvents(TH2D *histo[numberSector], const int numberSector){
  TCanvas * can = new TCanvas("can","",1200,800);
  can->Divide(3,2);
  
  TLatex sectorCaption;
  sectorCaption.SetNDC();
  sectorCaption.SetTextFont(12);

  for (int s=1; s<numberSector; s++){
    can->cd(s);
    histo[s]->Draw("colz");
    sectorCaption.DrawLatex(0.4, 0.925, Form("Sector %d",s));
  }

}

void makeSlicesX(TH2D *histo, TH1D *slices[numberSlices], const int numberSlices, int histoType){
  
  string sliceTitle;
  for (int slice=0; slice< numberSlices; slice++){
    if (histoType == 1){ sliceTitle = Form("keppelModel_wSlice%d",slice); }
    slices[slice] = histo->ProjectionX(sliceTitle.c_str(),slice+1,slice+2);
  }
  
}

void makeSlicesY(TH2D *histo, TH1D *slices[numberSlices], const int numberSlices, int histoType){
  
  string sliceTitle;
  for (int slice=0; slice< numberSlices; slice++){
    if (histoType == 1){ sliceTitle = Form("keppelModel_qqSlice%d",slice); }
    slices[slice] = histo->ProjectionY(sliceTitle.c_str(),slice+1,slice+2);
  }
  
}

void showHisto(TH1D *histo[numberSector][numberSlices], const int numberSector, const int numberSlices){
  int canHeight=0;
  while(canHeight*(canHeight+1) < numberSlices){canHeight++;}

  TCanvas * can = new TCanvas("can","",1200,800);
  can->Divide(canHeight+1,canHeight);
  
  TLatex sectorCaption;
  sectorCaption.SetNDC();
  sectorCaption.SetTextFont(12);

  for (int s=1; s<numberSlices; s++){
    can->cd(s);
    histo[0][s]->SetLineColor(kRed);
    histo[0][s]->Draw("h");
    sectorCaption.DrawLatex(0.4, 0.95, Form("Slice %d",s));
  }

}

void showIntegratedCombinedHisto(TH1D *histo[numberSector], const int numberSector){
  TCanvas * can = new TCanvas("can","",800,800);
  histo[0]->SetMarkerColor(kBlack);
  histo[0]->SetMarkerStyle(7);
  histo[0]->Draw("pe");
}

void showRatio0To1(TH1D *histo[numberSector], const int numberSector){
  TCanvas *can = new TCanvas("can","",800,800);
  histo[0]->SetMinimum(0.0);
  histo[0]->SetMaximum(1.1);
  histo[0]->SetLineColor(kRed);
  histo[0]->Draw("hist");
}

void printPurityStudy(TH1D *purity[numberSector][numberSlices], TH1D* rec[numberSector][numberSlices],
		      TH1D* gen[numberSector][numberSlices],TH1D* recAndGen[numberSector][numberSlices], const int numberSector, const int numberSlices, const int sectorToPrint, string pdfTitle){


  TCanvas *purityCanvas = new TCanvas("purityCanvas","",1600,800);
  purityCanvas->Divide(4,3);

  string openPdf = Form("%s[",pdfTitle.c_str());
  string closePdf = Form("%s]",pdfTitle.c_str());
  purityCanvas->Print(openPdf.c_str());
  purityCanvas->SetGridx();
  purityCanvas->SetGridy();

  TLatex xCaption, yCaption, sectorCaption;
  xCaption.SetNDC();
  xCaption.SetTextFont(22);
  yCaption.SetNDC();
  yCaption.SetTextAngle(90.0);
  yCaption.SetTextFont(22);
  sectorCaption.SetNDC();
  sectorCaption.SetTextFont(22);

  // For calculating which QQ bins we're in
  double qqMin = 1.0; double qqMax = 5.0; 
  double qqWidth = (qqMax-qqMin)/numberSlices; 

  int currentPad = 1; 
  for (int slice=0; slice<numberSlices; slice++){

    if (currentPad > 12){ 
      currentPad = 1; 
      purityCanvas->Print(pdfTitle.c_str());
    }

    double qq = qqMin + slice*qqWidth/2; 
    purityCanvas->cd(currentPad);

    gen[sectorToPrint][slice]->SetLineColor(kRed);
    rec[sectorToPrint][slice]->SetLineColor(kBlue);
    recAndGen[sectorToPrint][slice]->SetLineColor(kBlack);

    purityCanvas->SetLogy(1);
    gen[sectorToPrint][slice]->Draw("hist");
    rec[sectorToPrint][slice]->Draw("histsame");
    recAndGen[sectorToPrint][slice]->Draw("histsame");
    xCaption.DrawLatex(0.65, 0.05, "W (GeV/c^{2})");
    yCaption.DrawLatex(0.05, 0.65, "Events");
    purityCanvas->SetLogy(0);

    sectorCaption.SetTextColor(kBlue);
    sectorCaption.DrawLatex(0.6, 0.8, "#rightarrow rec");
    sectorCaption.SetTextColor(kRed);
    sectorCaption.DrawLatex(0.6, 0.75, "#rightarrow gen");
    sectorCaption.SetTextColor(kBlack);
    sectorCaption.DrawLatex(0.6, 0.7, "#rightarrow rec+gen");
    sectorCaption.DrawLatex(0.4, 0.95,Form("Q^{2} = %.3f (GeV^{2}/c^{2})",qq));

    purityCanvas->cd(currentPad+1);
    purity[sectorToPrint][slice]->SetLineColor(kBlack);
    purity[sectorToPrint][slice]->SetMinimum(0.0);
    purity[sectorToPrint][slice]->SetMaximum(1.0);
    purity[sectorToPrint][slice]->Draw("hist");
    xCaption.DrawLatex(0.65, 0.05, "W (GeV/c)");
    yCaption.DrawLatex(0.05, 0.65, "Purity");
    sectorCaption.DrawLatex(0.4, 0.95,Form("Q^{2} = %.3f (GeV^{2}/c^{2})",qq));

    currentPad += 2;
  }

  purityCanvas->Print(closePdf.c_str());

} 

void loadModel(TH2D *model, const int numberQQBins, const int numberWBins, string modelFile){
  ifstream inputDataFile(modelFile.c_str());


  int xBin, yBin; 
  double val;
  for (int qqBin=1; qqBin<= numberQQBins; qqBin++){
    for (int wBin=1; wBin<= numberWBins; wBin++){
      inputDataFile >> xBin; 
      inputDataFile >> yBin; 
      inputDataFile >> val; 
      cout << Form("Filling (%d, %d) = %f",xBin, yBin, val) << endl; 
      model->SetBinContent(xBin, yBin, val);
      model->SetBinError(xBin, yBin, 0.00);
    }
  }

  inputDataFile.close();
}

int getNumberOfCanvasDivisions(const int numberOfHistograms){
  int divs = 1;
  while( divs*(divs+1) < numberOfHistograms){ divs++; }
  return divs; 
}

void plot2Histos(TH1D *histo1[numberSector][numberSlices], TH1D *histo2[numberSector][numberSlices], const int numberSector, const int numberSlices, int histoType){

  // histoType 
  // 1 - Data & Simulation 
  TCanvas *compareCanvas = new TCanvas("compareCanvas","",1600,1200);
  TCanvas *singleCanvas = new TCanvas("singleCanvas","",800,800);
  TLatex xCaption, sectorCaption, histo1Caption, histo2Caption; 

  xCaption.SetNDC();
  sectorCaption.SetNDC();
  histo1Caption.SetNDC();
  histo2Caption.SetNDC();

  xCaption.SetTextFont(12);
  sectorCaption.SetTextFont(12);
  histo1Caption.SetTextFont(12);
  histo2Caption.SetTextFont(12);
  histo1Caption.SetTextColor(kRed);

  int divs = getNumberOfCanvasDivisions(numberSlices);
  compareCanvas->Divide(divs+1,divs);

  for (int s=1; s< numberSector; s++){
    double nEvents1 = 0; double nEvents2 = 0;

    for (int sl=0; sl< numberSlices; sl++){
      nEvents1 += histo1[s][sl]->Integral();
      nEvents2 += histo2[s][sl]->Integral();
    }

    for (int sl=0; sl< numberSlices; sl++){
      //      compareCanvas->cd(sl+1);
      histo1[s][sl]->Scale(numberSlices/nEvents1);
      histo2[s][sl]->Scale(numberSlices/nEvents2);
      histo1[s][sl]->SetLineColor(kRed);
      histo1[s][sl]->Draw("hist");
      histo2[s][sl]->Draw("histsame");

      sectorCaption.DrawLatex(0.4,0.95,Form("Sector %d Q^{2} Bin %d",s,sl));
      xCaption.DrawLatex(0.65, 0.025, "W (GeV/c^{2})");

      if (histoType == 1){
	histo1Caption.DrawLatex(0.18, 0.78, Form("Data Events = %.0f",histo1[s][sl]->GetEntries()));
	histo2Caption.DrawLatex(0.18, 0.72, Form("Sim Events = %.0f",histo2[s][sl]->GetEntries()));
      }

      if (histoType == 1) { singleCanvas->Print(Form("compareDataAndSim/compareDataSimSector%dSlice%d.png",s,sl)); }
    }

    //    if (histoType == 1){ compareCanvas->Print(Form("compareDataAndSim/compareDataSimSector%d.png",s)); }
  }

}

