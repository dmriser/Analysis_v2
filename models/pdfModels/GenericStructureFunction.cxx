////////////////////////////////////////////////////////
/*

  David Riser, University of Connecticut 
  Sept 11, 2016 

  GenericStructureFunction.cxx 

*/
////////////////////////////////////////////////////////

#ifndef generic_structure_function_cxx
#define generic_structure_function_cxx

#include <iostream>
#include <vector>
using namespace std; 

#include "CommonTools.h"
#include "GenericPDF.h"
#include "GenericQuark.h"
#include "GenericStructureFunction.h"

StructureFunction1D::StructureFunction1D(){
  
} 

StructureFunction1D::~StructureFunction1D(){
  
} 

double StructureFunction1D::getValue(double x){
  return 0.00; 
}

F1StructureFunction::F1StructureFunction(){
    

}

F1StructureFunction::~F1StructureFunction(){
    
}

double F1StructureFunction::getValue(double x){
    
    double value = 0.0;
    
    for (int iflavor = 0; iflavor < flavors.size(); iflavor++) {
        double thisQuarkCharge = flavors[iflavor]->getQuark().charge;
        double pdfValue = flavors[iflavor]->getValue(x);
        value += pdfValue*pow(thisQuarkCharge*electron_c,2);
    }
    
    return value/2; 
}


#endif
