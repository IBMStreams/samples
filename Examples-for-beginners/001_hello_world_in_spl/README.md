~~~~~~ Scala
namespace demo;
/*
This example is the simplest possible SPL application.
It uses a Beacon operator to generate tuples that carry
"Hello World" messages. A custom sink operator receives
the tuples from Beacon and displays it on the console.
*/
composite HelloWorld {
	graph
		stream <rstring message> Hi = Beacon() {
			param
				iterations: 105u;
			output
				Hi: message = "Hello World, this is tuple number " + ((rstring)IterationCount());
		} // End of Beacon.
		
		() as Sink = Custom(Hi) {
			logic	
				onTuple	Hi:
					// In the standalone build, you will see this message on your console.
					// In the distributed build, you will see this message inside the
					// console log file written on the machine where this Custom operator is running.
					// /tmp/Streams-<domain_id>/logs/<host_name>/instances/<streams_instance_name>/jobs/<job_id>/pec.pe.<pe_id>.stdouterror
					printStringLn(message);
		} // End of Custom.
} // End of HelloWorld composite.
~~~~~~

## Sample output

Hello World, this is streaming tuple number 0
Hello World, this is streaming tuple number 1
Hello World, this is streaming tuple number 2
Hello World, this is streaming tuple number 3
Hello World, this is streaming tuple number 4
Hello World, this is streaming tuple number 5
Hello World, this is streaming tuple number 6
Hello World, this is streaming tuple number 7
Hello World, this is streaming tuple number 8
