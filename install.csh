#/bin/tcsh 

# compiling libraries 
scons 

set NLIBS    = `ls -l lib/*.so | wc -l`
set NHEADERS = `ls -l */*.h | wc -l`

# moving to dest
cp lib/*.so $h22libs/lib/
cp */*.h    $h22libs/include/
cp models/pdfModels/*.h $h22libs/include/

cp models/pbmodel/*.hh $h22libs/include/
cp models/pbmodel/*.so $h22libs/lib/

cp models/Brasse/*.h $h22libs/include/
cp models/Brasse/*.so $h22libs/lib/

echo " >> Finished! Installed $NLIBS libraries and $NHEADERS header files! "
