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
  TH1D *pion_x_corr[numberZBins+1][numberPtBins+1];
  TH1D *kaon_x_corr[numberZBins+1][numberPtBins+1];

  TH1D *pion_z[numberPtBins+1][numberXBins+1];
  TH1D *kaon_z[numberPtBins+1][numberXBins+1];
  TH1D *pion_z_corr[numberPtBins+1][numberXBins+1];
  TH1D *kaon_z_corr[numberPtBins+1][numberXBins+1];

  TH1D *pion_pt[numberXBins+1][numberZBins+1];
  TH1D *kaon_pt[numberXBins+1][numberZBins+1];
  TH1D *pion_pt_corr[numberXBins+1][numberZBins+1];
  TH1D *kaon_pt_corr[numberXBins+1][numberZBins+1];


    for(int j=0; j<numberZBins+1; j++){
      for(int k=0; k<numberPtBins+1; k++){
	pion_x[j][k] = (TH1D*) inputFile->Get(Form("integrated/pp/h1_x_z%d_pt%d_base", j, k));
	pion_x[j][k]->SetLineColor(99); 
	pion_x[j][k]->SetFillColor(99); 
	pion_x[j][k]->SetMarkerColor(99); 
	pion_x[j][k]->SetMarkerStyle(8); 
	pion_x[j][k]->SetMarkerSize(0.7); 
	pion_x[j][k]->SetLineWidth(1.5); 
	pion_x[j][k]->SetMaximum(0.08); 
	pion_x[j][k]->SetMinimum(-0.08); 
	pion_x[j][k]->Draw("pe"); 

	pion_x_corr[j][k] = (TH1D*) inputFile->Get(Form("integrated/pp/h1_x_z%d_pt%d_corr", j, k));
	pion_x_corr[j][k]->SetLineColor(99); 
	pion_x_corr[j][k]->SetFillColor(99); 
	pion_x_corr[j][k]->SetMarkerColor(99); 
	pion_x_corr[j][k]->SetMarkerStyle(24); 
	pion_x_corr[j][k]->SetMarkerSize(0.7); 
	pion_x_corr[j][k]->SetLineWidth(1.5); 
	pion_x_corr[j][k]->SetMaximum(0.06); 
	pion_x_corr[j][k]->SetMinimum(-0.02); 
	pion_x_corr[j][k]->Draw("pesame"); 

	kaon_x[j][k] = (TH1D*) inputFile->Get(Form("integrated/kp/h1_x_z%d_pt%d_base", j, k));
	kaon_x[j][k]->SetLineColor(55); 
	kaon_x[j][k]->SetFillColor(55); 
	kaon_x[j][k]->SetMarkerColor(55); 
	kaon_x[j][k]->SetMarkerStyle(8); 
	kaon_x[j][k]->SetMarkerSize(0.7); 
	kaon_x[j][k]->SetLineWidth(1.5); 
	kaon_x[j][k]->SetMaximum(0.06); 
	kaon_x[j][k]->SetMinimum(-0.02); 
	kaon_x[j][k]->Draw("pesame"); 

	kaon_x_corr[j][k] = (TH1D*) inputFile->Get(Form("integrated/kp/h1_x_z%d_pt%d_corr", j, k));
	kaon_x_corr[j][k]->SetLineColor(55); 
	kaon_x_corr[j][k]->SetFillColor(55); 
	kaon_x_corr[j][k]->SetMarkerColor(55); 
	kaon_x_corr[j][k]->SetMarkerStyle(24); 
	kaon_x_corr[j][k]->SetMarkerSize(0.7); 
	kaon_x_corr[j][k]->SetLineWidth(1.5); 
	kaon_x_corr[j][k]->SetMaximum(0.06); 
	kaon_x_corr[j][k]->SetMinimum(-0.02); 
	kaon_x_corr[j][k]->Draw("pesame"); 

	title.DrawLatex(0.15, 0.88, Form("Integrated Asymmetry for bin (%d, %d) #color[99]{#pi^{+}} and #color[55]{K^{+}}", j, k)); 
	xtit.DrawLatex(0.65, 0.07, "x_{Bjorken}"); 
	ytit.DrawLatex(0.08, 0.45, "A_{LU}^{sin#phi}"); 


	can->Print(Form("%s/asymmetry_z%d_pt%d_pos.png", imagePath.c_str(), j, k)); 
	can->Clear(); 
      }
    }


    for(int j=0; j<numberPtBins+1; j++){
      for(int k=0; k<numberXBins+1; k++){
	pion_z[j][k] = (TH1D*) inputFile->Get(Form("integrated/pp/h1_z_pt%d_x%d_base", j, k));
	pion_z[j][k]->SetLineColor(99); 
	pion_z[j][k]->SetFillColor(99); 
	pion_z[j][k]->SetMarkerColor(99); 
	pion_z[j][k]->SetMarkerStyle(8); 
	pion_z[j][k]->SetMarkerSize(0.7); 
	pion_z[j][k]->SetLineWidth(1.5); 
	pion_z[j][k]->SetMaximum(0.06); 
	pion_z[j][k]->SetMinimum(-0.02); 
	pion_z[j][k]->Draw("pe"); 
 
	pion_z_corr[j][k] = (TH1D*) inputFile->Get(Form("integrated/pp/h1_z_pt%d_x%d_corr", j, k));
	pion_z_corr[j][k]->SetLineColor(99); 
	pion_z_corr[j][k]->SetFillColor(99); 
	pion_z_corr[j][k]->SetMarkerColor(99); 
	pion_z_corr[j][k]->SetMarkerStyle(24); 
	pion_z_corr[j][k]->SetMarkerSize(0.7); 
	pion_z_corr[j][k]->SetLineWidth(1.5); 
	pion_z_corr[j][k]->SetMaximum(0.06); 
	pion_z_corr[j][k]->SetMinimum(-0.02); 
	pion_z_corr[j][k]->Draw("pesame"); 

	kaon_z[j][k] = (TH1D*) inputFile->Get(Form("integrated/kp/h1_z_pt%d_x%d_base", j, k));
	kaon_z[j][k]->SetLineColor(55); 
	kaon_z[j][k]->SetFillColor(55); 
	kaon_z[j][k]->SetMarkerColor(55); 
	kaon_z[j][k]->SetMarkerStyle(8); 
	kaon_z[j][k]->SetMarkerSize(0.7); 
	kaon_z[j][k]->SetLineWidth(1.5); 
	kaon_z[j][k]->SetMaximum(0.06); 
	kaon_z[j][k]->SetMinimum(-0.02); 
	kaon_z[j][k]->Draw("pesame"); 


	kaon_z_corr[j][k] = (TH1D*) inputFile->Get(Form("integrated/kp/h1_z_pt%d_x%d_corr", j, k));
	kaon_z_corr[j][k]->SetLineColor(55); 
	kaon_z_corr[j][k]->SetFillColor(55); 
	kaon_z_corr[j][k]->SetMarkerColor(55); 
	kaon_z_corr[j][k]->SetMarkerStyle(24); 
	kaon_z_corr[j][k]->SetMarkerSize(0.7); 
	kaon_z_corr[j][k]->SetLineWidth(1.5); 
	kaon_z_corr[j][k]->SetMaximum(0.06); 
	kaon_z_corr[j][k]->SetMinimum(-0.02); 
	kaon_z_corr[j][k]->Draw("pesame"); 

	title.DrawLatex(0.3, 0.88, Form("Asymmetry for bin (%d, %d) #color[99]{#pi^{+}} and #color[55]{K^{+}}", j, k)); 
	xtit.DrawLatex(0.65, 0.07, "z"); 
	ytit.DrawLatex(0.08, 0.45, "A_{LU}^{sin#phi}"); 


	can->Print(Form("%s/asymmetry_pt%d_x%d_pos.png", imagePath.c_str(), j, k)); 
	can->Clear(); 
      }
    }


    for(int j=0; j<numberXBins+1; j++){
      for(int k=0; k<numberZBins+1; k++){
	pion_pt[j][k] = (TH1D*) inputFile->Get(Form("integrated/pp/h1_pt_x%d_z%d_base", j, k));
	pion_pt[j][k]->SetLineColor(99); 
	pion_pt[j][k]->SetFillColor(99); 
	pion_pt[j][k]->SetMarkerColor(99); 
	pion_pt[j][k]->SetMarkerStyle(8); 
	pion_pt[j][k]->SetMarkerSize(0.7); 
	pion_pt[j][k]->SetLineWidth(1.5); 
	pion_pt[j][k]->SetMaximum(0.06); 
	pion_pt[j][k]->SetMinimum(-0.02); 
	pion_pt[j][k]->Draw("pe"); 

	pion_pt_corr[j][k] = (TH1D*) inputFile->Get(Form("integrated/pp/h1_pt_x%d_z%d_corr", j, k));
	pion_pt_corr[j][k]->SetLineColor(99); 
	pion_pt_corr[j][k]->SetFillColor(99); 
	pion_pt_corr[j][k]->SetMarkerColor(99); 
	pion_pt_corr[j][k]->SetMarkerStyle(24); 
	pion_pt_corr[j][k]->SetMarkerSize(0.7); 
	pion_pt_corr[j][k]->SetLineWidth(1.5); 
	pion_pt_corr[j][k]->SetMaximum(0.06); 
	pion_pt_corr[j][k]->SetMinimum(-0.02); 
	pion_pt_corr[j][k]->Draw("pesame"); 
 
	kaon_pt[j][k] = (TH1D*) inputFile->Get(Form("integrated/kp/h1_pt_x%d_z%d_base", j, k));
	kaon_pt[j][k]->SetLineColor(55); 
	kaon_pt[j][k]->SetFillColor(55); 
	kaon_pt[j][k]->SetMarkerColor(55); 
	kaon_pt[j][k]->SetMarkerStyle(8); 
	kaon_pt[j][k]->SetMarkerSize(0.7); 
	kaon_pt[j][k]->SetLineWidth(1.5); 
	kaon_pt[j][k]->SetMaximum(0.06); 
	kaon_pt[j][k]->SetMinimum(-0.02); 
	kaon_pt[j][k]->Draw("pesame"); 

	kaon_pt_corr[j][k] = (TH1D*) inputFile->Get(Form("integrated/kp/h1_pt_x%d_z%d_corr", j, k));
	kaon_pt_corr[j][k]->SetLineColor(55); 
	kaon_pt_corr[j][k]->SetFillColor(55); 
	kaon_pt_corr[j][k]->SetMarkerColor(55); 
	kaon_pt_corr[j][k]->SetMarkerStyle(24); 
	kaon_pt_corr[j][k]->SetMarkerSize(0.7); 
	kaon_pt_corr[j][k]->SetLineWidth(1.5); 
	kaon_pt_corr[j][k]->SetMaximum(0.06); 
	kaon_pt_corr[j][k]->SetMinimum(-0.02); 
	kaon_pt_corr[j][k]->Draw("pesame"); 

	title.DrawLatex(0.3, 0.88, Form("Asymmetry for bin (%d, %d) #color[99]{#pi^{+}} and #color[55]{K^{+}}", j, k)); 
	xtit.DrawLatex(0.65, 0.07, "P_{T} (GeV/c)"); 
	ytit.DrawLatex(0.08, 0.45, "A_{LU}^{sin#phi}"); 


	can->Print(Form("%s/asymmetry_x%d_z%d_pos.png", imagePath.c_str(), j, k)); 
	can->Clear(); 
      }
    }


}
