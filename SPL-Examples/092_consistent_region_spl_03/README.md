~~~~~~
/* 
==========================================================================
Copyright (C) 2014-2015, International Business Machines Corporation
All Rights Reserved                                                 

This is one of many examples included in the "SPL Examples for
Beginners" collection to show the use of the consistent region feature
built into Streams 4.x and higher versions. 

This particular example shows how every single SPL based operator in an application
graph will take part in the consistent region. In this example, we will see 
how an aggregate operator will protect the window contents during a failure.
This example simulates the operator failure by aborting that operator automatically
when the application is in the middle of executing the logic. In fact, this example
will simulate multiple crashes of the Aggregate operator that holds an active window.
By doing that, the core fault tolerance feature of the consistent region will get triggered to
recover from a failure that occurred in an application graph. It will prove that the
tuples will not be missed and the window contents will not be
compromised during the course of the unexpected operator failure and the
subsequent recovery/restoration. 


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
   In the resulting output file in the data directory will show you that there is no
   gap in the aggregated values. If that is the case , then the
   consistent region feature worked correctly during and after the forced
   crash of this application.
==========================================================================
*/
namespace com.acme.test;

composite ConsistentRegion3 {
	graph
		// For the consistent region feature to work, there must be a 
		// Job Control Plane (JCP) operator inside a given graph.
		() as JCP = JobControlPlane() {}
		
		// Start a periodic consistent region.
		@consistent(trigger=operatorDriven)
		stream<int32 i> TestData = Beacon() {
			param
				iterations: 1000000;
				// Establish consistency (drain and checkpoint) for every 5000 tuples.
				triggerCount: 5000u;
				
			output
				TestData: i = (int32)IterationCount() + 1;
		}
		
		// In this operator, we are simply going to forward the incoming TestData tuples.
		// However, we will inject a WindowMarkert punctuation after every 1000 tuples.
		stream<TestData> ProcessedTestData = Custom(TestData as TD) {
			logic
				onTuple TD: {
					// Forward the incoming tuple as it is.
					submit(TD, ProcessedTestData);
					
					// Send a window marker punctuation for every 1000 tuples.
					if (TD.i % 1000 == 0) {
						submit(Sys.WindowMarker, ProcessedTestData);
					}					
				}
		}
		
		stream<int32 min, int32 max, int32 avg> AggregatedTestData = Aggregate(ProcessedTestData as PTD) {
			logic
				onTuple PTD: {
					// We will forcefully abort this operator multiple times when its window is partially full.
					// i.e. after we receive certain number of tuples. More precisely, we will fail
					// this operator 8 times. This will prove that the window contents will survive from
					// multiple crashes.
					mutable int32 valueForOperatorCrash = 0;
					int32 relaunchCnt = getRelaunchCount();
					
					if (relaunchCnt == 0) {
						valueForOperatorCrash = 5555;
					} else if (relaunchCnt == 1) {
						valueForOperatorCrash = 15555;
					} else if (relaunchCnt == 2) {
						valueForOperatorCrash = 85555;
					} else if (relaunchCnt == 3) {
						valueForOperatorCrash = 205555;
					} else if (relaunchCnt == 4) {
						valueForOperatorCrash = 568930;
					} else if (relaunchCnt == 5) {
						valueForOperatorCrash = 705555;
					} else if (relaunchCnt == 6) {
						valueForOperatorCrash = 820465;
					} else if (relaunchCnt == 7) {
						valueForOperatorCrash = 935555;
					} else {
						valueForOperatorCrash = 0;
					}
					
					if (PTD.i == valueForOperatorCrash) {
						// When this operator gets restarted automatically during the recovery process,
						// it should bring back its window contents to where it was at the time of the failure.
						printStringLn("Forcefully aborting the Aggregate operator at tuple #" + (rstring)PTD.i);
						abort();
					}
				}
				
			window
				PTD: tumbling, punct();
				
			output
				AggregatedTestData: min = Min(i), max = Max(i), avg = Average(i);
		}
		
		
		() as MySink1 = FileSink(AggregatedTestData) {
			param
				file: "result.txt";
		}
}

~~~~~~
