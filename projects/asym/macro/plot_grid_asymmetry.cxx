void plot_grid_asymmetry(std::string file){
  // --------------------------------------------
  //    user parameters 
  // --------------------------------------------
  TFile *inputFile      = TFile::Open(file.c_str()); 
  std::string imagePath = "/volatile/clas12/dmriser/plots/asymmetry";

  const int numberXBins  = 6; 
  const int numberZBins  = 6; 
  const int numberPtBins = 6; 

  // --------------------------------------------
  //    styling 
  // --------------------------------------------
  TStyle *style = new TStyle("style", "style"); 

  style->Reset("Modern"); 
  style->SetOptStat(0);
  //  style->SetOptTitle(0);
  //  style->SetOptDate(0);
  style->SetLabelSize(0.1,"xyz");
  style->SetTitleSize(0.1,"xyz");
  style->SetTitleFont(112,"xyz");
  style->SetLabelFont(112,"xyz");
  style->SetLabelSize(0.1);
  style->SetTitleOffset(1.2,"y");
  style->SetCanvasColor(0);
  style->SetCanvasBorderMode(0);
  style->SetCanvasBorderSize(0);
  style->SetPadBottomMargin(0.15);
  style->SetPadTopMargin(0.15);
  style->SetPadLeftMargin(0.15);
  style->SetPadRightMargin(0.15);
  style->SetPadGridX(0);
  style->SetPadGridY(0);
  style->SetPadTickX(1);
  style->SetPadTickY(1);
  style->SetFrameBorderMode(0);
  gROOT->SetStyle("style"); 
  
  

  // --------------------------------------------
  //     plotting 
  // --------------------------------------------
  TCanvas *can = new TCanvas("can","",800,500); 

  TLatex title, xtit, ytit; 
  title.SetNDC(); 
  title.SetTextSize(0.065); 

  xtit.SetNDC(); 
  xtit.SetTextSize(0.04); 

  ytit.SetNDC(); 
  ytit.SetTextSize(0.04); 
  ytit.SetTextAngle(90.0); 


  for(int i=0; i<numberXBins+1; i++){
    for(int j=0; j<numberZBins+1; j++){
      for(int k=0; k<numberPtBins+1; k++){

	can->Print(Form("%s/grid_x%d_z%d_pt%d_pos.png", imagePath.c_str(), i, j, k)); 
	can->Clear(); 
      }
    }
  }


}
