# Compile libraries for analysis classes. 

import os, glob 

# Compile PB Model 
fort_env = Environment(ENV = {'PATH':os.environ['PATH']})
fort_env.SharedLibrary('models/pbmodel/libF1F209','models/pbmodel/F1F209.f')

# Config h22 environment 
env = Environment(ENV = {'PATH':os.environ['PATH']})
env.ParseConfig("root-config --glibs --cflags")
env.Append(CCFLAGS = ['-g'])
env.Append(CPPPATH = ['models/pbmodel/'], LIBPATH = ['models/pbmodel/'],LIBS = ['F1F209']) # Get FORTRAN model 

# Build the h22 library 
h22_sources  = glob.glob('h22/*.cxx')
h22_sources += glob.glob('momCorr/*.cxx')	
h22_sources += glob.glob('models/pbmodel/*.cc')
h22_sources += glob.glob('models/pdfModels/*.cxx')
env.SharedLibrary('lib/h22',h22_sources)
