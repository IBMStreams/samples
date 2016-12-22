~~~~~~
/*
This example shows how to use the Streams C++ native function facility to recursively scan
a given directory and obtain the names of the files present. The logic for the recursive
directory scan polls the specified directory periodically and notifies the downstream 
operator with a new file that just appeared. There is a companion C++ project for this
SPL project. Please refer to the RecursiveDirScanLib project for the C++ logic.

Important sequence of logic for this application:

1) SPL code resolves the C++ native function in its native.function/function.xml file.
2) A call from the SPL code to the native function lands in the wrapper inline C++ functions
   defined in the RecursiveDirScanWrappers.h file of the companion C++ project.
3) From that wrapper function, it gets access to a singleton C++ object of the
   RecursiveDirScan class and then invokes the getFileNamesInDirectory C++ method.
4) When that C++ method returns, it will have the results stored in a list<string> reference that
   was passed to it.
5) Back in the SPL code, there is additional logic to cache the already seen files and to filter
   only the newly found files to send to the downstream operator.

Before doing anything with the SPL project, is is necessary to go and run the ./mk script
available in the companion C++ project (RecursiveDirScanLib). That script will compile the
C++ code to create shared object (.so) file that will be copied to the impl/lib directory of
the 050_recursive_dir_scan SPL project.


In order to test this application, please following these steps:

a) Ensure that the companion C++ project is built using the ./mk script in that project directory.
b) Ensure that the SPL project is built successfully.
c) Now, you can run the SPL application in standalone mode (either from a terminal window or inside 
   the Streams Studio)
d) From a terminal window, go to this SPL project directory and run the two test shell scripts that
   you will see there. (01_simulate_data_feeds.sh and 02_simulate_data_feeds.sh)
e) Observe the results being displayed on the console.
*/

namespace recursive.dir.scan;

composite recursive_dir_scan {
	param
		// In Streams 4.x, current working directory is no longer the data sub-directory inside the application directory.
		// Hence, it is necessary to get the data directory manually.
		expression <rstring>			$RSTRING_FEED_DIRECTORY : dataDirectory();
		expression <list <rstring>>		$LIST_RSTRING_FILES_TO_BE_IGNORED : [".start_batch", ".end_batch"];
		
	graph
		// This application is a test driver that puts the RecursiveDirScanLib (C++) project to use.
		// It shows, how names of all the files in a given directory can be obtained recusively.
		stream<int32 signal> NewSignal = Beacon() {
			param
				period: 2.0;
		} // End of NewSignal = Beacon()
		
		// In this operator, we will call a native function to collect all the 
		// filenames recursively in a given directory. We will also cache the
		// already seen files so that during the periodic directory scan, we will
		// not keep collecting the same files again and again.
		stream<rstring fileName> TransactionFile = Custom(NewSignal) {
			logic
				state: {
					// In real life situations, this set will keep growing big in memory over a
					// long period of time. Be aware of that.
					mutable set<rstring> setOfFilesAlreadySeen = {};
					mutable list<rstring> listOfFileNamesFound = [];
					mutable int32 recursionLevel = 0;
				}
				
				onTuple NewSignal: {
					clearM(listOfFileNamesFound);
					recursionLevel = 0;
					// Call a native function now to get the files present in the feed directory.
					// This function will return the names of all the files in the feed directory.
					// This will be returned via a list<rstring> reference variable passed as a 3rd
					// argument by the caller of the native function. Caller of this function will be required
					// to provide any filenames (even partial names) that should be ignored if found in the directory.
					// If there are no files to be ignored, caller should at the minimum pass an
					// empty list as the 2nd argument to this native function. For the 1st function argument,
					// caller should pass the feed directory name.  Caller should also pass a
					// mutable int32 variable initialized to 0 for the native function's use of storing the recursive level locally.
					// Other than passing this variable initialized to 0, caller has no other business with this variable.
					boolean result = getFileNamesInDirectory($RSTRING_FEED_DIRECTORY, 
						$LIST_RSTRING_FILES_TO_BE_IGNORED, listOfFileNamesFound, recursionLevel);
					
					if (result == true) {
						// In the list of returned files, we are interested only in those files
						// that we are seeing for the first time since this application started.
						// Let us compare the result list with the list of fileNames we have already seen.
						set<rstring> setOfFileNamesFound = toSet(listOfFileNamesFound);
						// Derive a new set by comparing the result set with our cached set.
						set<rstring> setOfNewFiles = setDifference(setOfFileNamesFound, setOfFilesAlreadySeen);
						// Let us now now join the new files to the set of files we have already seen.
						concatM(setOfFilesAlreadySeen, setOfNewFiles);
											
						for(rstring nameOfFile in setOfNewFiles) {
							TransactionFile tf = {fileName=nameOfFile};
							submit(tf, TransactionFile);	 
						} // End of for(rstring fileName in listOfFileNamesFound)
					} // End of if (result == false)
				} // End of onTuple StartBatchFile
		} // End of TransactionFile = Custom(StartBatchFile)
		
		() as FileNameDisplay = Custom(TransactionFile as TF) {
			logic
				onTuple TransactionFile: {
					printStringLn(TF.fileName);
				} // End of onTuple TransactionFile
		} // End of FileNameDisplay = Custom(TransactionFile as TF)
}

~~~~~~
