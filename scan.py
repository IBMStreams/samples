from glob import glob
import os.path

first_catalog_entry = True
def hasFile(dir, fileToCheckFor):
    return  os.path.isfile(dir+ "/" + fileToCheckFor)

def write(source, target): #copy the contents of the catalog.json (source) into the full-catalog.json file (target)
    global first_catalog_entry
    with open(source) as f:
        if (first_catalog_entry is True):
            first_catalog_entry = False
        else:
            target.write(",")
        lines = f.readlines()
        target.writelines(lines)
def check(pth, target): #check to see if this directory contains a catalog.json file
    if (hasFile(pth, "catalog.json")):
        write(pth+"catalog.json", target) #if it does, copy its catalog.json content into the full one
    elif (os.path.isdir(pth[:-1])):
        for dir in (glob(pth+'*/')): #if it doesn't, check its sub directories
            check(dir,target)


if __name__ == '__main__':
    home = os.getcwd()
    with open(home+"/full-catalog.json", "w") as target:
        target.write("[")
        paths = glob(home +"/*/") #for each directory in this folder
        for p in paths:
            check(p, target) #check for a catalog.json descriptor file
        target.write("]")
