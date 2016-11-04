#include <iostream>
using namespace std; 

#include "ParameterMetaData.h"

int main(){

  ParameterMetaData metaData; 
  metaData.setNameOfFile("fakeFile.dat"); 
  metaData.setNumberOfParameterSets(9); 
  metaData.setNumberOfFilesUsed(200);
  metaData.updateTimeOfLastEdit(); 
  metaData.print();
 
  return 0;
}
