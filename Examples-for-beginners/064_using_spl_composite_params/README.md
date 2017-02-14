~~~~~~ Scala 
/*
This example demonstrates how one can use different parameter mode
options available in a composite operator. More specifically,
it shows how the following parameter modes can be put to use while
invoking a composite operator.

1) attribute
2) expression
3) function
4) operator
5) type

This example idea was conceived and developed by our Streams colleague
James Cancilla. Very many thanks to him and full credit goes to him.
*/
namespace com.acme.test;

type MyInType = int32 i, int32 j;
type MyOutType = int32 y, int32 z;

// Define couple of SPL functions that will be used later.
int32 adder(int32 value) {
	return(value + 10);
}

int32 multiplier(int32 value) {
	return(value * 10);
}

// This is our main composite.
composite CompositeParams {
	graph
		stream<MyInType> TestData1 = Beacon() {
			param
				iterations: 5u;
				
			output
				TestData1: i =5, j=0;
		}
		
		// Invoke the attribute parameter mode composite.
		stream<MyOutType> TestResult1 = AttributeParamMode(TestData1) {
			param
				inputAttribute: i;
		}
		
		() as TestSink1 = FileSink(TestResult1) {
			param
				file: "/dev/stdout";
		}
		
		// Invoke the expression parameter mode composite.
		stream<MyOutType> TestResult2 = ExpressionParamMode(TestData1) {
			param
				inputExpression: i + 100 -j;
		}
		
		() as TestSink2 = FileSink(TestResult2) {
			param
				file: "/dev/stdout";
		}		
		
		// Invoke the function mode composite.
		stream<MyOutType> TestResult3 = FunctionParamMode(TestData1) {
			param
				inputExpression: 100;
				addFunction: adder;
				multiplyFunction: multiplier;
		}
		
		() as TestSink3 = FileSink(TestResult3) {
			param
				file: "/dev/stdout";
		}	
		
		// Invoke the operator mode composite.
		stream<MyOutType> TestResult4 = OperatorParamMode(TestData1) {
			param
				op: Custom;
		}
		
		() as TestSink4 = FileSink(TestResult4) {
			param
				file: "/dev/stdout";
		}						

		// Invoke the type mode composite.
		stream<MyOutType> TestResult5 = TypeParamMode(TestData1) {
			param
				paramType: float64;
		}
		
		() as TestSink5 = FileSink(TestResult5) {
			param
				file: "/dev/stdout";
		}		
}

// A composite that deals with an attribute parameter.
composite AttributeParamMode(input In0; output Out0) {
	param
		attribute $inputAttribute;
		
	graph
		stream<MyOutType> Out0 = Functor(In0) {
			output
				Out0: y = $inputAttribute + 10, z = $inputAttribute * 10;
		}
}

// A composite that deals with an expression parameter.
composite ExpressionParamMode(input In0; output Out0) {
	param
		expression $inputExpression;
		
	graph
		stream<MyOutType> Out0 = Functor(In0) {
			output
				Out0: y = $inputExpression + 10, z = $inputExpression * 10;
		}
}

// A composite that deals with a function parameter.
composite FunctionParamMode(input In0; output Out0) {
	param
		function $addFunction;
		function $multiplyFunction;
		expression $inputExpression;
		
	graph
		stream<MyOutType> Out0 = Functor(In0) {
			output
				Out0: y = $addFunction($inputExpression), z = $multiplyFunction($inputExpression);
		}
}

// A composite that deals with an operator parameter.
composite OperatorParamMode(input In0; output Out0) {
	param
		operator $op;
		
	graph
		stream<MyOutType> Out0 = $op(In0) {
			logic
				onTuple In0: {
					printStringLn("Inside the OperatorParamMode composite.");
				}
		}
}

// A composite that deals with a type parameter.
composite TypeParamMode(input In0; output Out0) {
	param
		type $paramType;
		
	graph
		stream<MyOutType> Out0 = Custom(In0) {
			logic
				state: {
					mutable $paramType val;
				}
				
				onTuple In0: {
					printStringLn("Inside the TypeParamMode composite.");
				}
		}
}
~~~~~~
