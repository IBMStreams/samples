~~~~~~ Scala 
/*
This is a very simple example that showcases a random collection of
powerful built-in SPL functions that are availble out of the box.
This application demonstrates how time, math, and collection type
functions can be used inside of an SPL application.
*/
namespace test.scratch;
// Define a use directive for the namespace required to use the time functions.
use spl.time::*;

composite Main {
	// Define the tuple type that we will use in this application.
	type
		temp1 = tuple<int32 i, rstring dateTime, timestamp time, uint32 secondValue, int64 seconds>;
		
	graph
		// Create tuples using the Beacon with pre-assigned values.
		stream <temp1> DefaultTempTuple1 = Beacon() {
			param
				// Send 5 tuples after an initial delay of 3 seconds.
				iterations: 5u;
				initDelay: 3.0f;		
		}
		
		// In this Custom operator, we are going to customize the tuple attributes.
		stream<temp1> TempTuple1 = Custom(DefaultTempTuple1 as DTT1) {
			logic
				// Define the state variables required.
				state: {
					// Mutable variables allow us to change the variable.
					mutable int32 n = 0;
					// Immutable list to store hardcoded date values.
					list<rstring> dateValues = ["", "4-OCT-2005", "25-MAR-2008", "15-AUG-2009", "25-DEC-2010", "2-FEB-2011"];	
				}
					
				onTuple DTT1: {
					++n;
					
					DTT1 = {
						i = n,
						dateTime = dateValues[n] + (rstring)" 20:00:00.000",
						time = timeStringtoTimestamp(dateValues[n], (rstring)"20:00:00.000", true),
						secondValue = 0u,
						seconds = 0l
					};
				
					submit(DTT1, TempTuple1);
				} // End of onTuple DTT1	
		} // End of Custom(DefaultTempTuple1)
		
		// Define a functor to get the second value and convert the timestamp value into seconds.
		stream <temp1> TempTuple2 = Functor(TempTuple1) {
			output
				// We will use two of the built-in SPL time functions here.
				TempTuple2: secondValue = second(time), seconds = getSeconds(time);
		}
		
		// A custom operator that will display the transformed tuple attributes.
		() as ScreenWriter1 = Custom(TempTuple2) {
			logic
				state: {
					// Let us also experiment with some of built-in functions to 
					// operate on the map type.
					mutable map <uint32, rstring> myMap = {};	
				} // End of state
				
				onTuple TempTuple2: {
					// We are using a built-in function from the math namespace.
					printStringLn("Random value = " + (rstring) spl.math::random());
					// Assign some random keys and assign a random value and store it in the map.
					printStringLn((rstring) TempTuple2);
					myMap[10u] = dateTime + " ABC";
					myMap[20u] = dateTime + " XYZ";
					myMap[30u] = dateTime + " MNP";
					// Let us display what is there in the map now.
					printStringLn("After Adding------>" + (rstring) myMap);
					// Use an SPL built-in function to remove an element from the SPL map type.
					myMap = remove (myMap, 20u);
					myMap = remove (myMap, 10u);
					printStringLn("After Removing------>" + (rstring) myMap);
				} // End of onTuple TempTuple2
		} // End of ScreenWriter1 = Custom(TempTuple2)	
} // End of composite Main

~~~~~~
