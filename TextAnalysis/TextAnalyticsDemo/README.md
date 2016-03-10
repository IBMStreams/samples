
This project contains the ProductSearch application referred to in this [article](https://developer.ibm.com/streamsdev/docs/real-time-text) on StreamsDev.
It analyzes incoming text data for mentions of Streams and other related technologies, displaying the search results in  a web browser.
It demonstrates how you can create a Streams application that analyzes text in real time using the IBM BigInsights Text Analytics web tool and the com.ibm.streams.text toolkit.

+ Running the application in the Streaming Analytics Service on Bluemix
The `bluemix` folder in this project contains a pre-built Streams application bundle file that is ready to be submitted to an instance of the Streaming Analytics service.
To run the application you simply submit the .sab file in the Streams Console. You can verify the results in the Streams Console by checking the Log Viewer of the PE containing the `OutputPrinter` operator.  

+ Running the application locally
To compile and run the application, you'll need:
- The com.ibm.streams.text toolkit in Streams 4.1
- The [version 2.5 prerelease](https://github.com/IBMStreams/streamsx.inet/releases) or greater of the com.ibm.streamsx.inet toolkit.
Once the application is compiled and you have submitted the job, open the following URL in a browser  `http://<pe_host>:9899/textAnalytics` to view the running application.
