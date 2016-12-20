/*
 ==========================================================================
First created on: Jan/22/2013
Last modified on: Feb/05/2013

This file includes a set of C++ wrapper functions to invoke
specific methods inside a global/static C++ object. This is the file
that is specified in the SPL Native Function Model XML file. When the
SPL application is built, Streams compiler will look inside this file
to resolve the native function being called and to create the necessary
linkage to call this user defined native C++ code.
==========================================================================
*/
#ifndef STREAMS_TO_PYTHON_WRAPPERS_H_
#define STREAMS_TO_PYTHON_WRAPPERS_H_

// Include the file that contains the class definition.
#include "StreamsToPython.h"

namespace calling_python_from_streams {
	// Establish a handle to the StreamsToPython caller to be
	// accessed within a PE.
	inline int32 initializeStreamsToPython(void) {
		return GlobalStreamsToPythonSession::
			getGlobalStreamsToPythonSession().initializeStreamsToPython();
	} // End of method initializeStreamsToPython

	// Get the IP address of a given URL.
	inline boolean getIpAddressFromUrl(rstring const & url,
		rstring & primaryHostName, rstring & alternateHostNames,
		rstring & ipAddressList, rstring & companyName) {
		return GlobalStreamsToPythonSession::
			getGlobalStreamsToPythonSession().
			getIpAddressFromUrl(url, primaryHostName,
				alternateHostNames, ipAddressList, companyName);
	} // End of method getIpAddressFromUrl
} // End of namespace calling_python_from_streams

#endif /* STREAMS_TO_PYTHON_WRAPPERS_H_ */
