try:
  from SimpleHTTPServer import SimpleHTTPRequestHandler as Handler
except ImportError:
  from http.server import SimpleHTTPRequestHandler as Handler
import cgi
import logging
import urllib.parse
import os
#This is a simple server that is used to download the zip files for the samples in ibmstreams github samples repository
#Samples are mirrored here in static/samples
#valid requests are of the form get=Category/Sample e.g get=ODM/GPXToTuple
#where Category is a folder within the samples directory
class DownloadZipHandler(Handler):


    def redirect(self, reason):
        #redirect this request to the main samples page.
        print ("Ignoring request %s, reason: %s " % (self.path, reason))
        self.send_response(301)
        self.send_header("Location", "https://ibmstreams.github.io/samples")
        self.end_headers()

#return false if this request is invalid (no path, missing param, or path is invalid)
#return the requested path otherwise.

    def is_valid_path(self):
        valid = False
        arg_index = self.path.find('?')
        print ("2. Checking arguments ")
        if arg_index >= 0:
            args = cgi.parse_qs(self.path[arg_index+1:])

            if ("get" in args):
                raw_path = args["get"][0]
                print ("3. requested path: " + raw_path)
                path=urllib.parse.unquote(raw_path)
                directory = os.getcwd() + "/samples/" + path+"/"
                print ("4. Checking for " + directory)
                if os.path.exists(directory):#"samples/" + path):
                    if "." in path or ".." in path or "~" in path or "*" in path:
                        print ("invalid characters in path")
                        return False
                    else:
                        return path
                else:
                    print (path + " is not in samples dir. ")
        return valid
    def do_GET(self):
        '''
        Handle a GET request.
        '''
        # Parse out the arguments.
        # The arguments follow a '?' in the URL. Here is an example:
        #   http://example.com?arg1=val1
        print ("1. got request " + self.path)
        return_value = self.is_valid_path()
        print ("5. is_valid_path return value: " + str(return_value))
        if return_value is False:
            self.redirect("6. Path does not exist.")
        else:
            path = return_value
            path = path.strip('/')
            folders = path.split("/")
            zipName = "samples/" + path + "/" + folders[-1] +".zip"
            baseName=folders[-1]+".zip"
            if os.path.exists(zipName):
                print ("6. File exists, opening")
                f = open(zipName,"rb")
                self.send_response(200)
                print ("7. Sending header")
                self.send_header('Content-type',    'application/zip')
                self.send_header('Content-Disposition','attachment; filename="'+baseName+'"')
                self.end_headers()
                self.wfile.write(f.read())
                f.close()
                print ("8. Handled request for " + self.path)
            else:
                print ("6. Ignoring request for " + zipName + " because exists : " + str(os.path.exists(zipName)))
                self.redirect("No zip file with name " + zipName+ " within folder")
