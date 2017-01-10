{

  gROOT->LoadMacro("utils.C");

  //  TFile * inputFile = TFile::Open("../out/crossSectionWithResonancePass1.root");
  TFile * inputFile = TFile::Open("../out/xs/xs_y0.81.root");

  const int numberSector = 7;
  const int numberSlices = 20;

  TH1D *data[numberSector][numberSlices];
  TH1D *model[numberSector][numberSlices];

  readHisto(data,inputFile,numberSector,numberSlices,"binCorrectedCrossSection_wByQQ");
  readHisto(model,inputFile,numberSector,numberSlices,"modelCrossSection_wByQQ");
 
  plot2Histos(data,model,numberSector,numberSlices,2);

}
