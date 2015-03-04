This sample demonstrates how you can display geospatial data onto a map using the HTTPTupleView operator, and OpenLayer APIs.  

This application has two custom operators that randomly generates geospatial locations of two types of entities around Hong Kong:

- Taxi entities are represented by red markers on the map.

- Public minivan entities are represented by green markers on the map
 
To see this sample in action, compile and submit the main application to a streams instance.
 
To compile the application, you need:

- Streams 3.2.2 Beta - for the new com.ibm.streams.geospatial toolkit

- com.ibm.streamsx.inet toolkit from here: [https://github.com/IBMStreams/streamsx.inet/releases/tag/streams_3.2.2beta-fix1]
  
In a browser, open the following URL:  http://<PEHost>:8080/map/map.html
 
To see popup on the map, open the following URL:  http://<PEHost>:8080/map/map.html?popup=true
