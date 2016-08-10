# Compile libraries for analysis classes. 

import os, glob 

env = Environment(ENV = {'PATH':os.environ['PATH']})
env.ParseConfig("root-config --glibs --cflags")
env.Append(CCFLAGS = ['-g','-lgfortran'])

h22_sources  = glob.glob('h22/*.cxx')
h22_sources += glob.glob('models/*/*.o')
env.SharedLibrary('lib/h22',h22_sources)
