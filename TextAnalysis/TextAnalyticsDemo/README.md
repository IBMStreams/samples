
This is the sample application discussed in this article: https://developer.ibm.com/streamsdev/docs/real-time-text
The application demonstrates how you can create a Streams application that analyzes text in real time using the IBM BigInsights Text Analytics web tool and the com.ibm.streams.text toolkit.
It analyzes incoming text data for mentions of Streams and other related technologies, displaying the search results in  a web browser.

Before you can run the demo, you'll need to create and export an extractor in the BigInsights Text Analytics web tool by following the steps outlined in the video.

Then, to run the application, you'll need:
- The extractor you created
- The com.ibm.streams.text toolkit in Streams 4.1
- The [version 2.5 prerelease](https://github.com/IBMStreams/streamsx.inet/releases) or greater of the com.ibm.streamsx.inet toolkit.


Compile and submit the main application to a streams instance, optionally specifying the port parameter. The default is 9899.

In a browser, open the following URL:  http://<pe_host>:<port>/textAnalytics to view the app, where port is the parameter you specified in the previous step.
