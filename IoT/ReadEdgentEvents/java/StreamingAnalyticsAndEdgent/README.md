This project contains a Streams application written in Java. It shows how to connect to the Watson IoT platform to process events sent from an [Edgent](https://edgent.apache.org) application, and how to send commands to the Edgent application.

## Prerequisites
See [the prerequisites here](https://github.com/IBMStreams/samples/tree/master/IoT/ReadEdgentEvents).

Once you have all the prerequisites, you are ready to run the Streams application.

## Run the Streams application
Required Streams version: 4.1+

There are 3 applications in this project:
- `ReadFromWatsonIoT` is the simplest, it shows how to read events sent from Edgent
- `ReadAndSendCommands` demonstrates reading events and then sending commands back to Edgent in response
- `ComputeRollingAverage` shows how to do aggregation on the events sent from Edgent.


### Instructions

1. Import the project into Streams Studio
2. Follow the instructions in this recipe to [add needed jars to your classpath](https://developer.ibm.com/recipes/tutorials/connect-apache-edgent-to-the-streaming-analytics-service-using-the-watson-iot-platform/#r_step5).  See the section "Set up your Streams development environment".
3. Edit lines 18 and 19 of `SubmitToService.java` with the path to the file containing credentials for your Streaming Analytics service.
4. Pick one of the applications mentioned above and select "Run as" > "Java Application"


Detailed instructions on how this application was created are discussed in this recipe on developerWorks: [Connect Apache Edgent running on Raspberry Pi to the Streaming Analytics service](https://developer.ibm.com/recipes/tutorials/connect-apache-edgent-to-the-streaming-analytics-service-using-the-watson-iot-platform/)


Once it is running, check the output of your Edgent application. You should see messages like this:

```
Alert!...
{"name":"B","reading":{"N":50,"MIN":-2.762152936700155,"MAX":5.7895310031906675,"MEAN":2.239131357345944,"STDDEV":2.199931362960884}}
```
