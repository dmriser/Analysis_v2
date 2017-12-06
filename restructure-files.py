#/usr/bin/env python 

import sys, os
import subprocess
import glob

def get_folders():
    return glob.glob('include/*')

def get_files(folder):
    return glob.glob(folder + '/*.h')

def main():
    folders = get_folders() 
    
    for folder in folders:
        print(folder)
        
        sub_folder = folder.split('/')[-1]

        files = get_files(folder)
        for f in files:
            filename = f.split('/')[-1].split('.')[0]
            target_file = str('src/' + filename + '.cxx')
            dest_file = str('src/'+sub_folder+'/'+filename+'.cxx')

            print('--> %s --> %s --> %s' % (filename, target_file, dest_file))
            
            command = 'cp %s %s' % (target_file, dest_file)
            subprocess.call(command, shell=True)
            

if __name__ == '__main__':
    main()
