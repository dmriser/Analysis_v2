#include <iostream>

#include "TH2.h"

class BaseDISHistograms{
 public:
  BaseDISHistograms(std::string filename, std::string name, std::string title, bool recalc=true);
  ~BaseDISHistograms();

  TH2D * xByQQ; 
  TH2D * wByQQ; 

  void Initialize();
  void Load(std::string filename); 
  void Save(std::string filename);
  void Fill(DSelector *fillCriteria, DEvent event, int particleIndex); 
};
