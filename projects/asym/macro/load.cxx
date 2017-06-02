#include "TF1.h"
#include "TFile.h"
#include "TH1.h"

void loadSignalBackgroundFits(TFile *file, TF1 *pion[nXBins][nZBins][nPtBins], TF1 *kaon[nXBins][nZBins][nPtBins], 
			      TF1 *background[nXBins][nZBins][nPtBins], TF1 *total[nXBins][nZBins][nPtBins], 
			      const int nXBins, const int nZBins, const int nPtBins){

  for(int i=0; i<nXBins+1; i++){
    for(int j=0; j<nZBins+1; j++){
      for(int k=0; k<nPtBins+1; k++){
	pion[i][j][k]       = (TF1*) file->Get(Form("signal_fits/h1_x_signal_p_x%d_z%d_pt%d", i, j, k)); 
	kaon[i][j][k]       = (TF1*) file->Get(Form("signal_fits/h1_x_signal_p_x%d_z%d_pt%d", i, j, k)); 
	background[i][j][k] = (TF1*) file->Get(Form("signal_fits/h1_x_signal_p_x%d_z%d_pt%d", i, j, k)); 
	total[i][j][k]      = (TF1*) file->Get(Form("signal_fits/h1_x_signal_p_x%d_z%d_pt%d", i, j, k)); 
      }
    }
  }

}


