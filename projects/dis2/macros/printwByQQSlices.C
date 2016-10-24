{

  gROOT->LoadMacro("utils.C");

  const int numberSector = 7;
  const int numberSlices = 40;

  TH1D *data[numberSector][numberSlices];
  TH1D *rec[numberSector][numberSlices];
  TH1D *gen[numberSector][numberSlices];
  TH1D *acceptance[numberSector][numberSlices];
  TH1D *crossSection[numberSector][numberSlices];
  TH1D *crossSectionRatio[numberSector][numberSlices];
  TH1D *binCorrection[numberSector][numberSlices];
  TH1D *binCorrectedCrossSection[numberSector][numberSlices];
  TH1D *binCorrectedCrossSectionRatio[numberSector][numberSlices];


  TFile *inputFile = TFile::Open("../out/crossSectionWithResonancePass1.root");
  
  readHisto(data, inputFile, numberSector, numberSlices, "dataEvents_wByQQ");
  readHisto(rec, inputFile, numberSector, numberSlices, "recEventsRad_wByQQ");
  readHisto(gen, inputFile, numberSector, numberSlices, "genEventsRad_wByQQ");
  readHisto(acceptance, inputFile, numberSector, numberSlices, "acceptance_wByQQ");
  readHisto(crossSection, inputFile, numberSector, numberSlices, "crossSection_wByQQ");
  readHisto(crossSectionRatio, inputFile, numberSector, numberSlices, "crossSectionRatio_wByQQ");
  readHisto(binCorrection, inputFile, numberSector, numberSlices, "binCenterCorrection_wByQQ");
  readHisto(binCorrectedCrossSection, inputFile, numberSector, numberSlices, "binCorrectedCrossSection_wByQQ");
  readHisto(binCorrectedCrossSectionRatio, inputFile, numberSector, numberSlices, "binCorrectedCrossSectionRatio_wByQQ");
  
  printSlices(data, numberSector, numberSlices, 2, 1);
  printSlices(rec, numberSector, numberSlices, 2, 2);
  printSlices(gen, numberSector, numberSlices, 2, 3);
  printSlices(acceptance, numberSector, numberSlices, 2, 4);
  printSlices(binCorrectedCrossSection, numberSector, numberSlices, 2, 5);
  printSlices(binCorrectedCrossSectionRatio, numberSector, numberSlices, 2, 6);

}
