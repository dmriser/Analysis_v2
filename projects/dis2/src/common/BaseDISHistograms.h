
// c++ 
#include <iostream>

// me 
#include "DBins.h"
#include "PhysicsEvent.h"

// root 
#include "TH2.h"

#ifndef base_dis_histograms_h
#define base_dis_histograms_h

class BaseDISHistograms{
 public:
  BaseDISHistograms();
  ~BaseDISHistograms();

  const int numberOfXBins = 200;
  const int numberOfQQBins = 200; 
  const int numberOfWBins = 200;

  const double xMin  = 0.1;  const double xMax  = 0.6; 
  const double qqMin = 1.0;  const double qqMax = 4.8;
  const double wMin  = 2.05; const double wMax  = 3.1; 
  
  TH2I * xByQQ[7]; 
  TH2I * wByQQ[7]; 

  void Init(std::string name, std::string title);
  void Load(std::string filename, std::string name); 
  void Save(std::string filename, std::string option);
  void Fill(PhysicsEvent event, int sector); 
  void Rebin2D(int xFactor, int yFactor);
  
  // These were added for recAndGen coincident in the same bin,
  // there should be a better way to accomplish this.  Keep thinkin.
  void FillxByQQ(double x, double qq, int sector);
  void FillwByQQ(double w, double qq, int sector);
  
  //ClassDef(BaseDISHistograms, 1); 
};

#endif
