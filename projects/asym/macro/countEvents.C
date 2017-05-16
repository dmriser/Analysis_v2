{

  gROOT->LoadMacro("loadHistos.C"); 

  TFile *f = TFile::Open("../out/asym_refactor_test.root");
  int eventThreshold = 5000; 

  std::vector<TH1D*> kp_phi = loadHistos(f, "phi/h1_phi_kp");
  std::vector<TH1D*> km_phi = loadHistos(f, "phi/h1_phi_km");

  for (i=0; i<kp_phi.size(); i++){
    if (kp_phi[i]->GetEntries() > eventThreshold){ 
      std::cout << kp_phi[i]->GetName() << " Entries = " << kp_phi[i]->GetEntries() << std::endl; 
    }
  }

  for (i=0; i<km_phi.size(); i++){
    if (km_phi[i]->GetEntries() > eventThreshold){ 
      std::cout << km_phi[i]->GetName() << " Entries = " << km_phi[i]->GetEntries() << std::endl; 
    }
  }



}
