#This script generates the catalog files for EACH of the SPL-100 samples.
# Right now it reads the existing catalog.json and updates attributes
#In the future need to run it again to update the URL for the samples.
import os
import sys
import json
#for each sample
#def gen_urls_on_github:
base_url = "https://github.com/natashadsilva/samples/tree/master/SPL-Examples/" #Note: change this when we pull into GITHUB
samples_dir="/Users/natashad/Documents/dev/git/samples/SPL-Examples/"
sample_list_file = open(samples_dir+"Sample_List")
sample_list = sample_list_file.readlines()
MAX_DESC_LEN=40
for sample in sample_list:
    sample = sample.strip()
    sample_json_file = samples_dir + sample + "/catalog.json"
    sample_json_object = json.load(open(sample_json_file))
    #fix capitalization
    name = sample_json_object["name"]
    capitalized1= [token.title() if len(token) > 2 else token for token in name.split()]
    capitalized= [token.lower() if token == "Work" else token for token in capitalized1]

    name = " ".join(capitalized)
    name = name.replace("Spl", "SPL")
    sample_json_object["name"] = name
    #fix url
    sample_json_object["url"] = base_url + sample
    #print sample_json_object
    #desc_list = sample_json_object["description"].split()
    #if (len(desc_list) > (MAX_DESC_LEN + 1)):
    #    sys.stdout.write(str(len(desc_list)))
    #    del desc_list[MAX_DESC_LEN:]
    #    desc_list[MAX_DESC_LEN-1]+="..."
    #    sample_json_object["description"] = " ".join(desc_list)
    #    print  " " + sample + " "  + sample_json_object["description"]
#    print str(len(desc_list)) + " "+ sample
    catalog = open("/Users/natashad/Documents/dev/git/samples/SPL-Examples/" + sample +"/catalog.json","w")
    catalog.write(json.dumps(sample_json_object))
    catalog.close()
