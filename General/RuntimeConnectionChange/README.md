If you have an application that uses the Import and Export operators, you might want to change the export properties or import subscription.  
This application demonstrates how you can do so without having to recompile the application using the following SPL functions:
[setOutputPortExportProperties](http://www.ibm.com/support/knowledgecenter/en/SSCRJU_4.2.0/com.ibm.streams.toolkits.doc/spldoc/dita/tk$spl/fc$spl.utility.html?view=kc#spldoc_functions__setOutputPortExportProperties.T.uint32) and [setInputPortImportSubscription](http://www.ibm.com/support/knowledgecenter/en/SSCRJU_4.2.0/com.ibm.streams.toolkits.doc/spldoc/dita/tk$spl/fc$spl.utility.html?view=kc#spldoc_functions__setInputPortImportSubscription.rstring.uint32)
to dynamically change  connections between running applications. This allows applications to dynamically subscribe and unsubscribe to data without having to restart.


Go directly to the relevant parts of the application:
- [Change export properties](https://github.com/IBMStreams/samples/blob/master/General/RuntimeConnectionChange/application/FilterAndExport.spl#L28) and
- [Change import subscription](https://github.com/IBMStreams/samples/blob/master/General/RuntimeConnectionChange/application/Importers.spl#L75)

To run the application, simply compile Main.spl in Streams Studio or use the provided Makefile.

This application can also be run in the Streaming Analytics service in Bluemix. 

Read the [article on StreamsDev](https://developer.ibm.com/streamsdev/docs/changing-connections-runtime-export-import)   that discusses this concept in detail.

