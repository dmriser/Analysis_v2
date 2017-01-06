{

  const int NCONF   = 3;
  string filename[NCONF]   = {"data.root","keppelRad.root","elastGen.root"};
  string configName[NCONF] = {"data","keppelRad","elastGen"};
  int configColor[NCONF]   = {55, 75, 95};


  //  gROOT->LoadMacro("utils.C");

  TFile      *files[NCONF];
  THnSparseI *events[NCONF];
  TH1D       *x[NCONF][3];
  TH1D       *y[NCONF][3];
  TH1D       *nu[NCONF][3];
  TH1D       *qq[NCONF][3];
  TH1D       *w[NCONF][3];
  TH1D       *theta[NCONF][3];
  TH1D       *phi[NCONF][3];
  TH1D       *p[NCONF][3];
  
  TH2D       *thetaPhi[NCONF][3];
  TH2D       *thetaP[NCONF][3];
  TH2D       *qqX[NCONF][3];
  TH2D       *qqW[NCONF][3];
  TH2D       *wY[NCONF][3];
  TH2D       *yTheta[NCONF][3];

  for(int c=0; c<NCONF; c++){
    files[c]  = TFile::Open(filename[c].c_str());
    events[c] = (THnSparseI*) files[c]->Get("events");
    events[c]->SetName(Form("%s_events",configName[c].c_str()));
    events[c]->SetTitle(Form("%s_events",configName[c].c_str()));

    // Grab histograms
    x[c][0]     = (TH1D*) events[c]->Projection(0); x[c][0]     ->SetName(Form("%s_x0",configName[c].c_str()));
    y[c][0]     = (TH1D*) events[c]->Projection(1); y[c][0]     ->SetName(Form("%s_y0",configName[c].c_str()));
    p[c][0]     = (TH1D*) events[c]->Projection(2); p[c][0]     ->SetName(Form("%s_p0",configName[c].c_str()));
    w[c][0]     = (TH1D*) events[c]->Projection(3); w[c][0]     ->SetName(Form("%s_w0",configName[c].c_str()));
    qq[c][0]    = (TH1D*) events[c]->Projection(4); qq[c][0]    ->SetName(Form("%s_qq0",configName[c].c_str()));
    theta[c][0] = (TH1D*) events[c]->Projection(5); theta[c][0] ->SetName(Form("%s_theta0",configName[c].c_str()));
    phi[c][0]   = (TH1D*) events[c]->Projection(6); phi[c][0]   ->SetName(Form("%s_phi0",configName[c].c_str()));
    nu[c][0]    = (TH1D*) events[c]->Projection(7); nu[c][0]    ->SetName(Form("%s_nu0",configName[c].c_str()));

    thetaPhi[c][0] = (TH2D*) events[c]->Projection(5,6); thetaPhi[c][0] ->SetName(Form("%s_thetaPhi0",configName[c].c_str()));
    thetaP[c][0]   = (TH2D*) events[c]->Projection(5,2); thetaP[c][0]   ->SetName(Form("%s_thetaP0",configName[c].c_str()));
    qqX[c][0]      = (TH2D*) events[c]->Projection(4,0); qqX[c][0]      ->SetName(Form("%s_qqX0",configName[c].c_str()));
    qqW[c][0]      = (TH2D*) events[c]->Projection(4,3); qqW[c][0]      ->SetName(Form("%s_qqW0",configName[c].c_str()));
    wY[c][0]       = (TH2D*) events[c]->Projection(1,3); wY[c][0]       ->SetName(Form("%s_wY0",configName[c].c_str()));
    yTheta[c][0]   = (TH2D*) events[c]->Projection(1,5); yTheta[c][0]   ->SetName(Form("%s_yTheta0",configName[c].c_str()));

    // Setting for W < 2
    int cutBin = events[c]->GetAxis(3)->FindBin(2.0);
    int endBin = events[c]->GetAxis(3)->GetNbins();
    events[c]->GetAxis(3)->SetRange(1,cutBin);
    x[c][1]     = (TH1D*) events[c]->Projection(0); x[c][1]     ->SetName(Form("%s_x1",configName[c].c_str()));
    y[c][1]     = (TH1D*) events[c]->Projection(1); y[c][1]     ->SetName(Form("%s_y1",configName[c].c_str()));
    p[c][1]     = (TH1D*) events[c]->Projection(2); p[c][1]     ->SetName(Form("%s_p1",configName[c].c_str()));
    w[c][1]     = (TH1D*) events[c]->Projection(3); w[c][1]     ->SetName(Form("%s_w1",configName[c].c_str()));
    qq[c][1]    = (TH1D*) events[c]->Projection(4); qq[c][1]    ->SetName(Form("%s_qq1",configName[c].c_str()));
    theta[c][1] = (TH1D*) events[c]->Projection(5); theta[c][1] ->SetName(Form("%s_theta1",configName[c].c_str()));
    phi[c][1]   = (TH1D*) events[c]->Projection(6); phi[c][1]   ->SetName(Form("%s_phi1",configName[c].c_str()));
    nu[c][1]    = (TH1D*) events[c]->Projection(7); nu[c][1]    ->SetName(Form("%s_nu1",configName[c].c_str()));

    thetaPhi[c][1] = (TH2D*) events[c]->Projection(5,6); thetaPhi[c][1] ->SetName(Form("%s_thetaPhi1",configName[c].c_str()));
    thetaP[c][1]   = (TH2D*) events[c]->Projection(5,2); thetaP[c][1]   ->SetName(Form("%s_thetaP1",configName[c].c_str()));
    qqX[c][1]      = (TH2D*) events[c]->Projection(4,0); qqX[c][1]      ->SetName(Form("%s_qqX1",configName[c].c_str()));
    qqW[c][1]      = (TH2D*) events[c]->Projection(4,3); qqW[c][1]      ->SetName(Form("%s_qqW1",configName[c].c_str()));
    wY[c][1]       = (TH2D*) events[c]->Projection(1,3); wY[c][1]       ->SetName(Form("%s_wY1",configName[c].c_str()));
    yTheta[c][1]   = (TH2D*) events[c]->Projection(1,5); yTheta[c][1]   ->SetName(Form("%s_yTheta1",configName[c].c_str()));


    // Setting for W > 2
    events[c]->GetAxis(3)->SetRange(cutBin,endBin);
    x[c][2]     = (TH1D*) events[c]->Projection(0); x[c][2]     ->SetName(Form("%s_x2",configName[c].c_str()));
    y[c][2]     = (TH1D*) events[c]->Projection(1); y[c][2]     ->SetName(Form("%s_y2",configName[c].c_str()));
    p[c][2]     = (TH1D*) events[c]->Projection(2); p[c][2]     ->SetName(Form("%s_p2",configName[c].c_str()));
    w[c][2]     = (TH1D*) events[c]->Projection(3); w[c][2]     ->SetName(Form("%s_w2",configName[c].c_str()));
    qq[c][2]    = (TH1D*) events[c]->Projection(4); qq[c][2]    ->SetName(Form("%s_qq2",configName[c].c_str()));
    theta[c][2] = (TH1D*) events[c]->Projection(5); theta[c][2] ->SetName(Form("%s_theta2",configName[c].c_str()));
    phi[c][2]   = (TH1D*) events[c]->Projection(6); phi[c][2]   ->SetName(Form("%s_phi2",configName[c].c_str()));
    nu[c][2]    = (TH1D*) events[c]->Projection(7); nu[c][2]    ->SetName(Form("%s_nu2",configName[c].c_str()));

    thetaPhi[c][2] = (TH2D*) events[c]->Projection(5,6); thetaPhi[c][2] ->SetName(Form("%s_thetaPhi2",configName[c].c_str()));
    thetaP[c][2]   = (TH2D*) events[c]->Projection(5,2); thetaP[c][2]   ->SetName(Form("%s_thetaP2",configName[c].c_str()));
    qqX[c][2]      = (TH2D*) events[c]->Projection(4,0); qqX[c][2]      ->SetName(Form("%s_qqX2",configName[c].c_str()));
    qqW[c][2]      = (TH2D*) events[c]->Projection(4,3); qqW[c][2]      ->SetName(Form("%s_qqW2",configName[c].c_str()));
    wY[c][2]       = (TH2D*) events[c]->Projection(1,3); wY[c][2]       ->SetName(Form("%s_wY2",configName[c].c_str()));
    yTheta[c][2]   = (TH2D*) events[c]->Projection(1,5); yTheta[c][2]   ->SetName(Form("%s_yTheta2",configName[c].c_str()));

    // Return to full W Range    
    events[c]->GetAxis(3)->SetRange(1,endBin);
  }

  TControlBar *bar = new TControlBar("horizontal","David");
  bar->AddButton("",""); 
  bar->AddButton("Show x(0)","draw(0,0)");   
  bar->AddButton("Show x(1)","draw(0,1)");   
  bar->AddButton("","");   
  bar->AddButton("Quit",".q"); 
  bar->Show();

}
