import os
import json
import sys
import urllib
import time
import datetime
#Requires Python 2
#tests that the links used to download zips from the server work.
#Use run_test.sh to run this script
#Assumption is that the initialize.sh script in the parent folder has already been run.
#failed samples are written to a file called failure_timestamp.txt

from subprocess import call, check_output



def download_and_verify_zips(server):
    failed_count = 0
    samples_dir = os.getcwd()+ "/../static/samples/"
    current_dir = os.getcwd()
    os.chdir(samples_dir)

    #1.generate the latest set of catalog entries using the script
    ret = call(["python","generate-full-catalog-json.py"])
    if ret !=0 :
        print "Failed to generate catalog"
        return 1

    json_file = open("full-catalog.json", "r")

    #parse the json file into an array
    sample_array = json.load(json_file)

    os.chdir(current_dir)
    failure_file_name = 'failure_{:%Y_%b_%d_%H:%M:%S}'.format(datetime.datetime.now())
    error_file =open(failure_file_name ,"w")
    existing_zip_dir = samples_dir

    base ="https://github.com/IBMStreams/samples/tree/master/"

    os.chdir("results")
    #for each element in the json file, get the url
    for sample in sample_array:
        url = sample["url"]

        folder = url
        if not sample.has_key("external"):
           idx = url.index(base) + len(base)
           folder = url[idx:]
        elif sample["external"]:
          continue


        #Uncomment out to debug failures
        #print "path relative to samples folder =" + folder

        folder = folder.strip("/")
        folders_in_path = folder.split("/")
        last_index = len(folders_in_path) - 1
        zipfile = folders_in_path[last_index]

        #print "target zip " + folder +  "/" +  zipfile
        if (os.path.exists(zipfile)):
            error_file.write(zipfile + " already exists, you might need to clean results directory first and re-run. I\n")
        else:
           #download the zip from the server using curl
           #escape special characters
            escaped_url= urllib.quote(folder)
            time.sleep(2.0)
            call(["curl","-X","GET","-H", "'Cache-Control: no-cache'","-O", server + escaped_url])
            escaped_zip_name = urllib.quote(zipfile)
            #if the file was downloaded
            if os.path.exists(escaped_zip_name):
                #compare it with what we have locally

                os.rename(escaped_zip_name, escaped_zip_name+".zip")
                escaped_zip_name = escaped_zip_name+".zip"
                ret = call(["diff", escaped_zip_name, existing_zip_dir + folder + "/" + zipfile+".zip"])
                if (ret != 0):
                    #check file size:
                    size_dl = os.path.getsize(escaped_zip_name)
                    size_local = os.path.getsize(existing_zip_dir + folder + "/" + zipfile+".zip")
                    if (size_dl is 0 or abs(size_dl -size_local) > 1000):
                        failed_count = failed_count + 1
                        error_file.write("downloaded zip size " +str(size_dl) + "\n")
                        error_file.write("downloaded zip mismatch " +escaped_zip_name + "\n")
                        error_file.write("\t folder = " +folder + "  zipfile = " + escaped_zip_name + " requested " + server+escaped_url+"\n")
                else:
                    print "Yay, " + escaped_zip_name + " passed!"
            else:
                #if an error occured downloading, print it
                failed_count = failed_count + 1
                print "error, downloading " + escaped_zip_name + "failed"
                error_file.write("download error " + escaped_zip_name + "\n")
                error_file.write("\turl = " +url + "  zipfile = " + zipfile + " requested " + server+escaped_url+"\n")

    os.chdir("..")

    json_file.close()
    error_file.write("Finished running tests. If there are no messages in this file, all tests pass. TradesApp might incorrectly report an error, this can be safely ignored.")
    error_file.close()
    return failed_count


if __name__ == '__main__':
    server = "https://streams-github-samples.mybluemix.net/?get="
    #change the server url to test localhost if needed.

    if len(sys.argv) > 1 and sys.argv[1] == "local":
        print "Testing local server"
        server = "http://127.0.0.1:8000/?get="
    ret_val = download_and_verify_zips(server)
    sys.exit(ret_val)
