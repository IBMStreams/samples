~~~~~~ Scala 
/*
==================================================================
This application shows how native functions written in C++ can be
called within an SPL application. Before working through this example,
you may want to install the Eclipse CDT plugin (C Development Tool)
from the CDT update site (http://download.eclipse.org/releases/galileo).

There are two ways in which native functions can be written in C++.
1) All the code for the C++ functions can be written in a C++ header file.
2) All the C++ functions can be written outside of the SPL project and 
   and packaged into a shared library (.so) file. All the SPL developer will
   have to work with are an .so file and a C++ header file.

This application demonstrates incorporating native functions built in both of those ways.

There are two native function models used in this application. Browse those
two XML files to learn how they are defined using the function model editor.

a) function.xml (in the functions.inside.header_file namespace).
 
b) function.xml (in the functions.inside.shared_lib namespace).

In the case of function model (a), you will find the C++ native functions implemented
in a C++ header (Functions.h) file that is present in the impl/include directory.
 
In the case of function model (b), the native function is provided through a
.so shared library that is built in a separate C++ project [NativeFunctionLib].
You can build that project by running the mk script. Make sure it is executable first. The script copies the C++ interface file and the .so file to the SPL project's impl/include and 
impl/lib directories respectively. That way, this SPL project will be
self-contained with the C++ interface file and the .so file inside of it.
================================================================== 
*/
namespace my.sample;
use functions.inside.header_file::*;
use functions.inside.shared_lib::*;

composite Main {
	type
		product = tuple<float32 price, rstring state1, float32 tax, float32 totalPrice>;
	
	graph
		stream <product> ProductRecord = FileSource () {
			param
				file: "product_details.txt";
				format: csv;
				hasDelayField: false;
				initDelay: 4.0;
		} // End of ProductRecord = FileSource
		
		// Let us compute tax using a native function defined inside a header file.
		stream <product> TotalPriceRecord1 = Functor(ProductRecord) {
			logic
				state: mutable float32 myTax = 0.0;
				
				onTuple ProductRecord:
					myTax = taxCalculation1(price, state1);
				
			param
				filter: true;
			
			output
				TotalPriceRecord1: tax = myTax, totalPrice = price + myTax;
		} // End of TotalPriceRecord1 = Functor(ProductRecord)
		
		// Display and log the results.
		() as Writer1 = Custom(TotalPriceRecord1) {
			logic
				onTuple TotalPriceRecord1: {
					printStringLn("a) " + (rstring) TotalPriceRecord1);
					// log(Sys.error, (rstring) TotalPriceRecord1);
				} // End of onTuple TotalPriceRecord:
			
		} // End of Writer1 = Custom(TotalPriceRecord1)

		// Let us compute tax using a native function defined inside a shared library.
		stream <product> TotalPriceRecord2 = Functor(ProductRecord) {
			logic
				state: mutable float32 myTax = 0.0;
				
				onTuple ProductRecord:
					myTax = taxCalculation2(price, state1);
				
			param
				filter: true;
			
			output
				TotalPriceRecord2: tax = myTax, totalPrice = price + myTax;
		} // End of TotalPriceRecord2 = Functor(ProductRecord)
		
		// Display and log the results.
		() as Writer2 = Custom(TotalPriceRecord2) {
			logic
				onTuple TotalPriceRecord2: {
					printStringLn("b) " + (rstring) TotalPriceRecord2);
					// log(Sys.error, (rstring) TotalPriceRecord1);
				} // End of onTuple TotalPriceRecord:
			
		} // End of Writer2 = Custom(TotalPriceRecord2)		
} // End of composite Main

~~~~~~
