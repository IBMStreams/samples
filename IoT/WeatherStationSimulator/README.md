## Weather Station Simulator

This is an [Apache Edgent](https://edgent.apache.org) application that sends simuated weather data to the Watson IoT platform for analysis by Streams.  The application simulates 20 weather stations in the cities of Toronto and Markham.  Every 0.5 seconds a reading is sent to the platform for a station, in a round robin sequence. Readings are in JSON and have the following format:

``` java
{"id":"tor_3", //station id 
 "temp":75.65775475394342, //temperature in Farenheit
"location":"tor", //location id
"lat":43.685, //station latitude and longitude
"lon":-79.347
}

```

### Run the simulation

#### Register a device with the Watson IoT Platform
To run this application, you need to register a device with the IoT platform. This can be done manually, or you may already have a registered device.
- *If you deployed the Streams IoT Starter Kit*, then you already have a registered device. From your starter kit home page, click "View all Credentials" and in the "Edgent Credentials" tab, Click "Download device.cfg" and save the file.
- *Otherwise, manually register a device* by [following these instructions](https://developer.ibm.com/recipes/tutorials/how-to-register-devices-in-ibm-iot-foundation/).

#### Run the application:

`./start-weather-simulations.sh path-to-your-device-cfg-file [run_time_in_minutes]`

Use `run_time_in_minutes` to run the application for a specified time. This is because the Watson IoT Platform places a limit on the amount of data that can be transferred for free plans.
If ommitted, the application will run for 10 minutes. You can re-run the application and configure it to run until manually terminated by specifying `0` as the timeout:
` ./start-weather-simulations.sh device.cfg 0`

### Extend the sample
First, download and unpack [Edgent](https://edgent.apache.org/docs/downloads.html).

#### Build with Ant:

An Ant build file is provided. To use it to modify the sample, you need to

- Edit the `build.xml` file at line 8 to set the value of `EDGENT_HOME` to the `EDGENT_HOME/java8` directory. 
- Run `ant` when you are ready to recompile. This will compile and re-package the `weather-station-simulator.jar` file.

Then to re-run the application, you can run the same script as mentioned above.

#### Build with Eclipse:
- Import the project into Eclipse.
- Add these jars to the project classpath:
  - `$EDGENT_HOME/java8//lib/edgent.providers.iot.jar`,
  - `$EDGENT_HOME/java8//connectors/iotp/lib/edgent.connectors.iotp.jar`,
  - `$EDGENT_HOME/java8//connectors/iot/lib/edgent.connectors.iot.jar`,
  - `$EDGENT_HOME/java8//ext/slf4j-jdk14-1.7.12.jar`
  
Build and run the `WeatherStationSimulator` class.