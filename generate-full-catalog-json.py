#This script searches all the directories in this repository for a file called catalog.json and builds a master catalog from the contents of each file.
from glob import glob
import os.path

first_catalog_entry = True
def hasFile(dir, fileToCheckFor):
    return  os.path.isfile(dir+ "/" + fileToCheckFor)

def copy_to_main_catalog(source, target): #copy the contents of the catalog.json (source) into the full-catalog.json file (target)
    global first_catalog_entry
    with open(source) as f:
        if (first_catalog_entry is True):
            first_catalog_entry = False
        else:
            target.write(",")
        lines = f.readlines()
        target.writelines(lines)
        target.write("\n")
def  add_catalog_entry(pth, target): #check to see if this directory contains a catalog.json file
    if (hasFile(pth, "catalog.json")):
        copy_to_main_catalog(pth+"catalog.json", target) #if it does, copy its catalog.json content into the full one
    elif (os.path.isdir(pth[:-1])):
        for dir in (glob(pth+'*/')): #if it doesn't, check its sub directories
           add_catalog_entry(dir,target)

if __name__ == '__main__':
    home = os.getcwd()
    with open(home+"/full-catalog.json", "w") as target:
        target.write("[")
        paths = glob(home +"/*/") #for each directory in this folder
        for p in paths:
            add_catalog_entry(p, target) #check for a catalog.json descriptor file
        copy_to_main_catalog("extSamples.json", target)
        target.write("]")
