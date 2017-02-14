~~~~~~ Scala 
/*
This example shows how a Java primitive operator can receive an incoming tuple and
process it in multiple threads inside of that Java operator. While processing an
incoming tuple, this Java operator example also shows how to do concurrent tuple
submissions from multiple threads.
*/
namespace com.acme.test;

composite JavaOpSubmitFromMultipleThreads {
	graph
		// Generate beacon signals 5 times for our test.
		stream<int32 i, float64 j, rstring threadId> MyRawData = Beacon() {
			param
				iterations: 5u;
				period: 2.0;
		}

		// Enrich the Beacon generated tuple with test data.
		stream<MyRawData> MyEnrichedData = Custom(MyRawData as MRD) {
			logic
				state: {
					mutable int32 _i = -1;
					mutable float64 _j = -1.0;
				}
				
				onTuple MRD: {
					_i++;
					_j++;
					
					MRD.i = _i;
					MRD.j = _j;
					MRD.threadId = "";
					submit(MRD, MyEnrichedData);
				}
		}
				
		// Invoke the Java operator that will do tuple submissions from
		// multiple threads. (See the commentary inside the Java operator
		// code for more details: impl/java/src directory.)
		stream<MyRawData> MyThreadSpecificData = MyJavaOp(MyEnrichedData) {
		
		}
		
		// Write the tuples sent by the Java operator to the screen.
		() as Sink1 = FileSink(MyThreadSpecificData as MTSD) {
			param
				file: "/dev/stdout";
				flush: 1u;
		}
}

~~~~~~
