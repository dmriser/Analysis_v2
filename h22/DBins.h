////////////////////////////////////////////////////
/*

  David Riser, University of Connecticut 
  August 5, 2016 

  DBins.h -> Shamelessly using my name for classes now. 

*/
////////////////////////////////////////////////////

#ifndef dbins_h
#define dbins_h

// c++ includes
#include <iostream>
#include <vector>

// root includes

// my includes

class DBins
{
  //Default Constr. Destr.
 public:
  DBins();
  ~DBins();

  DBins(int, double, double);

  // Datatypes
 private:
  int kNumber;
  double kMin, kMax, kWidth;
  std::vector<double> kBins;

  // Member functions 
 public:
  void refresh();
  void operator=(const DBins *);
  
  // Setters
  void set_min(double);
  void set_max(double);
  void set_number(int);

  // Getters
  int number(){ return kNumber; }
  double min(){ return kMin; }
  double max(){ return kMax; }
  double width(){ return kWidth; }
  std::vector<double> get_bins(){ return kBins; } 
  
};

#endif
