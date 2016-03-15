This is one of the sample applications discussed in this [article](https://developer.ibm.com/streamsdev/2016/03/14/real-time-text-analysis/).  It loads and runs the extractor created in the [BigInsights Starter Kit](https://ibm-open-platform.ibm.com/biginsights/starterkits/biginsights-starter-kit2-cloud/starterkit2cloud.html#/starterkit).

To compile the application, you'll need:
- The com.ibm.streams.text toolkit in Streams 4.1
- [Version 1.0](https://github.com/IBMStreams/streamsx.json/releases) or greater of the com.ibm.streamsx.json toolkit.

Before running the application, you also need to set the `TEXTANALYTICS_HOME` environment variable to `<STREAMS_INSTALL>/toolkits/com.ibm.streams.text/lib/TextAnalytics`. 
If running the application in distributed mode, follow these steps to add an environment variable to an instance from the Streams console:
-  From the Management Dashboard, select the instance, and click  "Manage Instance",
-  Click "Add a variable" under "Application Environment Variables.
After setting the environment variable, submit the job.  Check the logs of the `PrintResults` operator to see the results of running the application.  



