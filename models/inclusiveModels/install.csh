#/bin/tcsh 

# compiling libraries 
scons 

set NLIBS    = `ls -l *.so | wc -l`
set NHEADERS = `ls -l */*.f | wc -l`

# moving to dest
cp *.so $h22libs/lib/
cp */*.f $h22libs/include/
cp */*.h $h22libs/include/
cp */*.hh $h22libs/include/

echo " >> Finished! Installed $NLIBS libraries and $NHEADERS header files! "
