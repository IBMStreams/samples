/*
 * NativeFunctionsWithCollectionTypes.h
 *
 *  Created on: May 14, 2013

 This file contains the C++ interface class definition.
 There is a corresponding C++ implementation class file.
 */

#ifndef NATIVEFUNCTIONSWITHCOLLECTIONTYPES_H_
#define NATIVEFUNCTIONSWITHCOLLECTIONTYPES_H_

using namespace std;

// Include files that defines SPL types and functions.
// By including these, you can start using SPL data types
// and any SPL standard toolkit functions directly inside
// your C++ code. That is simply a great feature we get for free.
#include "SPL/Runtime/Function/SPLFunctions.h"
#include <SPL/Runtime/Utility/Mutex.h>
#include <SPL/Runtime/Type/ValueHandle.h>

// This will allow us to access the types and functions from SPL namespace.
using namespace SPL;

// Your #define constant definitions go here.

namespace native_functions_with_collection_types {

class NativeFunctionsWithCollectionTypes {
	private:
		// This member variable tells us if a global
		// NativeFunctionsWithCollectionTypes handle already
		// exists for a given PE/process.
		boolean nativeFunctionsWithCollectionTypesHandleExists;

	public:
		// Class constructor and destructor.
		NativeFunctionsWithCollectionTypes();
		virtual ~NativeFunctionsWithCollectionTypes();

		// This method establishes NativeFunctionsWithCollectionTypes handle for a given PE/process.
		int32 initializeNativeFunctionsWithCollectionTypes();
		// This method gets the IP address of a given URL.
		boolean getNewRandomNumbers(SPL::List & randomNumberList, int32 & countOfNewRandomNumbers);
		// This method converts the value portion of the members of the map to upper case.
		boolean toUpperOfMapValues(SPL::Map & myMap, int32 & countOfMapValues);
		// This method fills out all the attributes of the tuple passed by the caller with random values.
		boolean createEmployeeRecord(SPL::Tuple & employeeTuple);

		// Get the global (Singleton) NativeFunctionsWithCollectionTypes session object.
		static NativeFunctionsWithCollectionTypes & getNativeFunctionsWithCollectionTypesSession();
};
}

#endif /* NATIVEFUNCTIONSWITHCOLLECTIONTYPES_H_ */
