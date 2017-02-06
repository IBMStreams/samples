~~~~~~ Scala 
/* 
==========================================================================
Copyright (C) 2014-2015, International Business Machines Corporation
All Rights Reserved                                                 

This is one of many examples included in the "SPL Examples for
Beginners" collection to show the use of the consistent region feature
built into Streams 4.x and higher versions.

This particular example shows how a non-replay capable Source operator
will not be a show stopper when it comes to employing the consistent region feature
in such applications. When using sources (such as TCPSource) that can't realistically
replay data, there is way to configure your application with consistent region by
using an utility operator called ReplaybleStart (shipped with the Streams product).
In this example, we will use a topology that uses TCPSource along with ReplayableStart
to achieve application-level fault tolerance.  This example simulates the operator failure by 
aborting one of the operators automatically when the application is in the middle of executing the logic.
By doing that, the core fault tolerance feature of the consistent region will get
triggered to recover from a failure that occurred in an application graph.
It will prove that the tuples will not be missed and the Aggregate operator's window state will not
be compromised during the course of the unexpected operator failure and the
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
   In the resulting output file in the data directory (results.txt) will show you
   that there is no gap in the results. If that is the case , then the
   consistent region feature worked correctly during and after the forced
   crash of this application.

IMPORTANT: In order to run this example, you must have at least 4 machines available in
your Streams instance. That is the mandatory requirement for using the ReplayableStart operator.

At first, you can start this application (ConistentRegion6). After that, you can start
the utility application (TCPDataSender) that is part of this same project to pump
data via TCP into the ConsistentRegion6 application.
==========================================================================
*/
namespace com.acme.test;

composite ConsistentRegion6 {
	graph
		// JobControlPlane operator is mandatory in applications with consistent regions.
		// Simply include it anywhere in your application graph.
		() as JCP = JobControlPlane() {}
		
		// In this application, we are going to receive data via a TCPSource operator which is
		// not enabled for consistent region replay in case of a failure recovery.
		// We are going to demonstrate how we can receive data from the network and still
		// make use of the consistent region feature available in Streams 4.x and higher versions.
		// IMPORTANT: In order to receive data via TCP, we have provided another test application in
		// this same SPL project. All you have to do is to start this (ConsistentRegion6) application
		// first and after that start the TCPDataSender application available in this same project.
		stream<int32 i, rstring str> TCPData = TCPSource() {
			param
				format: csv;
				role: server;
				// Following name will be registered with the Streams runtime nameservice which will
				// provide the necessary network address and the TCP port.
				name: "ConsistentRegion6-TCP";
		}
		
		// When we use non-replay capable source operators, there is a way to use the
		// consistent region feature via the ReplayableStart operator as shown below.
		// It is an utility operator that is part of the Streams standard toolkit.
		// As mentioned above, your instance must have at least 4 machines in order to provide
		// redundancy for this operator. If you have less than 4 machines, then this application
		// submission will fail.
		@consistent(trigger=periodic, period=0.250)
		stream<TCPData> ReplayableData = ReplayableStart(TCPData) {
		}
		
		stream<int32 min, int32 max, int32 avg, rstring str> AggregatedData = Aggregate(ReplayableData as RD) {
			logic
				onTuple RD: {
					if ((RD.i == 45000) && (getRelaunchCount() == 0u)) {
						// Forcefully abort this operator only once.
						abort();
					}
				}
		
			window
				RD: tumbling, count(10000);
				
			output
				AggregatedData: min = Min(RD.i), max = Max(RD.i), avg = Average(RD.i), str = Any(RD.str);
		}

		() as MySink1 = FileSink(AggregatedData) {
			param
				file: "results.txt";
				flush: 1u;
		}
}

~~~~~~

---
~~~~~~ Scala 
/*
============================================================================
This is an utility application that is used to pump test data for testing
the ConsistentRegion6 application located in this same project directory.
After starting the ConsistentRegion6 application, you can start this
TCPDataSender application to get the data flowing via TCP sockets. 
============================================================================ 
*/
namespace com.acme.test;

composite TCPDataSender {
	graph
		// This entire graph is not in the consistent region.
		// It is simply an application to send data via TCP.
		stream<int32 i, rstring str> TestData = Beacon() {
			param
				iterations: 100000;
				initDelay: 4.0;
				
			output
				TestData: i = (int32)IterationCount() + 1,
					str = "A" + (rstring)((int32)IterationCount() + 1);
		}
		
		() as MySink1 = TCPSink(TestData) {
			param
				format: csv;
				flush: 1u;
				role: client;
				// Following is the name used in the TCPSource of the main application in this project.
				// That name will be registered with the Streams runtime nameservice which will
				// provide the necessary network address and the TCP port.
				name: "ConsistentRegion6-TCP";
				reconnectionPolicy: InfiniteRetry;
				retryFailedSends: true;
		}		
}

~~~~~~
