/*
 * NameServerLookup.h
 *
 *  Created on: Mar 10, 2011
 *      Author: sen
 *
 *  This is a companion project for the 036_shared_lib_primitve_operator_at_work
 *  SPL project present in the same Eclipse workspace. This C++ project
 *  gets compiled into a shared library (.so) file so that the SPL
 *  C++ primitve operator can access the C++ functions available in
 *  the .so file. In the SPL project, there will be an operator model
 *  that will define the location of the .so file, C++ function name and the
 *  C++ namespace. The C++ include file, CPP file, and the .so files
 *  should be manually copied into the impl directory of the SPL project.
 *  Read the commentary in the main composite of the SPL project to learn
 *  more about the exact locations where the files from the C++ project
 *  files need to be copied.
 *
 *  Before using the functions in this library inside a C++ primitive operator,
 *  it is advisable to test the .so library by using it inside a small CDT
 *  test driver. There is a standalone CDT C++ project named
 *  "UsePrimitiveOperatorLib" that can be used to test the shared library
 *  built from the file you are viewing now.
 *
 *  In order to build this library inside Eclipse CDT, we have to do
 *  a minor CDT buid configuration by appending the -fPIC option in the
 *  Miscellaneous section of the C++ build dialog.
 *  (Right click on the PrimitiveOperatorLib project,
 *  select properties. In the resulting dialog, expand the "C/C++ Build"
 *  section in the left pane and click "Settings". In the right pane, expand the
 *  "GCC C++ Compiler" section and select "Miscellaneous".)
 *
 */

#ifndef NAMESERVERLOOKUP_H_
#define NAMESERVERLOOKUP_H_

#include <tr1/unordered_map>

namespace primitive_operator_lib {

	class NameServerLookup {
		public:
			NameServerLookup();
			virtual ~NameServerLookup();
			// This is the method that will be made available through this library.
			std::tr1::unordered_map<std::string, std::string> getNSLookupResults(std::string const & nodeName);
	}; // End of class NameServerLookup

} // End of namespace primitive_operator_lib

#endif /* NAMESERVERLOOKUP_H_ */
