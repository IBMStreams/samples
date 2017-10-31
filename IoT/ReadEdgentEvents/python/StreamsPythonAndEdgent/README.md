This project contains a Streams application written in Python that processes events sent from an [Edgent](https://edgent.apache.org) application. It shows how to send commands to the Edgent application.

First, you need an Edgent application sending data to the Watson IoT platform.
The easiest way to get started is to [deploy the Streams IoT starter kit](https://github.com/IBMStreams/streamsx.iot.starterkit). Click the **Deploy to Bluemix** button to set up all the needed services.

Once that is done, you are ready to run the applications.

1. Run the IoT sensors sample from Edgent:
  - Download your `device.cfg` file from the "View all Credentials" page of the starter kit.
  - Download Edgent, choose a binary release
  - Unpack Edgent, run the IoT sensors application:
    ```
    cd <edgent>/java8/scripts/connectors/iotp
     ./runiotpsensors.sh device.cfg
     ```

2. Submit the `IotPlatformBluemix` application to your Streams instance. Click "Tools" on the starter kit home page and click *Submit IoTPlatform Job* if it is not running.

 
3. Run the Streams application 
You need version 1.7 or greater of the streamsx package:
`pip install streamsx`


### Submit to a local instance of Streams: 
- [Follow these steps](http://ibmstreams.github.io/streamsx.documentation/docs/python/1.6/python-appapi-devguide-3/#31-setting-up-your-environment)  under "Setting up your environment"
- Set `STREAMS_DOMAIN_ID`, `STREAMS_INSTALL` and `STREAMS_INSTANCE_ID` environment variables
- Run the application: 
`python3 edgentiot.py local [username] [password]`

### Submit to the Streaming Analytics service on Bluemix 
- [Follow these steps](http://ibmstreams.github.io/streamsx.documentation/docs/python/1.6/python-appapi-devguide-2a/#21-setting-up-your-python-environment) under "Setting up your environment"
- Edit line 27 of  `mysa_creds.py` with the name of your service
- Edit line 29 of the same file to add the credentials of your service
Run the application: 
`python3 edgentiot.py bluemix`
