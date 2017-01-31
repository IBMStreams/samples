~~~~~~
/*
This example shows how to launch/execute an external application within the Streams SPL code.
In this case, we defined a simple C++ native function in which we have the required C++ code to
launch an external application. That C++ code uses pipes to execute a given application.
This function would be useful to launch any custom script within the Streams application logic when
certain application specific conditions arise.
*/
namespace launch.external.apps;

composite launching_external_apps {
	graph
		// Create a simple source signal using a Beacon to kickstart this application.
		stream<int32 j> TestStream1 = Beacon() {
			param
				iterations: 1u;
		}

		// We will have a single Custom operator within which we will launch an external application.		
		() as ScreenWriter1 = Custom(TestStream1) {
			logic
				onTuple TestStream1: {
					printStringLn("About to launch a script ...");
					// It has to be a fully qualified path. (e-g: /tmp/test/my-script.sh)
					// In our case, we have a simple shell script available in this application's data directory.
					// This particular shell script will create the results in three different text files inside
					// the data directory of this SPL project.
					// This function is a C++ native function defined in the impl/include/Functions.h file.
					// Also check the native.function/function.xml file for the SPL native function model description.
					//
					// In Streams 4.x, we must get the data directory manually via an API for this to work correctly.
					// We will also pass the data directory as the first argument to the shell script we are launching below.
					rstring dataDir = dataDirectory();
					int32 rc = launch_app(dataDir + "/test-script.sh   " + dataDir);
					
					if (rc != 0) {
						printStringLn("Launching the given application failed.");
					} else {
						printStringLn("Launching the given application succeeded.");
						printStringLn("Check the launched script and its results inside the data directory of this application.");
					}
				}
		}
}

~~~~~~
