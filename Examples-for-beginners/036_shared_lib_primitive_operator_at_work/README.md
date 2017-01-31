~~~~~~
/*
This example demonstrates two important techniques that will be commonly
used in real-world usecases.

1) Creating a C++ primitive operator.
2) Calling a function available inside a .so shared library from 
   the C++ primitive operator logic.

Application logic here is to receive input tuples as hostnames and then
make the C++ primitive operator logic invoke a shared library function 
that does a nameserver lookup.
*/
namespace my.sample;
// Our C++ primitive operator is in the following namespace.
// Hence, we have to declare the use directory to access the C++ primitive operator. 
use ns.lookup::*;

/*
This application shows how a C++ primitive operator can use functions from a
C++ shared library (.so file). In order to accomplish that, it is going to use a shared
library that was built using the CDT perspective in another C++ project named 
"PrimitiveOperatorLib". You can find that CDT project in the same 
Eclipse workspace as this SPL project. Please refer to that project to know 
how that shared library is built.

After building the shared library using CDT, it is advisable to copy the 
C++ interface file and the .so file to the SPL project's impl/include and 
impl/lib directories respectively. That way, this SPL project will be
self-contained with the C++ interface file and the .so file inside of it.
For the sake of completion, it is also advisable to copy the C++ file to
the impl/src directory.

The following are the simple steps followed to create the C++ primitive operator:

a) 036_shared_lib_primitive_operator_at_work SPL project was first created.
b) Then, by right-clicking on this project, a new Main composite was created
   in the namespace my.sample.
c) Main composite file (Main.spl) was changed with the code shown below.
d) Then, by right-clicking this SPL project name, a new SPL Primitive Operator 
   option was selected. A separate namespace ns.lookup was entered there
   and a name (NSLookup) was entered for the C++ primitive operator. 
   When OK was clicked, behind the scenes all the skeleton files for the 
   primitive operator were created under a new directory called ns.lookup.
e) Next step was to go into that ns.lookup directory and start adding
   code for the operator logic inside the *_h.cgt and *_cpp.cgt files.
   You will see additional comments in those files.
*/

composite Main {
	// Define the tuple types we will use in this application.
	type
		nsLookupRequest = tuple<rstring nodeName>;
		nsLookupResponse = tuple<nsLookupRequest, 
			tuple<rstring nameServerName, rstring nameServerAddress, 
			rstring clientMachineName, rstring clientMachineAddress>>;
	
	graph
		// Let us read the input tuples using the FileSource operator.
		stream <nsLookupRequest> NSLookupRequest = FileSource() {
			param
				// By default, node names in this file point to 
				// test machines used in the Streams product team.
				// Before running this application, you have to
				// modity this file in the ./data directory to have
				// machines names in your network.
				file: "ns-lookup-nodes.txt";
				format: csv;
				hasDelayField: true;
				initDelay: 4.0f;
		} // End of NSLookupRequest = FileSource()
		
		// Invoke the NSLookup primitive operator.
		stream <nsLookupResponse> NSLookupResponse = NSLookup(NSLookupRequest) {
			param
				// NSLookup computes 4 different values as shown in the response tuple type.
				// By default all the 4 fields will be returned back to us.
				// Optionally we can also tell the operator to send or NOT send a value for a field.
				// Any field not specified below will have its value returned by the operator.
				nameServerNameNeeded: true;
				nameServerAddressNeeded: false;
				clientMachineNameNeeded: true;
				clientMachineAddressNeeded: true;
		} // End of NSLookupResponse = NSLookup(NSLookupRequest)
		
		// Store the NSLookup results to a sink.
		() as FileStore = FileSink(NSLookupResponse) {
			param
				file: "ns-lookup-results.txt";
				format: csv;
		} // End of FileStore = FileSink(NSLookupResponse)
} // End of composite Main

~~~~~~
