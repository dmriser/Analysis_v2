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


