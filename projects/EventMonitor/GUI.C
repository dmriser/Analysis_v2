{

  const int NCONF          = 3; 
  //  string inputFile[NCONF]  = {"out/debug_jobs6_gen.root","out/debug_jobs6_rec.root","out/dataOnlyNathansElectrons.root","newCook.root"};
  //  string configName[NCONF] = {"debug_jobs6_gen","debug_jobs6_rec","data_eid","newCook"};
  string inputFile[NCONF]  = {"data.root","keppelRad.root","elastGen.root"};
  string configName[NCONF] = {"data","keppelRad","elastGen"};
  
  // Getting file and events 
  TFile      *file[NCONF];
  THnSparseI *events[NCONF];

  TH1D *x[NCONF];
  TH1D *y[NCONF];
  TH1D *p[NCONF];
  TH1D *w[NCONF];
  TH1D *qq[NCONF];
  TH1D *theta[NCONF];
  TH1D *phi[NCONF];

  TH2D *thetaPhi[NCONF];
  TH2D *thetaP[NCONF];
  TH2D *qqX[NCONF];
  TH2D *qqW[NCONF];
  TH2D *wY[NCONF];

  for (int c=0; c<NCONF; ++c){
    file[c]   = TFile::Open(inputFile[c].c_str());
    events[c] = (THnSparseI*) file[c]->Get("events");
    events[c] ->SetName(Form("events_%s",configName[c].c_str()));

    // Grab histograms 
    x[c]     = (TH1D*) events[c]->Projection(0); x[c]     ->SetName(Form("%s_x",configName[c].c_str())); 
    y[c]     = (TH1D*) events[c]->Projection(1); y[c]     ->SetName(Form("%s_y",configName[c].c_str())); 
    p[c]     = (TH1D*) events[c]->Projection(2); p[c]     ->SetName(Form("%s_p",configName[c].c_str())); 
    w[c]     = (TH1D*) events[c]->Projection(3); w[c]     ->SetName(Form("%s_w",configName[c].c_str())); 
    qq[c]    = (TH1D*) events[c]->Projection(4); qq[c]    ->SetName(Form("%s_qq",configName[c].c_str())); 
    theta[c] = (TH1D*) events[c]->Projection(5); theta[c] ->SetName(Form("%s_theta",configName[c].c_str())); 
    phi[c]   = (TH1D*) events[c]->Projection(6); phi[c]   ->SetName(Form("%s_phi",configName[c].c_str())); 
    
    thetaPhi[c] = (TH2D*) events[c]->Projection(5,6); thetaPhi[c] ->SetName(Form("%s_thetaPhi",configName[c].c_str()));
    thetaP[c]   = (TH2D*) events[c]->Projection(5,2); thetaP[c]   ->SetName(Form("%s_thetaP",configName[c].c_str()));
    qqX[c]      = (TH2D*) events[c]->Projection(4,0); qqX[c]      ->SetName(Form("%s_qqX",configName[c].c_str()));
    qqW[c]      = (TH2D*) events[c]->Projection(4,3); qqW[c]      ->SetName(Form("%s_qqW",configName[c].c_str()));
    wY[c]       = (TH2D*) events[c]->Projection(1,3); wY[c]       ->SetName(Form("%s_wY",configName[c].c_str()));
  }

  gROOT->LoadMacro("utils.C");
  //  gStyle->SetPalette(62);
  gStyle->SetOptFit(0);
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  
  // Variables that live in the CINT universe somewhere 
  int config   = 0;
  int view     = 0;      // 0 or 1, can be viewed alone(0) or together(1) 
  bool print   = false; 
  bool setLogZ = true; 

  string histogramTitles1d[7] = {"x","y","p","w","qq","theta","phi"};
  string histogramTitles2d[5] = {"thetaPhi","qqX","qqW","wY","thetaP"};
  int fillColor[5]            = {52, 99, 71, 84, 61};

  // Build and show GUI
  TControlBar *bar = new TControlBar("vertical","Event Monitor by David Riser");
  bar->AddButton("Change Conf.","changeConfig()");
  bar->AddButton("Change View.","changeView()");
  bar->AddButton("Toggle Log-Z","toggleLogZ()");
  bar->AddButton("","");
  bar->AddButton("Show x",     "draw(0)");
  bar->AddButton("Show y",     "draw(1)");
  bar->AddButton("Show p",     "draw(2)");
  bar->AddButton("Show w",     "draw(3)");
  bar->AddButton("Show Q2",    "draw(4)");
  bar->AddButton("Show theta", "draw(5)");
  bar->AddButton("Show phi",   "draw(6)");
  bar->AddButton("","");
  bar->AddButton("Show theta vs. phi", "draw2d(0)");
  bar->AddButton("Show Q2 vs. x",      "draw2d(1)");
  bar->AddButton("Show Q2 vs. w",      "draw2d(2)");
  bar->AddButton("Show y vs. w",       "draw2d(3)");
  bar->AddButton("Show theta vs. P",       "draw2d(4)");
  bar->AddButton("","");
  bar->AddButton("Print","printAll()");
  bar->AddButton("","");
  bar->AddButton("Quit",".q");
  bar->Show();

}
