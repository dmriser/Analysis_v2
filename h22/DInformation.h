///////////////////////////////////////////////
/*
  
  David Riser, University of Connecticut 
  August 7, 2016 

  DInformation.h -> Defines class structure to 
  hold run information class. 

*/
///////////////////////////////////////////////

#ifndef dinfo_h
#define dinfo_h

// c++ includes
#include <iostream>
#include <map>
#include <vector>

// my includes

// root includes

class DInfo
{
 public:
  int runno;
  int dQ;
  int dN; 
};

class DInformation
{
 public:
  DInformation();
  ~DInformation();

  // Datatypes
  std::vector<std::map<int, DInfo>> info;

  // Methods
  void load(std::string);
  bool has_information();
};

#endif
