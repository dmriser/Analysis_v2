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

// ROOT 
#include "TString.h"

class GenericAnalysis : public h22Reader {
 public:
  GenericAnalysis(){ options["OUT"] = "out.root"; }
  ~GenericAnalysis(){ }

 public:
  std::map<std::string, std::string> options; 

 public:
  void AddFiles(std::vector<TString> files);
  void AddFile(TString file);

  virtual void RunAnalysis(int numberOfEvents);
  virtual void Loop(int numberOfEvents);
  virtual void ProcessEvent();
  virtual void Save();
  virtual void Initialize();
};

#endif
