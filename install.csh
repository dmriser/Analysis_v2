#/bin/tcsh 

setenv h22libs         `pwd`
setenv ANALTOP         `pwd`
setenv LD_LIBRARY_PATH $h22libs/lib/:$LD_LIBRARY_PATH

# compiling libraries 
scons 

# for useless spamming of users 
set NLIBS    = `ls -l lib/*.so | wc -l`
set NHEADERS = `ls -l include/*/*.h | wc -l`

# more useless spamming 
cat logo.dat
echo " >> Finished! Installed $NLIBS libraries and $NHEADERS header files! "
