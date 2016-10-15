{
  gROOT->LoadMacro("utils.C");

  const int numberSector = 7;
  TFile * inputFile = TFile::Open("../out/purityResultPass1.root");

  TH1D *purity[numberSector];

  readIntegratedHisto(purity, inputFile, numberSector, "purity200Bins_xByQQ");

}
