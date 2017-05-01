#/bin/bash

# compiling libraries 
scons 

# moving to dest
cp lib/*.dylib $h22libs/lib/
cp */*.h       $h22libs/include/

