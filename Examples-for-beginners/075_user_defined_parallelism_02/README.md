~~~~~~ Scala 
/*
This is example 2 in the series of 12 User Defined Parallelism (UDP) scenarios.
UDP is a great feature to parallelize an entire composite or a particular operator.

This example code is taken from the Streams InfoCenter and added here to benefit the
beginners of the Streams SPL programming model. Many thanks to our Streams colleague
Scott Schneider for coming up with this set of UDP examples. Full credit goes to him.

It is recommended that you run this example in Distributed mode and visualize the
parallel region in the Streams instance graph.
*/

namespace com.acme.test;

// In this example, operators A and B are fused in the composite operator, and the PE that contains 
// operators A and B does not contain operators from outside the parallel region.
// Parallel transformation replicates the PE that contains operators A and B.

composite UDP2 {
	graph
		stream<int32 i> MyData = Beacon() {
			param
				iterations: 5000; 
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
		}

		// Create 3 parallel versions of the composite Comp1.		
		@parallel (width=3)
		stream<EnrichedData> TransformedData = Comp1(EnrichedData) {
		}
		
		() as MySink = FileSink(TransformedData) {
			param
				file: "Test1.csv";
		}		
}


composite Comp1(input In; output B) {
	graph
		stream<int32 i> A = Custom(In) {
			logic
				onTuple In: {
					In.i = In.i + 25;
					submit(In, A);
				}
				
			config
				placement: partitionColocation("AB");
		}
		
		stream<A> B = Custom(A) {
			logic
				onTuple A: {
					A.i = A.i - 4;
					submit(A, B);
				}
				
			config
				placement: partitionColocation("AB");		
		}
}
~~~~~~
