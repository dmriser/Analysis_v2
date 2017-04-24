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

// c++ includes
#include <iostream>

// my includes
#include "h22Event.h"

class Corrections {

public:
    Corrections();
    ~Corrections();
    
    // Member Functions
    void correctEvent(h22Event *event, int runno, int GSIM);
    double vz(h22Event &event,int,int,int);           //! event, ipart, runno, GSIM 
    double electron_sct(h22Event &event,int,int,int);
    double hadron_sct(h22Event &event,int,int,int);
    bool good_sc_paddle(h22Event &event,int);         //! event, ipart
};

#endif
