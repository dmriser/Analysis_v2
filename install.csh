#/bin/tcsh 

# sourcing to get root libs 
#source /group/clas/builds/environment.csh 

# compiling libraries 
scons 

set NLIBS    = `ls -l lib/*.so | wc -l`
set NHEADERS = `ls -l */*.h | wc -l`

# moving to dest
cp lib/*.so /u/home/dmriser/lib/
cp */*.h /u/home/dmriser/include/

echo " >> Finished! Installed $NLIBS libraries and $NHEADERS header files! "
