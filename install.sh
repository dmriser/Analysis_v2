#/bin/bash

# compiling libraries 
scons 

export h22libs=/usr/local/

# moving to dest
cp lib/*.dylib $h22libs/lib/
cp */*.h       $h22libs/include/

