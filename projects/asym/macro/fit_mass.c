{

  TFile *f = TFile::Open("/volatile/clas12/dmriser/rootFiles/asymmetry/pass5.root");
  TH1D  *h = (TH1D*) f->Get("pid/pp/h1_tof_mass_pp_x1_z2_pt2_test");


  TF1 *f_pp  = new TF1("f_pp", "gaus", 0.05, 0.3);
  TF1 *f_kp  = new TF1("f_kp", "gaus", 0.43, 0.6);
  TF1 *f_tot = new TF1("f_tot", "gaus(0) + gaus(3) + pol2(6)");

  h->Fit("f_pp","RQ");
  h->Fit("f_kp","RQ");

  f_tot->SetParameter(0, f_pp->GetParameter(0)); 
  f_tot->SetParameter(1, f_pp->GetParameter(1)); 
  f_tot->SetParameter(2, f_pp->GetParameter(2)); 
  f_tot->SetParameter(3, f_kp->GetParameter(0)); 
  f_tot->SetParameter(4, f_kp->GetParameter(1)); 
  f_tot->SetParameter(5, f_kp->GetParameter(2)); 
  h->Fit("f_tot","RQ");

  TF1 *f_pp_final = new TF1("f_pp_final", "gaus");
  TF1 *f_kp_final = new TF1("f_kp_final", "gaus");
  TF1 *f_bg       = new TF1("f_bg", "pol2");

  f_pp_final->SetParameter(0, f_tot->GetParameter(0)); 
  f_pp_final->SetParameter(1, f_tot->GetParameter(1)); 
  f_pp_final->SetParameter(2, f_tot->GetParameter(2)); 

  f_kp_final->SetParameter(0, f_tot->GetParameter(3)); 
  f_kp_final->SetParameter(1, f_tot->GetParameter(4)); 
  f_kp_final->SetParameter(2, f_tot->GetParameter(5)); 

  f_bg->SetParameter(0, f_tot->GetParameter(6)); 
  f_bg->SetParameter(1, f_tot->GetParameter(7)); 
  f_bg->SetParameter(2, f_tot->GetParameter(8)); 

  f_pp_final->SetLineColor(99); 
  f_kp_final->SetLineColor(77);
  f_bg      ->SetLineColor(55);
  f_tot     ->SetLineColor(kBlack); 

  f_pp_final->SetLineWidth(0.5); 
  f_kp_final->SetLineWidth(0.5);
  f_bg      ->SetLineWidth(0.5);
  f_tot     ->SetLineWidth(0.5); 



  // calculate stuff 
  double min = 0.0; 
  double cut = 0.42; 
  double max = 0.9; 

  double s_pp = f_pp_final->Integral(min, cut)/(f_pp_final->Integral(min, cut) + f_kp_final->Integral(min, cut));
  double s_kp = f_kp_final->Integral(cut, max)/(f_pp_final->Integral(cut, max) + f_kp_final->Integral(cut, max));
  double b_pp = 1-s_kp; 
  double b_kp = 1-s_pp; 

  double pp_coef_pp = b_kp/(b_pp*b_kp -s_pp*s_kp); 
  double pp_coef_kp = -1*s_pp/(b_pp*b_kp -s_pp*s_kp); 

  double kp_coef_pp = -1*b_pp/(s_pp*s_kp -b_pp*b_kp); 
  double kp_coef_kp = s_pp/(s_pp*s_kp -b_pp*b_kp); 

  std::cout << pp_coef_pp << " "; 
  std::cout << pp_coef_kp << std::endl; 

  std::cout << kp_coef_pp << " "; 
  std::cout << kp_coef_kp << std::endl; 

  h->Draw("histw"); 
  f_tot->Draw("same");
  f_pp_final->Draw("same"); 
  f_kp_final->Draw("same"); 
  f_bg      ->Draw("same"); 

  TLine line(cut, 0.0, cut, h->GetMaximum()); 
  line.SetLineColor(99); 
  line.Draw("same"); 
  
  TLatex printer; 
  printer.SetNDC(); 
  printer.SetTextSize(0.04); 
  printer.DrawLatex(0.15, 0.9, Form("S_{#pi^{+}} = %.2f, S_{K^{+}} = %.2f, B_{#pi^{+}} = %.2f, B_{K^{+}} = %.2f", s_pp, s_kp, b_pp, b_kp)); 

}
