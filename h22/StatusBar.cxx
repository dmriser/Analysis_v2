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
  //  stopwatch.Start(); 

  fWidth = 40; 
  fLeft = '[';
  fRight = ']';
  fWhiteSpace = ' ';
  fFiller = '=';

}

StatusBar::~StatusBar(){
  //  cout << "\033[49m" << endl; 
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
  if (doneFraction > 0.001){
    cout << "[StatusBar] "; 
    cout << fLeft; 

    int fillNumber = floor(doneFraction*fWidth); 
    int currentChar = 0; 
    while (currentChar < fWidth){
      if (currentChar < fillNumber){
	cout << fFiller; 
      } else {
	cout << fWhiteSpace; 
      }

      currentChar++; 
    }

    cout << fRight << " "; 
    
    cout.width(6); cout << doneFraction*100 << " %, ("; 
    cout << GetTime() << "/"; 
    cout << (1-doneFraction)/doneFraction *GetTime() << ")" << endl; 
  }

}


void StatusBar::PrintStatusFancy(int done, int total){
  PrintStatus((double)done/total); 
}

void StatusBar::PrintStatusFancy(double doneFraction){
  if (doneFraction > 0.001){
    cout << "\r[StatusBar] "; 
    cout << fLeft; 

    int fillNumber = floor(doneFraction*fWidth); 
    int currentChar = 0; 
    while (currentChar < fWidth){
      if (currentChar < fillNumber){
	cout << fFiller; 
      } else {
	cout << fWhiteSpace; 
      }

      currentChar++; 
    }

    cout << fRight << " "; 
    
    cout.width(6); cout << doneFraction*100 << " %, ("; 
    cout << GetTime() << "/"; 
    cout << (1-doneFraction)/doneFraction *GetTime() << ")" << std::flush; 
  }

}


#endif
