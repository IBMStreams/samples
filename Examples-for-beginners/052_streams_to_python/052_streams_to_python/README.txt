===================================================================
Calling Python code from IBM InfoSphere Streams
(For a detailed technical article on this topic, you can refer to
 this URL: http://tinyurl.com/c3s56fq)

Mar/27/2015
===================================================================
This README file provides high-level implementation details about this project.
Main goal here is to show a simple example for calling Python code from an
IBM InfoSphere Streams application using the Streams C++ native function feature.
Developers with existing Python modules can make use of the Python assets easily by 
directly calling Python functions from Streams. That will allow reusing the
existing Python code in the context of InfoSphere Streams applications.

This project was successfully compiled and tested in the following test environment.

a) RedHat Enterprise Linux 6.1 (or an equivalent CentOS version)
b) gcc version 4.4.5 20110214 (Red Hat 4.4.5-6) (GCC)
c) Python 2.6.6 (r266:84292, Apr 11 2011, 15:50:32) [Shipped with RHEL6]
   (Please ensure that you have /usr/lib64/libpython2.6.so file and /usr/include/python2.6 directory.)
d) IBM InfoSphere Streams Version 4.x

Following are the project components that can be found in this workspace directory [workspace1].

1) StreamsToPythonLib C++ project
   ------------------------------
This C++ project contains the following files.

a) StreamsToPython.h          (C++ class interface file)
b) StreamsToPython.cpp        (C++ class implementation file)
c) StreamsToPythonWrapper.h   (C++ include file that contains the Streams native function code)
d) mk                         (A make file to build the shared object library of the C++ code shown above)

2) streams_to_python SPL project
   -----------------------------
This SPL project contains the following files.

a) README.txt                        (The file you are reading now)
b) python.wrapper.example            (SPL project directory)
      --> streams_to_python.spl      (Simple SPL file that invokes a C++ native function that
                                      in turn calls Python functions.)
      --> native.function            (SPL Native Function directory)
             --> function.xml        (SPL Native Function Model file)
c) data                              (SPL application's data directory)
      --> UrlInput.csv               (Input file containing test URL strings)
      --> UrlToIpAddress.py          (Simple Python script, whose functions will be called from Streams)
                                     (It is placed here, because this is the current working directory for a Streams application)
      --> Expected-UrlTo*.csv        (CSV file containing expected results from this application as of Feb/2013)
d) impl/lib                          (Directory where the .so library built above in the C++ project will be copied)
                                     (Streams Native Function Model file is configured to load .so from this directory)
e) impl/include                      (Directory where the include files from the above C++ project will be copied)
                                     (Streams Native Function Model file is configured to look here for the include files)
f) build-standalone.sh               (Script that will build a Standalone Streams executable - It doesn't need the Streams runtime)
g) build-distributed.sh              (Script that will build a Distributed Streams executable - It requires the Streams runtime)
h) run-standalone.sh                 (Script that will run the standalone executable of this application)
i) run-distributed.sh                (Script that will run the distributed executable of this application)
j) stop-streams-instance.sh          (Script that will stop a specified Streams instance)

High-level code details about this application
----------------------------------------------
As explained above, this application is made of a C++ project [StreamsToPythonLib] and a 
Streams SPL project (streams_to_python). 

Inside the C++ project, source code for the native function logic is included. Primarily, it uses
the Python C API to embed Python code during the execution of C++ code. Embedding Python in C++ code
is well documented as part of the Python documentation. A Wrapper include (.h) file is an important
one and that file provides an entry point for the SPL application to directly call a C++ class method.
All the C++ logic will be compiled into a shared object library (.so) file and made available to the
SPL application.

Inside the SPL project, a simple SPL flow graph is constructed to make a call chain (SPL<-->C++<-->Python).
This SPL code reads URLs from an input file in the data directory, calls the C++ native function to 
execute the python code, receives the results, and writes it to an output file in the data directory. Inside
the SPL project directory, a native function model XML file outlines the meta information needed to directly
call a C++ class method from SPL. This detail includes the C++ wrapper include file name, C++ namespace
containing the wrapper functions, C++ wrapper function prototype expressed using SPL syntax/types, 
name of the shared object library created from the C++ project, location of the shared object library,
location of the wrapper include file etc. 

Steps to build and run this application
---------------------------------------
A mentioned above, two projects (C++ and SPL) need to be built before attempting to run this application.

1) In a terminal window, switch to workspace1/StreamsToPythonLib directory and run ./mk
2) If ./mk worked correctly, it should have built and copied the .so file and the C++ include files to the following locations.
   workspace1/streams_to_python/impl/lib/x86_64.RHEL6
   workspace1/streams_to_python/impl/include

3) Switch to workspace1/streams_to_python directory and run either build-standalone.sh or build-distributed.sh.
4) To test the application, you can run either run-standalone.sh or run-distributed.sh.
   (Standalone mode application doesn't require the Streams runtime instance to be up and and running, whereas the
    distributed mode application uses the Streams runtime instance.
5) You can verify the application results in data/UrlToIpAddress-Result.csv file.
6) Your results should be very similar to the ones in the data/Expected-UrlToIpAddress-Result.csv file containing the
   results captured in Feb/2013.
7) If you ran the distributed mode application, you can now run the stop-streams-instance.sh.

Useful things to know
---------------------
1) When you run the standalone mode application, you may see this warning: 'import site' failed; use -v for traceback.
   You need not worry, as it will not affect the behavior of this application.

2) Python script (UrlToIpAddress.py) being called from the SPL application is stored in the streams_to_python/data directory.
   Inside the C++ native function, there is additional code to that does the necessary preparation for loading this
   Python script from this application's data sub-directory.

Good Luck and enjoy SPLashing in Streams along with Python.
============================================================
