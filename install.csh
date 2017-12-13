#/bin/tcsh 

setenv h22libs `pwd`
setenv ANALTOP `pwd`
setenv LD_LIBRARY_PATH $h22libs/lib/:$LD_LIBRARY_PATH

# compiling libraries 
scons 

set NLIBS    = `ls -l lib/*.so | wc -l`
set NHEADERS = `ls -l include/*/*.h | wc -l`

# moving to dest
#cp lib/*.so         $h22libs/lib/
#cp */*.h            $h22libs/include/
#cp include/*/*.h    $h22libs/include/

cat logo.dat
echo " >> Finished! Installed $NLIBS libraries and $NHEADERS header files! "
