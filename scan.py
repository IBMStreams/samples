from glob import glob
import os.path

first = True
def write(source, target):
    global first
    with open(source) as f:
        if (first is True):
            first = False
        else:
            target.write(",")
        lines = f.readlines()
        target.writelines(lines)
def check(pth, target):
    if (os.path.isfile(pth+"catalog.json")):
        write(pth+"catalog.json", target)
    elif (os.path.isdir(pth[:-1])):
        for dir in (glob(pth+'*/')):
            check(dir,target)

home = os.getcwd()

with open(home+"/full-catalog.json", "w") as target:
    target.write("[")
    paths = glob(home +"/*/")
    for p in paths:
        check(p, target)
    target.write("]")
