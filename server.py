import os
import genZips

from subprocess import call, check_output, CalledProcessError
import subprocess

import time
from glob import glob
from ZipServer import DownloadZipHandler
from http.server import HTTPServer as Server


def start_server(root_dir):
    # Read port selected by the cloud for our application
    PORT = int(os.getenv('PORT', 8000))
    # Change current directory to avoid exposure of control files
    os.chdir(root_dir+"/static")
    httpd = Server(("", PORT), DownloadZipHandler)
    try:
      print("Start serving at port %i" % PORT)
      httpd.serve_forever()
    except KeyboardInterrupt:
      pass
    httpd.server_close()

root_dir = os.getcwd()
print ("Starting server in  " + root_dir)
start_server(root_dir)
