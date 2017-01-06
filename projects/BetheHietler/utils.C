void draw(int type, int style){

  // type 
  // [0] - x
  // [1] - y
  // [2] - p
  // [3] - w
  // [4] - qq
  // [5] - theta
  // [6] - phi
  // [7] - nu 

  // style 
  // [0] - Draw together the x plots for different W 
  // [1] - Draw together the x plots for different config

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
  titlePosX  = 0.32;   titlePosY = 0.92;

  yLabelString = "Probability";
  titleString  = Form(" %s, N_{events} = %.1E ",configName[config].c_str(),
		      x[config]->GetEntries());

  TCanvas *c = new TCanvas("c","",800,800);
  c->Divide(); 

  if (type == 0){
    if (style == 0){
      normalize(x[0][0], x[0][1], x[0][2]);
      x[0][0]->Draw("hist");
      x[0][1]->Draw("histsame");
      x[0][2]->Draw("histsame");
    }

    else if (style == 1){
      normalize(x[0][0], x[0][1], x[0][2]);
      x[0][0]->Draw("hist");
      x[1][0]->Draw("histsame");
      x[2][0]->Draw("histsame");
    }
  }

  else if(type == 1){
    
  }
  
  xLabel.DrawLatex(xLabelPosX, xLabelPosY, xLabelString.c_str());
  yLabel.DrawLatex(yLabelPosX, yLabelPosY, yLabelString.c_str());
  title .DrawLatex(titlePosX,   titlePosY, titleString.c_str());
  
  gPad->SetMargin(0.15,0.15,0.15,0.15);
  gPad->SetGridx();
  gPad->SetGridy();
  

}

void normalize(TH1D *h1, TH1D *h2){
  h1->Scale(1.0/h1->Integral());
  h2->Scale(1.0/h2->Integral());
  double max = h1->GetMaximum(); 
  if (h2->GetMaximum() > max){ max = h2->GetMaximum(); }
  h1->SetMaximum(1.1*max);
  h2->SetMaximum(1.1*max);
}

void normalize(TH1D *h1, TH1D *h2, TH1D *h3){
  h1->Scale(1.0/h1->Integral());
  h2->Scale(1.0/h2->Integral());
  h3->Scale(1.0/h3->Integral());
  double max = h1->GetMaximum(); 
  if (h2->GetMaximum() > max){ max = h2->GetMaximum(); }
  if (h3->GetMaximum() > max){ max = h3->GetMaximum(); }
  h1->SetMaximum(1.1*max);
  h2->SetMaximum(1.1*max);
  h3->SetMaximum(1.1*max);
}
