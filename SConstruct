# Compile libraries for analysis classes. 

import os, glob 

env = Environment(ENV = {'PATH':os.environ['PATH']})
env.ParseConfig("root-config --glibs --cflags")
env.Append(CCFLAGS = ['-g','-lgfortran'])
env['STATIC_AND_SHARED_OBJECTS_ARE_THE_SAME']=1

h22_sources  = glob.glob('h22/*.cxx')
h22_sources += glob.glob('models/elastic/*.o')
env.SharedLibrary('lib/h22',h22_sources)
