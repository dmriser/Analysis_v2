////////////////////////////////////////////////////////
/*

  David Riser, University of Connecticut 
  Sept 11, 2016 

  GenericStructureFunction.h 

*/
////////////////////////////////////////////////////////

#ifndef generic_structure_function_h
#define generic_structure_function_h

#include <iostream>
#include <vector>

#include "GenericPDF.h"
#include "GenericQuark.h"

class StructureFunction1D{
 public:
  ~StructureFunction1D();

 protected:
  StructureFunction1D(); 
  std::vector<GenericPDF*> flavors; 

 public:
  void addPDF(GenericPDF *thisPDF){ flavors.push_back(thisPDF); }
  virtual double getValue(double x); 
};

class F1StructureFunction : public StructureFunction1D{
    
public:
    F1StructureFunction();
    ~F1StructureFunction();
    
    double getValue(double x);
    
};

#endif
