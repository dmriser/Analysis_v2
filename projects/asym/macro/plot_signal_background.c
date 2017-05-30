{
  // --------------------------------------------
  //    user parameters 
  // --------------------------------------------
  TFile *inputFile      = TFile::Open("/volatile/clas12/dmriser/rootFiles/asymmetry/pass12.root"); 
  std::string imagePath = "/volatile/clas12/dmriser/plots/asymmetry";

  const int numberXBins  = 6; 
  const int numberZBins  = 6; 
  const int numberPtBins = 6; 

  // --------------------------------------------
  //    styling 
  // --------------------------------------------
  TStyle *style = new TStyle("style", "style"); 

  style->Reset("Modern"); 
  style->SetOptStat(0);
  style->SetLabelSize(0.1,"xyz");
  style->SetTitleSize(0.1,"xyz");
  style->SetTitleFont(112,"xyz");
  style->SetLabelFont(112,"xyz");
  style->SetLabelSize(0.1);
  style->SetTitleOffset(1.2,"y");
  style->SetCanvasColor(0);
  style->SetCanvasBorderMode(0);
  style->SetCanvasBorderSize(0);
  style->SetPadBottomMargin(0.15);
  style->SetPadTopMargin(0.15);
  style->SetPadLeftMargin(0.15);
  style->SetPadRightMargin(0.15);
  style->SetPadGridX(0);
  style->SetPadGridY(0);
  style->SetPadTickX(1);
  style->SetPadTickY(1);
  style->SetFrameBorderMode(0);
  gROOT->SetStyle("style"); 
  
  // --------------------------------------------
  //     plotting 
  // --------------------------------------------
  TCanvas *can = new TCanvas("can","",800,500); 

  TLatex title, xtit, ytit; 
  title.SetNDC(); 
  title.SetTextSize(0.065); 

  xtit.SetNDC(); 
  xtit.SetTextSize(0.04); 

  ytit.SetNDC(); 
  ytit.SetTextSize(0.04); 
  ytit.SetTextAngle(90.0); 

  TH1D *pion_x[numberZBins+1][numberPtBins+1];
  TH1D *kaon_x[numberZBins+1][numberPtBins+1];

  TH1D *pion_z[numberPtBins+1][numberXBins+1];
  TH1D *kaon_z[numberPtBins+1][numberXBins+1];

  TH1D *pion_pt[numberXBins+1][numberZBins+1];
  TH1D *kaon_pt[numberXBins+1][numberZBins+1];


    for(int j=0; j<numberZBins+1; j++){
      for(int k=0; k<numberPtBins+1; k++){
	pion_x[j][k] = (TH1D*) inputFile->Get(Form("signal/h1_x_signal_p_z%d_pt%d_test", j, k));
	pion_x[j][k]->SetLineColor(99); 
	pion_x[j][k]->SetFillColor(99); 
	pion_x[j][k]->SetMarkerColor(99); 
	pion_x[j][k]->SetMarkerStyle(8); 
	pion_x[j][k]->SetMarkerSize(0.7); 
	pion_x[j][k]->SetLineWidth(1.5); 
	pion_x[j][k]->SetMaximum(1.2); 
	pion_x[j][k]->SetMinimum(0.2); 
	pion_x[j][k]->Draw("p"); 

	kaon_x[j][k] = (TH1D*) inputFile->Get(Form("signal/h1_x_signal_k_z%d_pt%d_test", j, k));
	kaon_x[j][k]->SetLineColor(55); 
	kaon_x[j][k]->SetFillColor(55); 
	kaon_x[j][k]->SetMarkerColor(55); 
	kaon_x[j][k]->SetMarkerStyle(8); 
	kaon_x[j][k]->SetMarkerSize(0.7); 
	kaon_x[j][k]->SetLineWidth(1.5); 
	kaon_x[j][k]->SetMaximum(1.2); 
	kaon_x[j][k]->SetMinimum(0.2); 
	kaon_x[j][k]->Draw("psame"); 

	title.DrawLatex(0.3, 0.88, Form("Signal Purity for bin (%d, %d) #color[99]{#pi^{+}} and #color[55]{K^{+}}", j, k)); 
	xtit.DrawLatex(0.65, 0.07, "x_{Bjorken}"); 
	ytit.DrawLatex(0.08, 0.45, "Signal"); 


	can->Print(Form("%s/signal_z%d_pt%d_pos.png", imagePath.c_str(), j, k)); 
	can->Clear(); 
      }
    }


    for(int j=0; j<numberPtBins+1; j++){
      for(int k=0; k<numberXBins+1; k++){
	pion_z[j][k] = (TH1D*) inputFile->Get(Form("signal/h1_z_signal_p_pt%d_x%d_test", j, k));
	pion_z[j][k]->SetLineColor(99); 
	pion_z[j][k]->SetFillColor(99); 
	pion_z[j][k]->SetMarkerColor(99); 
	pion_z[j][k]->SetMarkerStyle(8); 
	pion_z[j][k]->SetMarkerSize(0.7); 
	pion_z[j][k]->SetLineWidth(1.5); 
	pion_z[j][k]->SetMaximum(1.2); 
	pion_z[j][k]->SetMinimum(0.2); 
	pion_z[j][k]->Draw("p"); 

	kaon_z[j][k] = (TH1D*) inputFile->Get(Form("signal/h1_z_signal_k_pt%d_x%d_test", j, k));
	kaon_z[j][k]->SetLineColor(55); 
	kaon_z[j][k]->SetFillColor(55); 
	kaon_z[j][k]->SetMarkerColor(55); 
	kaon_z[j][k]->SetMarkerStyle(8); 
	kaon_z[j][k]->SetMarkerSize(0.7); 
	kaon_z[j][k]->SetLineWidth(1.5); 
	kaon_z[j][k]->SetMaximum(1.2); 
	kaon_z[j][k]->SetMinimum(0.2); 
	kaon_z[j][k]->Draw("psame"); 

	title.DrawLatex(0.3, 0.88, Form("Signal Purity for bin (%d, %d) #color[99]{#pi^{+}} and #color[55]{K^{+}}", j, k)); 
	xtit.DrawLatex(0.65, 0.07, "z"); 
	ytit.DrawLatex(0.08, 0.45, "Signal"); 


	can->Print(Form("%s/signal_pt%d_x%d_pos.png", imagePath.c_str(), j, k)); 
	can->Clear(); 
      }
    }


    for(int j=0; j<numberXBins+1; j++){
      for(int k=0; k<numberZBins+1; k++){
	pion_pt[j][k] = (TH1D*) inputFile->Get(Form("signal/h1_pt_signal_p_x%d_z%d_test", j, k));
	pion_pt[j][k]->SetLineColor(99); 
	pion_pt[j][k]->SetFillColor(99); 
	pion_pt[j][k]->SetMarkerColor(99); 
	pion_pt[j][k]->SetMarkerStyle(8); 
	pion_pt[j][k]->SetMarkerSize(0.7); 
	pion_pt[j][k]->SetLineWidth(1.5); 
	pion_pt[j][k]->SetMaximum(1.2); 
	pion_pt[j][k]->SetMinimum(0.2); 
	pion_pt[j][k]->Draw("p"); 

	kaon_pt[j][k] = (TH1D*) inputFile->Get(Form("signal/h1_pt_signal_k_x%d_z%d_test", j, k));
	kaon_pt[j][k]->SetLineColor(55); 
	kaon_pt[j][k]->SetFillColor(55); 
	kaon_pt[j][k]->SetMarkerColor(55); 
	kaon_pt[j][k]->SetMarkerStyle(8); 
	kaon_pt[j][k]->SetMarkerSize(0.7); 
	kaon_pt[j][k]->SetLineWidth(1.5); 
	kaon_pt[j][k]->SetMaximum(1.2); 
	kaon_pt[j][k]->SetMinimum(0.2); 
	kaon_pt[j][k]->Draw("psame"); 

	title.DrawLatex(0.3, 0.88, Form("Signal Purity for bin (%d, %d) #color[99]{#pi^{+}} and #color[55]{K^{+}}", j, k)); 
	xtit.DrawLatex(0.65, 0.07, "P_{T} (GeV/c)"); 
	ytit.DrawLatex(0.08, 0.45, "Signal"); 


	can->Print(Form("%s/signal_x%d_z%d_pos.png", imagePath.c_str(), j, k)); 
	can->Clear(); 
      }
    }


}
