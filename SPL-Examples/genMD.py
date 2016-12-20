import os
import re

exp = re.compile("^\d\d\d\_")
def generate_reamde_md_file():
    with open("fulldescriptions.txt") as f:
      lines = f.readlines()
      cur = None
      for line in lines:
          #if we are currently writing, append to current,
          #else open new file

          if (exp.match(line)):
            line = line.strip()
            #print "./"+line
            if (cur):
                cur.close()
            cur = open(os.getcwd()+"/"+line+"/README.md", "w")
          elif (cur):
              cur.write(line)

if __name__ == '__main__':
    generate_reamde_md_file()
