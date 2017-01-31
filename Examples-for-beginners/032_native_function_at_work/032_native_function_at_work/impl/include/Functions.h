#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_
// Define SPL types and functions.
#include "SPL/Runtime/Function/SPLFunctions.h"

// Define the C++ namespace here.
namespace test1 {
	// SPL authors recommend using the SPL namespace here.
	// This will allow us to use rich functions offered by the SPL API.
	using namespace SPL;

	// Define all the C++ function prototypes here.
	SPL::float32 calculateTax(SPL::float32 const & price, SPL::rstring const & state);

	inline float32 calculateTax(float32 const & price, rstring const & state) {
		if (state == "NY") {
			return (price * 7.5/100.0);
		} else if (state == "NC") {
			return (price * 5.25/100.0);
		} else if (state == "NJ") {
			return (price * 6.0/100.0);
		} else if (state == "CA") {
			return (price * 7.25/100.0);
		} else if (state == "FL") {
			return (price * 5.75/100.0);
		} else {
			// Return a flat tax of $1.00
			return (1.00);
		}
	} // End of function calculateTax(float32 const & price, rstring const & state)
} // End of namespace test1


#endif // End of FUNCTIONS_H_
