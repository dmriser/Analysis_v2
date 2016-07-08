#/bin/bash

# compiling libraries 
scons 

set NLIBS    = `ls -l lib/*.dylib | wc -l`
set NHEADERS = `ls -l */*.h | wc -l`

# moving to dest
cp lib/*.dylib /usr/local/lib/
cp */*.h /usr/local/include/

echo " >> Finished! Installed $NLIBS libraries and $NHEADERS header files! "
