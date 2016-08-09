////////////////////////////////////////////
/*

  David Riser, University of Connecticut 
  August 9, 2016 

  Dumper.h ->  Class Defs for dumping classes 
  used for rapid debugging. 

*/
////////////////////////////////////////////

#ifndef dumper_h
#define dumper_h

// c++ includes
#include <iostream>

// root includes
#include "TH1.h"

class HistogramDumper
{
 public:
  HistogramDumper();
  ~HistogramDumper();

  // Datatypes
 private:
  int pWidth, pHeight;
  double y_bin_size; 
  
  // Member functions
 public:
  void dump(TH1F *);
  
};

#endif
