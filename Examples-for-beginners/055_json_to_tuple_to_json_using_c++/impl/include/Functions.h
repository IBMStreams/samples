/*
This file provides the implementation for the two native functions that convert from Tuple to Json and vice versa.
This implementation uses the CAJUN C++ API which is an open source code with a BSD license.
By default, this implementation does a dummy conversion. In order to do the actual conversion, users of
this example should perform certain steps to download the CAJUN C++ API and make it part of this example.

Please read the detailed instructions as part of the comments written at the beginning of the
SPL file for this project.
*/

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

// Include this SPL file so that we can use the SPL functions and types in this C++ code.
#include "SPL/Runtime/Function/SPLFunctions.h"
#include <vector>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>

// #define CAJUN_API_AVAILABLE

#ifdef CAJUN_API_AVAILABLE
// Include Cajun-2.0.2 related include files.
#include "json/reader.h"
#include "json/writer.h"
#include "json/elements.h"

using namespace json;
using namespace std;

// Define a C++ namespace that will contain our native function code.
namespace json_utils {
	// By including this line, we will have access to the SPL namespace and anything defined within that.
	using namespace SPL;

		// Prototype for our native functions are declared here.
		boolean convertTupleAttributesToJson(rstring & jsonString,
			rstring const & firstName, rstring const & lastName, rstring const & ssn,
			rstring const & streetAddress, uint32 const & age, float32 const & height,
			boolean const & isEmployed, SPL::List const & favoriteMovies);

		boolean convertJsonToTupleAttributes(rstring const & jsonString,
			rstring & firstName, rstring & lastName, rstring & ssn, rstring & streetAddress,
			uint32 & age, float32 & height, boolean & isEmployed, SPL::list<rstring> & favoriteMovies);

		// Convert the tuple attributes to a JSON string.
		inline boolean convertTupleAttributesToJson(rstring & jsonString,
				rstring const & firstName, rstring const & lastName, rstring const & ssn,
				rstring const & streetAddress, uint32 const & age, float32 const & height,
				boolean const & isEmployed, SPL::list<rstring> const & favoriteMovies) {
			// Now create a top-level Json object.
			Object personData;
			personData["firstName"] = String(firstName);
			personData["lastName"] = String(lastName);
			personData["ssn"] = String(ssn);
			personData["streetAddress"] = String(streetAddress);
			personData["age"] = Number(age);
			personData["height"] = Number(height);
			personData["isEmployed"] = Boolean(isEmployed);
			Array favoriteMoviesArray;

			int32 sizeOfFavoriteMoviesList = SPL::Functions::Collections::size(favoriteMovies);

			// We will loop through the list and get all the favorite movie names.
			for (int32 cnt=0; cnt<sizeOfFavoriteMoviesList; cnt++) {
				// Insert into the JSON array.
				favoriteMoviesArray.Insert(String(favoriteMovies[cnt]));
			} // End of for loop.

			personData["favoriteMovies"] = favoriteMoviesArray;

			// Perform all the read operations on a constant reference.
			const Object& objRoot = personData;
			std::stringstream stream;
			// Turn the JSON complex C++ object structure into a C++ string object.
			// Please note that the Writer implementation in CAJUN will add new line, tab etc. in
			// the converted JSON string. If you don't want that behavior, you can
			// change the CAJUN code [writer.h]. You can email the author of this SPL example to
			// get tips about making that change.
			Writer::Write(objRoot, stream);
			// We can return the Json string to the caller now.
			jsonString = stream.str();
			return(true);
		}

		// Convert a JSON string into tuple attribute values.
		inline boolean convertJsonToTupleAttributes(rstring const & jsonString,
				rstring & firstName, rstring & lastName, rstring & ssn, rstring & streetAddress,
				uint32 & age, float32 & height, boolean & isEmployed, SPL::list<rstring> & favoriteMovies) {
			// convert the json string passed as a function argument into a C++ object and then assign
			// the object member values to the tuple attributes passed as function arguments.
			//
			// Let us get a C++ object representation of the JSON string that has been passed to us.
			Object obj;
			std::stringstream stream(jsonString);
			Reader::Read(obj, stream);

			// Let us now try to assign the tuple attribute values.
			try {
				firstName = (rstring)String(obj["firstName"]);
			} catch(const Exception& e) {
				firstName = "";
			}

			try {
				lastName = (rstring)String(obj["lastName"]);
			} catch(const Exception& e) {
				lastName = "";
			}

			try {
				ssn = (rstring)String(obj["ssn"]);
			} catch(const Exception& e) {
				ssn = "";
			}

			try {
				streetAddress = (rstring)String(obj["streetAddress"]);
			} catch(const Exception& e) {
				streetAddress = "";
			}

			try {
				age = Number(obj["age"]);
			} catch(const Exception& e) {
				age = 0lu;
			}

			try {
				height = Number(obj["height"]);
			} catch(const Exception& e) {
				height = 0lu;
			}

			try {
				isEmployed = Boolean(obj["isEmployed"]);
			} catch(const Exception& e) {
				isEmployed = false;
			}

			// In this data structure, we have an SPL collection type (list).
			// Let us parse the JSON array structure and populate the corresponding
			// tuple attribute (favoriteMovies).
			//
			// Perform all the read operations from JSON object on a constant reference.
			const Object& objRoot = obj;
			const Array& moviesArray = objRoot["favoriteMovies"];

			// Let us iterate through the JSON C++ object array now.
			Array::const_iterator itMovies(moviesArray.Begin()), itMoviesEnd(moviesArray.End());

			for (; itMovies != itMoviesEnd; ++itMovies) {
				// Get the current element inside the JSON C++ object array.
				rstring movie = "";

				try {
					movie = (rstring)String(*itMovies);
				} catch(const Exception& e) {
					movie = "";
				}

				// Add it to the tuple list attribute.
				SPL::Functions::Collections::appendM(favoriteMovies, movie);
			}

			return(true);
		}
}
#else
using namespace std;

// Define a C++ namespace that will contain our native function code.
namespace json_utils {
	// By including this line, we will have access to the SPL namespace and anything defined within that.
	using namespace SPL;

		// Prototype for our native functions are declared here.
		boolean convertTupleAttributesToJson(rstring & jsonString,
			rstring const & firstName, rstring const & lastName, rstring const & ssn,
			rstring const & streetAddress, uint32 const & age, float32 const & height,
			boolean const & isEmployed, SPL::List const & favoriteMovies);

		boolean convertJsonToTupleAttributes(rstring const & jsonString,
			rstring & firstName, rstring & lastName, rstring & ssn, rstring & streetAddress,
			uint32 & age, float32 & height, boolean & isEmployed, SPL::list<rstring> & favoriteMovies);

		// Convert the tuple attributes to a JSON string.
		inline boolean convertTupleAttributesToJson(rstring & jsonString,
				rstring const & firstName, rstring const & lastName, rstring const & ssn,
				rstring const & streetAddress, uint32 const & age, float32 const & height,
				boolean const & isEmployed, SPL::list<rstring> const & favoriteMovies) {
			// Since there is no CAJUN API available, let us have a dummy implementation.
			jsonString="C++ API for JSON is not active. Please read the comments at the top of the SPL file.";
			return(false);
		}

		// Convert a JSON string into tuple attribute values.
		inline boolean convertJsonToTupleAttributes(rstring const & jsonString,
				rstring & firstName, rstring & lastName, rstring & ssn, rstring & streetAddress,
				uint32 & age, float32 & height, boolean & isEmployed, SPL::list<rstring> & favoriteMovies) {
			// Since there is no CAJUN API available, let us have a dummy implementation.
			firstName="";
			lastName="";
			ssn="";
			streetAddress="";
			age=0;
			height=0.00;
			isEmployed=false;
			return(false);
		}
}
#endif

#endif
