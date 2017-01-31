~~~~~~
/*
This example shows the use of the Sort operator in the context of an application.
Sort operator is highly configurable with all kinds of windowing support.
In this example, the following window configurations are applied for sorting the
incoming tuples.
a) Count-based tumbling window.
b) Time-based tumbling window.
c) Punctuation-based tumbling window.
d) Delta-based tumbling window.
e) Count-based sliding window.
*/
namespace my.sample;

composite Main {
	type 
		employee = tuple<rstring name, uint32 id, rstring title, float32 salary>;

	graph
		stream <employee> EmployeeRecord = FileSource() {
			param
				file:	"EmployeeRecords.txt";
				format:	csv;
				hasDelayField: true;
				initDelay: 2.0;
		} // End of FileSource()
		
		stream <employee> SortByName = Sort(EmployeeRecord) {
			window
				EmployeeRecord: tumbling, count(5);
			
			param
				sortBy: name;
		} // End of SortByName = Sort(EmployeeRecord)
		
		stream <employee> SortById = Sort(EmployeeRecord) {
			window
				EmployeeRecord: tumbling, punct();
			
			param
				sortBy: id;
		} // End of SortById = Sort(EmployeeRecord)
		
		stream <employee> SortByTitle = Sort(EmployeeRecord) {
			window
				EmployeeRecord: tumbling, time(8);
			
			param
				sortBy: title;
		} // End of SortByTitle = Sort(EmployeeRecord)
		
		
		stream <employee> SortBySalary = Sort(EmployeeRecord) {
			window
				EmployeeRecord: tumbling, count(3), partitioned;
				
			param
				partitionBy: name;
				sortBy: salary;
		} // End of SortBySalary = Sort(EmployeeRecord)
		
		stream <employee> SortWithDeltaWindow = Sort(EmployeeRecord) {
			window
				EmployeeRecord: tumbling, delta(id, 20u);
			
			param
				sortBy: name;
		} // End of SortWithDeltaWindow = Sort(EmployeeRecord)
		
		stream <employee> SortWithSlidingWindow = Sort(EmployeeRecord) {
			window
				EmployeeRecord: sliding, count(3), count(1);
				
			param
				sortBy: name;
		} // End of SortWithSlidingWindow = Sort(EmployeeRecord)
		
		
		() as OutputWriter1 = Custom(SortByName) {
			logic
				state: 
					mutable int32 sortedTupleCnt = 0;
				
				onTuple SortByName: {
					if (sortedTupleCnt++ == 0) {	
						printStringLn ("\na) Tuples sorted by name with tumbling count(5)");
					} // End of Sort(EmployeeRecord)
					
					printStringLn((rstring) sortedTupleCnt + "a)" + (rstring)SortByName);
				} // End of onTuple SortByName
		} // End of Custom(SortByName)
		
		() as OutputWriter2 = Custom(SortById) {
			logic
				state: 
					mutable int32 sortedTupleCnt = 0;
				
				onTuple SortById: {
					if (sortedTupleCnt++ == 0) {	
						printStringLn ("\nb) Tuples sorted by id with tumbling punct()");
					} // End of Sort(EmployeeRecord)
					
					printStringLn((rstring) sortedTupleCnt + "b)" + (rstring)SortById);
				} // End of onTuple SortById
		} // End of Custom(SortById)

		() as OutputWriter3 = Custom(SortByTitle) {
			logic
				state: 
					mutable int32 sortedTupleCnt = 0;
				
				onTuple SortByTitle: {
					if (sortedTupleCnt++ == 0) {	
						printStringLn ("\nc) Tuples sorted by title with tumbling time(6)");
					} // End of Sort(EmployeeRecord)
					
					printStringLn((rstring) sortedTupleCnt + "c)" + (rstring)SortByTitle);
				} // End of onTuple SortByTitle
		} // End of Custom(SortByTitle)		
		
		() as OutputWriter4 = Custom(SortBySalary) {
			logic
				state: 
					mutable int32 sortedTupleCnt = 0;
				
				onTuple SortBySalary: {
					if (sortedTupleCnt++ == 0) {	
						printStringLn ("\nd) Tuples sorted by salary with partitioned tumbling count(3)");
					} // End of Sort(EmployeeRecord)
					
					printStringLn((rstring) sortedTupleCnt + "d)" + (rstring)SortBySalary);
				} // End of onTuple SortBySalary
		} // End of Custom(SortBySalary)				

		() as OutputWriter5 = Custom(SortWithDeltaWindow) {
			logic
				state: 
					mutable int32 sortedTupleCnt = 0;
				
				onPunct SortWithDeltaWindow: {
					printStringLn ("e) Punctuation arrived from tumbling delta(id, 20u) after " + (rstring)sortedTupleCnt + "e");
				} // End of onPunct SortWithDeltaWindow
				
				onTuple SortWithDeltaWindow: {
					if (sortedTupleCnt++ == 0) {	
						printStringLn ("\ne) Tuples sorted by name with tumbling delta(id, 20u)");
					} // End of Sort(EmployeeRecord)
					
					printStringLn((rstring) sortedTupleCnt + "e)" + (rstring)SortWithDeltaWindow);
				} // End of onTuple SortWithDeltaWindow
		} // End of Custom(SortWithDeltaWindow)

		() as OutputWriter6 = Custom(SortWithSlidingWindow) {
			logic
				state: 
					mutable int32 sortedTupleCnt = 0;
				
				onTuple SortWithSlidingWindow: {
					if (sortedTupleCnt++ == 0) {	
						printStringLn ("\nf) Tuples sorted by name with sliding, count(3)");
					} // End of Sort(EmployeeRecord)
					
					printStringLn((rstring) sortedTupleCnt + "f)" + (rstring)SortWithSlidingWindow);
				} // End of onTuple SortWithSlidingWindow
		} // End of Custom(SortWithSlidingWindow)

}

~~~~~~
