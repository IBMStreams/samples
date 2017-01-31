/*
 * RecursiveDirScan.h
 *
 *  Created on: May 13, 2013
 *      Author: sen
 */

#ifndef RECURSIVE_DIR_SCAN_H_
#define RECURSIVE_DIR_SCAN_H_

using namespace std;

// Include files that define SPL types and functions.
#include "SPL/Runtime/Function/SPLFunctions.h"
#include <SPL/Runtime/Utility/Mutex.h>
#include <SPL/Runtime/Type/ValueHandle.h>
// Include standard c++ include files.
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <sstream>

// This will allow us to access the types and methods in SPL namespace.
using namespace SPL;

// Your #define constant definitions go here.

// Class definition follows here.
namespace recursive_dir_scan {
	class RecursiveDirScan {
		public:
			RecursiveDirScan();
			virtual ~RecursiveDirScan();
			// This method returns the names of all the files present in a given directory name.
			boolean getFileNamesInDirectory(rstring dirName, SPL::list<rstring> listOfFileNamesToBeIgnored, SPL::list<rstring> & listOfFileNamesFound, int32 & recursionLevel);


			/// Get the global (Singleton) RecursiveDirScan object.
			static RecursiveDirScan & getRecursiveDirScanObject();
	}; // End of class RecursiveDirScan

} // End of namespace recursive_dir_scan
#endif /* RECURSIVE_DIR_SCAN_H_ */
