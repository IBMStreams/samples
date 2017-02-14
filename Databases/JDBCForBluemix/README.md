This sample SPL application is discussed in [this article on StreamsDev](https://developer.ibm.com/streamsdev/docs/developing-streams-applications-with-the-jdbcrun-operator/).  It demonstrates how to store analyzed data into a database using the JDBCRun operator.  The application first uses a HTTPGetStream operator from the inet source toolkit to retrieve 48 hours weather information in Markham by using the REST end point http://weatherappkandu.mybluemix.net/rest/weather/48hours_xml.    After analyzing the weather data , the JDBCRun operator is used to insert the data in to the target database. 

Steps to deploy the sample application on Bluemix 

1. Import this sample project to streams studio.

2. Go to https://console.ng.bluemix.net/home/ and sign up for a free trial account.

4. From the service catalog section, create two services. Streaming Analytics Standard and dashDB for Analytics Entry.

5. Obtain the JDBC connection URL, user and password information from the dashDB service credential section.

6. Update the JDBC connection URL, user and password information from the SPL source code and compile it.

6. Create the following table from the dashDB dashboard ddl interface.
create table weather(maxtemp int, mintemp int, maxtempfeelslike int, mintempfeelslike int, avgvisibility int, avgwindspeed int, decision char(150))

7. Compile the SPL application. 

8. You can simply deploy the this SPL application on your locals streams instance or submit the .sab file to the Streaming Analytics Service you created from step 2.

9. After the job becomes healthy, you should be able to see tuples being stored in the weather table you created in step 5 from dashDB service's monitoring console.


To compile this application on Streams 4.0 and 4.1, you need the following toolkits: 
* [com.ibm.streamx.inet](https://github.com/IBMStreams/streamsx.inet) [2.0.2]
* [com.ibm.streamsx.jdbc](https://github.com/IBMStreams/streamsx.jdbc) [1.0.0]

You do not need any additional toolkits for Streams 4.2+.
