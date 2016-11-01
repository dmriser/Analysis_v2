#/bin/tcsh 

# compiling libraries 
scons 

# moving to dest
cp *.so $h22libs/lib/

cp inclusiveModels/*/*.f $h22libs/include/
cp elasticModels/*/*.f $h22libs/include/

cp inclusiveModels/*/*.h $h22libs/include/
cp elasticModels/*/*.h $h22libs/include/

cp inclusiveModels/*/*.hh $h22libs/include/

echo "Finished"
