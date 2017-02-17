{
  // -----------------------------------------
  //   User Input 
  // -----------------------------------------
  TFile *inputFile = TFile::Open("../refTest.root"); 
  string imagePath = "";

  // -----------------------------------------
  //   Business
  // -----------------------------------------
  THnSparseI *pip  = (THnSparseI*) inputFile->Get("pipEvents");  
  THnSparseI *pim  = (THnSparseI*) inputFile->Get("pimEvents");  
  THnSparseI *piz  = (THnSparseI*) inputFile->Get("pizEvents");  

  TH1D *rapPip = pip->Projection(5);
  TH1D *rapPim = pim->Projection(5);
  TH1D *rapPiz = piz->Projection(5);

  TH2D *rapQQPip = pip->Projection(5,2);   
  TH2D *rapQQPim = pim->Projection(5,2);   
  TH2D *rapQQPiz = piz->Projection(5,2);     

  rapPip->SetLineColor(99);
  rapPim->SetLineColor(55);  
  rapPiz->SetLineColor(77);

  TCanvas *c1 = new TCanvas("c1","",800,500); 
  rapPip->Draw(); 
  rapPim->Draw("same"); 
  rapPiz->Draw("same"); 

  TLatex tit, xtit, ytit;
  tit.SetNDC(); 
  tit.SetTextSize(0.04); 
  tit.SetTextFont(42); 
  xtit.SetNDC(); 
  xtit.SetTextSize(0.04); 
  xtit.SetTextFont(42); 
  ytit.SetNDC(); 
  ytit.SetTextSize(0.04); 
  ytit.SetTextFont(42); 
  ytit.SetTextAngle(90.0);

  tit .DrawLatex(0.38,0.9,"Rapidity #color[99]{#pi^{+}}, #color[55]{#pi^{-}}, #color[77]{#pi^{0}}"); 
  xtit.DrawLatex(0.45, 0.05, "Rapidity #eta");
  ytit.DrawLatex(0.05,0.7,"Counts");

  c1->Print("hadronRapidity.png"); 
}
