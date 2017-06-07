{

  TFile *f = TFile::Open("/volatile/clas12/dmriser/rootFiles/asymmetry/asymmetry_exclusive_pass1.root"); 

  TH1D *pion = (TH1D*) f->Get("integrated/pp/h1_z_pt0_x0_exclusive_base"); 
  TH1D *kaon = (TH1D*) f->Get("integrated/kp/h1_z_pt0_x0_exclusive_base"); 


  pion->SetLineColor(55); 
  pion->SetMarkerColor(55); 
  pion->SetMarkerStyle(8); 
  pion->SetMarkerSize(0.6); 

  kaon->SetLineColor(99); 
  kaon->SetMarkerColor(99); 
  kaon->SetMarkerStyle(8); 
  kaon->SetMarkerSize(0.6); 

  pion->SetMinimum(0.0); 
  pion->SetMaximum(0.28); 
  

  TCanvas *can = new TCanvas("can", "", 800, 500);

  gPad->SetMargin(0.12, 0.12, 0.12, 0.12); 

  TLatex tit; 
  tit.SetNDC(); 
  tit.SetTextSize(0.04); 

  pion->Draw("pe");
  kaon->Draw("pesame");

  tit.DrawLatex(0.35, 0.92, "Exclusive Events #color[55]{#pi^{+}}, #color[99]{K^{+}}"); 

  can->Print("/volatile/clas12/dmriser/plots/asymmetry/exclusive.png"); 

}
