
## Bus Alerts sample application
This sample shows how you could create a smart advertising program to display alerts and ads in public transit buses. As buses move through the city, they periodically report their current location to this application. When a bus is near a point of interest, such as an area with a safety alert or a business with an advertisement, the application will detect this and send the alert.


Although the application retrieves live bus location data from the [NextBus service](https://www.nextbus.com) the alert system is not real, so the alerts for each bus are currently just printed.

Points of interest are specified in `data/poi.csv` within this folder.
For example, if a bus comes within 1km of the Golden Gate Bridge in San Francisco, the application will produce this output:

      Bus 5517:sf-muni:nextbus is near Golden Gate Bridge, this message will be displayed in the bus: Approaching Golden Gate Bridge, pedestrian bridge is closed.



This sample includes 2 versions of this application, one using cached data in a file, and another that actually connects to NextBus to connect to a live stream of bus locations.

These applications are in `my.name.space/BusAlerts_CachedData.spl` and `my.name.space/BusAlerts_LiveData.spl`, respectively.


If you submit the live application it will connect to NextBus. If you use the cached data it will use cached location data in `data/saved_BusLocations.txt`.
## Launching the sample

Import the sample into Atom or VS Code, select the file you wish to run, and  select **Build and submit job**.

When submitting the application you can set the `bus-agency` parameter to one of the municipalities that uses NextBus data. The default is `sf-muni`for San Francisco, and the points of interest in `data/poi.csv` are all in San Francisco.


## Viewing the results

From the Streams Console, find the job you submitted, hover over the `AlertPrinter` operator and click **View Operator Log**:

![Image showing how to view results of the app](/Users/natashad/Documents/dev/git/streamsx.documentation/images/viewresults.gif)


## How it works

![application graph](/Users/natashad/Documents/dev/git/streamsx.documentation/images/graph.png)

This description applies to the `BusAlerts_LiveData` application. The `BusAlertsCachedData` application works the same way except it does not connect to NextBus.
1. The `POI_FromFile` stream containing points of interest is created from the data/poi.csv file using a `FileSource`.
2. Using the `HTTPGetXMLContent` operator, the `RawData_Live` stream is created by polling the NextBus service every 30 seconds. This stream contains the current location of all the buses in the region. This returns the bus locations in XML.
3. The `ParseNextBusData` composite uses the `XMLParse` operator to parse the data and creates a stream of tuples, one for each bus. Each tuple has the following attributes: `rstring id,TimeMillis reportTime, float64 latitude, float64 longitude`.
4. The points of interest from step 1 and the stream of bus locations from step 3 are used in `Custom` operator. The operator uses the [`distance`](https://www.ibm.com/support/knowledgecenter/SSCRJU_4.3.0/com.ibm.streams.toolkits.doc/spldoc/dita/tk$com.ibm.streams.geospatial/fj$com.ibm.streams.geospatial.st.html) function from the geospatial toolkit to determine if the bus is near the point of interest.  If a bus is near a POI it submits a tuple containing the message that should be sent to the bus. This message is sent on the `BusesToAlert` stream.
5. The `AlertPrinter` operator receives the message and prints the message to the console.
