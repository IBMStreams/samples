#!/usr/bin/env python
import os
from glob import glob
import zipfile
from zipfile import ZipFile
import zlib
import sys
import scan
import shutil
def zipdir(path, ziph):
    # ziph is zipfile handle
    for root, dirs, files in os.walk(path):
        for file in files:
            ziph.write(os.path.join(root, file))

def zipdir(path, ziph):
    # ziph is zipfile handle
    for root, dirs, files in os.walk(path):
        for file in files:
            ziph.write(os.path.join(root, file))
if __name__ == '__main__':
    if (len(sys.argv) == 1):
        target_dir = os.getcwd()
    else:
        target_dir = sys.argv[1]
        os.chdir(target_dir)
    sub_dirs = glob("*/")
    for category in sub_dirs:
        #every top level folder is a category
        os.chdir(category)

        #print os.getcwd()
        #print category
        #  print glob("*/")
        for dir in glob('*/'): #if it doesn't, check its sub directories
        #    print dir
            project_name = dir[:-1]
            zip_name = project_name +".zip"
            #zipf = ZipFile(zip_name, 'w', zipfile.ZIP_DEFLATED)
            print "Creating " + zip_name  + " in " + os.getcwd()
            #zipdir(dir, zipf)
            #zipf.close()
            #shutil.move(zip_name, dir)
            #break
        os.chdir("..")
