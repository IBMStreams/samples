This application is discussed in [this article on StreamsDev](https://developer.ibm.com/streamsdev/docs/integrating-ibm-streams-apache-nifi/).  It demonstrates how you can use the `HTTPBlobInjection` operator to receive files and their metadata from Apache Nifi. Files are sent from Nifi using the `PostHTTP`  processor. The file and its metadat are then ingested by Streams using the `HTTPBLOBInjection` operator in the inet toolkit. 

# Prerequisites

- IBM Streams 4.x.
- Version 2.8.2 or greater of the [streamsx.inet toolkit](https://github.com/IBMStreams/streamsx.inet).
- Apache Nifi version 1.4+

# Run the sample

1. Download and build the most recent version of the [streamsx.inet toolkit](https://github.com/IBMStreams/streamsx.inet).
2. Import this sample project to Streams Studio.
3. Build the project and run the application. This will begin to wait for data from Nifi.
4. Follow the [instructions in the article](https://developer.ibm.com/streamsdev/docs/integrating-ibm-streams-apache-nifi/) to create a Nifi flow using the `PostHTTP` processor to send data to Streams.
