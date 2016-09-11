////////////////////////////////////////////////////////////////////////
/*

  David Riser, University of Connecticut 
  Sept. 11, 2016 

  Avakian2007PDF.cxx -> Define the rules for his model. 
  arXiv:0705.1553v2

*/
////////////////////////////////////////////////////////////////////////

#ifndef avakian_2007_pdf_cxx
#define avakian_2007_pdf_cxx

#include <cmath>
#include <iostream>
using namespace std;

#include "Avakian2007PDF.h"
#include "GenericPDF.h"
#include "GenericQuark.h"
#include "GenericStructureFunction.h"

PDFUpPlus_HA2007::PDFUpPlus_HA2007(){
  setName("Avakian 2007 u+");
  setQuarkFlavor('u'); 
  setQuarkCharge(2/3.0); 
  setHelicity(1); 

  alphaCoef = 1.313;
  ACoef = 3.088;
  BCoef = -3.010;
}

PDFUpPlus_HA2007::~PDFUpPlus_HA2007(){
  // Delete. 
}

double PDFUpPlus_HA2007::getValue(double x){

  double value = 0.0; 

  if (xIsBetween0and1(x)){
    value = pow(1/x,alphaCoef) * ( ACoef*pow(1-x,3) + BCoef*pow(1-x,4) ); 
  }

  return value; 
}

PDFUpMinus_HA2007::PDFUpMinus_HA2007(){
  setName("Avakian 2007 u-");
  setQuarkFlavor('u'); 
  setQuarkCharge(2/3.0); 
  setHelicity(-1); 

  alphaCoef = 1.313;
  CCoef = 2.143;
  CPrimeCoef = 0.493;
  DCoef = -2.065; 
}

PDFUpMinus_HA2007::~PDFUpMinus_HA2007(){
  // Delete. 
}

double PDFUpMinus_HA2007::getValue(double x){

  double value = 0.0; 

  if (xIsBetween0and1(x)) {
    value = pow(1/x,alphaCoef) * ( CCoef*pow(1-x,5) + CPrimeCoef*pow(1-x,5)*pow(log(1-x),2) + DCoef*pow(1-x,6) ); 
  }
  
  return value; 
}

PDFDownPlus_HA2007::PDFDownPlus_HA2007(){
  setName("Avakian 2007 d+");
  setQuarkFlavor('d'); 
  setQuarkCharge(1/3.0); 
  setHelicity(1); 

  alphaCoef = 1.313;
  ACoef = 0.343;
  BCoef = -0.265;
}

PDFDownPlus_HA2007::~PDFDownPlus_HA2007(){
  // Delete. 
}

double PDFDownPlus_HA2007::getValue(double x){

  double value = 0.0; 
  if (xIsBetween0and1(x)) {
    value = pow(1/x,alphaCoef) * ( ACoef*pow(1-x,3) + BCoef*pow(1-x,4) ); 
  }
  return value; 
}

PDFDownMinus_HA2007::PDFDownMinus_HA2007(){
  setName("Avakian 2007 d-");
  setQuarkFlavor('d'); 
  setQuarkCharge(-1/3.0); 
  setHelicity(-1); 

  alphaCoef = 1.313;
  CCoef = 1.689;
  CPrimeCoef = 1.592;
  DCoef = -1.610; 
}

PDFDownMinus_HA2007::~PDFDownMinus_HA2007(){
  // Delete. 
}

double PDFDownMinus_HA2007::getValue(double x){

  double value = 0.0;
  if (xIsBetween0and1(x)) {
    value = pow(1/x,alphaCoef) * ( CCoef*pow(1-x,5) + CPrimeCoef*pow(1-x,5)*pow(log(1-x),2) + DCoef*pow(1-x,6) ); 
  }
  return value; 
}

#endif
