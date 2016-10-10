void plotHistogram(TH2D *histo[numberSectors], const int numberSectors, int histoType){
  
  TCanvas *canvas = new TCanvas("canvas","",1200,800); 
  canvas->Divide(3,2);

  TLatex xCaption, yCaption, sectorCaption; 
  xCaption.SetNDC();
  xCaption.SetTextFont(22);
  yCaption.SetNDC();
  yCaption.SetTextFont(22);
  yCaption.SetTextAngle(90); 

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

 
}

void plotHistogram(TH1D *histo[numberSectors], const int numberSectors, int histoType){
  
  TCanvas *canvas = new TCanvas("canvas","",1200,800); 
  canvas->Divide(3,2);
  for(int s = 1; s < numberSectors; s++){
    canvas->cd(s);
    histo[s]->Draw("colz");
  }
 
}
