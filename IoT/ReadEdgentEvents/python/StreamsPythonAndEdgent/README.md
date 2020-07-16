This project contains a Streams application written in Python that processes events sent from an [Edgent](https://edgent.apache.org) application. It also shows how to send commands to the Edgent application.
## Prerequisites
See [the prerequisites here](https://github.com/IBMStreams/samples/tree/main/IoT/ReadEdgentEvents).

A notebook version of this sample [also exists](https://github.com/IBMStreams/samples/tree/main/IoT/ReadEdgentEvents/python/notebook).

Once you have all the prerequisites, you are ready to run the Streams application.


## Run the Streams application
You need version 1.7 or greater of the streamsx package:
`pip install streamsx`


### Submit to a local instance of Streams:
- [Follow these steps](http://ibmstreams.github.io/streamsx.documentation/docs/python/1.6/python-appapi-devguide-3/#31-setting-up-your-environment)  under "Setting up your environment"
- Set `STREAMS_DOMAIN_ID`, `STREAMS_INSTALL` and `STREAMS_INSTANCE_ID` environment variables
- Run the application:
`python3 edgentiot.py local [username] [password]`

### Submit to the Streaming Analytics service on IBM Cloud
- [Follow these steps](http://ibmstreams.github.io/streamsx.documentation/docs/python/1.6/python-appapi-devguide-2a/#21-setting-up-your-python-environment) under "Setting up your environment"
- Edit line 21 of  `read_from_edgent.py` with the name of your service. Check the `name`  attribute in your `credentials.cfg` file for this value.
- Edit line 19 with the path of the Streaming Analytics credentials file you downloaded earlier.  The path is relative to the current working directory.

Run the application:
`python3 edgentiot.py bluemix`

Once it is running, check the output of your Edgent application. You should see messages like this:

```
Message to Edgent from Streams:
Alert code: 249
{"name":"B","reading":{"N":50,"MIN":-2.762152936700155,"MAX":5.7895310031906675,"MEAN":2.239131357345944,"STDDEV":2.199931362960884}}
```
