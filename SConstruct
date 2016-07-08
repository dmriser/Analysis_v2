# Compile libraries for analysis classes. 

import os, glob 

env = Environment(ENV = {'PATH':os.environ['PATH']})
env.ParseConfig("root-config --glibs --cflags")
env.Append(CCFLAGS = ['-g'])

h22_sources  = glob.glob('h22/*.cxx')
#h22_sources += glob.glob('h22/*.h')

env.SharedLibrary('lib/h22',h22_sources)
