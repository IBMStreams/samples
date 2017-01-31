/*
 * NativeFunctionsWithCollectionTypesWrappers.h
 *
 *  Created on: May 14, 2013
 *
 *  This include file wraps the C++ native function methods defined in
 *  the C++ class. Streams SPL application code will use the inline
 *  C++ methods defined here as an entry point to call the C++ native
 *  function code. Inside the inline functions below, reference to a
 *  static object for the given C++ class is obtained first before
 *  calling the C++ native functions.
 */

#ifndef NATIVEFUNCTIONSWITHCOLLECTIONTYPESWRAPPERS_H_
#define NATIVEFUNCTIONSWITHCOLLECTIONTYPESWRAPPERS_H_

// Include the file that contains the class definition.
#include "NativeFunctionsWithCollectionTypes.h"

// Enclose all our inline C++ methods inside a namespace.
// For convenience, we will use the same namespace as the one
// that our C++ class in this project is using.
namespace native_functions_with_collection_types {
	// Establish a handle to the NativeFunctionsWithCollectionTypes session so that
	// the caller can let any of the state variables in these native functions to be
	// initialized ONLY once within a PE.
	inline int32 initializeNativeFunctionsWithCollectionTypes(void) {
		return NativeFunctionsWithCollectionTypes::
			getNativeFunctionsWithCollectionTypesSession().initializeNativeFunctionsWithCollectionTypes();
	}

	// Get a new set of random numbers into the SPL list that is passed as
	// a native function argument.
	inline boolean getNewRandomNumbers(SPL::List & randomNumberList, int32 & countOfNewRandomNumbers) {
		return NativeFunctionsWithCollectionTypes::
			getNativeFunctionsWithCollectionTypesSession().
			getNewRandomNumbers(randomNumberList, countOfNewRandomNumbers);
	}

	// Convert the value portion of the members of the map to upper case.
	inline boolean toUpperOfMapValues(SPL::Map & myMap, int32 & countOfMapValues) {
		return NativeFunctionsWithCollectionTypes::
			getNativeFunctionsWithCollectionTypesSession().
			toUpperOfMapValues(myMap, countOfMapValues);
	}

	// Create an employee record by filling out the tuple attributes.
	inline boolean createEmployeeRecord(SPL::Tuple & employeeTuple) {
		return NativeFunctionsWithCollectionTypes::
			getNativeFunctionsWithCollectionTypesSession().
			createEmployeeRecord(employeeTuple);
	}
}


#endif /* NATIVEFUNCTIONSWITHCOLLECTIONTYPESWRAPPERS_H_ */
