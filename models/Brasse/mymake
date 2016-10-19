#!/bin/bash
#
# 
# compileert alleen op michael 's machien
#
#
if [ -r $1.for ]; then
   SOURCEFILE=$1".for";
elif [ -r $1.f ]; then
   SOURCEFILE=$1".f";
else 
   echo "No source file found, exiting ..."
   exit;
fi
#
echo "Compiling "$SOURCEFILE 
#f77 $SOURCEFILE -L/export24/cernlib/98/lib -lpawlib -lmathlib -lpacklib -o $1

# voor bob s machien, met absoft

#g77 -ffixed-line-length-132 -w -C -Wall -s -O $1.f -L/u/site/cernlib/i386_redhat61/2001/lib/ -lpawlib -lpacklib -lkernlib -lmathlib -lpacklib-shift  -lg2c -lnsl  -lm -o $1 
f77  $1.f -m32 -L/apps/cernlib/i386_fc8/2005/lib/ -lpawlib -lpacklib -lkernlib -lmathlib  -o $1 

