//////////////////////////////////////////////////////////////////
/*

  David Riser, University of Connecticut 
  Sept. 11, 2016 

  GenericPDF.cxx 

*/
//////////////////////////////////////////////////////////////////

#ifndef generic_pdf_cxx
#define generic_pdf_cxx

#include "GenericPDF.h"

GenericPDF::GenericPDF(){

}

GenericPDF::~GenericPDF(){

}

double GenericPDF::getValue(double x){
  return 0.0; 
}

bool GenericPDF::xIsBetween0and1(double x){
  return (x > 0 && x < 1);
}

#endif
