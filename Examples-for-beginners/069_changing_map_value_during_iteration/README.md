~~~~~~ Scala 
/*
This example shows how a map value can be changed
while iterating over that collection. This is a new feature available
starting in the Streams 3.2.1 release. Hence, to test this code in 3.2.1 or later, 
please read the commentary below to comment and uncomment certain lines of code.

Primary reason for having this example here is to make the developers aware of this new feature.

=== This will work only in Streams 3.2.1 or later ===
*/
namespace com.acme.test;

composite ChangeMapValue {
	graph
		stream<int8 x> TestData1 = Beacon() {
			param
				iterations: 1u;
		}
		
		// In the Custom operator below, we will try to modify the values in
		// a map data type within the iteration code block.
		() as MySink1 = Custom(TestData1) {
			logic
				state: {
					mutable map<int32, rstring> myMap = {1:"", 2:"", 3:""};
				}
				
				onTuple TestData1: {
					// You can uncomment the full block of the for loop below if you are
					// running it under Streams 3.2.1 or later.
					// Loop through the map
					// In Streams 3.2.1, a new function called "keys" was added.
					for (int32 idx in keys(myMap)) {
					    // Unlike in the earlier versions of Streams, now
					    // we can change the map values inside a loop.
						if (idx == 1) {
							myMap[idx] = "one";
						} else if (idx == 2) {
							myMap[idx] = "two";
						} else if (idx == 3) {
							myMap[idx] = "three";
						}
					}

					printStringLn("Modified map = " + (rstring)myMap); 
				}
		}
}

~~~~~~
