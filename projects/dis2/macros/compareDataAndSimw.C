{

  gROOT->LoadMacro("utils.C");

  //  TFile * inputFile = TFile::Open("../out/crossSectionWithResonancePass1.root");
  TFile * inputFile = TFile::Open("../out/xsNewEID.root");

  const int numberSector = 7;
  const int numberSlices = 40;

  TH1D *sim[numberSector][numberSlices];
  TH1D *data[numberSector][numberSlices];

  readHisto(data,inputFile,numberSector,numberSlices,"dataEvents_wByQQ");
  readHisto(sim,inputFile,numberSector,numberSlices,"recEventsRad_wByQQ");
 
  plot2Histos(data,sim,numberSector,numberSlices,1);

}
