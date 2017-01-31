#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_
// Define SPL types and functions.
#include "SPL/Runtime/Function/SPLFunctions.h"

// Define the C++ namespace here.
namespace datagen_utils {
	// SPL authors recommend using the SPL namespace here.
	// This will allow us to use rich functions offered by the SPL API.
	using namespace SPL;

	// Define all the C++ function prototypes here.
	rstring my_sprintf(uint64 const & number, uint16 const & numberOfZeroPaddingsNeeded);

	// This function will use the C++ sprintf API to format the given number with the
	// requested number of zero paddings.
	inline rstring my_sprintf(uint64 const & number, uint16 const & numberOfZeroPaddingsNeeded) {
		char buffer[512];
		char formatSpecifier[512];
		sprintf(formatSpecifier, "%s%d%s", "%0", numberOfZeroPaddingsNeeded, "d");
		sprintf(buffer, formatSpecifier, (uint32)number);
		rstring tempStr = rstring(buffer);
		return(tempStr);
	} // End of function my_sprintf
} // End of namespace datagen_utils


#endif // End of FUNCTIONS_H_
