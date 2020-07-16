This project  demonstrates how to parse trip histories recorded in [GPS Exchange](http://www.topografix.com/gpx.asp) (GPX) format.  
It includes the GPXToTuple operator that produces a stream of tuples from a GPX file.  The project is discussed in this article on StreamsDev.

 The use of this operator is demonstrated in the ParseSingleGPXFile application that plots the trip history from a sample GPX file on a map. 

 
To compile the sample application, you need:

- Streams 4.0 or greater, for the  com.ibm.streams.geospatial toolkit
- The [MapViewer](https://github.com/IBMStreams/samples/tree/master/Geospatial/MapViewerSample) sample to visualize the data
- [Thie inet toolkit v2.5 or greater](https://github.com/IBMStreams/streamsx.inet/releases)
  
  
To see this sample in action, compile and submit the main application to a streams instance, optionally specifying the port for the web application.  The default port is 8081.
Once the application is running, in a browser, open the following URL:  http://PEHost:PORT/map/map.html
 
