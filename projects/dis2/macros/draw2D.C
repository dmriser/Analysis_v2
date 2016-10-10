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
  TH2D *recAndGenEventsRad_xByQQ[numberSectors]; 
  TH2D *recAndGenEventsRad_wByQQ[numberSectors]; 
  TH2D *acceptanceRad_xByQQ[numberSectors];
  TH2D *acceptanceRad_wByQQ[numberSectors];
  TH1D *acceptanceRad_xIntegrated[numberSectors];
  TH1D *acceptanceRad_wIntegrated[numberSectors];
  TH1D *dataEvents_xByQQ_slices[numberSectors][numberQQBins];
  TH1D *dataEvents_wByQQ_slices[numberSectors][numberQQBins];

  readEvents(dataEvents_xByQQ, inputFile, numberSectors, 100);
  readEvents(dataEvents_wByQQ, inputFile, numberSectors, 101);
  readEvents(recEventsRad_xByQQ, inputFile, numberSectors, 200);
  readEvents(recEventsRad_wByQQ, inputFile, numberSectors, 201);
  readEvents(genEventsRad_xByQQ, inputFile, numberSectors, 300);
  readEvents(genEventsRad_wByQQ, inputFile, numberSectors, 301);
  readEvents(recAndGenEventsRad_xByQQ, inputFile, numberSectors, 400);
  readEvents(recAndGenEventsRad_wByQQ, inputFile, numberSectors, 401);

  calculateAcceptance(recEventsRad_xByQQ, genEventsRad_xByQQ, acceptanceRad_xByQQ, numberSectors, 500);
  calculateAcceptance(recEventsRad_wByQQ, genEventsRad_wByQQ, acceptanceRad_wByQQ, numberSectors, 501);

  plotHistogram(dataEvents_xByQQ, numberSectors, 100);
  plotHistogram(dataEvents_wByQQ, numberSectors, 101);
  plotHistogram(recEventsRad_xByQQ, numberSectors, 200);
  plotHistogram(recEventsRad_wByQQ, numberSectors, 201);
  plotHistogram(genEventsRad_xByQQ, numberSectors, 300);
  plotHistogram(genEventsRad_wByQQ, numberSectors, 301);
  plotHistogram(recAndGenEventsRad_xByQQ, numberSectors, 400);
  plotHistogram(recAndGenEventsRad_wByQQ, numberSectors, 401);
  plotHistogram(acceptanceRad_xByQQ, numberSectors, 500);
  plotHistogram(acceptanceRad_wByQQ, numberSectors, 501);
}
