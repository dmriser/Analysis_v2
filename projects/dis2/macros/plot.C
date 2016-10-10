void plotHistogram(TH2D *histo[numberSectors], const int numberSectors, int histoType){
  
  TCanvas *canvas = new TCanvas("canvas","",1200,800); 
  canvas->Divide(3,2);

  TLatex xCaption, yCaption, sectorCaption; 
  xCaption.SetNDC();
  xCaption.SetTextFont(22);
  yCaption.SetNDC();
  yCaption.SetTextFont(22);
  yCaption.SetTextAngle(90); 
  sectorCaption.SetNDC();
  sectorCaption.SetTextFont(12);

  for(int s = 1; s < numberSectors; s++){
    canvas->cd(s);
    histo[s]->Draw("colz");
    if (histoType%100 == 0) { xCaption.DrawLatex(0.5, 0.05, "x"); }
    if (histoType%101 == 0) { xCaption.DrawLatex(0.5, 0.05, "W (GeV/c^{2})"); }
    yCaption.DrawLatex(0.025, 0.45, "Q^{2} (Gev^{2}/c^{2})");
    sectorCaption.DrawLatex(0.45, 0.95, Form("Sector %d",s)); 
  }
  
  if (histoType == 100){ canvas->SaveAs("dataEvents_xByQQ.png"); }
  if (histoType == 101){ canvas->SaveAs("dataEvents_wByQQ.png"); }
  if (histoType == 200){ canvas->SaveAs("recEventsRad_xByQQ.png"); }
  if (histoType == 201){ canvas->SaveAs("recEventsRad_wByQQ.png"); }
  if (histoType == 202){ canvas->SaveAs("recEventsNoRad_xByQQ.png"); }
  if (histoType == 203){ canvas->SaveAs("recEventsNoRad_wByQQ.png"); }
  if (histoType == 300){ canvas->SaveAs("genEventsRad_xByQQ.png"); }
  if (histoType == 301){ canvas->SaveAs("genEventsRad_wByQQ.png"); }
  if (histoType == 302){ canvas->SaveAs("genEventsNoRad_xByQQ.png"); }
  if (histoType == 303){ canvas->SaveAs("genEventsNoRad_wByQQ.png"); }
  if (histoType == 400){ canvas->SaveAs("recAndGenEventsRad_xByQQ.png"); }
  if (histoType == 401){ canvas->SaveAs("recAndGenEventsRad_wByQQ.png"); }
  if (histoType == 402){ canvas->SaveAs("recAndGenEventsNoRad_xByQQ.png"); }
  if (histoType == 403){ canvas->SaveAs("recAndGenEventsNoRad_wByQQ.png"); }
  if (histoType == 500){ canvas->SaveAs("acceptanceRad_xByQQ.png"); }
  if (histoType == 501){ canvas->SaveAs("acceptanceRad_wByQQ.png"); }

 
}

void plotHistogram(TH1D *histo[numberSectors], const int numberSectors, int histoType){
  
  TCanvas *canvas = new TCanvas("canvas","",1200,800); 
  canvas->Divide(3,2);
  for(int s = 1; s < numberSectors; s++){
    canvas->cd(s);
    histo[s]->Draw("colz");
  }

  if (histoType == 500){ canvas->SaveAs("acceptanceRad_xIntegrated.png"); }
  if (histoType == 501){ canvas->SaveAs("acceptanceRad_wIntegrated.png"); }
  if (histoType == 502){ canvas->SaveAs("acceptanceNoRad_xIntegrated.png"); }
  if (histoType == 503){ canvas->SaveAs("acceptanceNoRad_wIntegrated.png"); }
 
}

void plotProjections(TH1D *histo[numberSectors][numberQQBins], const int numberSectors, const int numberQQBins, int sectorToPlot, int histoType){
  
  TCanvas * sliceCanvas = new TCanvas("sliceCanvas","",1200,800);

  // Get canvas size
  int divs = 0; 
  while (divs*(divs+1) < numberQQBins){ divs++; }
  cout << "Splitting canvnas to size=" << divs << " for slicing. " << endl;
  sliceCanvas->Divide(divs+1, divs); 

  for (int s=0; s< numberQQBins; s++){
    sliceCanvas->cd(s+1);
    histo[sectorToPlot][s]->Draw("same");
  }

  if (histoType == 100) { sliceCanvas->SaveAs(Form("dataEvents_xByQQ_s%d_slices.png",sectorToPlot)); }
  if (histoType == 101) { sliceCanvas->SaveAs(Form("dataEvents_wByQQ_s%d_slices.png",sectorToPlot)); }
  if (histoType == 200) { sliceCanvas->SaveAs(Form("recEventsRad_xByQQ_s%d_slices.png",sectorToPlot)); }
  if (histoType == 201) { sliceCanvas->SaveAs(Form("recEventsRad_wByQQ_s%d_slices.png",sectorToPlot)); }
  if (histoType == 202) { sliceCanvas->SaveAs(Form("recEventsNoRad_xByQQ_s%d_slices.png",sectorToPlot)); }
  if (histoType == 203) { sliceCanvas->SaveAs(Form("recEventsNoRad_wByQQ_s%d_slices.png",sectorToPlot)); }
  if (histoType == 300) { sliceCanvas->SaveAs(Form("genEventsRad_xByQQ_s%d_slices.png",sectorToPlot)); }
  if (histoType == 301) { sliceCanvas->SaveAs(Form("genEventsRad_wByQQ_s%d_slices.png",sectorToPlot)); }
  if (histoType == 302) { sliceCanvas->SaveAs(Form("genEventsNoRad_xByQQ_s%d_slices.png",sectorToPlot)); }
  if (histoType == 303) { sliceCanvas->SaveAs(Form("genEventsNoRad_wByQQ_s%d_slices.png",sectorToPlot)); }
  if (histoType == 400) { sliceCanvas->SaveAs(Form("recAndGenEventsRad_xByQQ_s%d_slices.png",sectorToPlot)); }
  if (histoType == 401) { sliceCanvas->SaveAs(Form("recAndGenEventsRad_wByQQ_s%d_slices.png",sectorToPlot)); }
  if (histoType == 402) { sliceCanvas->SaveAs(Form("recAndGenEventsNoRad_xByQQ_s%d_slices.png",sectorToPlot)); }
  if (histoType == 403) { sliceCanvas->SaveAs(Form("recAndGenEventsNoRad_wByQQ_s%d_slices.png",sectorToPlot)); }
  if (histoType == 500) { sliceCanvas->SaveAs(Form("acceptanceRad_xByQQ_s%d_slices.png",sectorToPlot)); }
  if (histoType == 501) { sliceCanvas->SaveAs(Form("acceptanceRad_wByQQ_s%d_slices.png",sectorToPlot)); }
  if (histoType == 502) { sliceCanvas->SaveAs(Form("accpetanceNoRad_xByQQ_s%d_slices.png",sectorToPlot)); }
  if (histoType == 503) { sliceCanvas->SaveAs(Form("acceptanceNoRad_wByQQ_s%d_slices.png",sectorToPlot)); }

}

void plotProjectionWithoutSave(TH1D *histo[numberSectors][numberQQBins], const int numberSectors, const int numberQQBins, int sectorToDraw, int sliceToDraw){

  if (sectorToDraw == numberSectors){ sectorToDraw = 0; }
  if (sliceToDraw == numberQQBins){ sliceToDraw = 0; }

  TCanvas * sliceCanvas = new TCanvas("sliceCanvas","",800,800);
  histo[sectorToDraw][sliceToDraw]->Draw();

}
