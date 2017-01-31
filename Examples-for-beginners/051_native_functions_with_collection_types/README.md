~~~~~~
/*
=====================================================
This SPL composite includes a simple example to show
how to call C++ native functions that are compiled into a 
.so shared library. In addition, it demonstrates
how to pass SPL collection data types (list, map,
and tuple) to such C++ native functions.

Please refer to the README.txt file that contains
instructions to build and run this application.

Last modified on: May/14/2013
=====================================================
*/

namespace com.ibm.nf.test;

composite native_functions_with_collection_types {
	type
		DummySchema = tuple<int32 i>;
		
	graph
		stream<DummySchema> DummyStream = Beacon() {
			param
				// Simply send a signal to the downstream operator.
				iterations: 1;
		}
		
		// In this Custom operator, we will call the C++ native functions.
		() as ScreenWriter1 = Custom(DummyStream as DS) {
			logic
				state: {
					// Initialize the native function module only once for this PE.
					// This is just an option by which an SPL application can
					// give a chance to the C++ class containing the native functions
					// for initializing any of its internal state variables.
					int32 initRC = initializeNativeFunctionsWithCollectionTypes();
				}
				
				onTuple DS: {
					// 1) Let us experiment with the list collection type.
					mutable list<float64> randomNumbersList = [];
					// Let us insert some values in the list.
					appendM(randomNumbersList, 45.67);
					appendM(randomNumbersList, 83.12);
					appendM(randomNumbersList, 42.19);
					// We are going to a call a C++ native function now, which will
					// add a few random numbers to the end of our list.
					mutable int32 newRandomNumbersAdded = 0;
					printStringLn("===============================");
					printStringLn("1) Calling the native function: getNewRandomNumbers");					
					mutable boolean rc = getNewRandomNumbers(randomNumbersList, newRandomNumbersAdded);
					printStringLn("Return code from getNewRandomNumbers = " + (rstring)rc);
					printStringLn("newRandomNumbersAdded = " + (rstring)newRandomNumbersAdded);
					printStringLn("randomNumbersList = " + (rstring)randomNumbersList);

					
					// 2) Let us experiment with the map collection type.
					mutable map<rstring, rstring> stateCapitals = {};
					// Let us insert some (key, value) pairs in the map.
					insertM(stateCapitals, "New York", "Albany");
					insertM(stateCapitals, "Connecticut", "Hartford");
					insertM(stateCapitals, "California", "Sacramento");
					insertM(stateCapitals, "New Jersey", "Trenton");
					insertM(stateCapitals, "Idaho", "Boise");
					insertM(stateCapitals, "Pennsylvania", "Harrisburg");
					insertM(stateCapitals, "Iowa", "Des Moines");
					// We are going to call a C++ native function now, which will
					// convert the state capital names (map values) to upper case.
					mutable int32 numberOfMapValues = 0;
					printStringLn("===============================");
					printStringLn("2) Calling the native function: toUpperOfMapValues");					
					rc = toUpperOfMapValues(stateCapitals, numberOfMapValues);
					printStringLn("Return code from toUpperOfMapValues = " + (rstring)rc);
					printStringLn("numberOfMapValues = " + (rstring)numberOfMapValues);
					printStringLn("stateCapitals = " + (rstring)stateCapitals);					
					
					
					// 3) Let us experiment with the tuple collection type.
					mutable tuple<rstring name, uint8 age, float32 salary, list<rstring> companyDevicesOwned> employee = {};
					// We are going to call a C++ native function now, which will
					// populate this empty tuple with some random values.
					printStringLn("===============================");
					printStringLn("3) Calling the native function: createEmployeeRecord");	
					rc = createEmployeeRecord(employee);
					printStringLn("Return code from createEmployeeRecord = " + (rstring)rc);
					printStringLn("employeeRecord = " + (rstring)employee);								
					
					
					// We successfully tested how to pass SPL list, map, and tuple data types to C++ native functions.
					// We also verified how we can modify the collection typed method arguments inside our C++ code.
					// Note: If you don't want the C++ code to modify any of your method arguments, you can make them
					// non-mutable in SPL and then receive them as const & inside your C++ code.
					printStringLn("===============================");
				}
		
		}
	

}

~~~~~~
