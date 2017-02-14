~~~~~~ Scala 
/*
This example shows how Java native functions can be used very easily within
the SPL code. It also shows how one can maintain state within a Java function.

Refer to the impl/java/src/MyJavaFunctions.java file where our three Java native functions are defined.
In that file, you will also see simple instructions about creating Java native functions.
*/
namespace com.acme.test;

// Just for the sake of demonstrating it, we put three Java native functions in their own namespaces.
// Let us use those function namespaces here.
use com.acme.myutilx::*;
use com.acme.myutily::*;
use com.acme.myutilz::*;

composite JavaNativeFunctions {
	graph
		stream<int8 dummy> TestData1 = Beacon() {
			param
				iterations: 1u;
		}
		
		() as MySink1 = Custom(TestData1) {
			logic
				onTuple TestData1: {
					// Let us call our first Java native function.
					printStringLn("Result from the Java native function add = " + (rstring)add(5, 9));
					// Let us call our second Java native function.
					printStringLn("Result from the Java native function getUser = " + getUser());
					// Let us now test the effect of maintaining state inside the Java native functions.
					// Call the same Java native function three times and observe the cumulative result.
					printStringLn("First result from the Java native function cumulativeAdd = " + (rstring)cumulativeAdd(10, 20));
					printStringLn("Second result from the Java native function cumulativeAdd = " + (rstring)cumulativeAdd(10, 20));
					printStringLn("Third result from the Java native function cumulativeAdd = " + (rstring)cumulativeAdd(10, 20));
					// We can pass SPL list types to the Java function.
					mutable list<int32> myIntArray = [4, 5, 6, 7, 8, 9];
					printStringLn("Fourth result from the Java native function intArrayLength = " + (rstring)intArrayLength(myIntArray));
					// Let us call a Java native function that will return a double array.
					list<float64> myList = getDoubleArray();
					printStringLn("Fifth result from the Java native function getDoubleArray = " + (rstring)myList);
				}
		}
}

~~~~~~
