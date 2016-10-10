{
  
  gROOT->LoadMacro("calculate.C");
  gROOT->LoadMacro("plot.C");
  gROOT->LoadMacro("read.C");
  gROOT->LoadMacro("save.C");

  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);

  TFile * inputFile = TFile::Open("disHistogramsPass1.root"); 
  
  const int numberSectors = 7; 
  const int numberQQBins = 40; 
  
  TH2D *dataEvents_xByQQ[numberSectors]; 
  TH2D *dataEvents_wByQQ[numberSectors]; 
  TH2D *recEventsRad_xByQQ[numberSectors]; 
  TH2D *recEventsRad_wByQQ[numberSectors]; 
  TH2D *genEventsRad_xByQQ[numberSectors]; 
  TH2D *genEventsRad_wByQQ[numberSectors]; 
  TH2D *acceptanceRad_xByQQ[numberSectors];
  TH2D *acceptanceRad_wByQQ[numberSectors];

  TH1D *acceptanceRad_xIntegrated[numberSectors];
  TH1D *acceptanceRad_wIntegrated[numberSectors];

  TH1D *dataEvents_xByQQ_slices[numberSectors][numberQQBins];
  TH1D *dataEvents_wByQQ_slices[numberSectors][numberQQBins];
  TH1D *recEventsRad_xByQQ_slices[numberSectors][numberQQBins];
  TH1D *recEventsRad_wByQQ_slices[numberSectors][numberQQBins];
  TH1D *genEventsRad_xByQQ_slices[numberSectors][numberQQBins];
  TH1D *genEventsRad_wByQQ_slices[numberSectors][numberQQBins];
  TH1D *acceptanceRad_xByQQ_slices[numberSectors][numberQQBins];
  TH1D *acceptanceRad_wByQQ_slices[numberSectors][numberQQBins];

  readEvents(dataEvents_xByQQ, inputFile, numberSectors, 100);
  readEvents(dataEvents_wByQQ, inputFile, numberSectors, 101);
  readEvents(recEventsRad_xByQQ, inputFile, numberSectors, 200);
  readEvents(recEventsRad_wByQQ, inputFile, numberSectors, 201);
  readEvents(genEventsRad_xByQQ, inputFile, numberSectors, 300);
  readEvents(genEventsRad_wByQQ, inputFile, numberSectors, 301);

  for (int s=0; s< numberSectors; s++){
    dataEvents_xByQQ[s]->Rebin2D(5,5);
    dataEvents_wByQQ[s]->Rebin2D(5,5);
    recEventsRad_xByQQ[s]->Rebin2D(5,5);
    recEventsRad_wByQQ[s]->Rebin2D(5,5);
    genEventsRad_xByQQ[s]->Rebin2D(5,5);
    genEventsRad_wByQQ[s]->Rebin2D(5,5);
  }

  makeEventProjections(dataEvents_xByQQ, dataEvents_xByQQ_slices, numberSectors, numberQQBins, 100);
  makeEventProjections(dataEvents_wByQQ, dataEvents_wByQQ_slices, numberSectors, numberQQBins, 101);
  makeEventProjections(recEventsRad_xByQQ, recEventsRad_xByQQ_slices, numberSectors, numberQQBins, 200);
  makeEventProjections(recEventsRad_wByQQ, recEventsRad_wByQQ_slices, numberSectors, numberQQBins, 201);
  makeEventProjections(genEventsRad_xByQQ, genEventsRad_xByQQ_slices, numberSectors, numberQQBins, 300);
  makeEventProjections(genEventsRad_wByQQ, genEventsRad_wByQQ_slices, numberSectors, numberQQBins, 301);

  calculateAcceptance(recEventsRad_xByQQ, genEventsRad_xByQQ, acceptanceRad_xByQQ, numberSectors, 500);
  calculateAcceptance(recEventsRad_wByQQ, genEventsRad_wByQQ, acceptanceRad_wByQQ, numberSectors, 501);

  calculateAcceptanceSlices(recEventsRad_xByQQ_slices, genEventsRad_xByQQ_slices, acceptanceRad_xByQQ_slices, numberSectors, numberQQBins, 500);
  calculateAcceptanceSlices(recEventsRad_wByQQ_slices, genEventsRad_wByQQ_slices, acceptanceRad_wByQQ_slices, numberSectors, numberQQBins, 501);

  makeQQIntegratedProjections(acceptanceRad_xByQQ, acceptanceRad_xIntegrated, numberSectors, numberQQBins, 500);
  makeQQIntegratedProjections(acceptanceRad_wByQQ, acceptanceRad_wIntegrated, numberSectors, numberQQBins, 501);

  plotHistogram(acceptanceRad_xByQQ, numberSectors, 500);
  plotHistogram(acceptanceRad_wByQQ, numberSectors, 501);
  plotHistogram(acceptanceRad_xIntegrated, numberSectors, 500);
  plotHistogram(acceptanceRad_wIntegrated, numberSectors, 501);
  plotProjections(acceptanceRad_xByQQ_slices, numberSectors, numberQQBins, 0, 500);
  plotProjections(acceptanceRad_wByQQ_slices, numberSectors, numberQQBins, 0, 501);

  plotProjectionWithoutSave(acceptanceRad_xByQQ_slices, numberSectors, numberQQBins, 2, 10);

}
