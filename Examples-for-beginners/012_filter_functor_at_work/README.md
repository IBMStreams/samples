~~~~~~ Scala 
/*
This example puts the two commonly used standard toolkit operators to work.
They are Filter and Functor. Filter allows you to route tuples based on 
conditional checks. It provides two output ports to send the matched tuples
on the first output port and the unmatched tuples on the second output port.
Functor operator allows transforming the incoming tuple attributes and send 
it on many different output ports with different stream schemas.
*/
namespace my.sample;

composite Main {
	type
		Employee = tuple <rstring name, rstring title, int32 age, float32 salary>;
		Account = tuple <rstring accountHolder, rstring accountNumber, rstring ticker, int32 quantity>;
		
	graph
		stream <Employee> DefaultEmployeeRecord = Beacon() {			
			param
				iterations: 100u;
		} // End of Beacon().

		// In this Custom operator, let us customize the employee record tuples.
		stream<Employee> EmployeeRecord = Custom(DefaultEmployeeRecord as DER) {
			logic 
				state: mutable int32 n = 0;
				
				onTuple DER: {
					++n;
					DER = {
						name = "Name" + (rstring) n, 
						title = "title" + (rstring) n,
						age = (n+20 <= 60) ? n+20 : 60,
						salary = (n*20000 <= 180000) ? (float32)(n*20000) : (float32)180000};
					submit(DER, EmployeeRecord);
				} // End of onTuple DER
		} // End of Custom(DefaultEmployeeRecord)


		// Note that there are two output ports for the Filter operator.
		// First port is for the matched tuples and second port is for the unmatched tuples.
		(stream <Employee> MatchedEmployeeRecord; stream <Employee> UnmatchedEmployeeRecord) = Filter(EmployeeRecord) {
			param
				filter: age <= 45;
		} // End of Filter(EmployeeRecord)
		
		() as ScreenWriter1 = Custom(MatchedEmployeeRecord) {
			logic
				onTuple MatchedEmployeeRecord: printStringLn("Matched record->" + (rstring)MatchedEmployeeRecord);
		} // End of Custom(MatchedEmployeeRecord).

		() as ScreenWriter2 = Custom(UnmatchedEmployeeRecord) {
			logic
				onTuple UnmatchedEmployeeRecord: printStringLn("Unmatched record->" + (rstring)UnmatchedEmployeeRecord);
		} // End of Custom(MatchedEmployeeRecord).

		// ============== Functor related code follows ==============
		stream <Account> DefaultAccountRecord = Beacon() {
			param
				iterations: 100u;
		} // End of Beacon().

		// In this Custom operator, let us customize the account record tuples.
		stream<Account> AccountRecord = Custom(DefaultAccountRecord as DAR) {
			logic
				state: mutable int32 n = 0;
			
				onTuple DAR: {
					++n;
					DAR = {
						accountHolder = "AccountOwner" + (rstring) n,
						accountNumber = "AccountNumber" + (rstring) n,
						ticker = "Ticker" + (rstring)n, quantity = (n*1000 <= 8000) ? (n*1000) : 8000
					};
					
					submit(DAR, AccountRecord);
				} // End of onTuple DAR
		} // End of Custom(DefaultAccountRecord as DAR)

		// Functor operator allows multiple output ports with different schemas.
		(stream <rstring accountHolder, rstring accountNumber> FirstOutput;
		 stream <rstring accountHolder, rstring ticker> SecondOutput;
		 stream <rstring accountHolder, int32 quantity> ThirdOutput) = Functor(AccountRecord) {
		 	param
		 		filter: quantity < 8000;
		} // End of Functor(AccountRecord).
		
		() as ScreenWriter3 = Custom(FirstOutput) {
			logic
				state: mutable int32 err = 0;

			onTuple FirstOutput: {	
				printStringLn("FirstOutput->" + (rstring) FirstOutput);
				spl.file::fflush (0ul, err);
			} // End of onTuple FirstOutput
		} // End of Custom(FirstOutput).
		
		() as ScreenWriter4 = Custom(SecondOutput) {
			logic
				state: mutable int32 err = 0;

			onTuple SecondOutput: {	
				printStringLn("SecondOutput->" + (rstring) SecondOutput);
				spl.file::fflush (0ul, err);
			} // End of onTuple SecondOutput
		} // End of Custom(SecondOutput).
		
		() as ScreenWriter5 = Custom(ThirdOutput) {
			logic
				state: mutable int32 err = 0;

			onTuple ThirdOutput: {	
				printStringLn("ThirdOutput->" + (rstring) ThirdOutput);
				spl.file::fflush (0ul, err);
			} // End of onTuple ThirdOutput
		} // End of Custom(ThirdOutput).
} // End of composite Main.

~~~~~~
