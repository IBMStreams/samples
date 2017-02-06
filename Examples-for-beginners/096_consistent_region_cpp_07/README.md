~~~~~~ Scala 
/* 
==========================================================================
Copyright (C) 2014-2015, International Business Machines Corporation
All Rights Reserved                                                 

This is one of many examples included in the "SPL Examples for
Beginners" collection to show the use of the consistent region feature
built into Streams 4.x and higher versions.

This particular example shows how a C++ primitive operator can play a role inside a
consistent region. Please look at the CPP interface (.h) and the implementation (.cpp) files
inside the SimpleSourceOp sub-directory in this SPL project. There are certain callback
functions that the C++ operator developer needs to implement the checkpoint and restore
state events. This example simulates the operator failure by 
aborting one of the operators in the flow graph automatically when the application is 
in the middle of executing the logic. By doing that, the core fault tolerance feature of
the consistent region will get triggered to recover from a failure that occurred in an 
application graph. It will prove that the tuples will not be missed during the course of the
unexpected operator failure and the subsequent recovery/restoration.

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

composite ConsistentRegion7 {
	graph
		// JobControlPlane operator is mandatory in applications with consistent regions.
		// Simply include it anywhere in your application graph.
		() as JCP = JobControlPlane() {}
		
		// We are going to declare a consistent region for our user created
		// C++ primitive source operator.
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
