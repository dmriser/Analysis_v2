#include <iostream>
using std::string;
using std::cout; 
using std::endl;

#include "h22Option.h"
#include "LundGeneratedEventReader.h"

int main(int argc, char *argv[]){

  h22Options opts; 
  opts.set(argc, argv);

  LundGeneratedEventReader *lundConverter = new LundGeneratedEventReader();
  lundConverter->LoadEvents(opts.ifiles[0]);
  lundConverter->Execute( lundConverter->GetNumberOfEvents() );
  lundConverter->Save(opts.args["OUT"].args);

  return 0;
} 
