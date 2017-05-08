{

  gROOT->LoadMacro("utils.C"); 

  // -----------------------------------------
  //     User Information 
  // -----------------------------------------
  TFile *file = TFile::Open("/volatile/clas12/dmriser/rootFiles/helicity/pip_check.root"); 
  

  // -----------------------------------------
  TH1D *asym = new TH1D("asym","",80,-0.05, 0.05); 
  TRegexp reg("3[0-9][0-9][0-9][0-9]"); 

  std::vector<TF1*> fits = loadFits(file,"f_phiH");
  std::vector<int>    runs; 
  std::vector<double> values; 
  std::vector<double> errors; 
  std::vector<double> runErr; 

  std::ofstream outfile("asym.dat",std::ios::trunc);

  for (int i=0; i<fits.size(); i++){
    TString filename = fits[i]->GetName(); 
    TString runString(filename(10,5));
    int run = runString.Atoi(); 

    std::outfile.width(8);  std::outfile << run;
    std::outfile.width(16); std::outfile << fits[i]->GetParameter(0); 
    std::outfile.width(16); std::outfile << fits[i]->GetParError(0) << std::endl; 

    runs.push_back(run);
    runErr.push_back(0.0); 
    values.push_back(fits[i]->GetParameter(0)); 
    errors.push_back(fits[i]->GetParError(0)); 

    asym->Fill(fits[i]->GetParameter(0)); 
  }

  outfile.close();

  asym->Draw(); 
  
}
