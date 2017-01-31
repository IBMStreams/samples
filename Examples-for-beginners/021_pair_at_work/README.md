~~~~~~
/*
This example shows off the Pair operator that is used for pairing
tuples arriving on different input ports. Only when all the tuples
arrive at all the input ports, this operator will emit them one
after the other in their order of arrival.
*/
namespace my.sample;

composite Main {
	type
		bookInfo = tuple<rstring title, rstring isbn, rstring author, rstring publisher, float32 price>;
		
	graph
		stream <bookInfo> DefaultBoolInfo1 = Beacon() {
			param
				iterations: 100u;
		} // End of BookInfo1 = Beacon()

		// In a custom operator, let us set the attributes of the BookInfo1 tuples.
		stream<bookInfo> BookInfo1 = Custom(DefaultBoolInfo1 as DBF1) {
			logic
				state: mutable uint32 cnt = 0;
				
				onTuple DBF1: {
					++cnt;
					
					DBF1 = {
						title = "Title" + (rstring)cnt,
						isbn = "ISBN" + (rstring)cnt,
						author= "Author" + (rstring)cnt,
						publisher = "Publisher" + (rstring)cnt,
						price= (float32)random()*(float32)100.0				
					};
					
					submit(DBF1, BookInfo1);
				} // End of onTuple DBF1	
		} // End of Custom(DefaultBoolInfo1)

		stream <bookInfo> DefaultBoolInfo2 = Beacon() {
			param
				iterations: 100u;
		} // End of BookInfo2 = Beacon()

		// In a custom operator, let us set the attributes of the BookInfo1 tuples.
		stream<bookInfo> BookInfo2 = Custom(DefaultBoolInfo2 as DBF2) {
			logic
				state: mutable uint32 cnt = 400000;
				
				onTuple DBF2: {
					++cnt;
					
					DBF2 = {
						title = "Title" + (rstring)cnt,
						isbn = "ISBN" + (rstring)cnt,
						author= "Author" + (rstring)cnt,
						publisher = "Publisher" + (rstring)cnt,
						price= (float32)random()*(float32)100.0				
					};
					
					submit(DBF2, BookInfo2);
				} // End of onTuple DBF2	
		} // End of Custom(DefaultBoolInfo1)


		// Let us now pair these two streams.
		stream <bookInfo> PairedTuple = Pair(BookInfo1; BookInfo2) {
		} // End of PairedTuple = Pair(BookInfo1; BookInfo2)
		
		() as ScreenWriter1 = Custom(PairedTuple) {
			logic
				state: {
					mutable int32 cnt = 1;	
					mutable boolean startOfNewPair = true;
				} // End of state:
			
			onTuple PairedTuple: {
				if (cnt == 1 && startOfNewPair) {
					printStringLn("Paired tuples:");
				} // End of if (cnt++ == 0)
				
				if (startOfNewPair == true) {
					// Print the 1st tuple in the pair.
					printStringLn((rstring)cnt + "a)" + (rstring)PairedTuple);
					startOfNewPair = false;
				} else {
					// Print the 2nd tuple in the pair.
					printStringLn((rstring)(cnt++) + "b)" + (rstring)PairedTuple);					
					// After printing a pair, give an extra line break.
					printStringLn("");
					startOfNewPair = true;
				} // End of if (startOfNewPair == true)
			} // End of onTuple PairedTuple
		} // End of ScreenWriter1 = Custom(PairedTuple)
} // End of the main composite.

~~~~~~
