## Quick start samples
This project includes sample applications to help you get started quickly with Streams.
It contains 2 applications:

#### `TradesAppMain`
This is the sample application from the [Streams Quick Start Guide](https://developer.ibm.com/streamsdev/docs/streams-quick-start-guide/), published on StreamsDev.
It processes stock trades from a CSV file.  The application will filter out some of the stocks based on ticker names.  It will then calculate the average, maximum and minimum ask price for each of the stocks.  The results will be written to a file.
 
#### TradesAppCloud 
This is a starter application that is ready to be deployed to the Streaming Analytics service on Bluemix.  It is the same as the TradesApp sample discussed in the Quick Start guide, but it has been adapted for the cloud.  The results are not written to a file but to a console.  

#### Run the TradesAppCloud on the Streaming Analytics service in Bluemix
You can download the sab file from the `starterApp` folder and upload it to your instance of the service.
Once the job is running, you can look at the console to see the output, or create a view to see the data as it is processed by the application.


#### Running on a  local Streams installation
If you have Streams installed, follow [these instructions](https://developer.ibm.com/streamsdev/docs/studio-quick-start/#launching) to import the project into Streams Studio.
From the Project Explorer, expand the "application" node, select TradesAppCloud or TradesAppMain, right click, and select Launch> Launch Active Build Config to Running Instance
Click "Launch" in the dialog that appears.
The `TradesAppCloud` application will print output to the console.
You can see the output of `TradesAppMain` in data/tradesSummary.csv.

