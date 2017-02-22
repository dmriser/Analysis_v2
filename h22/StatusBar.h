#ifndef status_bar_h
#define status_bar_h

#include <iostream>
#include "TString.h"
#include "TStopwatch.h"
#include "CommonTools.h"

class StatusBar{
public:
  StatusBar();
  ~StatusBar(); 

protected:
TStopwatch stopwatch;

public:
 double GetTime(); 
 void PrintStatus(double doneFraction); 
 void PrintStatus(int done, int total);  

};

#endif
