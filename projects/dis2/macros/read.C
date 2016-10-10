void readEvents(TH2D *histo[numberSectors], TFile *file, const int numberSectors, int histoType){
  
  if (file->IsOpen()){
    for (int s = 0; s < numberSectors; s++){
      if (histoType == 100){ histo[s] = (TH2D*) file->Get(Form("dataEvents_xByQQ_s%d",s)); }
      if (histoType == 101){ histo[s] = (TH2D*) file->Get(Form("dataEvents_wByQQ_s%d",s)); }
      if (histoType == 200){ histo[s] = (TH2D*) file->Get(Form("recEventsRad_xByQQ_s%d",s)); }
      if (histoType == 201){ histo[s] = (TH2D*) file->Get(Form("recEventsRad_wByQQ_s%d",s)); }
      if (histoType == 202){ histo[s] = (TH2D*) file->Get(Form("recEventsNoRad_xByQQ_s%d",s)); }
      if (histoType == 203){ histo[s] = (TH2D*) file->Get(Form("recEventsNoRad_wByQQ_s%d",s)); }
      if (histoType == 300){ histo[s] = (TH2D*) file->Get(Form("genEventsRad_xByQQ_s%d",s)); }
      if (histoType == 301){ histo[s] = (TH2D*) file->Get(Form("genEventsRad_wByQQ_s%d",s)); }
      if (histoType == 302){ histo[s] = (TH2D*) file->Get(Form("genEventsNoRad_xByQQ_s%d",s)); }
      if (histoType == 303){ histo[s] = (TH2D*) file->Get(Form("genEventsNoRad_wByQQ_s%d",s)); }
      if (histoType == 400){ histo[s] = (TH2D*) file->Get(Form("recAndGenEventsRad_xByQQ_s%d",s)); }
      if (histoType == 401){ histo[s] = (TH2D*) file->Get(Form("recAndGenEventsRad_wByQQ_s%d",s)); }
      if (histoType == 402){ histo[s] = (TH2D*) file->Get(Form("recAndGenEventsNoRad_xByQQ_s%d",s)); }
      if (histoType == 403){ histo[s] = (TH2D*) file->Get(Form("recAndGenEventsNoRad_wByQQ_s%d",s)); }
    }
  } else {
    cout << endl; 
    cout << " >> File for reading histograms was unable to be opened. " << endl; 
    cout << endl; 
  }
  
}

