#ifndef histo_h
#define histo_h

#include "TF1.h"
#include "TGraphErrors.h"
#include "TH1.h"
#include "TH2.h"

#include "h22Event.h"

class Histograms{
	 public:
	 Histograms(){}
	 ~Histograms(){}

	 // Define histograms/graphs here
	 void Init();
	 void Fill(h22Event event);
};

#endif
