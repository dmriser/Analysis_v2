{
  gROOT->LoadMacro("color.C"); 
  setColor(); 

  std::string inputFile("../out/big_dingus.root");

  TFile *rootFile = TFile::Open(inputFile.c_str());

  TH2F *betaP[7];
  TH2F *betaPPion[7];
  TH2F *betaPKaon[7];

  TF1 *cutPion[2];
  TF1 *cutKaon[2];

  float NSIG = 5.0; 
  float pionMassRange[2] = {0.06, 0.35};
  float kaonMassRange[2] = {0.35, 0.60};
  //  float pionMassRange[2] = {0.1, 0.17};
  //  float kaonMassRange[2] = {0.42, 0.55};

  for(int s=0; s<7; s++){
    betaP[s] = (TH2F*) rootFile->Get(Form("MesonHistograms/h2_p_beta_%d_211_no_cut", s));
    betaPPion[s] = (TH2F*) rootFile->Get(Form("MesonHistograms/h2_p_beta_%d_211_all_cut", s));
    betaPKaon[s] = (TH2F*) rootFile->Get(Form("MesonHistograms/h2_p_beta_%d_321_all_cut", s));
  }

    // setting the upper and lower boundary lines 
  cutPion[0] = new TF1("cutPionLow", Form("1/sqrt(1+(%f/x)^2)", pionMassRange[1]), 0.2, 2.2); 
  cutPion[1] = new TF1("cutPionUp",  Form("1/sqrt(1+(%f/x)^2)", pionMassRange[0]), 0.2, 2.2); 
  cutKaon[0] = new TF1("cutKaonLow", Form("1/sqrt(1+(%f/x)^2)", kaonMassRange[1]), 0.2, 2.2); 
  cutKaon[1] = new TF1("cutKaonUp",  Form("1/sqrt(1+(%f/x)^2)", kaonMassRange[0]), 0.2, 2.2); 

  cutPion[0]->SetLineStyle(8);
  cutPion[0]->SetLineColor(kWhite);
  cutPion[0]->SetLineWidth(2);
  cutPion[1]->SetLineStyle(8);
  cutPion[1]->SetLineColor(kWhite);
  cutPion[1]->SetLineWidth(2);
  cutKaon[0]->SetLineStyle(8);
  cutKaon[0]->SetLineColor(kWhite);
  cutKaon[0]->SetLineWidth(2);
  cutKaon[1]->SetLineStyle(8);
  cutKaon[1]->SetLineColor(kWhite);
  cutKaon[1]->SetLineWidth(2);    


  TCanvas *can = new TCanvas("can", "", 800, 1200);

  can->Divide(2,3);
  for(int s=1; s<7; s++){
    can->cd(s);
    betaP[s]->Draw("colz");
    cutPion[0]->Draw("same"); 
    cutPion[1]->Draw("same"); 
    cutKaon[0]->Draw("same"); 
    cutKaon[1]->Draw("same"); 
    gPad->SetLogz();
  }
  can->Print("no_cut.pdf");
  can->Clear(); 

  TLatex *tex = new TLatex(); 
  tex->SetNDC(); 
  tex->SetTextSize(0.03); 
  tex->SetTextFont(42); 
  
  TCanvas *singleCan = new TCanvas("can", "", 1600, 800); 
  singleCan->Divide(2, 1); 

  singleCan->cd(1); 
  betaP[0]->Draw("colz"); 
  cutPion[0]->Draw("same");
  cutPion[1]->Draw("same");
  gPad->SetLogz(); 
  tex->DrawLatex(0.45, 0.05, "p [GeV/c]"); 
  tex->SetTextAngle(90.0); 
  tex->DrawLatex(0.035, 0.475, "#beta"); 
  tex->SetTextAngle(0.0); 

  singleCan->cd(2); 
  betaP[0]->Draw("colz"); 
  cutKaon[0]->Draw("same");
  cutKaon[1]->Draw("same");
  gPad->SetLogz(); 
  tex->DrawLatex(0.45, 0.05, "p [GeV/c]"); 
  tex->SetTextAngle(90.0); 
  tex->DrawLatex(0.035, 0.475, "#beta"); 
  
  singleCan->Print("positive_hadrons.pdf"); 

}
