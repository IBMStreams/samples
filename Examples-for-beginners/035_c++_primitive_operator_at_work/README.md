~~~~~~
/*
This example shows the mechanism used to create a C++ primitive operator 
from scratch. In this application, a C++ primitive operator model XML file
can be explored to learn how the different fields in that file are
configured. Then, the code generation template header and implementation
files (*_h.cgt and *_cpp.cgt) can be browsed to learn about the 
primitive operator logic. Additionally, this example demonstrates 
about including a Java operator and a C++ primitive operator as part of
the application flow.
*/

namespace my.sample;
// Our C++ primitive operator is in this namespace. 
// Hence, we have to declare a use directive to access the primitive operator.
use directory.lister::*;

/*
This application demonstrates two things.

1) Invoking an existing Java primitive operator.
2) Creating a new C++ primitive operator inside this application's toolkit directory.

We will use a simple scenario to do both of those steps.
We will use the standard toolkit operator JavaOp() to invoke a
Java "SystemPropertySource" operator that is shipped as part of the
Streams product. 

The output tuples from the "SystemPropertySource" operator will be sent
to the "DirectoryLister" C++ primitive operator that is created as part of
this application.

DirectoryLister C++ primitive operator will take a "listDirectory" operator
parameter, where a user can specify a list of directories in which the
file list needs to be returned.

[This application is modeled after the JavaOperators example in the
Streams example directory; except that it uses a C++ DirLister primitive
operator instead of a Java primitive operator.]

The following are the simple steps followed to create the C++ primitive operator:

a) 035_c++_primitive_operator_at_work SPL project was first created.
b) Then, by right-clicking on this project, a new Main composite was created
   in the namespace my.sample.
c) Main composite file (Main.spl) was changed with the code shown below.
d) Then, by right-clicking this SPL project name, a new SPL Primitive Operator 
   option was selected. A separate namespace directory.lister was entered there
   and a name (DirectoryLister) was entered for the C++ primitive operator. 
   When OK was clicked, behind the scenes all the skeleton files for the 
   primitive operator were created under a new directory called directory.lister.
e) Next step was to go into that directory.lister directory and start adding
   code for the operator logic inside the *_h.cgt and *_cpp.cgt files.
   You will see additional comments in those files.
*/

composite Main {
	// Define the tuple types that we will use.
	type
		systemProperties = tuple<ustring name, ustring value, set <ustring> tags>;
		dirListing = tuple<systemProperties, tuple<list <ustring> files>>;
	
	graph
		// Let us use the SystemPropertySource primitive operator to
		// emit the necessary system property tuples.
		// We will use the JavaOP operator to do the call-out to the
		// SystemPropertySource operator.
		stream <systemProperties> SystemPropertyTuple = JavaOp() {
			param
				className: "com.ibm.streams.operator.samples.sources.SystemPropertySource";
				classLibrary: "";
		} // End of SystemPropertyTuple = JavaOp()
	
		// Let us now invoke the C++ operator that will return 
		// the enriched tuples by listing the directory contents for
		// those directories opted below via the listDirectory parameter.
		stream <dirListing> DirectoryListerTuple = DirectoryLister(SystemPropertyTuple) {
			// This primitive operator accepts a single expression type parameter
			// that will take unbounded number of values.
			param
				listDirectory: "java.home", "user.dir", "java.io.tmpdir";	
		} // End of DirectoryListerTuple = DirectoryLister(SystemPropertyTuple)
		
		// Write the tuples coming out of the C++ primitive operators to 
		// the file sink.
		() as ResultsSink = FileSink(DirectoryListerTuple) {
			param
				file: "properties.txt";
		} // End of ResultsSink = FileSink(DirectoryListerTuple)
		
} // End of composite Main

~~~~~~
