/*
 * NativeFunctionsWithCollectionTypes.cpp
 *
 *  Created on: May 14, 2013
 *
 *  This implementation class shows an example of how to pass and return
 *  complex types to SPL native functions. This class has
 *  three C++ methods that show how to pass/return
 *  SPL collection types. (SPL list, map, and tuple).
 *  This is primarily done using the SPL C++ reflective APIs.
 *
 *  This C++ class will be compiled and packaged into a .so library
 *  so that the Streams SPL application code can directly call the
 *  C++ native functions in that .so file.
 *
 */

#include "NativeFunctionsWithCollectionTypes.h"

namespace native_functions_with_collection_types {

	NativeFunctionsWithCollectionTypes::NativeFunctionsWithCollectionTypes() {
		// Anything that needs to be done during the construction of
		// this C++ class will go here.
	}

	NativeFunctionsWithCollectionTypes::~NativeFunctionsWithCollectionTypes() {
		// If you have used any global resources during the lifetime of this
		// C++ class object (database or network connections), you can do a cleanup here.
	}

	// Define a method that creates and returns a singleton object to
	// our global NativeFunctionsWithCollectionTypes session object.
	// This will guarantee that there will always be only one instance of this class per PE.
	// Please refer to the wrapper include file in this directory that makes use
	// of this static method.
	NativeFunctionsWithCollectionTypes & NativeFunctionsWithCollectionTypes::getNativeFunctionsWithCollectionTypesSession() {
		// A static variable that will get initialized only once per process.
		static NativeFunctionsWithCollectionTypes nfwct;
		// Return our singleton global NativeFunctionsWithCollectionTypes session object.
		return nfwct;
	}

	// This method initializes the native functions with collection types C++ class module.
	// It is done only once per PE/process. (This is an optional method that
	// a Streams PE can call to do any application-specific initialization of
	// state information stored in this C++ class.)
	int32 NativeFunctionsWithCollectionTypes::initializeNativeFunctionsWithCollectionTypes() {
		SPLAPPTRC(L_ERROR, "initializeNativeFunctionsWithCollectionTypes request received from a thread id "
				<< pthread_self() << " within the process id " << getpid() << ".", "INITIALIZE_NATIVE_FUNCTIONS");

		// Acquire a lock so that we can avoid multiple threads colliding with each other below.
		///// AutoMutex am(mutex);

		// If we already have established a handle to this C++ class for this PE/process, then return now.
		if (nativeFunctionsWithCollectionTypesHandleExists == true) {
			SPLAPPTRC(L_ERROR, "A global and a shared NativeFunctionsWithCollectionTypes handle has " <<
				"already been made by another thread in the same process id.", "INITIALIZE_NATIVE_FUNCTIONS");
			return (0);
		}

		SPLAPPTRC(L_ERROR, "This global NativeFunctionsWithCollectionTypes handle can now be shared " <<
			"by multiple threads within the process id " << getpid() << ".", "INITIALIZE_NATIVE_FUNCTIONS");

		nativeFunctionsWithCollectionTypesHandleExists = true;

		// If you have any member variables in this class that store state, this is a good
		// place to initialize them. (An example of that would be establishing Database/Network connections).

		return (0);
	}

	boolean NativeFunctionsWithCollectionTypes::getNewRandomNumbers(SPL::List & randomNumberList, int32 & countOfNewRandomNumbers) {
		// This method will receive two arguments from the caller. Both are references.
		// Let us first ensure that the list passed by the caller can hold elements of an expected data type.
		// In our example, we are expecting the caller to pass us a list<float64>.
		assert(randomNumberList.getElementMetaType() == SPL::Meta::Type::FLOAT64);
		// We will add 5 random numbers to the existing list.
		// We will also return the number of new random numbers that are appended to the list passed by the caller.
		countOfNewRandomNumbers = 5;

		for (int32 i = 0; i < countOfNewRandomNumbers; i++) {
			// Let us use the SPL built-in function to get a random number.
			float64 newRandomNumber = SPL::Functions::Math::random();
			// Make it a number between 1 to 100.
			newRandomNumber *= 100.00;
			randomNumberList.pushBack(newRandomNumber);
		}

		return(true);
	}

	boolean NativeFunctionsWithCollectionTypes::toUpperOfMapValues(SPL::Map & myMap, int32 & countOfMapValues) {
		// This method converts the value portion of the members of the map to upper case.
		// Let us first ensure that the map passed by the caller contains the expected
		// types for the map key and the map value.
		assert(myMap.getKeyMetaType() == SPL::Meta::Type::RSTRING);
		assert(myMap.getValueMetaType() == SPL::Meta::Type::RSTRING);
		// Let us store the size of the map in the 2nd argument passed by the caller.
		countOfMapValues = myMap.getSize();

		// We can iterate over the map and convert every map value to uppercase.
		for (MapIterator mapIter=myMap.getBeginIterator();
			mapIter != myMap.getEndIterator(); ++mapIter) {
			// Get the current <key, value> element from the map.
			std::pair<ConstValueHandle, ValueHandle> mapElement = *mapIter;
			// Get a reference to the value part of the map element.
			rstring & mapValue = mapElement.second;
			// Map has <key, value>. Let us convert the map value to upper case.
			// This change will get reflected directly in the original map object.
			mapValue = SPL::Functions::String::upper(mapValue);
		}

		return(true);
	}

	boolean NativeFunctionsWithCollectionTypes::createEmployeeRecord(SPL::Tuple & employeeTuple) {
		// This method receives an SPL tuple object from the caller.
		// It uses the reflective APIs to verify the expected tuple attributes.
		// Expected format for this tuple is as shown below.
		//
		// tuple<rstring name, uint8 age, float32 salary, list<rstring> companyDevicesOwned>
		//
		// Then, it fills out the tuple attributes with random values.

		// Schema for this tuple is a mixture of SPL primitive and complex typed attributes.
		// Let us first try to assign the tuple attribute values that are of primitive SPL types.
		// After that, we will populate the attribute that will hold a list.
		// Clear all the attributes in the tuple object that was passed as an argument to this native function.
		employeeTuple.reset();

		// Do the tuple attribute value assignment using the SPL C++ tuple reflection APIs.
		// Find the attribute you are looking for inside the tuple passed to us.
		TupleIterator iter = employeeTuple.findAttribute("name");
		// Ensure that we are NOT at the end of the tuple object without finding what we are looking for.
		assert(iter != employeeTuple.getEndIterator());
		// Get the attribute object we found inside the tuple.
		TupleAttribute nameAttribute = *iter;
		// Obtain the value stored inside the attribute object we just found.
		ValueHandle nameHandle = nameAttribute.getValue();
		// Ensure that the value stored in the recently found attribute object matches the SPL type we are interested in.
		assert(nameHandle.getMetaType() == Meta::Type::RSTRING);
		// Get a reference to the attribute value using its correctly asserted SPL type.
		rstring & employeeName = nameHandle;
		// If we change the reference to the attribute's value handle, new value will directly take effect in the tuple.
		employeeName = "Peter Pan";

		// In a similar fashion, we can fill the remaining tuple attributes.
		iter = employeeTuple.findAttribute("age");
		assert(iter != employeeTuple.getEndIterator());
		TupleAttribute ageAttribute = *iter;
		ValueHandle ageHandle = ageAttribute.getValue();
		assert(ageHandle.getMetaType() == Meta::Type::UINT8);
		uint8 & employeeAge = ageHandle;
		// Following statement will set the tuple attribute value.
		employeeAge = 14;

		iter = employeeTuple.findAttribute("salary");
		assert(iter != employeeTuple.getEndIterator());
		TupleAttribute salaryAttribute = *iter;
		ValueHandle salaryHandle = salaryAttribute.getValue();
		assert(salaryHandle.getMetaType() == Meta::Type::FLOAT32);
		float32 & employeeSalary = salaryHandle;
		// Following statement will set the tuple attribute value.
		employeeSalary = 100001.00;

		// As we mentioned above, this tuple also carries a complex SPL typed attribute.
		// In this case, it will be a list of strings: list<rstring>
		iter = employeeTuple.findAttribute("companyDevicesOwned");
		assert(iter != employeeTuple.getEndIterator());
		TupleAttribute cdoAttribute = *iter;
		ValueHandle cdoHandle = cdoAttribute.getValue();
		assert(cdoHandle.getMetaType() == Meta::Type::LIST);
		SPL::List & cdoList = cdoHandle;
		// Ensure that this list can hold elements of type RSTRING.
		assert(cdoList.getElementMetaType() == Meta::Type::RSTRING);

		rstring devicesOwned[5] = {"MacBook", "iPhone", "Ferrari", "OLED TV", "Rolex Watch"};

		for (int i = 0; i < 5; i++) {
			// Use a C++ SPL API to create a new SPL list element.
			// Then, add that newly created element to the SPL events list attribute.
			// This SPL API is very powerful in that it will create a
			// new element for the exact SPL type that the SPL list attribute is configured to hold.
			// In our case, it is a list of rstring.
			ValueHandle deviceNameHandle = cdoList.createElement();
			rstring & deviceName = deviceNameHandle;
			deviceName = devicesOwned[i];
			// Add this new tuple element to the end of the list.
			cdoList.pushBack(deviceNameHandle);
			// We must delete the dynamic memory allocated by the SPL createElement C++ API we used above.
			deviceNameHandle.deleteValue();
		}

		return(true);
	}
}

