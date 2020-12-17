### REST Endpoints Sample

This sample demonstrates how to use the `EndpointSink` and `EndpointSource` operators to send and receive data in a Streams application by REST requests.

The demo application is a simple calculator that ingests two numbers and returns the result of adding the 2 numbers.
The data is received via the `EndpointSource` operator and the result is sent to the `EndpointSink` operator.

See this [tutorial for more information about this sample](https://community.ibm.com/community/user/cloudpakfordata/viewdocument/enable-data-ingest-and-retrieval-vi?CommunityKey=c0c16ff2-10ef-4b50-ae4c-57d769937235&tab=librarydocuments) and how to run it.

## Contents

- sample/BasicCalculator.spl: This application is the calculator without any REST endpoints. You can follow the tutorial to see how to modify it to enable REST.
- sample/CalculatorRESTService.spl: This is the completed application with REST endpoints.
  
- sample_client.ipynb - Python notebook to use to connect to the Streams application once it is running.
  