/*
============================================================================
Name        : UsePrimitiveOperatorLib.cpp
Description : NSLookUp in C++, Ansi-style

This is a client a.k.a test driver program that uses a shared library.
This stand-alone test application calls a function available inside the
PrimitiveOperatorLib shared (.so) library. In the CDT, there are specific
project settings that should be made to link with the .so library.

Following are the specific project configuration settings to link with the
.so shared library.

1)  Right-click on the test driver project and select properties.
2)  In the resulting dialog, expand "C/C++ Build" in the
    left pane and click on Settings.
3)  In the right pane, expand "GCC C++ Compiler" and select "Directories".
4)  In the "Include Paths" section, click on the "Add" icon.
5)  In the resulting "Add Directory Path" dialog, click on the "Workspace" button.
6)  Scroll down the list, select "PrimitiveOperatorLib" project and select OK twice.
7)  In the right pane, under the "GCC C++ Compiler" section, select "Miscellaneous".
8)  In the "Other Flags" field, add the -fPIC option.
9)  In the right pane, expand "GCC C++ Linker" and select "Libraries".
10) In the "Libraries" section, click on the "Add" icon.
11) In the resulting dialog, type "PrimitiveOperatorLib" and click OK.
12) In the same dialog, you will see the "Library Search Path" section at the bottom.
13) In that section, click on the "Add" icon.
14) In the resulting "Add Directory Path" dialog, click on the "Workspace" button.
15) Scroll down the list, select "PrimitiveOperatorLib-->Release" and select OK twice.
16) In order to find the shared library at runtime, select the "Miscellaneous"
    option under the "GCC C++ Linker" section. Now in the "Linker Flags" field, enter the following.
    -Wl,-rpath -Wl,"${workspace_loc:/PrimitiveOperatorLib/Release}"
17) Finally, remember to set the Release build as the active build by clicking on the
    "Manage Configurations" button located at the top right of the dialog.

That is it. If you do a build of the test driver, it should compile fine.
============================================================================
*/

#include <cstdlib>
#include <iostream>
#include "NameServerLookup.h"

using namespace std;
using namespace std::tr1;
using namespace primitive_operator_lib;

int main() {
	NameServerLookup nsl;
	unordered_map<string, string> resultMap ;
	resultMap = nsl.getNSLookupResults("a0217b10e1");
    int resultMapSize = resultMap.size();
	cout << "NS Lookup result map size=" << resultMapSize << endl;

	unordered_map<string, string>::iterator mapIterator;

	for (mapIterator = resultMap.begin(); mapIterator != resultMap.end(); ++mapIterator) {
		cout << mapIterator->first << "=" << mapIterator->second << endl;
	} // End of for (int32_t cnt = 1; cnt <= resultMapSize; cnt++)

	return 0;
}
