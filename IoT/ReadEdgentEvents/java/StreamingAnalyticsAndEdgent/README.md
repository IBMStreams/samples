This project contains a Streams application written in Java. It shows how to connect to the Watson IoT platform to process events sent from an [Edgent](https://edgent.apache.org) application, and how to send commands to the Edgent application.

Detailed instructions on how to run this application are discussed in this recipe on developerWorks: [Connect Apache Edgent running on Raspberry Pi to the Streaming Analytics service](https://developer.ibm.com/recipes/tutorials/connect-apache-edgent-to-the-streaming-analytics-service-using-the-watson-iot-platform/)


Required Streams version: 4.1+

## How to run this sample

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
 
There are 3 applications in this project:
- `ReadFromWatsonIoT` is the simplest, it shows how to read events sent from Edgent
- `ReadAndSendCommands` demonstrates reading events and then sending commands back to Edgent in response
- `ComputeRollingAverage` shows how to do aggregation on the events sent from Edgent.


### Instructions

1. Import the project into Streams Studio
2. Follow the instructions in this recipe to [add needed jars to your classpath](https://developer.ibm.com/recipes/tutorials/connect-apache-edgent-to-the-streaming-analytics-service-using-the-watson-iot-platform/#r_step5).  See the section "Set up your Streams development environment".
3. (Optional) If you are running this application in the IBM Cloud, get the credentials for your Streaming Analytics service:
  - If you used the Streams IoT starter kit, the credentials are available by clicking **View All Credentials** > **Streams Credentials** >> **Streaming Analytics Service credentials**.  Copy the file contents and save them in a file on your local machine. 
  - If you did not set up the starter kit, See the section under ["Getting your Streaming Analytics Service Credentials"](https://developer.ibm.com/recipes/tutorials/connect-apache-edgent-to-the-streaming-analytics-service-using-the-watson-iot-platform/#r_step3) for steps.
 - Edit lines 18 and 19 of `SubmitToService.java` with the path to the credentials file you just created.
4. Pick one of the applications mentioned above and select "Run as" > "Java Application"
