/*
This file contains three Java native functions that can be called inside the SPL code.

Simple instructions to create Java native functions within the Streams Studio.

1) In the project explorer's Resources section, right click on impl/java/src and create a new class file.
2) Name your class file with the name you want.
3) In the Java editor, import the class as shown below in the import statement.
4) Then, simply write your Java functions with proper annotations.
   (namespace attribute is purely optional.)
5) As you can see below, you can also maintain state for a given Java function.
6) Save the file. 

Streams build process will automatically create a StreamsModel file and the required
javaFunction.xml file along with the C++ header file with JNI functions.
*/
import com.ibm.streams.function.model.Function;


public class MyJavaFunctions {
	// This static variable will be used to maintain state across
	// different calls made to one of the Java functions below.
	static int cumulativeResult = 0;
	
	@Function(name="add", description="Add two integers", namespace="com.acme.myutilx")
	public static int add(int a, int b) {
		return(a+b);
	}
	
	@Function(name="getUser", description="Returns the current Linux user name", namespace="com.acme.myutily")
	public static String getUser() {
		return (System.getenv("USER"));
	}
	
	@Function(name="cumulativeAdd", description="Adds two integers and then add the current result to the cumulative result stored in a state variable", namespace="com.acme.myutilz")
	public static int cumulativeAdd(int a, int b) {
		// Update the state variable by adding the current result to the cumulative result.
		cumulativeResult += a+b;
		return(cumulativeResult);
	}
	
	@Function(name="intArrayLength", description="Returns the length of an integer array", namespace="com.acme.myutilz")
	public static int intArrayLength(int[] a) {
		return(a.length);
	}
	
	@Function(name="getDoubleArray", description="Returns a nested list of double values", namespace="com.acme.myutilz")
	public static double[] getDoubleArray() {
		double doubleArray[] = {1.2, 2.2, 3.2, 1.5, 2.5, 3.5, 1.8, 2.8, 3.8};
		return(doubleArray);
	}	
	
}
