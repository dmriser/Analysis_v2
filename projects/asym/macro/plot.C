{

  TFile *f = TFile::Open("../out/asym_refactor_test.root"); 

  const int nx = 4; 
  const int nz = 4; 

  TH1D *asym[nx][nz]; 
  TF1  *fit[nx][nz];

  float gap = 0.05; 
  float xPadStart = 0.1; 
  float xPadStop  = 0.9; 
  float yPadStart = 0.1; 
  float yPadStop  = 0.9; 
  float padWidth  = ((xPadStop-xPadStart) - nx*gap)/nx; 
  float padHeight = ((yPadStop-yPadStart) - nz*gap)/nz; 

  TPad *pad[nx][nz];

  TCanvas *can = new TCanvas("can","",4,4,1000,1000); 
  
  for(int i=0; i<nx; i++){
     for(int j=0; j<nz; j++){
       asym[i][j] = (TH1D*) f->Get(Form("asym/h1_asym_kp_sect0_x%d_z%d_pt0_test",i+1,j+1)); 
       fit[i][j] = (TF1*) f->Get(Form("fit/fit_asym_kp_sect0_x%d_z%d_pt0_test",i+1,j+1)); 

       //       pad[i][j] = new TPad(Form("pad%d%d",i,j), "", xPadStart+i*padWidth+i*gap, yPadStart+j*padHeight+j*gap,
       //			    xPadStart+(i+1)*padWidth+i*gap, yPadStart+(j+1)*padHeight+j*gap); 
     } 
  }

  pad[0][0] = new TPad("pad00", "", 0.1, 0.1, 0.25, 0.25); 
  pad[1][0] = new TPad("pad10", "", 0.3, 0.1, 0.45, 0.25); 
  pad[2][0] = new TPad("pad20", "", 0.5, 0.1, 0.65, 0.25); 
  pad[3][0] = new TPad("pad30", "", 0.7, 0.1, 0.85, 0.25); 
  pad[0][1] = new TPad("pad01", "", 0.1, 0.1, 0.25, 0.25); 
  pad[1][1] = new TPad("pad11", "", 0.1, 0.3, 0.25, 0.45); 
  pad[2][1] = new TPad("pad21", "", 0.1, 0.5, 0.25, 0.65); 
  pad[3][1] = new TPad("pad31", "", 0.1, 0.7, 0.25, 0.85); 
  pad[0][2] = new TPad("pad02", "", 0.1, 0.25, 0.25, 0.45); 
  pad[1][2] = new TPad("pad12", "", 0.3, 0.25, 0.45, 0.45); 
  pad[2][2] = new TPad("pad22", "", 0.5, 0.25, 0.65, 0.45); 
  pad[3][2] = new TPad("pad32", "", 0.7, 0.25, 0.85, 0.45); 
  pad[0][3] = new TPad("pad03", "", 0.3, 0.1, 0.25, 0.25); 
  pad[1][3] = new TPad("pad13", "", 0.3, 0.1, 0.25, 0.25); 
  pad[2][3] = new TPad("pad23", "", 0.3, 0.1, 0.25, 0.25); 
  pad[3][3] = new TPad("pad33", "", 0.3, 0.1, 0.25, 0.25); 

  gStyle->SetErrorX(0.0);


  can->cd();
  for(int i=0; i<nx; i++){
    for(int j=0; j<nz; j++){
      pad[i][j]->Draw(); 
      pad[i][j]->cd();       

      //      asym[i][j]->SetLineColor(kBlack);
      //      asym[i][j]->SetLineWidth(2);
      //      asym[i][j]->SetMarkerStyle(25);
      //      asym[i][j]->SetMarkerSize(2); 
      //      asym[i][j]->SetMarkerColor(kBlack);
      asym[i][j]->Draw("pe"); 
      
      //      fit[i][j]->SetLineColor(kBlack);
      //      fit[i][j]->SetLineWidth(2);  
      fit[i][j]->Draw("same");
    }
  }

}
