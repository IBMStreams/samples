/*
==========================================================================
First created on: Jan/22/2013
Last modified on: Feb/12/2013

This file provides the class implementation for the StreamsToPython calls.
==========================================================================
*/

#include "StreamsToPython.h"
#include <dlfcn.h>

namespace calling_python_from_streams {
	// Initialize the static member variables in this class.
	boolean GlobalStreamsToPythonSession::pyInitialized = false;
	boolean GlobalStreamsToPythonSession::importFailed = false;

	// Define a constructor for the global streams to python session class.
	GlobalStreamsToPythonSession::GlobalStreamsToPythonSession() {
		// No streams to python session handle exists at this time.
		streamsToPythonHandleExists = false;
		pFunc1 = NULL;
		pFunc2 = NULL;

		// Let us load the python shared library.
		// This was tested using Python 2.6 that shipped with RHEL6.1.
		// If you are testing it on a different version of RHEL,
		// please load the correct version of python below.
		SPLAPPTRC(L_ERROR, "Calling dlopen", "STREAMS_TO_PYTHON");
		void* handle = dlopen("libpython2.6.so", RTLD_NOW|RTLD_GLOBAL);

		if (handle == false) {
			SPLAPPTRC(L_ERROR, "dlopen failed " << dlerror(), "STREAMS_TO_PYTHON");
		} // End of if (handle == false)

		// Per PE/process, we want to call the python initialize API only once.
		if ((handle != false) && (pyInitialized == false)) {
			// We expect the actual Python script we will execute below to be
			// available in the Streams application's data directory.
			// That data directory is the current working directory for the
			// Streams application. Let us add the current working directory
			// to the PYTHONPATH environment variable.
			// Without doing this, we will not be able to load the
			// Python script later using the PyImport_Import API.
			setenv("PYTHONPATH", ".", 1);
			Py_Initialize();
			pyInitialized = true;
		} // End of if (pyInitialized == false)
	} // End of constructor

	// Define a destructor method to do any cleanup.
	GlobalStreamsToPythonSession::~GlobalStreamsToPythonSession() {
		if (streamsToPythonHandleExists == true) {
			streamsToPythonHandleExists = false;
		}

		// We can decrement the reference count for the Python functions.
		if (pFunc1 != NULL) {
			Py_DECREF(pFunc1);
		} // End of if (pFunc1 != NULL)

		if (pFunc2 != NULL) {
			Py_DECREF(pFunc2);
		} // End of if (pFunc2 != NULL)
	} // End of destructor

	// Define a method that creates and returns a singleton access to
	// our global StreamsToPython session object. This will guarantee us that there will
        // always be only one instance of this class per PE.
	GlobalStreamsToPythonSession & GlobalStreamsToPythonSession::getGlobalStreamsToPythonSession() {
		// A static variable that will get initialized only once per process.
		static GlobalStreamsToPythonSession gstps;
		// Return our singleton global streams to python session object.
		return gstps;
	} // End of method getGlobalStreamsToPythonSession

	// This method initializes the streams to python native function module.
	// It is done only once per PE/process. (This is an optional method that
        // a Streams PE can call to do any application-specific initialization of
        // state information stored in this C++ class.)
	int32 GlobalStreamsToPythonSession::initializeStreamsToPython() {
		SPLAPPTRC(L_ERROR, "initializeStreamsToPython request received from a thread id "
			<< pthread_self() << " within the process id " <<
			getpid() << ".", "STREAMS_TO_PYTHON");

		// Acquire a lock so that we can avoid multiple threads colliding with each other below.
		///// AutoMutex am(mutex);

		// If we already have established a streams to python handle for this PE/process, then return now.
		if (streamsToPythonHandleExists == true) {
			SPLAPPTRC(L_ERROR, "A global and a shared StreamsToPython handle has " <<
				"already been made by another thread in the same process id.", "STREAMS_TO_PYTHON");
			return (0);
		} // End of if (streamsToPythonHandleExists == true)

		SPLAPPTRC(L_ERROR, "This global StreamsToPython handle can now be shared " <<
			"by multiple threads within the process id " <<	getpid() << ".", "STREAMS_TO_PYTHON");

		streamsToPythonHandleExists = true;
		return (0);
	} // End of method initializeStreamsToPython

        // This is the C++ native function method that can be called from SPL application code to
        // invoke Python functions for getting IP address details of a given URL.
	boolean GlobalStreamsToPythonSession::getIpAddressFromUrl(
		rstring const & url,
		rstring & primaryHostName, rstring & alternateHostNames,
		rstring & ipAddressList, rstring & companyName) {

		// We must first get a pointer to the Python functions we want to call.
		// If an earlier attempt to access the python functions failed, we can't proceed.
		if (importFailed == true) {
			return(false);
		}

		// For the very first time we are here, let us get a pointer to the first python function.
		if (pFunc1 == NULL) {
			PyObject *pName, *pModule;
			pName = pModule = NULL;
			// In Streams 4.x, data sub-directory inside the application directory is no longer the current working directory.
			// This is because of the application bundles getting placed in a random directory on target servers.
			// Hence, it is necessary to obtain the data directory manually via an API and add it to the Python system path.
			std::string dataDirectory = SPL::Functions::Utility::dataDirectory();
			std::string pythonPathAppendString = "import sys\nsys.path.append('" + dataDirectory + "')";
			PyRun_SimpleString(pythonPathAppendString.c_str());

			// Get a python string for the name of our Python module (i.e. Python source file name)
			pName = PyString_FromString("UrlToIpAddress");

			if (pName == NULL) {
				SPLAPPTRC(L_ERROR, "Failed to convert UrlToIpAddress to python string.", "STREAMS_TO_PYTHON");

				if (PyErr_Occurred()) {
					PyErr_Print();
				}

				importFailed = true;
				return(false);
			} // End of if (pName == NULL)

			// Using the python string carrying the python file name, let us import the module now.
			pModule = PyImport_Import(pName);
			// We can release pName since we don't need it anymore.
			Py_DECREF(pName);

			if (pModule == NULL) {
				SPLAPPTRC(L_ERROR, "Failed to import UrlToIpAddress python module.", "STREAMS_TO_PYTHON");

				if (PyErr_Occurred()) {
					PyErr_Print();
				}

				importFailed = true;
				return(false);
			} // End of if (pModule == NULL)

			// We have to check if this python module has our first function getIpAddressFromUrl.
			if (PyObject_HasAttrString(pModule, "getIpAddressFromUrl") == 0) {
				SPLAPPTRC(L_ERROR, "Module doesn't have the getIpAddressFromUrl function.", "STREAMS_TO_PYTHON");

				if (PyErr_Occurred()) {
					PyErr_Print();
				}

				importFailed = true;
				// Decrement the reference count for the module.
				Py_DECREF(pModule);
				return(false);
			} // End of if (PyObject_HasAttrString(pModule, "getIpAddressFromUrl") == 0)

			// Let us get a pointer to the python function we want to call.
			pFunc1 = PyObject_GetAttrString(pModule, "getIpAddressFromUrl");

			if (pFunc1 == NULL) {
				SPLAPPTRC(L_ERROR, "Unable to get the getIpAddressFromUrl function from the python module.", "STREAMS_TO_PYTHON");

				if (PyErr_Occurred()) {
					PyErr_Print();
				}

				importFailed = true;
				// Decrement the reference count for the module.
				Py_DECREF(pModule);
				return(false);
			} // End of if (pFunct1 == NULL)

			// Now that we have a pointer to the python function, let us verify that
			// we can indeed make a call to that function.
			if (PyCallable_Check(pFunc1) == 0) {
				SPLAPPTRC(L_ERROR, "Python function getIpAddressFromUrl is not callable.", "STREAMS_TO_PYTHON");

				if (PyErr_Occurred()) {
					PyErr_Print();
				}

				importFailed = true;
				// Decrement the reference count for the module.
				Py_DECREF(pModule);
				return(false);
			} // End of if (PyCallable_Check(pFunc1) == 0)

			// Let us get a pointer to our second python function as well now.
			// We have to check if this python module has our second function getCompanyNameFromUrl.
			if (PyObject_HasAttrString(pModule, "getCompanyNameFromUrl") == 0) {
				SPLAPPTRC(L_ERROR, "Module doesn't have the getCompanyNameFromUrl function.", "STREAMS_TO_PYTHON");

				if (PyErr_Occurred()) {
					PyErr_Print();
				}

				importFailed = true;
				// Decrement the reference count for the module.
				Py_DECREF(pModule);
				return(false);
			} // End of if (PyObject_HasAttrString(pModule, "getCompanyNameFromUrl") == 0)

			// Let us get a pointer to the second python function we want to call.
			pFunc2 = PyObject_GetAttrString(pModule, "getCompanyNameFromUrl");

			if (pFunc2 == NULL) {
				SPLAPPTRC(L_ERROR, "Unable to get the getCompanyNameFromUrl function from the python module.", "STREAMS_TO_PYTHON");

				if (PyErr_Occurred()) {
					PyErr_Print();
				}

				importFailed = true;
				// Decrement the reference count for the module.
				Py_DECREF(pModule);
				return(false);
			} // End of if (pFunct2 == NULL)

			// Now that we have a pointer to the second python function, let us verify that
			// we can indeed make a call to that function.
			if (PyCallable_Check(pFunc2) == 0) {
				SPLAPPTRC(L_ERROR, "Python function getCompanyNameFromUrl is not callable.", "STREAMS_TO_PYTHON");

				if (PyErr_Occurred()) {
					PyErr_Print();
				}

				importFailed = true;
				// Decrement the reference count for the module.
				Py_DECREF(pModule);
				return(false);
			} // End of if (PyCallable_Check(pFunc2) == 0)

			// Everything went well.
			// We will keep the references to our python functions for making future calls.
			// We can dereference the pointer to the module.
			Py_DECREF(pModule);
		} // End of if (pFunc1 == NULL)

		// We can now call the python functions.
		rstring resultFromPythonFunction1 = "";
		rstring resultFromPythonFunction2 = "";
		PyObject *pValue = NULL;
		pValue = PyObject_CallFunction(pFunc1, (char*) "s", url.c_str());

		// pFunc1 returns a tuple with three items in it.
		if (pValue != NULL) {
			const char* pStr = PyString_AsString(pValue);

			// Decrement the reference count for pValue.
			Py_DECREF(pValue);

			if (pStr != NULL) {
				resultFromPythonFunction1 = pStr;
			} else {
				if (PyErr_Occurred()) {
					PyErr_Print();
				}

				return(false);
			} // End of if (pStr != NULL)
		} else {
			SPLAPPTRC(L_ERROR, "Python function call to getIpAddressFromUrl failed.", "STREAMS_TO_PYTHON");

			if (PyErr_Occurred()) {
				PyErr_Print();
			}

			return(false);
		} // End of if (pValue != NULL)

		// Let us call the 2nd python function.
		pValue = PyObject_CallFunction(pFunc2, (char*) "s", url.c_str());

		if (pValue != NULL) {
			const char* pStr = PyString_AsString(pValue);

			// Decrement the reference count for pValue.
			Py_DECREF(pValue);

			if (pStr != NULL) {
				resultFromPythonFunction2 = pStr;
			} else {
				return(false);
			} // End of if (pStr != NULL)
		} else {
			SPLAPPTRC(L_ERROR, "Python function call to getCompanyNameFromUrl failed.", "STREAMS_TO_PYTHON");

			if (PyErr_Occurred()) {
				PyErr_Print();
			}

			return(false);
		} // End of if (pValue != NULL)

		// We have successfully called two python functions.
		// We have to parse the result strings from those function calls and
		// assign the results to the references passed to this C++ method.
		//
		// Result from the first python call is a triple containing three parts separated by commas.
		// Part1: Primary host name.
		// Part2: Zero or more alternative hostnames returned in a list i.e. ['host1', 'host2', 'host3']
		// Part3: IP Address list. i.e. ['157.166.240.11' '157.166.240.13' '157.166.241.10']
		SPL::list<rstring> tokens = SPL::Functions::String::csvTokenize(resultFromPythonFunction1);

		if (SPL::Functions::Collections::size(tokens) < 3) {
			SPLAPPTRC(L_ERROR, "Python function getIpAddressFromUrl didn't return correct results.", "STREAMS_TO_PYTHON");
			return(false);
		} // End of if (SPL::Functions::Collections::size(tokens) < 3)

		// First element in the list is the primary hostname.
		primaryHostName = tokens[0];
		// Alternate hostnames will be in the rest of the list except the very last element.
		alternateHostNames = tokens[1];
		// IP addresses will be in the very last element of the list.
		ipAddressList = tokens[SPL::Functions::Collections::size(tokens)-1];
		companyName = resultFromPythonFunction2;
		// SPLAPPTRC(L_ERROR, "PHN=" << primaryHostName << ", AHN=" <<  alternateHostNames <<
		//	", IPAL=" << ipAddressList << ", CN=" << companyName, "STREAMS_TO_PYTHON");
		return(true);
	} // End of method getIpAddressFromUrl
} // End of namespace calling_python_from_streams
