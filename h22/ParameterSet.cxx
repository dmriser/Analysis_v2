// *****************************************************************************
/*

  David Riser, University of Connecticut 
  November 4, 2016 

  ParameterSet.cxx
  
  -> Defines generic structure of a parameter set, will be used 
  as entries in a parameter file. Held in a contianer. 

 */
// *****************************************************************************

#ifndef PARAMETER_SET_CXX
#define PARAMETER_SET_CXX

#include <iostream>
#include <string>
#include <vector>
using std::string; 
using std::vector; 
using std::cout; 
using std::endl; 

#include "ParameterSet.h"

ParameterSet::ParameterSet(){
}

ParameterSet::~ParameterSet(){
}

double ParameterSet::getValue(int index){
  if (index < value.size()){
    return value[index];
  }
  cout << "[ParameterSet::getValue] Error: Requested parameter index exceeds size of vector of parameters! " << endl;
  return 0.0; 
}

double ParameterSet::getError(int index){
  if (index < error.size()){
    return error[index];
  }
  cout << "[ParameterSet::getValue] Error: Requested parameter index exceeds size of vector of parameters! " << endl;
  return 0.0; 
}

vector<double> ParameterSet::getValues(){
  return value;
}

vector<double> ParameterSet::getErrors(){
  return error;
}

string ParameterSet::getName(){
  return name;
}

string ParameterSet::getDescription(){
  return description;
}

void ParameterSet::setName(std::string inputName){
  name = inputName; 
}

void ParameterSet::setDescription(std::string inputDescription){
  description = inputDescription; 
}

void ParameterSet::addValue(double inputValue){
  value.push_back(inputValue);
}

void ParameterSet::addError(double inputError){
  error.push_back(inputError);
}

void ParameterSet::addValueAndError(double inputValue, double inputError){
  value.push_back(inputValue);
  error.push_back(inputError);
}

#endif 
