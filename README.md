## README --  IBMStreams samples

This repository contains sample applications for IBM Streams.

### [Search the samples using the samples catalog](http://ibmstreams.github.io/samples)

You can also [Contribute a sample](https://github.com/IBMStreams/samples/wiki/Adding-a-sample-to-the-catalog-and-repo)


To learn more about Streams:
* [IBM Streams on Github](http://ibmstreams.github.io)
* [Streams Community](https://ibm.biz/streams-community)


## About this repository
Each branch in the repo is related and serves a different purpose.
- `main` branch: has the actual code samples organized by feature or task, e.g. Read data, Analyze data, etc.


- `gh-pages` branch has the code for the catalog, ibmstreams.github.io/samples/
  - AngularJS code
- `fileserver` branch: This has the code for the file server. The file server is used by the catalog to provide a zip archive of each individual sample.

### Adding a new sample to the catalog 
You need to install jekyll. 
Background info:

-  Each individual sample has a `catalog.json` that describes the sample and is used by the gh-pages branch to display the catalog.
- `extSamples.json` contains information about additional samples that are listed on the website but hosted in other repositories

To add a sample to the catalog:

- If the sample is going to be in this repo, 
  - add the sample folder to the tree in the appropriate folder, e.g. if it is a Database sample, add it to ReadAndStore data.
  - Add a `catalog.json` file to the top level folder of the new sample, use any of the existing `catalog.json` as an example

- If it is going to be hosted in another repo, 
  - edit the `extSamples.json` file to add the information about the sample.
- Update the  categories array  in the JSON file is based on the categories listed in categoryKey.txt
- Commit your changes to the main branch.
- Run the updateCatalogDB.sh script. This script will 
  - run the `generate-catalog.py` script combines the `extSamples.json` file with the entry from each catalog.json in the repo to create  a master JSON file listing every entry in the catalog. 
  - Switch to the `gh-pages` branch and start a local copy of the server at 127.0.0.1:4000/
- 127.0.0.1:4000/ to see the catalog and verify the new sample is listed. Commit and push the changes to the gh-pages branch.

*Note*: Run the data/main.json file through a JSON minimizer before commiting it to remove whitespace.
