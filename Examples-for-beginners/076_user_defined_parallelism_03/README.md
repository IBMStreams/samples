~~~~~~
/*
This is example 3 in the series of 12 User Defined Parallelism (UDP) scenarios.
UDP is a great feature to parallelize an entire composite or a particular operator.

This example code is taken from the Streams InfoCenter and added here to benefit the
beginners of the Streams SPL programming model. Many thanks to our Streams colleague
Scott Schneider for coming up with this set of UDP examples. Full credit goes to him.

It is recommended that you run this example in Distributed mode and visualize the
parallel region in the Streams instance graph.
*/
namespace com.acme.test;

// In this example of user-defined parallelism, a logical processing element (PE) contains operators 
// from outside the parallel region, a splitter is fused inside the PE, and the merge point is 
// not inside the PE. The Src operator is fused with operators A and B in the same PE. 
// The parallel transformation does not replicate the PE. Instead, operators A and B are replicated, 
// and threaded ports are inserted between the Src operator and the replicas of operators A and B. 
// Because the merge point is not inside the PE, extra PE output ports are also created.

// In addition, this example is same as the UDP2 example except that the operators A and B are not fused inside the
// composite operator. Instead, the entire composite elements are fused at the time of invoking the
// composite. The existing semantics specify that the config placement option must be applied to 
// each operator that is inside the composite operator. The PE that contains 
// operators A and B does not contain operators from outside the parallel region.
// Parallel transformation replicates the PE that contains operators A and B.

composite UDP3 {
	graph
		stream<int32 i> MyData = Beacon() {
			param
				iterations: 5000; 
				
			config
				placement: partitionColocation("AB");
		}

		stream<MyData> EnrichedData = Custom(MyData) {
			logic
				state: {
					mutable int32 _i = 0;
				}
				
				onTuple MyData: {
					_i++;
					MyData.i = _i;
					submit(MyData, EnrichedData);
				}
				
			config
				placement: partitionColocation("AB");
		}
		
		// Create 3 parallel copies of the composite Comp2.
		@parallel (width=3)
		stream<EnrichedData> TransformedData = Comp2(EnrichedData) {
			config
				placement: partitionColocation("AB");
		}
		
		() as MySink = FileSink(TransformedData) {
			param
				file: "Test1.csv";
		}		
}


composite Comp2(input In; output B) {
	graph
		stream<int32 i> A = Custom(In) {
			logic
				onTuple In: {
					In.i = In.i + 25;
					submit(In, A);
				}
		}
		
		stream<A> B = Custom(A) {
			logic
				onTuple A: {
					A.i = A.i - 4;
					submit(A, B);
				}
		}
}
~~~~~~
