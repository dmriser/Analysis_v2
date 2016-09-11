////////////////////////////////////////////////////////////////
/*

  David Riser, University of Connecticut 
  Sept. 11, 2016 

  GenericPDF.h -> Define class structure for a generic pdf. 

*/
////////////////////////////////////////////////////////////////

#ifndef generic_pdf_h
#define generic_pdf_h

#include <iostream>
#include "GenericQuark.h"

class GenericPDF{
 public:
  virtual ~GenericPDF();

 protected:
  GenericPDF();
  std::string name;
  int helicity;
  GenericQuark quark; 
  bool xIsBetween0and1(double x); 
  
 public:
  void setName( std::string n ){ name = n; } 
  void setHelicity(int h){ quark.helicity = h; }
  void setQuarkCharge(double c){ quark.charge = c; }
  void setQuarkFlavor(char q){ quark.flavor = q; }
  
  GenericQuark getQuark(){ return quark; }
  std::string getName(){ return name; }
  int getHelicity(){ return quark.helicity; };

  virtual double getValue(double x); 
};


#endif
