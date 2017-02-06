~~~~~~ Scala 
/*
This example shows how a Punctor operator could be used in an application.
Punctor operator allows the transformation of the input tuples and then
inject puncuation markers either before or after the output tuple as cconfigured.
*/
namespace my.sample;

composite Main {
	type
		Student = tuple <rstring name, int32 id, int32 rank, rstring major>;
	
	graph
		stream <Student> DefaultStudentRecord = Beacon() {
			param
				iterations: 100u;
		} // End of Beacon().
		
		// Let us customize the attributes of the student record tuples.
		stream<Student> StudentRecord = Custom(DefaultStudentRecord as DSR) {
			logic
				state: mutable int32 n = 0;
				
				onTuple DSR: {
					++n;
					
					DSR = {
						name = "Name" + (rstring)n,
						id = n+1000, rank = (n%5 == 0) ? 5 : n%5, 
						major = "Major" + (rstring)n
					};
					
					submit(DSR, StudentRecord);
				} // End of onTuple DSR			
		} // End of Custom(DefaultStudentRecord as DSR)
		
		// We are going to configure the Punctor operator to send punctuation
		// markers for specific student ids.
		stream <Student> PunctorControlledStudentRecord = Punctor(StudentRecord) {
			param
				punctuate: (id == 1026 || id == 1051 || id == 1076 || id >= 1100);
				position: before;
		} // End of Punctor(StudentRecord)
		
		() as ScreenWriter1 = Custom(PunctorControlledStudentRecord) {
			logic
				state: {
					mutable boolean punctorArrived = false; 
					mutable int32 err = 0;
				} // End of state.
				
				onPunct PunctorControlledStudentRecord: 
					punctorArrived = true;
				
				onTuple PunctorControlledStudentRecord: 
					if (punctorArrived) {
						punctorArrived = false;
						printStringLn ("Punctuation arrived before->" + (rstring) PunctorControlledStudentRecord);	
						spl.file::fflush (0ul, err);
					} // End of if (punctorArrived)
		} // End of Custom(PunctorControlledStudentRecord)
} // End of composite Main.

~~~~~~
