~~~~~~
/*
================================================================================
This example shows an important feature of Streams that is made possible by one of 
our open source toolkits (com.ibm.streamsx.inet). Couple of operators from this toolkit
will help in the integration of Streams applications with your web apps running
on Internet browsers. This toolkit was fully developed by our Streams colleague
Dan Debrunner who then contributed it to the Streams open source efforts. Many thanks
and credits are due to Dan and two of our other colleagues (Martin Siegenthaler
and Kris Hildrum).

If there is a need to send data from a web app into your Streams application
and do vice versa (i.e. sending data from your Streams application to a web app),
you can use couple of operators from this fantastic toolkit. These operators
include an embedded Jetty lightweight web server. This example contains very
simple SPL code to show how you can use those operators to attach a web
front-end to a Streams application whenever it is desired. Not all Streams
application results may be suitable for integrating with web apps due to the
speed mismatch. So, put the power of this toolkit to your business advantage when
the situation permits.

This example shows a very basic built-in HTML GUI. However, one can build very
sophisticated custom web front-end to plot Streaming results using third party 
JavaScript libraries such as d3js, dygraph etc.

In order to run this example, you must do the following.

1) Download an open source release version of the com.ibm.streamsx.inet toolkit from here:
   https://github.com/IBMStreams/streamsx.inet/releases
   
   * download the Inet toolkit 2.5 for Streams 4.x or a later version.

2) Unzip the downloaded toolkit tgz file into a directory in your Streams machine.
   (You can unzip it in any sub-directory you want inside your home folder.)
   
   * After unzipping, change directory to your newly unzipped toolkit directory and run the following command there:
   
     touch toolkit.xml

3) Start Streams Studio (if not running already) and go to the Streams Explorer view.
   
   * Right-click on "InfoSphere Streams Installations-->InfoSphere Streams x.y.z.a-->Toolkit Locations" and select "Add Toolkit Location".
   * In the resulting dialog box, click on the "Directory" button and browse to the root folder of the toolkit that you unzipped above.
     (e-g: /home/myuserdir/com.ibm.streamsx.inet)
   * Click OK to close the dialog box.

4) Now, in your Streams Studio's project explorer view, expand this example project (089_integrating_streams_apps_with_web_apps).
   Right-click on Dependencies and select "Edit Dependencies" to add the
   com.ibm.streamsx.inet toolkit version you added above as a dependency for our example SPL project.

5) Now, uncomment the block comments in the SPL code below. (in two different places as
   marked below) and save the SPL file. Ensure that the SPL code is successfully built.

6) Browse the code to understand the simple logic.

7) Run this application by using its Standalone configuration.

8) Now open a web browser either on your Streams server or on some other
   machine (You should have network connectivity from the browser to your Streams server machine.)

9) Create two different tabs in your web browser so that we can view two
   different web pages (one for the HTTPTupleInjection operator and the other for
   the HTTPTupleView operator).

10) In the first tab, go to this URL:
	http://localhost:8077/Request/ports/output/0/form
	[Please ensure that the host and port fields in the URL match your machine name and port number.]
	
	In the resulting HTML form, you can input two integer operand values and
	one of the four arithmetic operations (add, subtract, multiply, divide) as 
	a string value and push the submit button.
	These three input values will be sent from the browser to our
	Streams app (in particular to the HTTPTupleInjection operator we used below).
  
11) In the second tab, go to this URL:
    http://localhost:8077/streamsx.inet.resources/dojo/viewall.html
    [Please ensure that the host and port fields in the URL match your machine name and port number.]
    
    In the resulting page, you can either click on the "Live Data" hyper link to see the results
    sent by the Streams app (in particular by our HTTPTupleView operator) in an automatically
    updated human readable HTML table or click on the hyper link under the "JSON Data" column for
    getting the results in a machine readable JSON format.
    
    [See the additional comments appearing above the HTTPTupleView operator code below for more URL options to
     view the data held inside the operator window.] 

12) You can switch to the first tab and send more input values for
    different arithmetic operations. You will be able to see all the results in
    the second tab in an automatically updated HTML table or as JSON data.
    
    Using this basic idea, you can build your own sophisticated
    web applications to do a bidirectional integration with Streams applications. 
================================================================================
*/

namespace com.acme.test;

// 1) Uncomment the following block comment in order to run this application after you have configured the com.ibm.streamsx.inet toolkit.
/*
use com.ibm.streamsx.inet.rest::*;
*/

composite WebCalculator {
	type
		MyRequest = tuple<int32 x, int32 y, rstring operation>;
		MyResponse = MyRequest, tuple<int32 result>;
	
	graph
		// This is a dummy operator placed here to avoid
		// SPL compiler errors when the main logic of this application is
		// fully commented out. Simply leave this operator here and ignore it forever.
		stream<int32 x> Dummy = Beacon() {
			param
				iterations: 1u;
		}
		
		// 2) Uncomment the following block comment in order to run this application after you have configured the com.ibm.streamsx.inet toolkit.
		/*
		// Use this URL to talk to this operator from a web browser:
		// http://localhost:8077/Request/ports/output/0/form
		// [Please ensure that the host and port fields in the URL match your machine name and port number.]
		//
		//
		// This operator from the streamsx.inet toolkit will allow us to
		// send data in CSV format from a browser form into a Streams application.
		stream<MyRequest> Request = HTTPTupleInjection() {
			param
				port: 8077;
				
			config
				// Ensure that the HTTP operators are in a single PE to have a single web-server
				placement: partitionColocation("MyWeb");
		} 
		
		// Consume the data sent by the browser app and do simple arithmetic calculations.
		stream<MyResponse> Response = Custom(Request) {
			logic
				onTuple Request: {
					mutable int32 myResult = 0;
					
					if (Request.operation == "add") {
						myResult = Request.x + Request.y;
					} else if (Request.operation == "subtract") {
						myResult = Request.x - Request.y;
					} else if (Request.operation == "multiply") {
						myResult = Request.x * Request.y;
					} else if (Request.operation == "divide") {
						myResult = Request.x / Request.y;
					}
					
					mutable Response oTuple = {};
					assignFrom(oTuple, Request);
					oTuple.result = myResult;
					submit(oTuple, Response);
				}
		}

		// This operator from the streamsx.inet toolkit will allow us to send the results
		// stored in a window to a browser application.
		// We are storing the last 10 calculation results in this operator's sliding window.
		// Also note that the window defined below is partitioned with the partitionKey being one or more
		// attributes present in the incoming stream (e-g: partitionKey: "attribute1", "attribute3", "attribute4";
		// Use this URL to talk to this operator from a web browser:
		// http://localhost:8077/streamsx.inet.resources/dojo/viewall.html
		// [Please ensure that the host and port fields in the URL match your machine name and port number.]
		//
		// 1) In the resulting page for that URL, you will see different options to query the HTTPTupleView window contents as
		// a raw HTML table, a live HTML table and a pure JSON formatted string.
		//
		// 2) In the resulting page for that URL, if you click on the hyperlink to view the pure JSON formatted result,
		// it will by default display all the results for all the window partitions. If you have a partitioned window and 
		// if you want to view the results belonging to only a particular partition, then you can click on the hyperlink
		// to view the pure JSON formatted result and add the following query string to the end of that URL.
		// ?partition=xyz&partition=abc&partition=rst
		// e-g: ?partition=divide     [Adding this query string to that URL will fetch the results from the 
		// window for all the divide operations performed.]
		//
		() as WebSink1 = HTTPTupleView(Response) {
			window
				Response: sliding, count(10), count(1), partitioned;

			param
				port: 8077;
				// We will use the operation attribute in the incoming stream as a partitionkey.
				partitionKey: "operation";
			
			config
				// Ensure that the HTTP operators are in a single PE to have a single web-server
				placement: partitionColocation("MyWeb");
		}
	 	*/
}

~~~~~~
