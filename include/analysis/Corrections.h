////////////////////////////////////////////////
/*
 
 Corrections.h -> Class that holds the hard work
 other people did.  (Nathan H. & Marco)
 
 Contains:
 - Z-Vertex Corrections (Nathan@JLAB)
 - SC Time Corrections for Electron and Hadronic (Nathan@JLAB)
 - Electron Momentum Corrections (Marco@INFN)
 */
////////////////////////////////////////////////

#ifndef CORR_H
#define CORR_H

// my includes
#include "h22Event.h"

namespace vertex {
  struct parameters {
    static float rotationVector[3][6]; 
  };
};

class Corrections {

public:
    Corrections();
    ~Corrections();
    
    // Member Functions
    static void correctEvent(h22Event *event, int runno, int GSIM);
    static double vz(h22Event &event,int,int);           //! event, ipart, runno, GSIM 
    static double electron_sct(h22Event &event,int,int,int);
    static double hadron_sct(h22Event &event,int,int,int);
    static bool good_sc_paddle(h22Event &event,int);         //! event, ipart
};

#endif
