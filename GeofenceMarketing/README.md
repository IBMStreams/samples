This sample demonstrates how you can use the Geofence operator from the com.ibm.streams.geospatial toolkit for a smart
marketing campaign.  

This application has a custom operator that randomly generates cell phone user GPS locations around a shopping center North of Toronto.
The application has also sent up a geofence around the shopping center that we are interested in running the smart marketing campaign for.
As cell phone users are detected to enter the shopping center, the application sends targeted promotions to the users.
 
To see this sample in action, compile and submit the main application to a streams instance.
 
To compile the application, you need:

- Streams 4.0 - for the new com.ibm.streams.geospatial toolkit

- com.ibm.streamsx.inet toolkit v2.5 prerelease from here: https://github.com/IBMStreams/streamsx.inet/releases
  
In a browser, open the following URL:  http://PEHost:8080/map/map.html
 
To see popup on the map, open the following URL:  http://PEHost:8080/map/map.html?popup=true
