#/bin/tcsh 

setenv $h22libs `pwd`
setenv $ANALTOP `pwd`

# compiling libraries 
scons 

set NLIBS    = `ls -l lib/*.so | wc -l`
set NHEADERS = `ls -l include/*/*.h | wc -l`

# moving to dest
cp lib/*.so         $h22libs/lib/
cp */*.h            $h22libs/include/
cp include/*/*.h    $h22libs/include/

cat logo.dat
echo " >> Finished! Installed $NLIBS libraries and $NHEADERS header files! "
