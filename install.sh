#/bin/bash 

currentDir=$(pwd)

echo "Setting analysis environment to ${currentDir}"

export h22libs=${currentDir}
export ANALTOP=${currentDir}
export LD_LIBRARY_PATH=$h22libs/lib/:$LD_LIBRARY_PATH

# compiling libraries 
scons 

# more useless spamming 
cat logo.dat
echo " >> Finished!"
