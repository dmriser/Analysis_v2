{

  gROOT->LoadMacro("utils.C");
  
  TFile *inputFile = TFile::Open("MCInelastic.root");

  const int numberSectors = 6; 
  const int numberSlices  = 25;

  TH1D *slices[numberSectors][numberSlices];
  
  loadSlices(inputFile, slices, numberSectors, numberSlices, "ecSlices");
  printSlices(slices, numberSectors, numberSlices, 0);
}
