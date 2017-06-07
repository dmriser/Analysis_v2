{
  gROOT->LoadMacro("utils.C"); 

  TFile *inputFile = TFile::Open("/volatile/clas12/dmriser/rootFiles/asymmetry/asymmetry_pass20.root"); 

  const int nx    = 6;
  const int nq2   = 2;
  const int nz    = 6;
  const int npt   = 6;
  const int npart = 2;   

  TH1D *x[npart][nq2+1][nz+1][npt+1], *q2[npart][nz+1][npt+1][nx+1], 
    *z[npart][npt+1][nx+1][nq2+1], *pt[npart][nx+1][nq2+1][nz+1]; 

  std::string particle[npart] = {"pp", "kp"};
  std::string name("base"); 

  for (int i=0; i<nx+1; i++){
    for (int j=0; j<nq2+1; j++){
      for (int k=0; k<nz+1; k++){
	for (int p=0; p<npart; p++){
	  loadIntegratedHisto(inputFile, pt[p][i][j][k], particle[p], name, 4, i, j, k); 

	  std::cout << pt[p][i][j][k]->GetEntries() << std::endl;
	}
      }
    }
  }



}
