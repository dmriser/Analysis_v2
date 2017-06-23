{

  gROOT->LoadMacro("utils.C"); 
  gStyle->SetErrorX(0.0); 

  TFile *inputFile = TFile::Open("/volatile/clas12/dmriser/rootFiles/asymmetry/asymmetry_pass24.root"); 

  const int npart            = 4; 
  std::string name[npart]    = {"pm", "pp", "km", "kp"};
  std::string texName[npart] = {"#pi^{-}", "#pi^{+}", "K^{-}", "K^{+}"};
  int color[npart]           = {99, kBlack, 77, 55}; 

  TH1D *x[npart], *q2[npart], *z[npart], *pt[npart]; 

  if (inputFile && inputFile->IsOpen()){
    for (int p=0; p<npart; p++){
      x[p]  = (TH1D*) inputFile->Get(Form("integrated/%s/h1_x_q20_z0_pt0_base", name[p].c_str())); 
      x[p]->SetLineColor(color[p]); 
      x[p]->SetLineWidth(2); 
      x[p]->SetMarkerColor(color[p]); 
      x[p]->SetMarkerStyle(8); 
      x[p]->SetMinimum(-0.07); 
      x[p]->SetMaximum(0.07); 
      x[p]->GetXaxis()->SetNdivisions(505); 
      x[p]->SetLabelSize(0.1, "x"); 
      x[p]->SetLabelSize(0.1, "y"); 

      q2[p] = (TH1D*) inputFile->Get(Form("integrated/%s/h1_q2_z0_pt0_x0_base", name[p].c_str())); 
      q2[p]->SetLineColor(color[p]); 
      q2[p]->SetMarkerColor(color[p]); 
      q2[p]->SetMarkerStyle(8); 
      q2[p]->SetMinimum(-0.07); 
      q2[p]->SetMaximum(0.07); 
      q2[p]->GetXaxis()->SetNdivisions(505); 
      q2[p]->SetLabelSize(0.1, "x"); 
      q2[p]->SetLabelSize(0.1, "y"); 

      z[p]  = (TH1D*) inputFile->Get(Form("integrated/%s/h1_z_pt0_x0_q20_base", name[p].c_str())); 
      z[p]->SetLineColor(color[p]); 
      z[p]->SetMarkerColor(color[p]); 
      z[p]->SetMarkerStyle(8); 
      z[p]->GetXaxis()->SetNdivisions(505); 
      z[p]->SetMinimum(-0.07); 
      z[p]->SetMaximum(0.07); 
      z[p]->SetLabelSize(0.1, "x"); 
      z[p]->SetLabelSize(0.1, "y"); 

      pt[p] = (TH1D*) inputFile->Get(Form("integrated/%s/h1_pt_x0_q20_z0_base", name[p].c_str())); 
      pt[p]->SetLineColor(color[p]); 
      pt[p]->SetMarkerColor(color[p]); 
      pt[p]->GetXaxis()->SetNdivisions(505); 
      pt[p]->SetMinimum(-0.07); 
      pt[p]->SetMaximum(0.07); 
      pt[p]->SetMarkerStyle(8); 
      pt[p]->SetLabelSize(0.1, "x"); 
      pt[p]->SetLabelSize(0.1, "y"); 
    }
  }

  TLine *xLine  = new TLine(x[0]->GetXaxis()->GetXmin(),  0.0, x[0]->GetXaxis()->GetXmax(),  0.0); 
  TLine *q2Line = new TLine(q2[0]->GetXaxis()->GetXmin(), 0.0, q2[0]->GetXaxis()->GetXmax(), 0.0); 
  TLine *zLine  = new TLine(z[0]->GetXaxis()->GetXmin(),  0.0, z[0]->GetXaxis()->GetXmax(),  0.0); 
  TLine *ptLine = new TLine(pt[0]->GetXaxis()->GetXmin(), 0.0, pt[0]->GetXaxis()->GetXmax(), 0.0); 

  TLine *zLeft  = new TLine(0.3, -0.07, 0.3, 0.07); 
  TLine *zRight = new TLine(0.7, -0.07, 0.7, 0.07); 

  zLeft->SetLineColor(99);
  zLeft->SetLineStyle(7); 
  zRight->SetLineColor(99);
  zRight->SetLineStyle(7); 

  xLine->SetLineColor(kBlack);
  xLine->SetLineStyle(7);  

  q2Line->SetLineColor(kBlack);
  q2Line->SetLineStyle(7);  

  zLine->SetLineColor(kBlack);
  zLine->SetLineStyle(7);  

  ptLine->SetLineColor(kBlack);
  ptLine->SetLineStyle(7);  

  TLatex lab; 
  lab.SetNDC(); 
  lab.SetTextSize(0.15); 

  TCanvas *can = new TCanvas("can", "", 800, 500);
  can->cd(); 
  gPad->SetMargin(0.15, 0.15, 0.15, 0.15); 
  
  //  can->Divide(4, 4, -1, -1); 
  can->Divide(4, 4); 

  float inner = 0.1; 
  float outer = 0.1; 

  for (int p=0; p<npart; p++){
    int index = p*npart +1; 

    can->cd(index); 
    setupPad(index, 4, 4, inner, outer); 
    x[p]->Draw("pe"); 
    xLine->Draw(); 
    if (padIsBottom(index, 4, 4)) lab.DrawLatex(0.48, 0.18, "x"); 
    lab.DrawLatex(0.75, 0.75, Form("%s",texName[p].c_str())); 

    index++;
    can->cd(index); 
    setupPad(index, 4, 4, inner, outer); 
    q2[p]->Draw("pe"); 
    q2Line->Draw(); 
    if (padIsBottom(index, 4, 4)) lab.DrawLatex(0.30, 0.18, "Q^{2} (GeV^{2}/c^{2})"); 

    index++;
    can->cd(index); 
    setupPad(index, 4, 4, inner, outer); 
    z[p]->Draw("pe"); 
    zLine->Draw("same"); 
    zLeft->Draw("same");
    zRight->Draw("same");
    if (padIsBottom(index, 4, 4)) lab.DrawLatex(0.48, 0.18, "z"); 

    index++;
    can->cd(index); 
    setupPad(index, 4, 4, inner, outer); 
    pt[p]->Draw("pe");     
    ptLine->Draw(); 
    if (padIsBottom(index, 4, 4)) lab.DrawLatex(0.34, 0.18, "P_{T} (GeV/c)"); 
  }


  can->Print("/volatile/clas12/dmriser/plots/asymmetry/integrated_all.png");
  can->Clear();
  can->Divide(4, 1, -1, -1); 

  inner = 0.15; 
  outer = 0.15;
 
  for (int p=0; p<npart; p++){
    can->cd(1); 
    x[p]->Draw("pesame"); 
    xLine->Draw(); 

    can->cd(2); 
    q2[p]->Draw("pesame"); 
    q2Line->Draw(); 

    can->cd(3); 
    z[p]->Draw("pesame"); 
    zLine->Draw("same"); 
    zLeft->Draw("same");
    zRight->Draw("same");

    can->cd(4); 
    pt[p]->Draw("pesame");     
    ptLine->Draw(); 
  }

  lab.DrawLatex(0.1, 0.98, "A_{LU}^{sin #phi_{h}}"); 

  can->Print("/volatile/clas12/dmriser/plots/asymmetry/integrated_combined_all.png");

}
