/*
 * RecursiveDirScanWrappers.h
 *
 *  Created on: May 13, 2013
 *      Author: sen
 */

#ifndef RECURSIVE_DIR_SCAN_WRAPPERS_H_
#define RECURSIVE_DIR_SCAN_WRAPPERS_H_

// Include the file that contains the class definition.
#include "RecursiveDirScan.h"

namespace recursive_dir_scan {
   /// Get a list of all the file names in a given directory.
   inline boolean getFileNamesInDirectory(rstring const dirName, SPL::list<rstring> const listOfFileNamesToBeIgnored, SPL::list<rstring> & listOfFileNamesFound, int32 & recursionLevel) {
      return RecursiveDirScan::getRecursiveDirScanObject().getFileNamesInDirectory(dirName, listOfFileNamesToBeIgnored, listOfFileNamesFound, recursionLevel);
   } // End of method getFileNamesInDirectory
} // End of namespace recursive_dir_scan

#endif /* RECURSIVE_DIR_SCAN_WRAPPERS_H_ */
