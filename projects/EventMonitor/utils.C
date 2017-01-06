void changeConfig(){
  config++;
  if (config == NCONF){
    config = 0; 
  }
  cout << " >>> Config changed to " << configName[config] << endl; 
}

void changeView(){
  view++; 

  if(view == 2){ view = 0; }
  cout << " >>> View changed to " << view << endl;
}

void toggleLogZ(){
  setLogZ = !setLogZ;
}

void draw(int type){
  // type 
  // [0] - x
  // [1] - y
  // [2] - p
  // [3] - w
  // [4] - qq
  // [5] - theta
  // [6] - phi


  int color = 97;

  TLatex xLabel, yLabel, title;
  xLabel.SetNDC();
  xLabel.SetTextSize(0.04);

  yLabel.SetNDC();
  yLabel.SetTextSize(0.04);
  yLabel.SetTextAngle(90.0);

  title.SetNDC();
  title.SetTextSize(0.04);

  string xLabelString, yLabelString, titleString;
  double xLabelPosX, xLabelPosY;
  double yLabelPosX, yLabelPosY;
  double titlePosX,  titlePosY;

  xLabelPosX = 0.74;  xLabelPosY = 0.06;
  yLabelPosX = 0.045; yLabelPosY = 0.72;
  titlePosX = 0.32;    titlePosY = 0.92;

  yLabelString = "Counts";
  titleString  = Form(" %s, N_{events} = %.1e ",configName[config].c_str(),
		      x[config]->GetEntries());

  TCanvas *c = new TCanvas("c","",800,800);

  if (type == 0) {

    if (view == 0) {
      x[config]->SetFillColorAlpha(color, 0.5);
      x[config]->SetLineColor(color);
      x[config]->Draw();
    } 

    else if (view == 1){
      for (int c=0; c<NCONF; c++){
	x[c]->SetFillColorAlpha(fillColor[c], 0.5);
	x[c]->SetLineColor(fillColor[c]);
	x[c]->Draw("same");
      }
    }

    xLabelString = "x";
  }
  
  else if (type == 1) {

    if (view == 0){
      y[config]->SetFillColorAlpha(color, 0.5);
      y[config]->SetLineColor(color);
      y[config]->Draw();
    }
 
    else if (view == 1){
      for (int c=0; c<NCONF; c++){
	y[c]->SetFillColorAlpha(fillColor[c], 0.5);
	y[c]->SetLineColor(fillColor[c]);
	y[c]->Draw("same");
      }
    }
    xLabelString = "y";
  }
  
  else if (type == 2) {

    if (view == 0){
      p[config]->SetFillColorAlpha(color, 0.5);
      p[config]->SetLineColor(color);
      p[config]->Draw();
    }
 
    else if (view == 1){
      for (int c=0; c<NCONF; c++){
	p[c]->SetFillColorAlpha(fillColor[c], 0.5);
	p[c]->SetLineColor(fillColor[c]);
	p[c]->Draw("same");
      }
    }
    xLabelString = "p [GeV/c]";
  }
  
  else if (type == 3) {

    if (view == 0){
      w[config]->SetFillColorAlpha(color, 0.5);
      w[config]->SetLineColor(color);
      w[config]->Draw();
    }
 
    else if (view == 1){
      for (int c=0; c<NCONF; c++){
	w[c]->SetFillColorAlpha(fillColor[c], 0.5);
	w[c]->SetLineColor(fillColor[c]);
	w[c]->Draw("same");
      }
    }
    xLabelString = "W [GeV/c^{2}]";
  }
  
  else if (type == 4) {

    if (view == 0){
      qq[config]->SetFillColorAlpha(color, 0.5);
      qq[config]->SetLineColor(color);
      qq[config]->Draw();
    }
 
    else if (view == 1){
      for (int c=0; c<NCONF; c++){
	qq[c]->SetFillColorAlpha(fillColor[c], 0.5);
	qq[c]->SetLineColor(fillColor[c]);
	qq[c]->Draw("same");
      }
    }
    xLabelString = "Q^{2} [GeV^{2}/c^{2}]";
  }

  else if (type == 5) {

    if (view == 0){
      theta[config]->SetFillColorAlpha(color, 0.5);
      theta[config]->SetLineColor(color);
      theta[config]->Draw();
    }
 
    else if (view == 1){
      for (int c=0; c<NCONF; c++){
	theta[c]->SetFillColorAlpha(fillColor[c], 0.5);
	theta[c]->SetLineColor(fillColor[c]);
	theta[c]->Draw("same");
      }
    }
    xLabelString = "#theta [Degrees]";
  }

  else if (type == 6) {

    if (view == 0){
      phi[config]->SetFillColorAlpha(color, 0.5);
      phi[config]->SetLineColor(color);
      phi[config]->Draw();
    }
 
    else if (view == 1){
      for (int c=0; c<NCONF; c++){
	phi[c]->SetFillColorAlpha(fillColor[c], 0.5);
	phi[c]->SetLineColor(fillColor[c]);
	phi[c]->Draw("same");
      }
    }
    xLabelString = "#phi [Degrees]";
  }
  
  
    xLabel.DrawLatex(xLabelPosX, xLabelPosY, xLabelString.c_str());
    yLabel.DrawLatex(yLabelPosX, yLabelPosY, yLabelString.c_str());
    title .DrawLatex(titlePosX,   titlePosY, titleString.c_str());

    gPad->SetMargin(0.15,0.15,0.15,0.15);
    gPad->SetGridx();
    gPad->SetGridy();

    if(print){
      c->Update();
      c->SaveAs(Form("img/%s_%s.png",configName[config].c_str(),histogramTitles1d[type].c_str()));
    }

}

void draw2d(int type){
  // type 
  // [0] - theta phi
  // [1] - qqX
  // [2] - qqW
  // [3] - wY
  // [4] - theta phi

  TLatex xLabel, yLabel, title;
  xLabel.SetNDC();
  xLabel.SetTextSize(0.04);

  yLabel.SetNDC();
  yLabel.SetTextSize(0.04);
  yLabel.SetTextAngle(90.0);

  title.SetNDC();
  title.SetTextSize(0.04);

  string xLabelString, yLabelString, titleString;
  double xLabelPosX, xLabelPosY;
  double yLabelPosX, yLabelPosY;
  double titlePosX,  titlePosY;

  xLabelPosX = 0.72;  xLabelPosY = 0.06;
  yLabelPosX = 0.06;  yLabelPosY = 0.72;
  titlePosX = 0.32;    titlePosY = 0.92;

  titleString  = Form(" %s, N_{events} = %.1e ",configName[config].c_str(),
		      x[config]->GetEntries());

  TCanvas *c = new TCanvas("c","",800,800);

  if (type == 0){
    thetaPhi[config]->Draw("colz");
    xLabelString = "#phi [Deg]";
    yLabelString = "#theta [Deg]";
  } 

  else if (type == 1){
    qqX[config]->Draw("colz");
    xLabelString = "x";
    yLabelString = "Q^{2} [GeV^{2}/c^{2}]";
  } 

  else if (type == 2){
    qqW[config]->Draw("colz");
    xLabelString = "W [GeV/c^{2}]";
    yLabelString = "Q^{2} [GeV^{2}/c^{2}]";
  } 

  else if (type == 3){
    wY[config]->Draw("colz");
    xLabelString = "W [GeV/c^{2}]";
    yLabelString = "y";
  } 

  else if (type == 4){
    thetaP[config]->Draw("colz");
    xLabelString = "P [GeV/c]";
    yLabelString = "#theta [Deg]";
  } 

  xLabel.DrawLatex(xLabelPosX, xLabelPosY, xLabelString.c_str());
  yLabel.DrawLatex(yLabelPosX, yLabelPosY, yLabelString.c_str());
  title .DrawLatex(titlePosX,   titlePosY, titleString.c_str());

  gPad->SetMargin(0.15,0.15,0.15,0.15);
  if (setLogZ) { gPad->SetLogz(); }
  gPad->SetGridx();
  gPad->SetGridy();

  if(print){
    c->Update();
    c->SaveAs(Form("img/%s_%s.png",configName[config].c_str(),histogramTitles2d[type].c_str()));
  }

}

void printAll(){
  print = true;
  int oldConfig = config; 

  for (int c=0; c<NCONF; c++){
    config = c;
    for(int type=0; type<7; type++){
      draw(type);
    }
    
    for(int type=0; type<5; type++){
      draw2d(type);
    }
  }

  config = oldConfig; 
  print = false; 
}

void normalize(TH1D *h1, TH1D *h2){
  h1->Scale(h1->Integral());
  h2->Scale(h2->Integral());
  double max = h1->GetMaximum(); 
  if (h2->GetMaximum() > max){ max = h2->GetMaximum(); }
  h1->SetMaximum(1.1*max);
  h2->SetMaximum(1.1*max);
}

void normalize(TH1D *h1, TH1D *h2, TH1D *h3){
  h1->Scale(h1->Integral());
  h2->Scale(h2->Integral());
  h3->Scale(h3->Integral());
  double max = h1->GetMaximum(); 
  if (h2->GetMaximum() > max){ max = h2->GetMaximum(); }
  if (h3->GetMaximum() > max){ max = h3->GetMaximum(); }
  h1->SetMaximum(1.1*max);
  h2->SetMaximum(1.1*max);
  h3->SetMaximum(1.1*max);
}
