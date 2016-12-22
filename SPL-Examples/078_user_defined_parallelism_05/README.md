~~~~~~
/*
This is example 5 in the series of 12 User Defined Parallelism (UDP) scenarios.
UDP is a great feature to parallelize an entire composite or a particular operator.

This example code is taken from the Streams InfoCenter and added here to benefit the
beginners of the Streams SPL programming model. Many thanks to our Streams colleague
Scott Schneider for coming up with this set of UDP examples. Full credit goes to him.

It is recommended that you run this example in Distributed mode and visualize the
parallel region in the Streams instance graph.
*/
namespace com.acme.test;

// In this example of user-defined parallelism, a logical PE contains operators from outside the 
// parallel region and the merge point is not inside the PE. The Snk operator is fused with operators 
// A and B, but the Src operator is not. The parallel transformation replicates operators A and B 
// inside the PE, and creates new input ports on that PE. The splitter communicates with the new input ports.

composite UDP5 {
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
		
		// Create 3 parallel copies of the composite Comp4.
		@parallel (width=3)
		stream<EnrichedData> TransformedData = Comp4(EnrichedData) {
			config
				placement: partitionColocation("AB");
		}
		
		() as MySink = FileSink(TransformedData) {
			param
				file: "Test1.csv";

			config
				placement: partitionColocation("AB");
		}		
}


composite Comp4(input In; output B) {
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
