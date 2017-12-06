/****************************************************************** 
 *     David Riser, University of Connecticut
 *     Dec 5, 2016 
 *     
 *     Generic analysis code to be extended by specific analysis 
 *     packages.  
 *
 ********************************************************************/

#ifndef generic_analysis_h
#define generic_analysis_h

//C++
#include <iostream>
#include <map>

// MINE
#include "h22Reader.h"
#include "h22Option.h"
#include "StatusBar.h"

// ROOT 
#include "TString.h"
#include "TObject.h"

class GenericAnalysis : public h22Reader {
 public:
 GenericAnalysis(h22Options *opts) : options(opts) {  }
  ~GenericAnalysis(){ }

 public:
  h22Options            *options; 
  std::vector<TObject*>  rootObjects; 
  StatusBar status; 
 public:
  void AddFiles(std::vector<TString> files);
  void AddFile(TString file);
  void AddFile(std::string file);
 
  virtual bool RunAnalysis();
  virtual void Loop(int numberOfEvents);
  virtual void ProcessEvent();
  virtual void Save();
  virtual void Initialize();
  virtual void OptimizeLoop(int);
};

#endif
