#!/bin/bash
# Use this script to get an automatically generated tuple schema printed on the screen for a 
# given JSON file containing the JSON formatted data.
# (e-g) ./bin/run.sh   ./data/json_input1.dat

fil=$@


bname=$(basename $0)
if [ -z "$fil" ]; then
    echo "Usage: $bname input_file"
    exit 1
fi

if [ -z "$STREAMS_INSTALL" ]; then
    echo "ERROR: STREAMS_INSTALL must be set in your environment"
    exit 1
fi



if [ ! -e $fil ]; then
    echo "ERROR: File \"$fil\" does not exist"
    exit 1
fi



jspath=$(dirname $0)/../
cp=${jspath}/impl/lib/com.ibm.streamsx.json.jar:${STREAMS_INSTALL}/ext/lib/JSON4J.jar
java -cp $cp com.ibm.streamsx.json.JSONMain $fil
