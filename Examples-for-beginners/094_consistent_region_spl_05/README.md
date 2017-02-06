~~~~~~ Scala 
/* 
==========================================================================
Copyright (C) 2014-2015, International Business Machines Corporation
All Rights Reserved                                                 

This is one of many examples included in the "SPL Examples for
Beginners" collection to show the use of the consistent region feature
built into Streams 4.x and higher versions.

This particular example shows how only a portion of the topology will take
part in the consistent region by having an autonomous section in the application
graph. This example simulates the operator failure by aborting one of the
operators automatically when the application is in the middle of executing the logic.
By doing that, the core fault tolerance feature of the consistent region will get
triggered to recover from a failure that occurred in an application graph.
It will prove that the tuples will not be missed and the Join operator's window state will not
be compromised during the course of the unexpected operator failure and the
subsequent recovery/restoration. At the same time, parts of the application that is
in the autonomous area will get duplicate tuples during a crash recovery happening in
the consistent region of this application graph. This example's purpose is to make the
users aware of this fact. In the autonomous area, measures need to be taken to
do deduplication.

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
   In the resulting output file in the data directory (result1.txt) will show you
   that there is no gap in the data. If that is the case , then the
   consistent region feature worked correctly during and after the forced
   crash of this application. At the same time, result2.txt will show you that
   there are duplicate tuples in the autonomous path of the graph.
==========================================================================
*/
namespace com.acme.test;

composite ConsistentRegion5 {
	graph
		// JobControlPlane operator is mandatory in applications with consistent regions.
		// Simply include it anywhere in your application graph.
		() as JCP = JobControlPlane() {}
		
		// We will have an operator driven consistent region for the Beacon operator.
		@consistent (trigger=operatorDriven)
		stream<int32 i> TestData = Beacon() {
			param
				iterations: 200;
				initDelay: 3.0;
				// For every 25 tuples, we will establish a consistent region.
				// triggerCount is the way to do that inside a Beacon.
				triggerCount: 25u;
				
			output
				TestData: i = (int32)IterationCount() + 1;
		}

		stream<int32 i, rstring str> TransformedData1 = Custom(TestData as TD) {
			logic
				state: {
					list<rstring>[5] _nyc = ["Manhattan", "Brooklyn", "Queens", "The Bronx", "Staten Island"];
				}
				
				onTuple TD: {
					mutable TransformedData1 oTuple = {};
					int32 idx = TD.i % 5;
					oTuple.i = TD.i;
					oTuple.str = _nyc[idx];
					submit(oTuple, TransformedData1);
				}
		}
		
		// In this operator, let us forcefully abort it when it is tuple #85.
		stream<TransformedData1> FinalData = Custom(TransformedData1 as TD) {
			logic
				onTuple TransformedData1: {
					if ((TD.i == 85) && (getRelaunchCount() == 0u)) {
						abort();
					} else {
						submit(TD, FinalData);
					}
				}		
		} 
		
		() as MySink1 = FileSink(FinalData) {
			param
				file: "result1.txt";
				flush: 1u;
		}
		
		
		// Let us make a portion of this application graph not to be part of the consistent region.
		// That can be done via the @autonomous annotation.
		@autonomous
		stream<int32 i, rstring str> TransformedData2 = Custom(TestData as TD) {
			logic
				state: {
					list<rstring>[5] _westchesterTowns = ["Hawthorne", "Millwood", "Yorktown", "Chappaqua", "Elmsford"];
				}
				
				onTuple TD: {
					mutable TransformedData2 oTuple = {};
					int32 idx = TD.i % 5;
					oTuple.i = TD.i;
					oTuple.str = _westchesterTowns[idx];
					submit(oTuple, TransformedData2);
				}
		}
		
		() as MySink2 = FileSink(TransformedData2) {
			param
				file: "result2.txt";
				flush: 1u;
		}
}

~~~~~~
