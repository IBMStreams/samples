# File server for hosting Streams samples
Since it is not currently possible to download individual folders from Github, this small app, hosted on the IBM Cloud,
hosts and serves the zip files for the samples in the [Streams samples catalog](https://ibmstreams.github.io/samples).

The code in  `initialize.sh` script is run automatically from IBM Cloud whenever a change is made to this repository. 


Server written for Python 2

# Run the server locally,
Run `initialize.sh` then run `python server.py`
This will start a local server

# Tests
The `test`  folder has tests that use `curl` to verify the that the generated zips are the same as what is served by the fileserver.
Run tests:
- Make sure server is running (using instructions above)
```
cd test
./run_test.sh [local]
```
Use `local` to test a local instance of the server. Omit to test live server at https://streams-github-samples.mybluemix.net.

Check the latest `failure_20xx` file for results.
