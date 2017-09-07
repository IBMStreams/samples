This project contains a Streams application written in Python that processes events sent from an [Edgent](https://edgent.apache.org) application. It shows how to send commands to the Edgent application.

Follow the [instructions in this recipe](https://developer.ibm.com/recipes/tutorials/connect-apache-edgent-to-the-streaming-analytics-service-using-the-watson-iot-platform/)
to generate events from Edgent.
To  instructions on how to run this application are discussed in this recipe on developerWorks: 

To run this application, you need:
 1. Credentials for the Watson IoT Platform and an instance of the Streaming Analytics service on Bluemix. [See step 3 of the recipe](https://developer.ibm.com/recipes/tutorials/connect-apache-edgent-to-the-streaming-analytics-service-using-the-watson-iot-platform/#r_step3)
 2. An Edgent application generating data.  Learn how to [create an Edgent application that generates events](https://developer.ibm.com/recipes/tutorials/send-events-to-the-watson-iot-platform-from-a-raspberry-pi-running-apache-edgent/).
 3. Submit the `IotPlatformBluemix` application to your Streams instance.  Step 4 of the recipe discusses [how to submit the `IotPlatformBluemix` application](https://developer.ibm.com/recipes/tutorials/connect-apache-edgent-to-the-streaming-analytics-service-using-the-watson-iot-platform/#r_step4).
 
## Instructions
You need version 1.7 or greater of the streamsx package:
`pip install streamsx==1.7b1`

Update your PYTHONPATH to include the new 

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
