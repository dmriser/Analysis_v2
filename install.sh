#/bin/bash

# compiling libraries 
scons 

# moving to dest
cp lib/*.dylib $h22libs/lib/
cp */*.h       $h22libs/include/

# moving model-pbmodel
#cp models/pbmodel/*.dylib $h22libs/lib/
#cp models/pbmodel/*.hh    $h22libs/include/
#cp models/pdfModels/*.h   $h22libs/include/
