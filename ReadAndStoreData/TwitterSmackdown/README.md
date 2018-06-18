
# Twitter Smackdown
This sample demonstrates how to:
* Read data from Twitter using Streams
* Track mentions of keywords
* Connect two applications using the `Export` and `Import` operators.
This sample contains 2 applications:

1) `TwitterStream.spl` which reads data from Twitter and exports a stream of tweets that can be consumed by any other job.
2) `Smackdown.spl` connects to the stream of tweets produced by the TwitterStream app. Then it ranks any three keywords of your choice by popularity, based on the number of Twitter statuses that mention each keyword. For example, the screenshot below shows the number of times each  of the keywords "red", "green" and "blue" were mentioned in the last 5 minutes.

   ![screen shot of red, green and blue](https://developer.ibm.com/streamsdev/wp-content/uploads/sites/15/2016/06/ConsoleCreateChart3.png)

This application is discussed in the [Streaming Analytics development guide](https://developer.ibm.com/streamsdev/docs/streaming-analytics-dev-guide/) on Streamsdev, where you will find instructions on compiling and running it.



