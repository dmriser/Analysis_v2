{

  TFile *inputFile = TFile::Open("BetheHeitlerEvents.root");
 
  THnSparseI *events = (THnSparseI*) inputFile->Get("events"); 

  TH1D *mass      = events->Projection(1); 
  TH1D *w         = events->Projection(2); 
  TH1D *angle     = events->Projection(0); 
  TH2D *massAngle = events->Projection(0,1); 
  TH2D *massW     = events->Projection(2,1); 

  gROOT->LoadMacro("functions.C"); 

  // Mass Angle 2D-Plot 
  TCanvas *c1 = new TCanvas("c1","",800,500); 
  TLatex *title = new TLatex(); 
  title->SetNDC(); 
  title->SetTextSize(0.05);

  TLatex *xtitle = new TLatex(); 
  xtitle->SetNDC(); 
  xtitle->SetTextSize(0.04);

  TLatex *ytitle = new TLatex(); 
  ytitle->SetNDC(); 
  ytitle->SetTextSize(0.04);
  ytitle->SetTextAngle(90.0); 
 
  gPad->SetGridx(); 
  gPad->SetGridy(); 
  gPad->SetLogz(); 
  massAngle->Draw("colz"); 
  
  title->DrawLatex(0.4,0.9,"ep #rightarrow epX"); 
  xtitle->DrawLatex(0.75,0.06,"M^{2}_{X}"); 
  ytitle->DrawLatex(0.08,0.72,"#theta_{X}"); 
  c1->Print("MissingMassVsAngle.png"); 

  // Mass W 2D-Plot 
  TCanvas *c3 = new TCanvas("c3","",800,500); 
  gPad->SetGridx(); 
  gPad->SetGridy(); 
  gPad->SetLogz(); 
  massW->Draw("colz"); 
  
  title->DrawLatex(0.4,0.9,"ep #rightarrow epX"); 
  xtitle->DrawLatex(0.75,0.06,"M^{2}_{X}"); 
  ytitle->DrawLatex(0.08,0.72,"W [GeV/c^{2}]"); 
  c3->Print("MissingMassVsW.png"); 

}
