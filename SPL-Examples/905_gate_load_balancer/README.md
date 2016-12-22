~~~~~~
/*
This example is the same code that can be found in the SPL Standard Toolkit PDF file.
Please see that PDF file for a description about what this application does.
*/
namespace my.sample ;
// Implementing Load Balancing of Tuples

// A ThreadedSplit operator may be used to split a stream, 
// and process tuples in parallel.
// This works for many applications, but if the processing time of 
// a tuple varies considerably depending on the tuple data, it may 
// cause problems where a tuple with a long processing time may cause 
// subsequent tuples to be backed up in the stream waiting for processing,
// even though there may be another thread available and idle.
// This may be aggravated by tuples that are in a TCP/IP connection to 
// another PE. In order to ensure load balancing, a ThreadedSplit 
// operator with a buffer size of 1 may be tied to two or more Gate 
// operators with a maximum unacknowled tuple count of 1 or more.
// The ThreadedSplit and the  Gate operators must be on the same PE, to 
// avoid tuples queuing between PEs. A partitionColocation placement 
// config is used to ensure this.
//
// The Gate operator will allow only the specified number of tuples 
// (1 in this case)  to pass at a time.  It will wait until a 
// subsequent operator has acknowledged  receipt of the tuple before 
// passing the next tuple.  In this manner, no tuple will  ever be 
// queued behind another, waiting to be processed.
//
composite Main {
	graph
		// Generate a stream of data to process
		stream <uint32 i > I = Beacon(){
			logic
				state : mutable uint32 c = 0 ;

			output I : 
				i = c ++ ;
		} // End of I = Beacon()
		
		// Split the stream into 2 streams.  Use a following Gate to ensure load balancing
		( stream < I > X ; stream < I > Y) =ThreadedSplit ( I){
			param 
				bufferSize : 1u ;
				
			config
				placement : partitionColocation( "Split") , // ensure same PE as the Gates
				partitionExlocation( "Process") ;
		}  // End of (... stream < I > Y) =ThreadedSplit(I)
		
		stream <I > O0 = Gate ( X ; Control0 ) {
			param
				maxUnackedTupleCount : 1u ; 
				
			config 
				placement : partitionColocation( "Split") ; // ensure same PE as ThreadedSplit
		} // End of O0 = Gate ( X ; Control0 )
		
		stream <I > O1 = Gate ( Y ; Control1 ) { 
			param 
				maxUnackedTupleCount : 1u ; 

			config 
				placement : partitionColocation("Split") ; // ensure same PE as ThreadedSplit
		} // End of O1 = Gate ( Y ; Control1 )

		( stream <I > R0 as out ; stream < uint32 i > Control0 as control) = Custom ( O0 as In ) { 
			logic 
				onTuple In : { 
					// do some processing
					submit ( In , out ) ; // forward tuple
					submit({i = 1u} , control ) ; 
				} // End of onTupe In
				
			// Place on a different PE from Gate or other processing operator
			config 
				placement : partitionExlocation ( "Process" ) ; 
		} // End of (... Control0 as control) = Custom ( O0 as In )
		
		( stream < I > R1 as out ; stream < uint32 i > Control1 as control ) = Custom ( O1 as In ) {
			logic
				onTuple In : { 
					// do some processing
					submit ( In , out ) ; // forward tuple
					submit ( { i = 1u } , control ) ; 
				} 

			// Place on a different PE from Gate and other processing operator
			config
				placement : partitionExlocation ( "Process" ) ; 
		} // End of (... Control1 as control ) = Custom ( O1 as In )
} // End of composite Main













~~~~~~
