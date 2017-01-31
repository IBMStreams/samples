~~~~~~
/* 
==========================================================================
Copyright (C) 2014-2015, International Business Machines Corporation
All Rights Reserved                                                 

This is one of many examples included in the "SPL Examples for
Beginners" collection to show the use of the consistent region feature
built into Streams 4.x and higher versions.

This particular example shows how a Java primitive operator can play a role inside a
consistent region. Please look at the Java file in the impl/java/src sub-directory
in this SPL project. There are certain callback functions that the Java operator developer 
needs to implement the checkpoint and restore state events. This example simulates 
the operator failure by aborting one of the operators in the flow graph automatically 
when the application is  in the middle of executing the logic. By doing that, the core
fault tolerance feature of the consistent region will get triggered to recover from a 
failure that occurred in an application graph. It will prove that the tuples will not be
missed during the course of the unexpected operator failure and the subsequent 
recovery/restoration.

Initial Streams setup needed before running this example
---------------------------------------------------------
To use the consistent region feature, one must run the application in the
Distributed mode. Before that, certain configuration needs to be completed;
i.e. Streams checkpoint back-end related properties must be set. One can use
the file system or an external Redis infrastructure as a checkpoint back-end.
In this example, we will use the filesystem by setting the following
Streams instance properties:

streamtool setproperty instance.checkpointRepository=fileSystem -d <YOUR_DOMAIN_ID> -i <YOUR_INSTANCE_ID>
streamtool setproperty instance.checkpointRepositoryConfiguration={\"Dir\":\"/your/checkpoint/directory/here/\"}

Resolving Java errors you may see after importing this project
--------------------------------------------------------------
For this particular project, if you right click on this project
in Eclipse and select Properties, a dialog box will open up.
In that, click on "Java Build Path" in the left navigation bar.
On the right hand side, select the "Libraries" tab and you will
have to ensure that the following two required JAR files are added to this project.
If not, you have to add them using the "Add External Jars" button.
If they are already added and if they point to a different Streams installation
directory than yours, you have to delete them and add them fresh from your
Streams installation directory.
Those JAR files are added from the Streams installation directory.
i.e. $STREAMS_INSTALL/<STREAMS_VERSION>/lib

1) $STREAMS_INSTALL/<STREAMS_VERSION>/lib/com.ibm.streams.operator.jar
2) $STREAMS_INSTALL/<STREAMS_VERSION>/lib/com.ibm.streams.operator.samples.jar

Compile and Run
---------------
1) You can either compile this application inside the Streams Studio or from a Linux
   terminal window via the sc (Streams compiler) command.
 
2) You can launch from Streams Studio or submit via the streamtool command.
   In the resulting output file in the data directory (results.txt) will show you
   that there is no gap in the results. If that is the case , then the
   consistent region feature worked correctly during and after the forced
   crash of this application.
==========================================================================
*/
namespace com.acme.test;

composite ConsistentRegion9 {
	graph
		// JobControlPlane operator is mandatory in applications with consistent regions.
		// Simply include it anywhere in your application graph.
		() as JCP = JobControlPlane() {}
		
		// We are going to declare a consistent region for our user created
		// Java primitive source operator.
		@consistent(trigger=periodic, period=0.500)
		stream<int32 serialNumber, int32 productId, rstring productName, int32 quantity> ProductData = SimpleSourceOp() {
		}
		
		// We will forcefully fail this pass through operator exactly at tuple number 51
		stream<ProductData> SameProductData as SPD = Custom(ProductData as PD) {
			logic
				onTuple PD: {
					if ((PD.serialNumber == 51) && (getRelaunchCount() == 0u)) {
						abort();
					} else {
						submit(PD, SPD);
					}
				}
		}
		
		() as MySink1 = FileSink(SameProductData) {
			param
				file: "results.txt";
				flush: 1u;
		}
}

~~~~~~
