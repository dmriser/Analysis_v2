# Compile libraries for analysis classes. 

import os, glob 

# Moved to it's own SConscript in models/
#fort_env = Environment(ENV = {'PATH':os.environ['PATH']})
#fort_env.SharedLibrary('models/pbmodel/libF1F209','models/pbmodel/F1F209.f')
#fort_env.SharedLibrary('models/Brasse/libBrasse','models/Brasse/brasse.f')

# Config h22 environment 
env = Environment(ENV = {'PATH':os.environ['PATH']})
env.ParseConfig("root-config --glibs --cflags")
env.Append(CCFLAGS = ['-g','-std=c++11'])
env.Append(CPPPATH = ['include/analysis', 'include/io', 'include/config', 'include/database', 'include/tools', 'include/histogram', 'include/nathan'])
#env.Append(CPPPATH = ['models/pbmodel/'], LIBPATH = ['models/pbmodel/'],LIBS = ['CPP_F1F209']) # Get FORTRAN model 
#env.Append(CPPPATH = ['models/Brasse/'], LIBPATH = ['models/Brasse/'],LIBS = ['CPP_BRASSE']) # Get FORTRAN model 

# Build the h22 library 
h22_sources  = glob.glob('src/*.cxx')
h22_sources  = glob.glob('src/*/*.cxx')
h22_sources += glob.glob('momCorr/*.cxx')	
#h22_sources += glob.glob('models/pbmodel/*.cc')
#h22_sources += glob.glob('models/Brasse/*.cxx')
#h22_sources += glob.glob('models/pdfModels/*.cxx')
env.SharedLibrary('lib/h22',h22_sources)
