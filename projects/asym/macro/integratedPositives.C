{
  TFile *inputFile = TFile::Open("/volatile/clas12/dmriser/rootFiles/asymmetry/asymmetry_pass20.root"); 

  const int npart         = 4; 
  std::string name[npart] = {"pm", "pp", "km", "kp"};
  int color[npart]        = {99, kBlack, 77, 55}; 

  TH1D *x[npart], *q2[npart], *z[npart], *pt[npart]; 

  if (inputFile && inputFile->IsOpen()){
    for (int p=0; p<npart; p++){
      x[p]  = (TH1D*) inputFile->Get(Form("integrated/%s/h1_x_q20_z0_pt0_base", name[p].c_str())); 
      x[p]->SetLineColor(color[p]); 
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

  xLine->SetLineColor(kBlack);
  xLine->SetLineStyle(7);  

  q2Line->SetLineColor(kBlack);
  q2Line->SetLineStyle(7);  

  zLine->SetLineColor(kBlack);
  zLine->SetLineStyle(7);  

  ptLine->SetLineColor(kBlack);
  ptLine->SetLineStyle(7);  

  TCanvas *can = new TCanvas("can", "", 800, 500);
  can->Divide(4, 4, -1, -1); 

  for (int p=0; p<npart; p++){
    can->cd(p*4+1); 
    x[p]->Draw("pe"); 
    xLine->Draw(); 

    can->cd(p*4+2); 
    q2[p]->Draw("pe"); 
    q2Line->Draw(); 

    can->cd(p*4+3); 
    z[p]->Draw("pe"); 
    zLine->Draw(); 

    can->cd(p*4+4); 
    pt[p]->Draw("pe");     
    ptLine->Draw(); 
  }
}
