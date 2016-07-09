#/bin/bash

# compiling libraries 
scons 

# moving to dest
cp lib/*.dylib /usr/local/lib/
cp */*.h /usr/local/include/
