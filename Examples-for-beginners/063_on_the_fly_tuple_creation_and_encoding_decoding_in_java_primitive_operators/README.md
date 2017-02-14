~~~~~~ Scala 
/*
======================================================================
This example shows how to create a tuple on the fly within a 
Java primitive operator. After that it shows how to encode a
tuple into a blob and decode a blob into a tuple.
======================================================================
*/
namespace application;

composite Main {
	graph
		stream<int32 dummy> Dummy1 = Beacon() {
			param
				iterations: 1u;
		}
		
		// Invoke the Java primitive operator.
		stream<Dummy1> Dummy2 = MyJavaEncodeDecode(Dummy1) {
		}
		
		() as MySink1 = Custom(Dummy2) {
			logic
				onTuple Dummy2: {
					printStringLn("Result=" + (rstring)Dummy2);
				}
		}
}

~~~~~~
