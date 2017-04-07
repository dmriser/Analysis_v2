#include <iostream>
using namespace std;

#include "h22Option.h"

int main(int argc, char*argv[]){

  h22Options runtimeOptions;

  // Extra expected flags go here
  runtimeOptions.args["MACHINE"].args = "UNSET";
  runtimeOptions.args["MACHINE"].type = 1;  // This is a string 
  runtimeOptions.args["MACHINE"].name = "Machine being used";

  runtimeOptions.args["TYPE"].args = "UNSET";
  runtimeOptions.args["TYPE"].type = 1;  // This is a string 
  runtimeOptions.args["TYPE"].name = "type";

  runtimeOptions.args["MAGICNUMBER"].args = 42;
  runtimeOptions.args["MAGICNUMBER"].type = 0;  // This is a number 
  runtimeOptions.args["MAGICNUMBER"].name = "Magic Number";
  
  // You have to set the options
  runtimeOptions.set(argc, argv); 

  if (runtimeOptions.args["CONFIG"].args != "UNSET") { 
    runtimeOptions.parseConfigFile(runtimeOptions.args["CONFIG"].args); 
  }
  
  return 0;
}
