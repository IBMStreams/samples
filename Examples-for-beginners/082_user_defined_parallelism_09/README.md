~~~~~~ Scala 
/*
This is example 9 in the series of 12 User Defined Parallelism (UDP) scenarios.
UDP is a great feature to parallelize an entire composite or a particular operator.

This example code is taken from the Streams InfoCenter and added here to benefit the
beginners of the Streams SPL programming model. Many thanks to our Streams colleague
Scott Schneider for coming up with this set of UDP examples. Full credit goes to him.

It is recommended that you run this example in Distributed mode and visualize the
parallel region in the Streams instance graph.
*/
namespace com.acme.test;

// In this example of user-defined parallelism, no operators from outside the parallel region are fused 
// with the operators in the parallel region. The parallel region has no incoming streams.
// The processing element (PE) is replicated because no operators from outside the parallel region are 
// fused with the operators in the parallel region. Each replicated PE happens to contain a Source operator.
// If you want to divide the data that each source handles, you must invoke the Source operators in a way that divides the data.

composite UDP9 {
	graph
		// Create two parallel copies of the composite Comp9.
		@parallel (width=2)
		() as TransformedData = Comp9() {
			config
				placement: partitionColocation("All");
		}	
}


composite Comp9() {
	graph
		stream<int32 i> MyData = Beacon() {
			param
				iterations: 5000; 
		}

		stream<MyData> A = Custom(MyData) {
			logic
				state: {
					mutable int32 _i = 0;
				}
				
				onTuple MyData: {
					_i++;
					MyData.i = _i;
					submit(MyData, A);
				}
		}
		
		() as MySink = FileSink(A) {
			param
				file: "Test1.csv";
		}			
}

~~~~~~
