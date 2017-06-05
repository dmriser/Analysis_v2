void plot_systematics(){
  
  const int npar = 1; 
  const int nvar = 400; 

  const int nx  = 6; 
  const int nz  = 6; 
  const int npt = 6; 
  int bins = (nx+1)*(nz+1)*(npt+1);

  TFile *file[nvar];
  TH2D  *shifts; 
  
  TFile *nominalFile = TFile::Open("/volatile/clas12/dmriser/rootFiles/asymmetry/asymmetry_pass18.root"); 
  TH1D *asymmetry_x_nom  = (TH1D*) nominalFile->Get("integrated/kp/h1_x_z1_pt0_corr"); 
  TH1D *asymmetry_z_nom  = (TH1D*) nominalFile->Get("integrated/kp/h1_z_pt1_x0_corr"); 
  TH1D *asymmetry_pt_nom = (TH1D*) nominalFile->Get("integrated/kp/h1_pt_x1_z0_corr"); 

  TH1D *asymmetry_x[nvar];
  TH1D *asymmetry_z[nvar];
  TH1D *asymmetry_pt[nvar];

  shifts  = new TH2D("par0","",100,-0.05, 0.06,nx+nz+npt,1,nx+nz+npt); 


  std::ofstream outputCsv; 
  outputCsv.open("errors.csv"); 

  std::outputCsv << "FILE, A, dA, STAT" << std::endl; 
  for (int f=0; f<nvar; f++){
    std::string title(Form("/volatile/clas12/dmriser/rootFiles/asymmetry/systematic/random_var%d.root", f));
    file[f] = TFile::Open(title.c_str()); 


    if (file[f])
      if (file[f]->IsOpen()){
	
	asymmetry_x[f]  = (TH1D*) file[f]->Get("integrated/kp/h1_x_z1_pt0_corr"); 
	asymmetry_z[f]  = (TH1D*) file[f]->Get("integrated/kp/h1_z_pt1_x0_corr"); 
	asymmetry_pt[f] = (TH1D*) file[f]->Get("integrated/kp/h1_pt_x1_z0_corr"); 
	
	double cont = asymmetry_x[f]->GetBinContent(2); 
	std::outputCsv << f << ", " <<  cont << ", " << cont-asymmetry_x_nom->GetBinContent(2) << ", " << asymmetry_x[f]->GetBinError(2) << std::endl; 
	
	for (int i=1; i<asymmetry_x[f]->GetXaxis()->GetNbins(); i++){
	  double nom = asymmetry_x_nom->GetBinContent(i); 
	  shifts->Fill(asymmetry_x[f]->GetBinContent(i), i); 
	}
	for (int i=1; i<asymmetry_z[f]->GetXaxis()->GetNbins(); i++){
	  double nom = asymmetry_z_nom->GetBinContent(i); 
	  shifts->Fill(asymmetry_z[f]->GetBinContent(i), nx+i); 
	}
	for (int i=1; i<asymmetry_pt[f]->GetXaxis()->GetNbins(); i++){
	  double nom = asymmetry_pt_nom->GetBinContent(i); 
	  shifts->Fill(asymmetry_pt[f]->GetBinContent(i), nx+nz+i); 
	}
	
	file[f]->Close();       
      }
  }
  
  outputCsv.close(); 

  shifts->Draw("colz");
}
