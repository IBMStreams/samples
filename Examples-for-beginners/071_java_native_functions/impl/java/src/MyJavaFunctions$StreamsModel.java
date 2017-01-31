
@com.ibm.streams.operator.internal.model.ShadowClass("MyJavaFunctions")
@javax.annotation.Generated("com.ibm.streams.operator.internal.model.processors.ShadowClassGenerator")
public class MyJavaFunctions$StreamsModel {

@com.ibm.streams.function.model.Function(name="add", description="Add two integers", namespace="com.acme.myutilx")
public static int add(int a, int b) {return 0; }

@com.ibm.streams.function.model.Function(name="getUser", description="Returns the current Linux user name", namespace="com.acme.myutily")
public static java.lang.String getUser() {return null; }

@com.ibm.streams.function.model.Function(name="cumulativeAdd", description="Adds two integers and then add the current result to the cumulative result stored in a state variable", namespace="com.acme.myutilz")
public static int cumulativeAdd(int a, int b) {return 0; }

@com.ibm.streams.function.model.Function(name="intArrayLength", description="Returns the length of an integer array", namespace="com.acme.myutilz")
public static int intArrayLength(int[] a) {return 0; }

@com.ibm.streams.function.model.Function(name="getDoubleArray", description="Returns a nested list of double values", namespace="com.acme.myutilz")
public static double[] getDoubleArray() {return null; }
}