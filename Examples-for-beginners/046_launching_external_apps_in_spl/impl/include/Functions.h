#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

// Include this SPL file so that we can use the SPL functions and types in this C++ code.
#include "SPL/Runtime/Function/SPLFunctions.h"
#include <vector>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>

// Define a C++ namespace that will contain our native function code.
namespace cpp_app_launcher {
	// By including this line, we will have access to the SPL namespace and anything defined within that.
	using namespace SPL;

	// Our native function prototype.
	int32 launch_app(rstring const & appName);

	// Inline native function to launch an external application within the SPL code.
	// This function takes one rstring argument through which the caller must pass the
	// name of the application to launch (as a fully qualified path: /tmp/test/my_script.sh).
	inline int32 launch_app(rstring const & appName) {
	   FILE *fpipe;
	   int32 rc = 0;

	   // Open a pipe with the application name as provided by the caller.
	   if (!(fpipe = (FILE*)popen(appName.c_str(), "r"))) {
		   SPLAPPTRC(L_ERROR, "Failure while launching " + appName, "APP_LAUNCHER");
		   rc = 1;
		   return(rc);
	   }

	   // Close the pipe.
	   pclose (fpipe);
	   return(rc);
	}
}
#endif
