/*
 ==========================================================================
First created on: Jan/22/2013
Last modified on: Feb/05/2013

This file provides the class interface for the StreamsToPython caller.
==========================================================================
*/
#ifndef STREAMS_TO_PYTHON_H_
#define STREAMS_TO_PYTHON_H_

using namespace std;

// To avoid a redefinition compiler error, undefine the following.
#undef _POSIX_C_SOURCE
#undef _XOPEN_SOURCE
// This should be the first include file (according to Python documentation)
#include "Python.h"

// Include files that defines SPL types and functions.
#include "SPL/Runtime/Function/SPLFunctions.h"
#include <SPL/Runtime/Utility/Mutex.h>
#include <SPL/Runtime/Type/ValueHandle.h>
// Include standard C++ include files.
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <sstream>

// This will allow us to access the types and functions from SPL namespace.
using namespace SPL;

// Your #define constant definitions go here.

// Class definition follows here.
namespace calling_python_from_streams {
	class GlobalStreamsToPythonSession {
		private:
			// This member variable tells us if a global
			// streams to python caller handle already
			// exists for a given PE/process.
			boolean streamsToPythonHandleExists;

			// Following member variables are required for
			// calling Python C APIs.
			static boolean pyInitialized;
			static boolean importFailed;
			PyObject* pFunc1;
			PyObject* pFunc2;

		public:
			GlobalStreamsToPythonSession();
			virtual ~GlobalStreamsToPythonSession();

			// This method establishes StreamsToPython handle for a given PE/process.
			int32 initializeStreamsToPython();
			// This method gets the IP address of a given URL.
			boolean getIpAddressFromUrl(rstring const & url,
				rstring & primaryHostName, rstring & alternateHostNames,
				rstring & ipAddressList, rstring & companyName);

			// Get the global (Singleton) Streams to Python session object.
			static GlobalStreamsToPythonSession & getGlobalStreamsToPythonSession();
	}; // End of class GlobalStreamsToPythonSession

} // End of namespace calling_python_from_streams

#endif /* STREAMS_TO_PYTHON_H_ */
