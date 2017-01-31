~~~~~~
/*
This is example 10 in the series of 12 User Defined Parallelism (UDP) scenarios.
UDP is a great feature to parallelize an entire composite or a particular operator.

This example code is taken from the Streams InfoCenter and added here to benefit the
beginners of the Streams SPL programming model. Many thanks to our Streams colleague
Scott Schneider for coming up with this set of UDP examples. Full credit goes to him.

It is recommended that you run this example in Distributed mode and visualize the
parallel region in the Streams instance graph.
*/
namespace com.acme.test;

// In this example of user-defined parallelism, the source is an Import operator
// rather than a typical Source operator. This example is similar to Example 8, 
// however the source is an Import operator. The parallel transformation replicates
// the Import operator port and the PE.	

composite UDP10 {
	graph
		stream<int32 i> MyData = Beacon() {
			param
				iterations: 5000; 
				initDelay: 7.0;
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
		
		() as ExportedStream1 = Export(A) {
			param
				streamId: "EnrichedData";
		}
			
		@parallel (width=2)
		stream<A> TransformedData = Comp10() {
			config
				placement: partitionColocation("SrcA");
		}	
		
		() as MySink = FileSink(TransformedData) {
			param
				file: "Test1.csv";
		}					
}


composite Comp10(output A) {
	graph
		stream<int32 i> Src = Import() {
			param
				applicationName: "com.acme.test::UDP10";
				streamId: "EnrichedData";
				// When do we use the following filter?
				// filter: attr == getChannel();
		}
		
		stream<Src> A = Custom(Src) {
			logic
				onTuple Src: {
					Src.i = Src.i + 14;
					submit(Src, A);
				}
		}
}

~~~~~~
