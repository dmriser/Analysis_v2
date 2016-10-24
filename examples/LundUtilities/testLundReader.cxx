#include <iostream>
using namespace std; 

#include "LundEvent.h"
#include "LundReader.h"

int main(int argc, char *argv[]){

  if (argc != 2){
    cout << "Please input file" << endl;
    return 0;
  }

  string fileName(argv[1]);
  LundReader testReader(fileName);

  while(testReader.HasEvent()){
    LundEvent event = testReader.GetEvent();
    event.PrintEvent();
  }

  cout << "Events processed: " << testReader.GetNumberOfEvents() << endl; 

  return 0;
}
