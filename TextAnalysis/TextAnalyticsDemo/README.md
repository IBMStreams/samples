This project contains the ProductSearch application referred to in this [article](https://developer.ibm.com/streamsdev/2016/03/14/real-time-text-analysis/) on StreamsDev.
It analyzes incoming text data for mentions of Streams and other related technologies, displaying the search results in  a web browser.
It demonstrates how you can create a Streams application that analyzes text in real time using the IBM BigInsights Text Analytics web tool and the com.ibm.streams.text toolkit.

Update for Streams 4.2:
A new application has been added to demonstrate the external resource update feature added in Streams 4.2.  The new application is called ProductSearch with update.  
See this [article][https://link] to learn more about the new feature.

+ Running the applications 
To compile and run these applications you'll need: 
- The [version 2.5 prerelease](https://github.com/IBMStreams/streamse.inet/releases) or greater of the com.ibm.streamsx.inet toolkit.
For the ProductSearch application, you'll need the com.ibm.streams.text toolkit from Streams 4.1 or later, while the ProductSearchWithUpdate application requires Streams 4.2 or later.

Once the application is compiled and you have submitted the job, open the following URL in a browser: `http://<pe_host>:9899/textAnalytics` to view the output.
