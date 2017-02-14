import os
from glob import glob
exclude = ["DbLoader/","static/","JDBCForBluemix/"]
lines = open("/Users/natashad/Documents/dev/git/samples/JDBC/JDBCForBluemix/WeatherAnalysis/catalog.json").readlines()
print lines
for dir in glob("*/"):
  dir = dir.strip()
  if dir not in exclude:
      os.chdir(dir)
      children =  glob("*/")
      for child in children:
        child = child.strip()
        if (os.path.exists(child+"catalog.json")):
            print child + " exists"
        else:
            fd = open(child + "/catalog.json", "w")
            print child + "addng new" # + " " + str(fd)
            fd.writelines(lines)
            fd.close()
      os.chdir("/Users/natashad/Documents/dev/git/samples")
#
