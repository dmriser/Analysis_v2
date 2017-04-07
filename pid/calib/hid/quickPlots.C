{

  TFile *inputFile = TFile::Open("oldCook.root");
  
  TH2D *h2_p_db_pip = (TH2D*) inputFile->Get("h2_p_db_pip_0");
  TH2D *h2_p_db_pim = (TH2D*) inputFile->Get("h2_p_db_pim_0");

  TCanvas *can = new TCanvas("can","",800,500); 
  can->cd();


  h2_p_db_pip->Draw("colz");
  gPad->SetMargin(0.15, 0.15, 0.15, 0.15);
  gPad->SetLogz(); 
  gPad->SetGridx();
  gPad->SetGridy();

  TLatex title, xtitle, ytitle; 
  title.SetNDC();
  title.SetTextFont(102);
  title.SetTextSize(0.05);
  xtitle.SetNDC();
  xtitle.SetTextFont(102);
  xtitle.SetTextSize(0.04);
  ytitle.SetNDC();
  ytitle.SetTextFont(102);
  ytitle.SetTextSize(0.04);
  ytitle.SetTextAngle(90.0);

  title.DrawLatex(0.36, 0.89, "#Delta#beta for m = m_{#pi}");
  xtitle.DrawLatex(0.47, 0.07, "p [GeV/c]");
  ytitle.DrawLatex(0.09, 0.47, "#Delta#beta");

  can->Print("deltaBetaPip.png");
}

