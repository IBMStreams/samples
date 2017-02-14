~~~~~~ Scala 
/*
This example demonstrates how to assign tuple attributes at the 
time of job submission inside a custom operator. When the
incoming tuples arrive at the Custom operator in this example,
values entered by the user at the application startup time are
assigned to the tuple attributes.

When you launch this application from the IDE, you will be asked to
provide the submission time values in the application launc dialog.

You may enter the values as shown below.
You will see default values for param1 and param3 already filled.
You have to enter the values only for param2 and param4 as shown below.

param2=67
param4=90,91,92
*/
composite Main {
	type 
		myTuple1 = tuple <int32 i, int32 j>; 
		myTuple2 = tuple<rstring str1, rstring str2, rstring str3, rstring str4, rstring str5, rstring str6, rstring str7>;
		myTuple3 = tuple<myTuple1, myTuple2>;
	
	graph
		stream <myTuple1> Input = Beacon() {							
			logic	state:	{
				mutable int32 m=0; 
				mutable int32 n=0;
			}
			
			param iterations:	2u;			
			
			output Input: i = ++m, j = ++n;
		} // End of Beacon.
	
		stream <myTuple3> Output = Custom(Input) {
			logic 
				onTuple Input: {
					myTuple3 m1 = {
						i = Input.i, 
						j = Input.j,
						// Note that except param2 and param4, the remaining ones 
						// use default values. You can override them at job
						// submission time.
						str1 = getSubmissionTimeValue ("param1", "5"),
						str2 = getSubmissionTimeValue ("param2"),
						str3 = getSubmissionTimeListValue("param3", ["t1", "t2", "t3"]) [0],
						str4 = getSubmissionTimeListValue("param3", ["t1", "t2", "t3"]) [1],
						str5 = getSubmissionTimeListValue("param3", ["t1", "t2", "t3"]) [2],
						str6 = getSubmissionTimeListValue("param4") [0],
						str7 = getSubmissionTimeListValue("param4") [1]					
				};
				
				submit (m1, Output);
			} // End of logic onTuple Input	
		} // End of Custom(Input)
		
		() as ScreenWriter = Custom(Output) {
			logic 
				onTuple 
					Output: printStringLn("The tuple = " + (rstring)Output + "\n");
		} // End of Custom(Output)
} // End of composite Main.


~~~~~~
