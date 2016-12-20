This example shows how to use the Streams C++ native function facility to recursively scan a given directory and obtain the names of the files present. The logic for the recursive directory scan polls the specified directory periodically and notifies the downstream operator with a new file that just appeared. There is a companion C++ project for this SPL project. Please refer to the RecursiveDirScanLib project for the C++ logic.

Important sequence of logic for this application:

1) SPL code resolves the C++ native function in its native.function/function.xml file.
2) A call from the SPL code to the native function lands in the wrapper inline C++ function defined in the RecursiveDirScanWrappers.h file of the companion C++ project.
3) From that wrapper function, it gets access to a singleton C++ object of the RecursiveDirScan class and then invokes the getFileNamesInDirectory C++ method.
4) When that C++ method returns, it will have the results stored in a list<string> reference that was passed to it.
5) Back in the SPL code, there is additional logic to cache the already seen files and to filter only the newly found files to send to the downstream operator.

In order to test this application, please refer to the commentary at the top of the SPL file in this project.

[This example has a companion C++ project called RecursiveDirScanLib.]

