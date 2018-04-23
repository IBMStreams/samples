#!/bin/sh
# Generates the json DB for the web app and then starts the server.
#Run from top level samples directory, on the master branch.
# Commit all changes to master branch first before running this script otherwise it will fail.
python generate-full-catalog-json.py
mv full-catalog.json /tmp/full-catalog.json
echo "Switching to gh-pages branch"
git checkout gh-pages
mv /tmp/full-catalog.json data/main.json
echo "Starting server"
bundle exec jekyll serve
