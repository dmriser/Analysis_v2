void calculateAcceptance(TH2D *recEvents[numberSectors], TH2D *genEvents[numberSectors], TH2D *acceptance[numberSectors], const int numberSectors, int histoType){

  for (int s=0; s< numberSectors; s++){
    acceptance[s] = (TH2D*) (recEvents[s]->Clone(Form("acceptanceTemporaryName_s%d",s)));
    acceptance[s]->Divide(genEvents[s]);
    acceptance[s]->SetMinimum(0.0);
    acceptance[s]->SetMaximum(1.0);

    if (histoType == 500){ acceptance[s]->SetName(Form("acceptanceRad_xByQQ_s%d",s)); }
    if (histoType == 501){ acceptance[s]->SetName(Form("acceptanceRad_wByQQ_s%d",s)); }    
    if (histoType == 502){ acceptance[s]->SetName(Form("acceptanceNoRad_xByQQ_s%d",s)); }
    if (histoType == 503){ acceptance[s]->SetName(Form("acceptanceNoRad_wByQQ_s%d",s)); }

  }

  cout << " >> Finished acceptance. " << endl; 
}

void calculateAcceptanceSlices(TH1D *recEvents[numberSectors][numberQQBins], TH1D *genEvents[numberSectors][numberQQBins], TH1D *acceptance[numberSectors][numberQQBins], const int numberSectors, const int numberQQBins, int histoType){
  
  for (int s=0; s< numberSectors; s++){
    for (int b=0; b< numberQQBins; b++) {
      acceptance[s][b] = (TH1D*) (recEvents[s][b]->Clone(Form("tempName%d%d",s,b)));
      acceptance[s][b]->Divide(genEvents[s][b]);
      
      if (histoType == 100){ acceptance[s][b]->SetName(Form("acceptanceRad_xByQQ_s%d_slice%d",s, b)); }
      if (histoType == 101){ acceptance[s][b]->SetName(Form("acceptanceRad_wByQQ_s%d_slice%d",s, b)); }    
      if (histoType == 102){ acceptance[s][b]->SetName(Form("acceptanceNoRad_xByQQ_s%d_slice%d",s, b)); }
      if (histoType == 103){ acceptance[s][b]->SetName(Form("acceptanceNoRad_wByQQ_s%d_slice%d",s, b)); }
    }
  }
}

void makeQQIntegratedProjections(TH2D *histo[numberSectors], TH1D *projection[numberSectors], const int numberSectors, const int numberQQBins, int histoType){
  
  for (int s=0; s< numberSectors; s++){
    projection[s] = histo[s]->ProjectionX();
    projection[s]->Scale((double) 1/numberQQBins);
    if (histoType == 100){ projection[s]->SetName(Form("acceptanceRad_xIntegrated_s%d",s)); }
    if (histoType == 101){ projection[s]->SetName(Form("acceptanceRad_wIntegrated_s%d",s)); }
    if (histoType == 102){ projection[s]->SetName(Form("acceptanceNoRad_xIntegrated_s%d",s)); }
    if (histoType == 103){ projection[s]->SetName(Form("acceptanceNoRad_wIntegrated_s%d",s)); }
  }

}

void makeEventProjections(TH2D *histo[numberSectors], TH1D *slices[numberSectors][numberQQBins], const int numberSectors, const int numberQQBins, int histoType){

  for (int s=0; s< numberSectors; s++){
    for (int b=0; b< numberQQBins; b++){
      slices[s][b] = histo[s]->ProjectionX("", b+1, b+2);
      slices[s][b]->SetDirectory(0);
      if (histoType == 100) { slices[s][b]->SetName(Form("dataEvents_xByQQ_s%d_slice%d",s,b)); }
      if (histoType == 101) { slices[s][b]->SetName(Form("dataEvents_wByQQ_s%d_slice%d",s,b)); }
      if (histoType == 200) { slices[s][b]->SetName(Form("recEventsRad_xByQQ_s%d_slice%d",s,b)); }
      if (histoType == 201) { slices[s][b]->SetName(Form("recEventsRad_wByQQ_s%d_slice%d",s,b)); }
      if (histoType == 202) { slices[s][b]->SetName(Form("recEventsNoRad_xByQQ_s%d_slice%d",s,b)); }
      if (histoType == 203) { slices[s][b]->SetName(Form("recEventsNoRad_wByQQ_s%d_slice%d",s,b)); }
      if (histoType == 300) { slices[s][b]->SetName(Form("genEventsRad_xByQQ_s%d_slice%d",s,b)); }
      if (histoType == 301) { slices[s][b]->SetName(Form("genEventsRad_wByQQ_s%d_slice%d",s,b)); }
      if (histoType == 302) { slices[s][b]->SetName(Form("genEventsNoRad_xByQQ_s%d_slice%d",s,b)); }
      if (histoType == 303) { slices[s][b]->SetName(Form("genEventsNoRad_wByQQ_s%d_slice%d",s,b)); }
      if (histoType == 400) { slices[s][b]->SetName(Form("recAndGenEventsRad_xByQQ_s%d_slice%d",s,b)); }
      if (histoType == 401) { slices[s][b]->SetName(Form("recAndGenEventsRad_wByQQ_s%d_slice%d",s,b)); }
      if (histoType == 402) { slices[s][b]->SetName(Form("recAndGenEventsNoRad_xByQQ_s%d_slice%d",s,b)); }
      if (histoType == 403) { slices[s][b]->SetName(Form("recAndGenEventsNoRad_wByQQ_s%d_slice%d",s,b)); }
    }
  }

}
