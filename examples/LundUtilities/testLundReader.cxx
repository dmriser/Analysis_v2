#include <iostream>
using namespace std; 

#include "LundReader.h"

int main(int argc, char *argv[]){

  if (argc != 2){
    cout << "Please input file" << endl;
    return 0;
  }

  string fileName(argv[1]);
  LundReader testReader(fileName);

  return 0;
}
