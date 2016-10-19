extern"C"{
  double brasse_(double *beamEnergy, double *qq, double *w);
}

void loadBrasseModel(TH2D *model, const int numberQQBins, const int numberWBins, const double qqMin, const double qqMax, const double wMin, const double wMax, double beamEnergy){
  for (int qqBin=0; qqBin< numberQQBins; qqBin++){
    for (int wBin=0; wBin< numberWBins; wBin++){
      double qq = qqMin + qqBin*(qqMax-qqMin)/numberQQBins; 
      double w = wMin + wBin*(wMax-wMin)/numberWBins; 
      double value = brasse_(&beamEnergy, &qq, &w);
      model->SetBinContent(qqBin+1, wBin+1, value);
    }
  }
}
