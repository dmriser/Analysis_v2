# Compile libraries for analysis classes. 

import os, glob 

env = Environment(ENV = {'PATH':os.environ['PATH']})
env.ParseConfig("root-config --glibs --cflags")
env.Append(CCFLAGS = ['-g','-lgfortran'])
env.Append(CPPPATH = ['/Users/dmriser/Work/analysis/models/elastic/'], LIBPATH = ['/Users/dmriser/Work/analysis/models/elastic/'],LIBS = ['elaslib.o'])

h22_sources  = glob.glob('h22/*.cxx')
env.SharedLibrary('lib/h22',h22_sources)
