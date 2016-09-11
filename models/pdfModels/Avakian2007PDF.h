////////////////////////////////////////////////////////////
/*

  David Riser, Univeristy of Connecicut 
  Sept. 11, 2016 

  Model from arXiv:0705.1553v2
  Avakian et. al. 

*/
////////////////////////////////////////////////////////////

#ifndef avakian_2007_pdf_h
#define avakian_2007_pdf_h

#include "GenericPDF.h"
#include "GenericQuark.h"
#include "GenericStructureFunction.h"

class PDFUpPlus_HA2007 : public GenericPDF{

 public:
  PDFUpPlus_HA2007();
  ~PDFUpPlus_HA2007();

  double alphaCoef = 0.0; 
  double ACoef = 0.0;
  double BCoef = 0.0;

  double getValue(double x);
};

class PDFUpMinus_HA2007 : public GenericPDF{

 public:
  PDFUpMinus_HA2007();
  ~PDFUpMinus_HA2007();

  double alphaCoef = 0.0; 
  double CCoef = 0.0;
  double CPrimeCoef = 0.0;
  double DCoef = 0.0; 
  
  double getValue(double x);
};

class PDFDownPlus_HA2007 : public GenericPDF{

 public:
  PDFDownPlus_HA2007();
  ~PDFDownPlus_HA2007();

  double alphaCoef = 0.0; 
  double ACoef = 0.0;
  double BCoef = 0.0;
  
  double getValue(double x);
};

class PDFDownMinus_HA2007 : public GenericPDF{

 public:
  PDFDownMinus_HA2007();
  ~PDFDownMinus_HA2007();

  double alphaCoef = 0.0; 
  double CCoef = 0.0;
  double CPrimeCoef = 0.0;
  double DCoef = 0.0; 
  
  double getValue(double x);
};

#endif
