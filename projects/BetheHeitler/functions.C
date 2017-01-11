void testCut(double angleCut, double massCut){
  

  int angleBin = events->GetAxis(0)->FindBin(angleCut);
  int massBin  = events->GetAxis(1)->FindBin(massCut);

  events->GetAxis(0)->SetRange(angleBin, events->GetAxis(0)->GetNbins());
  events->GetAxis(1)->SetRange(massBin,  events->GetAxis(1)->GetNbins());

  TH1D *wPass = events->Projection(2);
  massW = events->Projection(2,1);

  TLine *massLine = new TLine(massCut, 0, massCut, 65); 
  massLine->SetLineColor(99);
  massLine->SetLineStyle(4); 
  massLine->SetLineWidth(2);

  TLine *angleLine = new TLine(0, angleCut, 5, angleCut); 
  angleLine->SetLineColor(99);
  angleLine->SetLineStyle(4); 
  angleLine->SetLineWidth(2);

  TLatex *tit, *xtit, *ytit;
  tit = new TLatex();
  xtit = new TLatex();
  ytit = new TLatex();

  tit->SetNDC(); 
  tit->SetTextSize(0.05);

  xtit->SetNDC(); 
  xtit->SetTextSize(0.04);

  ytit->SetNDC(); 
  ytit->SetTextSize(0.04);
  ytit->SetTextAngle(90.0); 

  TCanvas *can1 = new TCanvas("can1","",800,500);
  w->SetLineColor(99);
  w->SetFillColorAlpha(99, 0.3);
  wPass->SetLineColor(55);
  wPass->SetFillColorAlpha(55, 0.3);
  w     ->Draw();
  wPass ->Draw("same");

  tit->DrawLatex(0.4, 0.9, "#color[99]{No Cuts} and #color[55]{Cuts}"); 
  xtit->DrawLatex(0.7, 0.06, "W [GeV/c^{2}]");
  ytit->DrawLatex(0.06, 0.7, "Counts"); 

  TCanvas *can2 = new TCanvas("can2","",800,500); 
  gPad->SetLogz();
  gPad->SetGridx();
  gPad->SetGridy();
  massAngle ->Draw("colz");
  massLine  ->Draw();
  angleLine ->Draw();
  
  tit->DrawLatex(0.4, 0.9, "ep #rightarrow epX"); 
  xtit->DrawLatex(0.7, 0.06, "M_{X}^{2}");
  ytit->DrawLatex(0.06, 0.7, "#theta_{X}"); 



}
