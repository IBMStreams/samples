/*
 * NameServerLookup.cpp
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
 *  "GCC C++ Compiler" section and select "Miscellaneous". Also remember to do these
 *  changes for both the Debug and Release building configurations. Finally, set the
 *  Release build as the active build by clicking on the "Manage Configurations" button located
 *  at the top right of the dialog.)
 *
 *  Alternately, you can simply build this library by running the
 *  mk file found in this project directory from a terminal window.
 *
 */

#include "NameServerLookup.h"
#include <vector>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <iterator>

using namespace std;
using namespace std::tr1;

namespace primitive_operator_lib {
	NameServerLookup::NameServerLookup() {
		// TODO Auto-generated constructor stub
	}

	NameServerLookup::~NameServerLookup() {
		// TODO Auto-generated destructor stub
	}

	// In this method, we will execute the nslookup command and return back
	// four pieces of information in a map ash shown below.
	// "Name Server Name" => "Name of the name server"
	// "Name Server Address" => "IP address of the name server"
	// "Client Machine Name" => "Fully qualified name of the client machine"
	// "Client Machine Address" => "IP address of the client machine"
	unordered_map<string, string> NameServerLookup::getNSLookupResults(string const & nodeName) {
		FILE *fpipe;
		string nsLookupCommand = (string)"nslookup " + nodeName;
		char line[256];
		unordered_map <string, string> result;
		string replyLineKey1 = "Name Server Name";
		string replyLineKey2 = "Name Server Address";
		string replyLineKey3 = "Client Machine Name";
		string replyLineKey4 = "Client Machine Address";

		if (!(fpipe = (FILE*)popen(nsLookupCommand.c_str(),"r"))) {
			// If fpipe is NULL
			// Mark the errors in the map.
			result[replyLineKey1] = "Pipe failure";
			result[replyLineKey2] = "Pipe failure";
			result[replyLineKey3] = "Pipe failure";
			result[replyLineKey4] = "Pipe failure";
			return (result);
		} // End of if (!(fpipe = (FILE*)popen(command,"r")))

		int32_t cnt = 1;

		while (fgets( line, sizeof line, fpipe)) {
			// nslookup result looks like this.
			// Server:         10.4.24.230
			// Address:        10.4.24.230#53
			//
			// Name:   a0217b10e1.hny.distillery.ibm.com
			// Address: 10.4.40.210
			//
			// Let us parse only the last token in every line as that is what we want.
			//
			std::string resultStr = string(line);
			// construct a stream from the string
			std::stringstream strStream(resultStr);

			// use stream iterators to copy the stream to the vector as whitespace separated strings
			std::istream_iterator<std::string> it(strStream);
			std::istream_iterator<std::string> end;
			std::vector<std::string> results(it, end);
			// cout << "Vector size = " << results.size() << endl;

			// Let us now get the last token in the vector.
			std::string lastToken = "";

			if (results.size() > 0) {
				lastToken = results.at(results.size() - 1);
			} // End of if (results.size() > 0)

			switch (cnt++) {
				case 1:
					result[replyLineKey1] = lastToken;
					break;

				case 2:
					result[replyLineKey2] = lastToken;
					break;

				case 3:
					// This must be an empty line in the nslookup result.
					break;

				case 4:
					result[replyLineKey3] = lastToken;
					break;

				case 5:
					result[replyLineKey4] = lastToken;
					break;
			} // End of switch (cnt++)

			// If we already processed 5 lines, we have
			// collected everything we need from the result.
			if (cnt > 5) {
				// Break from the while loop.
				break;
			} // End of if (cnt > 5)

			// printf("%s", line);
		} // End of while (fgets( line, sizeof line, fpipe))

		pclose (fpipe);
		return (result);
	} // End of method getNSLookupResults(string & nodeName).

} // End of namespace primitive_operator_lib

