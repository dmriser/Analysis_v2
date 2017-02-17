{
  // ----------------------------------------
  //  User Parameters 
  // ----------------------------------------
  TFile *inputFile = TFile::Open("../out/sectorFix.root");
  string imagePath = "/volatile/clas12/dmriser/plots/elastic/";

  const int thetaBins = 40;  
  const int phiBins   = 3; 

  // ----------------------------------------
  //  Other Things
  // ----------------------------------------
  THnSparseI *events = (THnSparseI*) inputFile->Get("dataEvents");
  TH2D     *thetaPhi = events->Projection(2,3); 


  TCanvas *c1 = new TCanvas("c1","",800,500);
  thetaPhi->Draw("colz");
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();


  int fontIndex = 102;
  TLatex tit, xtit, ytit;
  tit.SetNDC();
  tit.SetTextFont(fontIndex);
  tit.SetTextSize(0.05);

  xtit.SetNDC();
  xtit.SetTextFont(fontIndex);
  xtit.SetTextSize(0.05);

  ytit.SetNDC();
  ytit.SetTextFont(fontIndex);
  ytit.SetTextSize(0.05);
  ytit.SetTextAngle(90.0);

  tit.DrawLatex(0.37, 0.9, "E1-F Elastic Sample");
  xtit.DrawLatex(0.48, 0.07, "#phi_{Rel}");
  ytit.DrawLatex(0.07, 0.48, "#theta_{e'}");

  c1->Print(Form("%sthetaPhiSector0.png",imagePath.c_str()));

  TCanvas *c2 = new TCanvas("c2","",800,500);
  thetaPhi->Draw("colz");
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();

  TLine *xLines[phiBins+1], *yLines[thetaBins+1];
  double xMin   = events->GetAxis(2)->GetBinLowEdge(1);
  double xMax   = events->GetAxis(2)->GetBinUpEdge( events->GetAxis(2)->GetNbins() );
  double yMin   = events->GetAxis(3)->GetBinLowEdge(1);
  double yMax   = events->GetAxis(3)->GetBinUpEdge( events->GetAxis(3)->GetNbins() );
  double xWidth = (xMax-xMin)/phiBins; 
  double yWidth = (yMax-yMin)/thetaBins; 

  cout << "xMin = " << xMin << endl; 
  cout << "xMax = " << xMax << endl; 
  cout << "yMin = " << yMin << endl; 
  cout << "yMax = " << yMax << endl; 
  cout << "xWidth = " << xWidth << endl; 
  cout << "yWidth = " << yWidth << endl; 

  for(int xbin=0; xbin<phiBins; xbin++){
    xLines[xbin] = new TLine(xbin*xWidth+xMin, yMin, xbin*xWidth+xMin, yMax);
    xLines[xbin]->SetLineWidth(2);
    xLines[xbin]->SetLineColor(kBlack);
    xLines[xbin]->Draw();
  }

  for(int ybin=0; ybin<thetaBins; ybin++){
    yLines[ybin] = new TLine(xMin, yMin+ybin*yWidth, xMax, yMin+ybin*yWidth);
    yLines[ybin]->Draw();
  }

  tit.DrawLatex(0.37, 0.9, "E1-F Elastic Sample");
  xtit.DrawLatex(0.48, 0.07, "#phi_{Rel}");
  ytit.DrawLatex(0.07, 0.48, "#theta_{e'}");

  c2->Print(Form("%sthetaPhiSector0Binned.png",imagePath.c_str()));


}
