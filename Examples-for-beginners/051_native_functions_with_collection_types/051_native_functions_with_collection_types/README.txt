Last modified on: May/14/2013

This application is a simple example to show
how to call C++ native functions that are compiled into a 
.so shared library. In addition, it demonstrates
how to pass SPL collection data types (list, map,
and tuple) to such C++ native functions.

There are 4 major parts that constitute this application.

1) SPL source code file.
2) A C++ class interface file and a C++ class implementation file.
3) A C++ wrapper include file that provides a direct entry point for
   the SPL code into the C++ native functions defined in the
   C++ files mentioned above in (2).
4) A native function model XML file that tells the SPL compiler
   about the function prototypes of the available C++ functions and
   the location of the .so library housing those C++ functions.


This project is made of two different sub-projects (a C++ project and an SPL project).

C++ Project:  NativeFunctionsWithCollectionTypesLib
SPL Project:  051_native_functions_with_collection_types

Every source file in these two projects contains suitable comments
for you to understand how all of this works.

In order to compile this, you can follow these two steps.

a) In a terminal window, switch to the C++ project and run  ./mk
   --> This will build the platform specific .so library
       i.e. RHEL5 versus RHEL6, 32 bit versus 64 bit, x86 versus PPC
   --> It copies the .so library file and the C++ include files to
       the SPL project's impl/lib and impl/include directories.

b) Switch to the SPL project and run the following command
   --> ./mk

To run this application, follow this simple step.
   --> Switch to the SPL project and run ./run-standalone.sh

(OR)

If you wish, you can also import this project in Streams Studio and
run it from there.

Extra tip
=========
In the native function model XML file, you will see a reference about
"archLevel" within the Library section. This is a simple shell script that
will get executed at the time of loading the .so library to find out the
correct RHEL version, CPU type (32 bit versus 64 bit) and the system
architecture type (x86 versus PPC). The make script (mk) in the C++
project builds a platform-specific version of the .so library and copies it to
to a corresponding platform specific directory inside the SPL project's
impl/lib directory. Hence, this "archLevel" script is useful to determine
which .so file is needed and this script is already included in the
SPL project's impl/bin directory.
