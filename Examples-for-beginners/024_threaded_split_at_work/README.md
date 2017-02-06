~~~~~~ Scala 
/*
This example demonstrates an important standard toolkit operator named
ThreadedSplit. It is a multi-threaded split that is differnt from the other
content-based Split opertor. ThreadedSplit splits the incoming tuples to the
available output ports to improve concurrency. This will speed up the 
distribution of tuples by using the individual threads assigned to each of
the output ports. 
*/
namespace my.sample;

composite Main {
	type
		bookInfo = tuple<rstring title, rstring isbn, rstring author, rstring publisher, float32 price>;
		
	graph
		stream <bookInfo> DefaultBoolInfo = Beacon() {
			param
				iterations: 500000u;
		} // End of BookInfo1 = Beacon()

		// In a custom operator, let us set the attributes of the BookInfo1 tuples.
		stream<bookInfo> BookInfo = Custom(DefaultBoolInfo as DBF) {
			logic
				state: mutable uint32 cnt = 0;
				
				onTuple DBF: {
					++cnt;
					
					DBF = {
						title = "Title" + (rstring)cnt,
						isbn = "ISBN" + (rstring)cnt,
						author= "Author" + (rstring)cnt,
						publisher = "Publisher" + (rstring)cnt,
						price= (float32)random()*(float32)100.0				
					};
					
					submit(DBF, BookInfo);
				} // End of onTuple DBF1	
		} // End of Custom(DefaultBoolInfo1)

		// Let us fan-out this heavy load into 10 threaded output ports.
		(stream <bookInfo> BookInfo1;
		 stream <bookInfo> BookInfo2;
		 stream <bookInfo> BookInfo3;
		 stream <bookInfo> BookInfo4;
		 stream <bookInfo> BookInfo5;
		 stream <bookInfo> BookInfo6;
		 stream <bookInfo> BookInfo7;
		 stream <bookInfo> BookInfo8;
		 stream <bookInfo> BookInfo9;
		 stream <bookInfo> BookInfo10) = ThreadedSplit(BookInfo) {
		 	param
		 		bufferSize: 3000u;
		 } // End of (... BookInfo10) = ThreadedSplit(BookInfo)
		 
		 // Let us try to display all the output streams.s
		 // Ideally, the following code should be wrapped in a PERL loop.
		 // But, that is part of the DMM exercise for another time.
		() as ScreenWriter1 = Custom(BookInfo1) {
			logic
				state: 
					mutable int32 splitTupleCnt = 0;
				
				onTuple BookInfo1: {
					if (splitTupleCnt++ == 0) {
						printStringLn("\na)Split tuples from output port 1 of the ThrededSplit:");
					} // End of if (splitTupleCnt++ == 0)
					
					printStringLn ((rstring) splitTupleCnt + "a)" + (rstring) BookInfo1);
				} // End of onTuple BookInfo1
		} // End of ScreenWriter1 = Custom(BookInfo1)

		() as ScreenWriter2 = Custom(BookInfo2) {
			logic
				state: 
					mutable int32 splitTupleCnt = 0;
				
				onTuple BookInfo2: {
					if (splitTupleCnt++ == 0) {
						printStringLn("\nb)Split tuples from output port 2 of the ThrededSplit:");
					} // End of if (splitTupleCnt++ == 0)
					
					printStringLn ((rstring) splitTupleCnt + "b)" + (rstring) BookInfo2);
				} // End of onTuple BookInfo2
		} // End of ScreenWriter2 = Custom(BookInfo2)

		() as ScreenWriter3 = Custom(BookInfo3) {
			logic
				state: 
					mutable int32 splitTupleCnt = 0;
				
				onTuple BookInfo3: {
					if (splitTupleCnt++ == 0) {
						printStringLn("\nc)Split tuples from output port 3 of the ThrededSplit:");
					} // End of if (splitTupleCnt++ == 0)
					
					printStringLn ((rstring) splitTupleCnt + "c)" + (rstring) BookInfo3);
				} // End of onTuple BookInfo3
		} // End of ScreenWriter3 = Custom(BookInfo3)

		() as ScreenWriter4 = Custom(BookInfo4) {
			logic
				state: 
					mutable int32 splitTupleCnt = 0;
				
				onTuple BookInfo4: {
					if (splitTupleCnt++ == 0) {
						printStringLn("\nd)Split tuples from output port 4 of the ThrededSplit:");
					} // End of if (splitTupleCnt++ == 0)
					
					printStringLn ((rstring) splitTupleCnt + "d)" + (rstring) BookInfo4);
				} // End of onTuple BookInfo4
		} // End of ScreenWriter4 = Custom(BookInfo4)

		() as ScreenWriter5 = Custom(BookInfo5) {
			logic
				state: 
					mutable int32 splitTupleCnt = 0;
				
				onTuple BookInfo5: {
					if (splitTupleCnt++ == 0) {
						printStringLn("\ne)Split tuples from output port 5 of the ThrededSplit:");
					} // End of if (splitTupleCnt++ == 0)
					
					printStringLn ((rstring) splitTupleCnt + "e)" + (rstring) BookInfo5);
				} // End of onTuple BookInfo5
		} // End of ScreenWriter5 = Custom(BookInfo5)

		() as ScreenWriter6 = Custom(BookInfo6) {
			logic
				state: 
					mutable int32 splitTupleCnt = 0;
				
				onTuple BookInfo6: {
					if (splitTupleCnt++ == 0) {
						printStringLn("\nf)Split tuples from output port 6 of the ThrededSplit:");
					} // End of if (splitTupleCnt++ == 0)
					
					printStringLn ((rstring) splitTupleCnt + "f)" + (rstring) BookInfo6);
				} // End of onTuple BookInfo6
		} // End of ScreenWriter6 = Custom(BookInfo6)

		() as ScreenWriter7 = Custom(BookInfo7) {
			logic
				state: 
					mutable int32 splitTupleCnt = 0;
				
				onTuple BookInfo7: {
					if (splitTupleCnt++ == 0) {
						printStringLn("\ng)Split tuples from output port 7 of the ThrededSplit:");
					} // End of if (splitTupleCnt++ == 0)
					
					printStringLn ((rstring) splitTupleCnt + "g)" + (rstring) BookInfo7);
				} // End of onTuple BookInfo7
		} // End of ScreenWriter1 = Custom(BookInfo7)

		() as ScreenWriter8 = Custom(BookInfo8) {
			logic
				state: 
					mutable int32 splitTupleCnt = 0;
				
				onTuple BookInfo8: {
					if (splitTupleCnt++ == 0) {
						printStringLn("\nh)Split tuples from output port 8 of the ThrededSplit:");
					} // End of if (splitTupleCnt++ == 0)
					
					printStringLn ((rstring) splitTupleCnt + "h)" + (rstring) BookInfo8);
				} // End of onTuple BookInfo8
		} // End of ScreenWriter8 = Custom(BookInfo8)

		() as ScreenWriter9 = Custom(BookInfo9) {
			logic
				state: 
					mutable int32 splitTupleCnt = 0;
				
				onTuple BookInfo9: {
					if (splitTupleCnt++ == 0) {
						printStringLn("\ni)Split tuples from output port 9 of the ThrededSplit:");
					} // End of if (splitTupleCnt++ == 0)
					
					printStringLn ((rstring) splitTupleCnt + "i)" + (rstring) BookInfo9);
				} // End of onTuple BookInfo9
		} // End of ScreenWriter9 = Custom(BookInfo9)

		() as ScreenWriter10 = Custom(BookInfo10) {
			logic
				state: 
					mutable int32 splitTupleCnt = 0;
				
				onTuple BookInfo10: {
					if (splitTupleCnt++ == 0) {
						printStringLn("\nj)Split tuples from output port 10 of the ThrededSplit:");
					} // End of if (splitTupleCnt++ == 0)
					
					printStringLn ((rstring) splitTupleCnt + "j)" + (rstring) BookInfo10);
				} // End of onTuple BookInfo10
		} // End of ScreenWriter10 = Custom(BookInfo10)
} // End of the composite main

~~~~~~
