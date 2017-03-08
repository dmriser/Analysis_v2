#ifndef status_bar_cxx
#define status_bar_cxx

#include <iostream>
using std::cout; 
using std::endl; 

#include "StatusBar.h"
#include "TString.h"
#include "TStopwatch.h"
#include "CommonTools.h"

StatusBar::StatusBar(){
  stopwatch.Start(); 
}

StatusBar::~StatusBar(){
  cout << "\033[49m" << endl; 
}

double StatusBar::GetTime(){
  double time = stopwatch.RealTime(); 
  stopwatch.Continue(); 
  return time; 
}

void StatusBar::PrintStatus(int done, int total){
  PrintStatus((double)done/total); 
}

void StatusBar::PrintStatus(double doneFraction){
  cout << "[StatusBar::PrintStatus] Finished "; 
  cout.width(10); cout << doneFraction*100 << " %, Time Elapsed = "; 
  cout.width(10); cout << secondsToTimeString(GetTime()) << " Time left = "; 
  cout.width(10); cout << secondsToTimeString((1-doneFraction)/doneFraction *GetTime()) <<  endl; 
}


#endif
