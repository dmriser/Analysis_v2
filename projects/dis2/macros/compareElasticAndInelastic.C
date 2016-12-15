{

  gROOT->LoadMacro("utils.C");

  TFile * inputFile = TFile::Open("../out/xs3.root");

  const int numberSector = 1;
  const int numberSlices = 40;

  TH1D *elastic[numberSector][numberSlices];
  TH1D *inelastic[numberSector][numberSlices];
  TH1D *fraction[numberSector][numberSlices];

  readHisto(elastic,  inputFile,numberSector,numberSlices,"recEventsElastic_wByQQ");
  readHisto(inelastic,inputFile,numberSector,numberSlices,"recEventsInelastic_wByQQ");
  readHisto(fraction, inputFile,numberSector,numberSlices,"inelasticFraction_wByQQ");
 
  plot2Histos(elastic,inelastic,numberSector,numberSlices,3);
  printSlices(fraction, numberSector, numberSlices, 2, 9);

}
