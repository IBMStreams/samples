import os
import json
#for each sample
sample_list_file = open("/Users/natashad/Documents/dev/git/samples/SPL-Examples/Sample_List")
sample_list = sample_list_file.readlines()
elements = json.load(open("/Users/natashad/Documents/dev/git/IBM-Bluemix.github.io/data/main.json"))
for sample in sample_list:
    sample = sample.strip()
    myjson = [elem for elem in elements if sample in elem["url"]]
    if len(myjson) == 0:
        print sample
    else:
        line = json.dumps(myjson[0])
        catalog = open("/Users/natashad/Documents/dev/git/samples/SPL-Examples/" + sample +"/catalog.json","w")
        catalog.write(line)
        catalog.close()
