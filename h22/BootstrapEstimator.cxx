#ifndef BOOTSTRAP_CXX
#define BOOTSTRAP_CXX

#include "BootstrapEstimator.h"

#include "TF1.h"
#include "TH1.h"
#include "TRandom3.h"

void BootstrapEstimator::Fit(TH1F *sourceHisto){
  
  for(int rep = 0; rep < fNumberReplicas; rep++){
    
    TH1F *currentRep = (TH1F*) sourceHisto->Clone(); 
    for (int bin=1; bin <= currentRep->GetXaxis()->GetNbins(); bin++){
      currentRep->SetBinContent(bin, fRandom->Gaus(sourceHisto->GetBinContent(bin), 
						   sourceHisto->GetBinError(bin)));
    }

    fReplicas.push_back(currentRep);
  }

}

#endif
