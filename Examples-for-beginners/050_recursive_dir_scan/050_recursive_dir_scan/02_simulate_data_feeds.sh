#!/bin/bash
# ========================
# First created on: May/13/2013
# Last modified on: May/13/2013
#
# Use this script to start simulating the data feeds for
# testing the 050_recursive_dir_scan SPL application.
# This script will drop a recursive directory into our watch directory.
# ========================
date
# Let us first delete all the files in the data directory.
rm -f ./data/*.start_batch
rm -f ./data/*.end_batch
rm -f ./data/*.csv

# Let us stay in a loop and create 2  batches each with a recursive directory.
# In between each batch, we will induce a delay of 10 seconds.
for (( idx=1; idx<=2; idx++ ))
do
   echo "Copying a multi-level recursive directory into our watch directory ..."
   # Copy a decently recursive directory into our watch directory.
   cp -ufr $STREAMS_INSTALL/ext/include   ./data
   
   # Copy a start batch signal file now.
   startBatchFileName=$idx.start_batch
   endBatchFileName=$idx.end_batch
   cp ~/.bashrc ./data/$startBatchFileName
   cp ~/.bashrc ./data/$endBatchFileName
   echo "Copied all the transaction files for batch $idx"
   echo "Waiting for 10 seconds ..."
   sleep 10
done

echo "Finished copying the transaction files for all the batches."
rm -f ./data/*.start_batch
rm -f ./data/*.end_batch
rm -f ./data/*.csv
rm -rf ./data/include
date