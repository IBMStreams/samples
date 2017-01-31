/*
 * RecursiveDirScan.cpp
 *
 *  Created on: May 13, 2013
 *      Author: sen
 */

#include "RecursiveDirScan.h"

namespace recursive_dir_scan {

	RecursiveDirScan::RecursiveDirScan() {
		// TODO Auto-generated constructor stub
	} // End of constructor

	RecursiveDirScan::~RecursiveDirScan() {
		// TODO Auto-generated destructor stub
	} // End of destructor

	// Define a method that creates and returns a singleton access to
	// our global RecursiveDirScan object.
	RecursiveDirScan & RecursiveDirScan::getRecursiveDirScanObject() {
		// A static variable that will get initialized only once per process.
		static RecursiveDirScan rds;
		// Return our singleton global RecursiveDirScan object.
		return rds;
	} // End of method getRecursiveDirScanObject

	// This method will get all the names of the files present in the given directory.
	// While collecting the file names, it will ignore any files that even partially match any
	// of the names in the ignore files list supplied by the caller. (See 2nd method
	// argument below.)
    // PLEASE NOTE THAT THIS METHOD IS RECURSIVE AND NO SPECIAL CHECK IS DONE FOR RECUSRION DEPTH.
    // SO, DON'T TRY TO RECURSIVELY ITERATE THROUGH TOO MANY SUB-DIRECTORIES.
	boolean RecursiveDirScan::getFileNamesInDirectory(rstring dirName, SPL::list<rstring> listOfFileNamesToBeIgnored, SPL::list<rstring> & listOfFileNamesFound, int32 & recursionLevel) {
		DIR *dp;
		struct dirent *dirp;

                // Increment the recursion level.
                recursionLevel++;

		if((dp  = opendir(dirName.c_str())) == NULL) {
    	      return(false);
		}

		// Clear the list passed to us only during the very first entry into this recursive method.
		if (recursionLevel == 1) {
			SPL::Functions::Collections::clearM(listOfFileNamesFound);
		}

		while ((dirp = readdir(dp)) != NULL) {
			string tempStr = string(dirp->d_name);

			// Check and filter if this file is . or ..
			if (tempStr.compare(".") == 0) {
				continue;
			}

			if (tempStr.compare("..") == 0) {
				continue;
			}


			rstring fullPathName = dirName + "/" + tempStr;
        
			// If it is a sub-directory, recurse into it and look for more files.
			if (dirp->d_type == DT_DIR) {
				// Do recursive search now.
				getFileNamesInDirectory(fullPathName, listOfFileNamesToBeIgnored, listOfFileNamesFound, recursionLevel);
			} else {
				boolean ignoreThisFile = false;
				int32 sizeOfIgnoreFilesList = SPL::Functions::Collections::size(listOfFileNamesToBeIgnored);

				// We will ignore this file, if it even partially matches with
				// any of the user-specified file names in the ignoreFiles list.
				for (int32 cnt=0; cnt<sizeOfIgnoreFilesList; cnt++) {
					if (tempStr.find(listOfFileNamesToBeIgnored[cnt]) != string::npos) {
					// There is a partial match between the current filename found inside
					// the directory and one of the filenames in the ignoreFiles list.
					ignoreThisFile = true;
					break;
					}
				} // End of for loop.

				if (ignoreThisFile == true) {
					// Continue the while loop to get the next file that is present.
					continue;
				}

				// Append the filename to the list.
				SPL::Functions::Collections::appendM(listOfFileNamesFound, fullPathName);
			} // End of if(dirp->d_type == DT_DIR)
		} // End of while ((dirp = readdir(dp)) != NULL)

		closedir(dp);
		int32 sizeOfFileNamesFoundList = SPL::Functions::Collections::size(listOfFileNamesFound);

		// If there are no files found in the directory, return false.
		if (sizeOfFileNamesFoundList > 0) {
			return(true);
		} else {
			return(false);
		}
	} // End of method getFileNamesInDirectory
} /* namespace recursive_dir_scan */
