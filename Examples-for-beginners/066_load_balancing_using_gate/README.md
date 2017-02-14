~~~~~~ Scala 
/*
This application demonstrates how we can combine the ThreadedSplit and Gate operators to
achieve load balancing of tuples. This idea appears in the Streams InfoCenter documentation for
the Gate operator. I thought it is useful to include that example here to benefit our users.
Full credit goes to the InfoCenter authors for the following problem description and the code.

When using ThreadedSplit, a tuple with a long processing time may cause subsequent tuples to be
backed up in the stream waiting for processing (even though there may be another thread available and
idle. This may be aggravated by tuples that are in a TCP/IP connection to another PE.

In order to ensure load balancing, a ThreadedSplit operator with a buffer size of 1 may be tied to 
two or more Gate operators with a maximum unacknowledged tuple count of 1 or more. The ThreadedSplit
and the Gate operators must be on the same PE, to avoid tuples queuing between PEs. A partitionColocation
placement config is used to ensure this.

The Gate operator will allow only the specified number of tuples (1 in this case) to pass at a time.
It will wait until a subsequent operator has acknowledged receipt of the tuple before passing the next
tuple. In this manner, no tuple will ever be queued behind another, waiting to be processed.

(To see the actual effect of this example, it is better to run this application in Distributed Mode.) 
*/
namespace com.acme.test;

composite LoadBalancingUsingGate {
	graph
		// Generate a stream of data to process                                                    
		stream<uint32 i> I = Beacon(){                                                             
			logic                                                                                    
				state : mutable uint32 c = 0 ;                                                         
			
			output                                                                                     
				I : i = c ++ ;                                                                         
		}
		
		// Split the stream into 2 streams.  Use a following Gate to ensure load balancing
		// Fuse the ThreadedSplit and the Gate operators so that there is no queuing between them.         
		(stream<I> X ; stream<I> Y)= ThreadedSplit(I){                                             
			param                                                                                    
				bufferSize : 1u ;                                                                      
		
			config                                                                                   
				placement : partitionColocation("Split"), // ensure same PE as the Gates
				// Make it away from the downstream processing operator.               
				partitionExlocation("Process");                                            
    	}
    	
		stream<I> O0 = Gate(X ; Control0){                                                         
			param                                                                                    
				maxUnackedTupleCount : 1u ;
				                                                            
			config                                                                                   
				placement : partitionColocation("Split"); // ensure same PE as ThreadedSplit                                                                                                          
		}    	

		stream<I> O1 = Gate(Y ; Control1){                                                         
			param                                                                                    
				maxUnackedTupleCount : 1u ;
				                                                            
			config                                                                                   
				placement : partitionColocation("Split"); // ensure same PE as ThreadedSplit           
    	}

		(stream<I> R0 as out ; stream<uint32 i> Control0 as control)= Custom(O0 as In) {            
			logic                                                                                    
				onTuple In : {                                                                         
					// do some processing                                                                
					submit(In, out); // forward tuple                                                    
					submit({ i = 1u }, control);                                                         
				}                                                                                      

			// Place on a different PE from Gate or other processing operator                                                                                                                     
			config                                                                                   
			placement : partitionExlocation("Process");                                            
		}

		(stream<I> R1 as out ; stream<uint32 i> Control1 as control)= Custom(O1 as In){            
			logic                                                                                    
				onTuple In : {                                                                         
					// do some processing                                                                
					submit(In, out); // forward tuple                                                    
					submit({ i = 1u }, control);                                                         
				}
				                                                                                      
			// Place on a different PE from Gate and other processing operator                     
			config                                                                                   
				placement : partitionExlocation("Process");                                            
		}
		
		() as Sink1 = FileSink(R0) {
			param
				file: "/dev/null";
		}
		
		() as Sink2 = FileSink(R1) {
			param
				file: "/dev/null";
		}
}

~~~~~~
