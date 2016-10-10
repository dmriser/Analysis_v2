void saveHisto(TH2D *histo[numberSectors], TFile * file, const int numberSectors){
  file->cd();
  if (file->IsOpen()){
    for (int s=0; s< numberSectors; s++){
      histo[s]->Write();
    }
    file->Write();
  } else {
    cout << "Output file not opened successfully" << endl;
  }
}

void saveHisto(TH1D *histo[numberSectors][numberQQBins], TFile * file, const int numberSectors, const int numberQQBins){
  file->cd();
  if (file->IsOpen()){
    for (int s=0; s< numberSectors; s++){
      for (int b=0; b< numberQQBins; b++){
	histo[s][b]->Write();
      }}
    file->Write();
  } else {
    cout << "Output file not opened successfully" << endl;
  }
}
 
