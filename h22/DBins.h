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
  void operator=(DBins);
    
  // Setters
  void set_min(double);
  void set_max(double);
  void set_number(int);

  // Getters
  int number(){ return kNumber; }
  int find_bin(double);
  double min(){ return kMin; }
  double max(){ return kMax; }
  double width(){ return kWidth; }
  double bin_center(int);
  std::vector<double> get_bins(){ return kBins; } 
  
};

// For making asymmetric binning schemes 
class DBinsAsymmetric
{
 public:
  DBinsAsymmetric();
  ~DBinsAsymmetric();

  std::vector<double> bins;

  int find_bin(double);
  int number(){ return bins.size()-1; }

  void add_bin_edge(double bin){ bins.push_back(bin); } 

  double width(int);
  double center(int);
  double min(int);
  double max(int);
  
};

#endif
