void testCutOnXAngleXMass(double angleCut, double massCut){
  int angleBin = events->GetAxis(0)->FindBin(angleCut);
  int massBin  = events->GetAxis(1)->FindBin(massCut);

  events->GetAxis(0)->SetRange(angleBin, events->GetAxis(0)->GetNbins());
  events->GetAxis(1)->SetRange(massBin,  events->GetAxis(1)->GetNbins());

  TH1D *wPass = events->Projection(2);
  //  massW       = events->Projection(2,1);

  // Set to region in top left which also passes 
  events->GetAxis(0)->SetRange(angleBin, events->GetAxis(0)->GetNbins());
  events->GetAxis(1)->SetRange(1, massBin);
  TH1D *wPass1 = events->Projection(2);

  // Set to region in top left which also passes 
  events->GetAxis(0)->SetRange(1, angleBin);
  events->GetAxis(1)->SetRange(massBin,  events->GetAxis(1)->GetNbins());
  TH1D *wPass2 = events->Projection(2);

  // Sum up passing 
  wPass->Add(wPass1); 
  wPass->Add(wPass2); 

  // Set to failing region 
  events->GetAxis(0)->SetRange(1, angleBin);
  events->GetAxis(1)->SetRange(1, massBin);
  TH1D *wFail = events->Projection(2);

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
  w->SetLineColor(55);
  wPass->SetLineColor(75);
  wFail->SetLineColor(99);
  w     ->Draw();
  wPass ->Draw("same");
  wFail ->Draw("same");

  tit->DrawLatex(0.25, 0.9, "#color[55]{No Cuts}, #color[99]{Fail Cuts}, and #color[75]{Pass Cuts}"); 
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

void setRange(string title, double min, double max){

  int axisToBeSet = -1;

  if (title == "xAngle")           { axisToBeSet = 0; }
  else if(title == "missingMass")  { axisToBeSet = 1; }
  else if(title == "w")            { axisToBeSet = 2; }
  else if(title == "coplanarAngle"){ axisToBeSet = 3; }
  else if(title == "ePrimeXAngle") { axisToBeSet = 4; }

  if (axisToBeSet > -1){
    int startBin = events->GetAxis(axisToBeSet)->FindBin(min);
    int endBin   = events->GetAxis(axisToBeSet)->FindBin(max);
    events->GetAxis(axisToBeSet)->SetRange(startBin, endBin);
    cout << "[setRange] Axis " << title << " is set. " << endl;
  }
  else {
    cout << "[setRange] Axis " << title << " is invalid. " << endl;
  }
}

void testMissingMassCut(double min, double max){

  setRange("missingMass",min,max);
  TH1D *missingMassPass   = events->Projection(1);
  TH1D *coplanarAnglePass = events->Projection(3);

  TLine *mmMinLine = new TLine(min, 0, min, mass->GetMaximum()); 
  TLine *mmMaxLine = new TLine(max, 0, max, mass->GetMaximum()); 

  TLatex *tit = new TLatex();
  TLatex *xtit = new TLatex();
  TLatex *ytit = new TLatex();

  tit->SetNDC();
  tit->SetTextSize(0.05);
  tit->SetTextFont(42); 

  xtit->SetNDC();
  xtit->SetTextSize(0.04);
  xtit->SetTextFont(42); 

  ytit->SetNDC();
  ytit->SetTextSize(0.04);
  ytit->SetTextAngle(90.0);
  ytit->SetTextFont(42); 

  mmMinLine->SetLineColor(99);
  mmMinLine->SetLineWidth(2);
  mmMinLine->SetLineStyle(4);

  mmMaxLine->SetLineColor(99);
  mmMaxLine->SetLineWidth(2);
  mmMaxLine->SetLineStyle(4);

  TCanvas *can1 = new TCanvas("can1","",800,500); 
  mass           ->SetLineColor(55);
  missingMassPass->SetLineColor(77);

  mass           ->Draw();
  missingMassPass->Draw("same");
  mmMinLine->Draw();
  mmMaxLine->Draw();
  gPad->SetGridx(); 
  gPad->SetGridy(); 

  tit->DrawLatex(0.2, 0.9, " Missing Mass Sq. #color[55]{All} and #color[77]{Pass}"); 
  xtit->DrawLatex(0.7, 0.08, "M^{2}_{X}"); 
  ytit->DrawLatex(0.05, 0.72, "Counts");

  TCanvas *can2 = new TCanvas("can2","",800,500); 
  coplanarAngle     ->SetLineColor(55); 
  coplanarAnglePass ->SetLineColor(77); 
  coplanarAngle     ->Draw(); 
  coplanarAnglePass ->Draw("same"); 
  gPad->SetGridx(); 
  gPad->SetGridy(); 

  tit->DrawLatex(0.15, 0.9, " eP Coplanar Angle #color[55]{All} and #color[77]{Pass}"); 
  xtit->DrawLatex(0.7, 0.08, "#phi_{eP}"); 
  ytit->DrawLatex(0.05, 0.72, "Counts");

}

void displayElectronAngles(){

  TH1D *eX      = events->Projection(0); 
  TH1D *ePrimeX = events->Projection(4); 

  TLatex *tit = new TLatex();
  TLatex *xtit = new TLatex();
  TLatex *ytit = new TLatex();

  tit->SetNDC();
  tit->SetTextSize(0.05);
  tit->SetTextFont(42); 

  xtit->SetNDC();
  xtit->SetTextSize(0.04);
  xtit->SetTextFont(42); 

  ytit->SetNDC();
  ytit->SetTextSize(0.04);
  ytit->SetTextAngle(90.0);
  ytit->SetTextFont(42); 

  double max = eX->GetMaximum(); 
  if(ePrimeX->GetMaximum() > max) max = ePrimeX->GetMaximum();

  TCanvas *can1 = new TCanvas("can1","",800,500); 
  eX       ->SetLineColor(99);
  ePrimeX  ->SetLineColor(83);
  eX       ->SetMaximum(1.1*max);
  ePrimeX  ->SetMaximum(1.1*max);
  eX       ->Draw();
  ePrimeX  ->Draw("same");

  tit->DrawLatex(0.4, 0.9, "ep #rightarrow epX #color[99]{#theta_{eX}} and #color[83]{#theta_{e'X}}"); 
  xtit->DrawLatex(0.49, 0.08, "#theta");
  ytit->DrawLatex(0.08, 0.75, "Counts");

  gPad->SetGridx();
  gPad->SetGridy();


}

void displayW(){

  TH1D *wPass = events->Projection(2);
  double max = w->GetMaximum();

  TLatex *tit = new TLatex();
  TLatex *xtit = new TLatex();
  TLatex *ytit = new TLatex();

  tit->SetNDC();
  tit->SetTextSize(0.05);
  tit->SetTextFont(42); 

  xtit->SetNDC();
  xtit->SetTextSize(0.04);
  xtit->SetTextFont(42); 

  ytit->SetNDC();
  ytit->SetTextSize(0.04);
  ytit->SetTextAngle(90.0);
  ytit->SetTextFont(42); 

  TCanvas *can1 = new TCanvas("can1","",800,500); 
  w       ->SetLineColor(99);
  wPass   ->SetLineColor(65);
  w       ->SetMaximum(1.1*max);
  wPass   ->SetMaximum(1.1*max);
  w       ->Draw();
  wPass   ->Draw("same");

  tit ->DrawLatex(0.25, 0.9, "ep #rightarrow epX #color[99]{All} and #color[65]{Pass B.H.Cuts}"); 
  xtit->DrawLatex(0.7, 0.08, "W [GeV/c^{2}]");
  ytit->DrawLatex(0.08, 0.75, "Counts");

  gPad->SetGridx();
  gPad->SetGridy();

}

void applyCuts(double massMin, double massMax, double angleMin, double angleMax){
  setRange("missingMass", massMin, massMax);
  setRange("coplanarAngle", angleMin, angleMax);
}
