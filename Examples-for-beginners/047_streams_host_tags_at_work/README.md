~~~~~~
/*
This example shows how to create host tags for a given Streams instance and then use those
host tags inside an SPL application. By using host tags, it is possible to avoid hard-coding
the host names inside the SPL application code. Detailed instructions about creating and using
host tags are explained in this example.

Even though this application doesn't produce any verifiable results when executed, it is a good example to
learn about using the host tags. Before you can run this example in distributed mode, you have to
create host tags within your Streams instance and then add them to your hosts. For more tips on that 
topic, please read the following file that is present in the same directory as this SPL file.

tips-on-using-streams-host-tags.txt
 
Since this example requires host tags to be created prior to running it, we deliberately removed the
launch configuration in Streams Studio. If you don't see a launch configuration for this main composite
inside the Streams Studio, please go ahead and create the host tags as shown in the tips file mentioned
above and then create a Distributed launch configuration for this main composite yourself. After that,
you can build and run this example inside Streams Studio using that launch configuration.
*/
namespace host.tags;

composite streams_host_tags_at_work {
	type
		employee = tuple<rstring name, uint32 age, uint64 salary>;
	
	graph
		// This example has 3 TCP sources and 3 corresponding TCP sinks.
		stream<employee> Employee1 = TCPSource() {    
			param   
				role : server;  
				// You can avoid hard-coding the port number by getting it from your configuration file.
				port : 23001u;  
				
			config
				// Rather than hard-coding the host name, we are getting it from the 
				// host pool defined at the bottom of this composite.
				placement: partitionColocation("Src1"), host(SourcePool1[0]);
		} 
		
		stream<employee> Employee2 = TCPSource() {    
			param   
				role : server;  
				port : 23002u;  
				
			config
				placement: partitionColocation("Src2"), host(SourcePool2[0]);
		}
		
		stream<employee> Employee3 = TCPSource() {    
			param   
				role : server;  
				port : 23003u;  
				
			config
				placement: partitionColocation("Src3"), host(SourcePool3[0]);
		}
		   
		() as DataSink1 = TCPSink(Employee1) {  
			param     
				role : server;   
				port : 23101u;  
				
			config
				placement: partitionColocation("Sink1"), host(SinkPool1[0]);
		}                     
	
		() as DataSink2 = TCPSink(Employee2) {  
			param     
				role : server;   
				port : 23102u;  

			config
				placement: partitionColocation("Sink2"), host(SinkPool2[0]);
		}     
		
		() as DataSink3 = TCPSink(Employee3) {  
			param     
				role : server;   
				port : 23103u;  
				
			config
				placement: partitionColocation("Sink3"), host(SinkPool3[0]);
		}              		
		         
	config
		hostPool: 
			// You can define host pools of different nature by using the host tags.
			// As you can see, there is no hard-coding of host names here.
			//
			// Read tips-on-using-streams-host-tags.txt from this project directory for
			// more details about creating host tags.
			// IMPORTAT: Please note that this example is expected to be tested with 
			// six machines in your instance. And, we are asking below for an exclusive access to these machines.
			// If you are trying to run this example with only one machine in your instance, then it will fail due
			// to the "Exclusive" constraint. In that case, you may want to change Exclusive to Shared.
			SourcePool1 = createPool({size=1u, tags=["tcpsource1"]}, Sys.Exclusive),
			SourcePool2 = createPool({size=1u, tags=["tcpsource2"]}, Sys.Exclusive),
			SourcePool3 = createPool({size=1u, tags=["tcpsource3"]}, Sys.Exclusive),
		  	SinkPool1 = createPool({size=1u, tags=["tcpsink1"]}, Sys.Exclusive),
		  	SinkPool2 = createPool({size=1u, tags=["tcpsink2"]}, Sys.Exclusive),
		  	SinkPool3 = createPool({size=1u, tags=["tcpsink3"]}, Sys.Exclusive);
}

~~~~~~
