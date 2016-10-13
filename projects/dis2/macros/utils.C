void readIntegratedHisto(TH1D *histo[numberSector], TFile *file, const int numberSector, string title){
  for (int s=0; s< numberSector; s++){
    string fullTitle = Form("%s_s%d",title.c_str(),s);
    histo[s] = (TH1D*) file->Get(fullTitle.c_str());
    cout << " >>> Getting "  << fullTitle << " with " << histo[s]->GetEntries() << " entries. " << endl; 
  }
}

void showIntegratedHisto(TH1D *histo[numberSector], const int numberSector){
  TCanvas * can = new TCanvas("can","",1200,800);
  can->Divide(3,2);
  
  for (int s=1; s<numberSector; s++){
    can->cd(s);
    histo[s]->Draw();
  }

}

void showIntegratedCombinedHisto(TH1D *histo[numberSector], const int numberSector){
  TCanvas * can = new TCanvas("can","",800,800);
  histo[0]->SetMarkerColor(kBlack);
  histo[0]->SetMarkerStyle(7);
  histo[0]->Draw("pe");
}
