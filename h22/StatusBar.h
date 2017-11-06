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

 double GetTime(); 
 void PrintStatus(double doneFraction); 
 void PrintStatus(int done, int total);  

 void PrintStatusFancy(double doneFraction); 
 void PrintStatusFancy(int done, int total); 

protected:
 TStopwatch stopwatch;
 char       fWhiteSpace; 
 char       fFiller; 
 char       fLeft; 
 char       fRight;
 int        fWidth; 


};

#endif
