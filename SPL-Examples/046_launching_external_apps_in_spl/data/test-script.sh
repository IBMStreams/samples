#!/bin/sh
# This script must receive one argument which will point to the 
# directory where the result files will be written.
streamtool lsinstance > $1/1.txt
streamtool version > $1/2.txt
uptime > $1/3.txt
