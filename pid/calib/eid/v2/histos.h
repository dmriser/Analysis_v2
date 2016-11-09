#ifndef histo_h
#define histo_h

#include "TF1.h"
#include "TGraphErrors.h"
#include "TH1.h"
#include "TH2.h"

#include "h22Event.h"

class Histograms{
 public:
  Histograms(){ Init(); }
  ~Histograms(){}
  
  // Define histograms/graphs here
  TH2I *h2_ecSampling[7]; 
  TH2I *h2_ccThetaSegment[7]; 

  TH1I *h1_ecu[7];
  TH1I *h1_ecv[7];
  TH1I *h1_ecw[7];

  void Init();
  void Fill(h22Event event, int ipart);
  void Draw();
};

#endif
