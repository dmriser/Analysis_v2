// *****************************************************************************
/*

  David Riser, University of Connecticut 
  November 4, 2016 

  ParameterSet.h 
  
  -> Defines generic structure of a parameter set, will be used 
  as entries in a parameter file. Held in a contianer. 

 */
// *****************************************************************************

#ifndef PARAMETER_SET_H
#define PARAMETER_SET_H

#include <string>
#include <vector>
#include <map>

class ParameterSet{
 public:
  ParameterSet();
  ~ParameterSet();

 protected:
  std::string name; 
  std::string description; 
  std::vector<double> value; 
  std::vector<double> error; 

 public:
  double getValue(int index);
  double getError(int index);
  std::string getName();
  std::string getDescription();
  std::vector<double> getValues(); 
  std::vector<double> getErrors(); 
  int getNumberOfValues(){ return value.size(); }
  int getNumberOfErrors(){ return error.size(); }

  void setName(std::string inputName);
  void setDescription(std::string inputDescription);
  void addValue(double inputValue);
  void addError(double inputError);
  void addValueAndError(double inputValue, double inputError);
};

#endif 
