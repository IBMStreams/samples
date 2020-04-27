#!/usr/bin/env python
import os
from glob import glob
import zipfile
from zipfile import ZipFile
import zlib
import sys
import shutil

# Assumes that the stremas github samples repo has been cloned at static/samples in the current directory, or
# that path to the cloned repo is specified at run tim.
# Use  the initialize.sh script to run this script instead of running it directly.

def zipdir(path, ziph):
    # ziph is zipfile handle
    for root, dirs, files in os.walk(path):
        for file in files:
            if "catalog.json" != file:
                ziph.write(os.path.join(root, file))

def should_zip(folder):
    #if a folder has a catalog.json entry then we should create af it.
    return os.path.exists(folder+"/catalog.json")

def zip_single_dir(dir):
    project_name = dir[:-1]
    zip_name = project_name +".zip"
    zipf = ZipFile(zip_name, 'w', zipfile.ZIP_DEFLATED)
    #print "Creating " + zip_name  + " in " + os.getcwd()
    zipdir(dir, zipf)

    zipf.close()
    #move the zip file into the directory
    if os.path.exists(dir+zip_name):
        os.remove(dir+zip_name) #if already exists

    shutil.move(zip_name, dir)

def zip_recursively(dir):
    num_zipped = 0
    if should_zip(dir):
        #this is a single project
        zip_single_dir(dir)
        num_zipped = 1
    else:
        #check the sub directories for a project
        os.chdir(dir)
        for child_directory in glob('*/'):
            num_zipped  += zip_recursively(child_directory)
        os.chdir("..")
    return num_zipped

def zip_root_directory():
    num_zipped = 0
    sub_dirs = glob("*/")

    for category in sub_dirs:
        #every top level folder is a category

        num_zipped += zip_recursively(category)

    print ("Done...Zipped " + str(num_zipped) + " Directories")

if __name__ == '__main__':
    #Usage:  genZips.py [samples directory]
    if (len(sys.argv) == 1):
        target_dir = os.getcwd()
    else:
        target_dir = sys.argv[1]
    os.chdir(target_dir)
    zip_root_directory()
