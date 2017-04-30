{

  TFile *f = TFile::Open("slices_fit.root"); 

  TH1D *eff = (TH1D*) f->Get("efficiency/h1_eff_slice14_sect3"); 
  TH1D *cont = (TH1D*) f->Get("contamination/h1_cont_slice14_sect3"); 
  TH1D *stat = (TH1D*) f->Get("test_statistic/h1_stat_slice14_sect3"); 

  eff->SetLineColor(99); 
  cont->SetLineColor(55); 
  stat->SetLineColor(77); 

  eff->SetMaximum(1.0); 
  eff->SetMinimum(0.0); 

  TCanvas *c1 = new TCanvas("c1","",800,500); 
  eff->Draw("l"); 
  cont->Draw("lsame");

  TCanvas *c2 = new TCanvas("c2","",800,500); 
  stat->Draw("l");

}
