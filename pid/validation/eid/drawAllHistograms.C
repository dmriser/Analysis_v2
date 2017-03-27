{

  TFile * file = TFile::Open("dataFullSVM.root");
  TCanvas * c1 = new TCanvas("c1", "", 800, 800);


  // Electron Identification Histograms [2][7]                                                                                                                                                                      // [0] - without cuts,  [1] - with cuts                                                                                                                                                                           // [0] - all sectors,   [1-6] - sectors 1-6                                                                                                                                                                  
  string type[12] = {"allNegatives", "cuts", "Z_VERTEX","CC_FID","CC_PHI","CC_THETA","DC_R1_FID","DC_R3_FID","EC_FID","EC_IN_OUT","EC_SAMPLING","SVM"};
  string sect[7] = {"all", "s1", "s2", "s3", "s4", "s5", "s6"};

  string cutApplied[12] = {"None", "All", "Z-Vertex", "CC Fiducial", "CC #phi-Matching", "#theta_{CC} Segment-Matching", "DC Fiducial (R1)", 
			   "DC Fiducial (R3)", "EC Fiducial UVW", "EC Inner-EDep", "Sampling Fraction", "SVM and FID"};

  string xAxisTitle = "x";
  string yAxisTitle = "y";

  float xPosFrac = 0.75; 
  float yPosFrac = 0.95; 

  float xPosCut = 0.2; 
  float yPosCut = 0.87; 

  TLatex lab, xlabel, ylabel; 
  lab.SetNDC();
  lab.SetTextFont(102);
  lab.SetTextSize(0.04);

  xlabel.SetNDC();
  xlabel.SetTextFont(102);
  xlabel.SetTextSize(0.03);

  ylabel.SetNDC();
  ylabel.SetTextFont(102);
  ylabel.SetTextSize(0.03);
  ylabel.SetTextAngle(90.0);

  // 1-D
  TH1D * h1_nphe[12][7];
  TH1F * h1_ec_edep_inner[12][7];
  TH1F * h1_ec_edep_outer[12][7];
  TH1F * h1_p[12][7];
  TH1F * h1_z_vertex[12][7];

  // 2-D
  TH2F * h2_cc_theta[12][7];
  TH2F * h2_etot_p[12][7];
  TH2F * h2_ang_fid[12][7];
  TH2F * h2_ec_edep[12][7];
  TH2F * h2_dcr1_fid[12][7];
  TH2F * h2_dcr3_fid[12][7];
  TH2F * h2_ec_fid[12][7];
      
  string printer = "";
                        
  for (int itype = 0; itype < 12; itype++)
    for(int isect = 0; isect < 1; isect++){
	// 1d                                                                                                                                                                                                   
	h1_nphe[itype][isect]          = (TH1D*) file->Get(Form("h1_nphe_%s_%s",type[itype].c_str(),sect[isect].c_str()));
	h1_nphe[itype][isect]->SetFillColorAlpha(85, 0.4);
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	h1_nphe[itype][isect]->Draw();
	printer = Form("Pass: %.3f ", (double) h1_nphe[itype][isect]->GetEntries()/h1_nphe[0][isect]->GetEntries() );
	lab.DrawLatex(xPosFrac, yPosFrac, printer.c_str());

	// Draw things 
	xAxisTitle = "Number of Photoelectrons"; 
	yAxisTitle = "Events"; 
	xlabel.DrawLatex(0.41, 0.05, xAxisTitle.c_str()); 
	ylabel.DrawLatex(0.05, 0.47, yAxisTitle.c_str()); 
	lab.DrawLatex(xPosCut, yPosCut, Form("Applied Cut: %s", cutApplied[itype].c_str())); 

	c1->Print(Form("img/h1_nphe_%s_%s.png",type[itype].c_str(),sect[isect].c_str()));

	h1_ec_edep_inner[itype][isect] = (TH1F*) file->Get(Form("h1_ec_edep_inner_%s_%s",type[itype].c_str(),sect[isect].c_str()));
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	h1_ec_edep_inner[itype][isect]->Draw();
	printer = Form(" #rightarrow pass: %f ", (double) h1_ec_edep_inner[itype][isect]->GetEntries()/h1_nphe[0][isect]->GetEntries() );

	// Draw things 
	xAxisTitle = "Energy Deposited (GeV)"; 
	yAxisTitle = "Events"; 
	xlabel.DrawLatex(0.41, 0.05, xAxisTitle.c_str()); 
	ylabel.DrawLatex(0.05, 0.47, yAxisTitle.c_str()); 
	lab.DrawLatex(xPosCut, yPosCut, Form("Applied Cut: %s", cutApplied[itype].c_str())); 
	lab.DrawLatex(xPosFrac, yPosFrac, printer.c_str());
	c1->Print(Form("img/h1_ec_edep_inner_%s_%s.png",type[itype].c_str(),sect[isect].c_str())); 

	h1_ec_edep_outer[itype][isect] = (TH1F*) file->Get(Form("h1_ec_edep_outer_%s_%s",type[itype].c_str(),sect[isect].c_str()));
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	h1_ec_edep_outer[itype][isect]->Draw();
	printer = Form(" #rightarrow pass: %f ", (double) h1_ec_edep_outer[itype][isect]->GetEntries()/h1_nphe[0][isect]->GetEntries() );
	// Draw things 
	xAxisTitle = "Energy Deposited (GeV)"; 
	yAxisTitle = "Events"; 
	xlabel.DrawLatex(0.41, 0.05, xAxisTitle.c_str()); 
	ylabel.DrawLatex(0.05, 0.47, yAxisTitle.c_str()); 
	lab.DrawLatex(xPosFrac, yPosFrac, printer.c_str());
	lab.DrawLatex(xPosCut, yPosCut, Form("Applied Cut: %s", cutApplied[itype].c_str())); 
	c1->Print(Form("img/h1_ec_edep_outer_%s_%s.png",type[itype].c_str(),sect[isect].c_str()));

	h1_p[itype][isect]             = (TH1F*) file->Get(Form("h1_p_%s_%s",type[itype].c_str(),sect[isect].c_str()));
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	h1_p[itype][isect]->SetFillColorAlpha(62, 0.4);
	h1_p[itype][isect]->Draw();
	printer = Form(" #rightarrow pass: %f ", (double) h1_p[itype][isect]->GetEntries()/h1_nphe[0][isect]->GetEntries() );
	// Draw things 
	xAxisTitle = "P (GeV/c)"; 
	yAxisTitle = "Events"; 
	xlabel.DrawLatex(0.48, 0.05, xAxisTitle.c_str()); 
	ylabel.DrawLatex(0.05, 0.47, yAxisTitle.c_str()); 
	lab.DrawLatex(xPosCut, yPosCut, Form("Applied Cut: %s", cutApplied[itype].c_str())); 
	lab.DrawLatex(xPosFrac, yPosFrac, printer.c_str());
	c1->Print(Form("img/h1_ec_edep_inner_%s_%s.png",type[itype].c_str(),sect[isect].c_str())); 

	c1->Print(Form("img/h1_p_%s_%s.png",type[itype].c_str(),sect[isect].c_str()));

	h1_z_vertex[itype][isect]      = (TH1F*) file->Get(Form("h1_z_vertex_%s_%s",type[itype].c_str(),sect[isect].c_str()));
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	h1_z_vertex[itype][isect]->SetFillColorAlpha(55, 0.4);
	h1_z_vertex[itype][isect]->Draw();
	printer = Form(" #rightarrow pass: %f ", (double) h1_z_vertex[itype][isect]->GetEntries()/h1_nphe[0][isect]->GetEntries() );
	lab.DrawLatex(xPosFrac, yPosFrac, printer.c_str());
	lab.DrawLatex(xPosCut, yPosCut, Form("Applied Cut: %s", cutApplied[itype].c_str())); 
	c1->Print(Form("img/h1_z_vertex_%s_%s.png",type[itype].c_str(),sect[isect].c_str()));
	
	// 2d                                                                                                                                                                                                   
	h2_cc_theta[itype][isect] = (TH2F*) file->Get(Form("h_cc_theta_%s_%s",type[itype].c_str(),sect[isect].c_str()));
	h2_cc_theta[itype][isect]->Draw("colz");
	printer = Form("Pass: %.2f ", (double) h2_cc_theta[itype][isect]->GetEntries()/h1_nphe[0][isect]->GetEntries() );
	// Draw things 
	xAxisTitle = "CC Segment"; 
	yAxisTitle = "#theta_{CC}"; 
	xlabel.DrawLatex(0.48, 0.05, xAxisTitle.c_str()); 
	ylabel.DrawLatex(0.05, 0.47, yAxisTitle.c_str()); 
	lab.DrawLatex(xPosCut, yPosCut, Form("Applied Cut: %s", cutApplied[itype].c_str())); 
	lab.DrawLatex(xPosFrac, yPosFrac, printer.c_str());
	c1->Print(Form("img/h_cc_theta_%s_%s.png",type[itype].c_str(),sect[isect].c_str()));

	h2_etot_p[itype][isect]   = (TH2F*) file->Get(Form("h_etot_p_%s_%s",type[itype].c_str(),sect[isect].c_str()));
	h2_etot_p[itype][isect]->Draw("colz");
	printer = Form("Pass: %.3f ", (double) h2_etot_p[itype][isect]->GetEntries()/h1_nphe[0][isect]->GetEntries() );
	// Draw things 
	xAxisTitle = "P (GeV/c)"; 
	yAxisTitle = "E_{dep}/P"; 
	xlabel.DrawLatex(0.48, 0.05, xAxisTitle.c_str()); 
	ylabel.DrawLatex(0.05, 0.47, yAxisTitle.c_str()); 
	lab.DrawLatex(xPosCut, yPosCut, Form("Applied Cut: %s", cutApplied[itype].c_str())); 
	lab.DrawLatex(xPosFrac, yPosFrac, printer.c_str());
	c1->Print(Form("img/h_etot_p_%s_%s.png",type[itype].c_str(),sect[isect].c_str()));

	h2_ang_fid[itype][isect]  = (TH2F*) file->Get(Form("h_ang_fid_%s_%s",type[itype].c_str(),sect[isect].c_str()));
	h2_ang_fid[itype][isect]->Draw("colz");
	printer = Form("Pass: %.3f ", (double) h2_ang_fid[itype][isect]->GetEntries()/h1_nphe[0][isect]->GetEntries() );
	xAxisTitle = "#phi_{Rel} (Deg)"; 
	yAxisTitle = "#theta (Deg)"; 
	xlabel.DrawLatex(0.48, 0.05, xAxisTitle.c_str()); 
	ylabel.DrawLatex(0.05, 0.47, yAxisTitle.c_str()); 
	lab.DrawLatex(xPosFrac, yPosFrac, printer.c_str());
	lab.DrawLatex(xPosCut, yPosCut, Form("Applied Cut: %s", cutApplied[itype].c_str())); 
	c1->Print(Form("img/h_ang_fid_%s_%s.png",type[itype].c_str(),sect[isect].c_str()));

	h2_ec_edep[itype][isect]  = (TH2F*) file->Get(Form("h_ec_edep_%s_%s",type[itype].c_str(),sect[isect].c_str()));
	gPad->SetGridx();
	gPad->SetGridy();
	gPad->SetLogz(); 
	h2_ec_edep[itype][isect]->Draw("colz"); 
	printer = Form(" #rightarrow pass: %f ", (double) h2_ec_edep[itype][isect]->GetEntries()/h1_nphe[0][isect]->GetEntries() );
	xAxisTitle = "E_{Dep} Inner (GeV)"; 
	yAxisTitle = "E_{Dep} Outer (Gev)"; 
	xlabel.DrawLatex(0.48, 0.05, xAxisTitle.c_str()); 
	ylabel.DrawLatex(0.05, 0.47, yAxisTitle.c_str()); 
	lab.DrawLatex(xPosCut, yPosCut, Form("Applied Cut: %s", cutApplied[itype].c_str())); 
	lab.DrawLatex(xPosFrac, yPosFrac, printer.c_str());
	c1->Print(Form("img/h_ec_edep_%s_%s.png",type[itype].c_str(),sect[isect].c_str()));

	h2_dcr1_fid[itype][isect] = (TH2F*) file->Get(Form("h_dcr1_fid_%s_%s",type[itype].c_str(),sect[isect].c_str()));
	h2_dcr1_fid[itype][isect]->Draw("colz");
	gPad->SetGridx();
	gPad->SetGridy();
	gPad->SetLogz();
	printer = Form(" #rightarrow pass: %f ", (double) h2_dcr1_fid[itype][isect]->GetEntries()/h1_nphe[0][isect]->GetEntries() );
	xAxisTitle = "x (cm)"; 
	yAxisTitle = "y (cm)"; 
	xlabel.DrawLatex(0.48, 0.05, xAxisTitle.c_str()); 
	ylabel.DrawLatex(0.05, 0.47, yAxisTitle.c_str()); 
	lab.DrawLatex(xPosFrac, yPosFrac, printer.c_str());
	lab.DrawLatex(xPosCut, yPosCut, Form("Applied Cut: %s", cutApplied[itype].c_str())); 
	c1->Print(Form("img/h_dcr1_fid_%s_%s.png",type[itype].c_str(),sect[isect].c_str()));

	h2_dcr3_fid[itype][isect] = (TH2F*) file->Get(Form("h_dcr3_fid_%s_%s",type[itype].c_str(),sect[isect].c_str()));
	h2_dcr3_fid[itype][isect]->Draw("colz");
	gPad->SetGridx();
	gPad->SetGridy();
	gPad->SetLogz();
	printer = Form(" #rightarrow pass: %f ", (double) h2_dcr3_fid[itype][isect]->GetEntries()/h1_nphe[0][isect]->GetEntries() );
	xAxisTitle = "x (cm)"; 
	yAxisTitle = "y (cm)"; 
	xlabel.DrawLatex(0.48, 0.05, xAxisTitle.c_str()); 
	ylabel.DrawLatex(0.05, 0.47, yAxisTitle.c_str()); 
	lab.DrawLatex(xPosCut, yPosCut, Form("Applied Cut: %s", cutApplied[itype].c_str())); 
	lab.DrawLatex(xPosFrac, yPosFrac, printer.c_str());
	c1->Print(Form("img/h_dcr3_fid_%s_%s.png",type[itype].c_str(),sect[isect].c_str()));

	h2_ec_fid[itype][isect]   = (TH2F*) file->Get(Form("h_ec_fid_%s_%s",type[itype].c_str(),sect[isect].c_str()));
	h2_ec_fid[itype][isect]->Draw("colz");
	gPad->SetGridx();
	gPad->SetGridy();
	gPad->SetLogz(0);
	printer = Form(" #rightarrow pass: %f ", (double) h2_ec_fid[itype][isect]->GetEntries()/h1_nphe[0][isect]->GetEntries() );
	xAxisTitle = "x (cm)"; 
	yAxisTitle = "y (cm)"; 
	xlabel.DrawLatex(0.48, 0.05, xAxisTitle.c_str()); 
	ylabel.DrawLatex(0.05, 0.47, yAxisTitle.c_str()); 
	lab.DrawLatex(xPosCut, yPosCut, Form("Applied Cut: %s", cutApplied[itype].c_str())); 
	lab.DrawLatex(xPosFrac, yPosFrac, printer.c_str());
	c1->Print(Form("img/h_ec_fid_%s_%s.png",type[itype].c_str(),sect[isect].c_str()));
      }

  // ------------ end histograms ---------------  

}
