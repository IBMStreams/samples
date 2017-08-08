This project contains a Streams application written in Java. It shows how to connect to the Watson IoT platform to process events sent from an [Edgent](https://edgent.apache.org) application, and how to send commands to the Edgent application.
The application is discussed in this recipe on developerWorks: [Connect Apache Edgent running on Raspberry Pi to the Streaming Analytics service](https://developer.ibm.com/recipes/tutorials/connect-apache-edgent-to-the-streaming-analytics-service-using-the-watson-iot-platform/)


To run this application, you need:
 1. Credentials for the Watson IoT Platform and an instance of the Streaming Analytics service on Bluemix.
 2. An Edgent application generating data.  Learn how to [create an Edgent application that generates events](https://developer.ibm.com/recipes/tutorials/send-events-to-the-watson-iot-platform-from-a-raspberry-pi-running-apache-edgent/).
 3. Submit the `IotPlatformBluemix` application to your Streams instance.  Step 4 of the recipe discusses [how to submit the `IotPlatformBluemix` application](https://developer.ibm.com/recipes/tutorials/connect-apache-edgent-to-the-streaming-analytics-service-using-the-watson-iot-platform/#r_step4).
 
There are 3 applications in this project:
- `ReadFromWatsonIoT` is the simplest, it shows how to read events sent from Edgent
- `ReadAndSendCommands` demonstrates reading events and then sending commands back to Edgent in response
- `ComputeRollingAverage` shows how to do aggregation on the events sent from Edgent.


### Instructions

- Import the project into Streams Studio
- Add the following jars to your classpath:
   - STREAMS_INSTALL/toolkits/com.ibm.streamsx.topology/lib/com.ibm.streamsx.topology.jar
   - STREAMS_INSTALL/toolkits/com.ibm.streamsx.iot/lib/com.ibm.streamsx.iot.jar
   - STREAMS_INSTALL/lib/com.ibm.streams.operator.samples.jar
   - STREAMS_INSTALL/toolkits/com.ibm.streamsx.datetime/lib/com.ibm.streamsx.datetime.jar
- Get the credentials for your Streaming Analytics service. Save them in a file on your local machine. See the section under ["Getting your Streaming Analytics Service Credentials"](https://developer.ibm.com/recipes/tutorials/connect-apache-edgent-to-the-streaming-analytics-service-using-the-watson-iot-platform/#r_step3) for steps.
- Edit lines 18 and 19 of `SubmitToService.java` with the path to the credentials file you just created.
- Pick one of the applications mentioned above and select "Run as" > "Java Application"
