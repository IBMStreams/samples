~~~~~~ Scala 
/*
This example shows a simple mechanism to serialize and deserialize SPL tuples.
When large tuples need to be cached inside an operator's state variable, it will
consume a lot of memory. To cut down the memory consumption, one possible option is 
to convert the large tuples into compact blobs through tuple serialization.
When the actual tuple representation is needed, blob values can be converted back
to the actual tuples through tuple deserialization. This same method can also be
used to transmit data as blobs across operators. This serialization/deserialization
approach is likely to add extra CPU overhead. If that extra overhead can be tolerated,
then this approach would serve to be useful.

In this example, two C++ native functions will provide a way to serialize tuples into
blob data and deserialize blob data into SPL tuples. You can refer to the native function
model XML file about these two functions. Refer to ../../impl/include/Functions.h file for
the implementation of these two functions.
*/
namespace com.acme.test;

composite serialize_deserialize_tuples {
	// Let us define a tuple type
	type
		PersonData = tuple<rstring firstName, rstring lastName, rstring ssn,
			rstring streetAddress, uint32 age, float32 height, boolean isEmployed>;
			
	graph
		// Read the person data from a file.
		stream<PersonData> Person1 = FileSource() {
			param
				format: csv;
				file: "PersonData.txt";
		}
		
		// In this Custom operator, we will convert every incoming tuple into a blob by
		// serializing the tuple. Then, we will store the serialized data as a blob
		// inside a state variable. When the end of file marker arrives, we will
		// iterate through the state variable array and deserialize the blob values
		// into tuples and then submit them.
		stream<PersonData> Person2 = Custom(Person1) {
			logic
				state: {
					mutable list<blob> _personCache = [];
				}
				
				onTuple Person1: {
					// Serialize the incoming tuple.
					mutable blob myBlob = [];
					// Call a native function to do the Tuple-->Blob conversion.
					serializeTuple(Person1, myBlob);
					appendM(_personCache, myBlob);
				}
				
				onPunct Person1: {
					// When an end of file marker punctuation arrives, 
					// deserialize the blob values into their tuple
					// representation and submit them.
					if (currentPunct() == Sys.FinalMarker) {
						// Stay in a loop and submit everything we stored in memory.
						for (blob myBlob in _personCache) {
							// It is a good idea to check whether this
							// application is being shutdown externally.
							if (isShutdown() == true) {
								break;
							}
							
							// Let us deserialize the blob stored in our cache to its original tuple form.
							mutable PersonData _tuple = (PersonData){};
							// Call a native function to do the Blob-->Tuple conversion.
							deserializeTuple(_tuple, myBlob);							
							// Keep submitting the cached tuples.
							submit(_tuple, Person2);
						}
					}
				}
		}
		
		// Receive the deserialized tuple data and print it on the screen.
		()as Sink = FileSink(Person2){
			param
				// Write the results to stdout error.
				file : "/dev/stderr" ;
				flush : 1u ;
		}
}

~~~~~~
