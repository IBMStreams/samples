/*
 * LibFunctions.cpp
 *
 *  Created on: Feb 23, 2011
 *      Author: sen
 *
 *  This is a companion project for the 032_native_function_at_work SPL
 *  project present in the same Eclipse workspace. This C++ project
 *  gets compiled into a shared library (.so) file so that the SPL
 *  project can access the native C++ functions available in the .so file.
 *  In the SPL project, there will be a native function model that will
 *  define the location of the .so file, C++ function name and the
 *  C++ namespace. The C++ include file, CPP file, and the .so files
 *  should be manually copied into the impl directory of the SPL project.
 *  Read the commentary in the main composite of the SPL project to learn
 *  more about the exact locations where the files from the C++ project
 *  files need to be copied.
 *
 *  In order to build this library inside Eclipse CDT, we have to do some
 *  special stuff. As it is shown below, we are going to make use of the
 *  SPL types and functions. In order to do that, we have to include
 *  certain files from the SPL runtime. We will configure this project's
 *  CDT C++ build dialog in its Miscellaneous section. In order to know
 *  what to add there, on a xterm window execute the following and
 *  append the result into the Miscellaneous section of the
 *  C++ build dialog. (Right click on the NativeFunctionLib project,
 *  select properties. In the resulting dialog, expand the "C/C++ Build"
 *  section in the left pane and click "Settings". In the right pane,
 *  expand the GCC C++ Compiler" section and select "Miscellaneous". Do this change
 *  for both the Debug and Release builds. Then, set the Release build as the active build by
 *  clicking on the "Manage Configurations" button at the top right of the dialog.)
 *
 *  $STREAMS_INSTALL/bin/dst-pe-pkg-config.sh --cflags dst-spl-pe-install
 *
 *  Alternately, you can simply build this library by running the
 *  mk file found in this project directory from a terminal window.
 */

#include "LibFunctions.h"

namespace test2 {
	float32 calculateTax(float32 const & price, rstring const & state) {
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
		} // End of if (state == "NY")
	} // End of function calculateTax(float32 const & price, rstring const & state)
} // End of namespace test2
