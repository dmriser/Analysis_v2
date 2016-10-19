{

  gROOT->LoadMacro("loadBrasseModel.C");
  
  const int numberQQBins = 100; 
  const int numberWBins = 100; 

  const double qqMin = 1.0;
  const double qqMax = 5.0;
  const double wMin = 1.1; 
  const double wMax = 2.9;

  double beamEnergy = 5.75; 

  TH2D *model = new TH2D("model","",numberQQBins, qqMin, qqMax, numberWBins, wMin, wMax);
  loadBrasseModel(model, numberQQBins, numberWBins, qqMin, qqMax, wMin, wMax, beamEnergy);

  TCanvas *can = new TCanvas("can","",800,800);
  model->Draw("colz");


}
