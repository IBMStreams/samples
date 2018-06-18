#!/bin/bash

cd static
echo "Current working directory is " `pwd`
#Clone the samples repository
#This way, we can generate the latest zips on deployment.


if [ -d "samples" ]; then
  echo "Samples directory exists"
  cd samples
  git pull
else
  if !( git clone https://github.com/IBMStreams/samples.git) ; then
    echo  "cloning repo failed"
    exit 1
  else
     cd samples
  fi
fi

echo "Creating zips.."
python ../../genZips.py `pwd`
