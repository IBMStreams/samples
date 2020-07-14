# SPL sample application for Edgent data.
This project shows a Streams application written in SPL that processes events sent from an [Edgent](https://edgent.apache.org) application. It also shows how to send commands to the Edgent application.
The source for this application is in the IoT toolkit.

## Prerequisites
See [the prerequisites here](https://github.com/IBMStreams/samples/tree/main/IoT/ReadEdgentEvents).

Once you have all the prerequisites, you are ready to run the Streams application.

- Go to the [releases page](https://github.com/IBMStreams/streamsx.iot/releases) and download `com.ibm.streamsx.iot.sample.edgent.IotpSensors.sab`
- Submit the application to your Streams instance
- You should see output in your Edgent application:

```
Alert!B=2.04151232298089
```
The `Alert` messages are sent by the `IotpSensors` application you just submitted.

[SPL source for the  sample application](https://github.com/IBMStreams/streamsx.iot/tree/develop/samples/Edgent)
