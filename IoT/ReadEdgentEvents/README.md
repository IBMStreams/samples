This project contains Streams applications written in Java, SPL and Python. It shows how to connect to the Watson IoT platform to process events sent from an [Edgent](https://edgent.apache.org) application, and how to send commands to the Edgent application.

**If you have already deployed the Streams IoT Starter Kit and completed the setup steps to run a sample, you can skip the prerequisites section below**. Your next step is to run a sample in the language of your choice.

- [Run the SPL sample](https://github.com/IBMStreams/samples/tree/main/IoT/ReadEdgentEvents/spl)
- [Download the Java](https://streams-github-samples.mybluemix.net/?get=IoT%2FReadEdgentEvents%2Fjava%2FStreamingAnalyticsAndEdgent) or [Python sample](https://streams-github-samples.mybluemix.net/?get=IoT%2FReadEdgentEvents%2Fpython%2FStreamsPythonAndEdgent%2F)

## Prerequisites

- An instance of the Watson IoT platform and the Streaming Analytics service
- An Edgent application or other application that is registered with and sending events to the Watson IoT platform.
- `IotPlatformBluemix` or `IotPlatform` application running in your Streams instance
- Java and Python: Credentials for your Streaming Analytics service in a file.


The easiest way to get started is to [deploy the Streams IoT starter kit](https://github.com/IBMStreams/streamsx.iot.starterkit). Click the **Deploy to IBM Cloud** button to set up all the needed services.  Then, [follow these steps](https://github.com/IBMStreams/streamsx.iot.starterkit/blob/develop/README.md#run-a-complete-edgent-streams-scenario)) to run an Edgent application that will send events to the Watson IoT Platform.

If you choose not to deploy the starter kit, follow [steps 1-3 of this recipe](https://developer.ibm.com/recipes/tutorials/connect-apache-edgent-to-the-streaming-analytics-service-using-the-watson-iot-platform/).


### Credentials for the Streaming Analytics service

If you used the Streams IoT starter kit, the Streaming analtyics service credentials are available by clicking **View All Credentials** > **Streams Credentials** >> **Streaming Analytics Service credentials** >> **Download credentials as JSON**. Save the `credentials.cfg` file on your local machine.

If you did not set up the starter kit, See the section under ["How to get your Streaming Analytics Credentials"](https://developer.ibm.com/streamsdev/docs/setup-instructions-connecting-edgent-streams-applications-watson-iot-platform/#sacreds) for steps.

Once you have all the prerequisites, you are ready to run the Streams application. Samples for each language are in their respective folders.
