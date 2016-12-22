~~~~~~
/*
This example shows a simple method for converting "tuple data to JSON" and 
"JSON data to tuple". In this example, this conversion is done using C++ code.
There is another example (049_json_to_tuple_to_json_using_java) that shows the
same technique using Java. 

This particular example was added just to give the users a flavor of doing this
conversion using C++ rather than using Java. There is a general belief that code
written using C++ tends to perform better than the same logic written using Java.
Users can decide what works better for them regarding the choice between C++ and Java.

In this example, two C++ native functions will provide a way to convert tuples into
JSON data and convert JSON data into SPL tuples. You can refer to the native function
model XML file about these two functions. Refer to ../../impl/include/Functions.h file for
the implementation of these two functions.

Important Note
--------------
This example will require an open source component called CAJUN - C++ API for JSON.
To avoid the code redistribution issues, this example is not packaged with that open source component.
That component is available from sourceforge with a BSD license. If your work environment doesn't 
permit you to download the BSD licensed open source components, then you can't run this example.
If the CAJUN BSD licensing policy is acceptable for your needs, then you can download the
CAJUN component by following the instructions shown below and store it as part of this SPL project
directory. 

1) Download cajun-2.0.2.zip from this URL and store it inside the home directory of your Linux system.

http://sourceforge.net/projects/cajun-jsonapi/files/latest/download

2) Unzip the cajun-2.0.2.zip file inside the home directory of your Linux system.
   Following command will create a directory named "cajun-2.0.2".

cd ~/
unzip cajun-2.0.2.zip

3) Move the json directory from the cajun-2.0.2 directory into this project directory as shown below.
   (Following command assumes that your SPL examples directory is in your home directory. If not,
    you have to modify the following command as needed.)
    
mv ~/cajun-2.0.2/json   ~/SPL-Examples-For-Beginners/055_json_to_tuple_to_json_using_c++/impl/include

4) You can carefully delete the cajun-2.0.2 directory.

rm -r ~/cajun-2.0.2

5) As mentioned above, this example has default code that does dummy conversion to avoid any
open source code redistribution issues. If you want to do a real Tuple<-->JSON conversion, 
then you must perform the 4 steps mentioned above. After that, you have to edit the following file.

a) ~/SPL-Examples-For-Beginners/055_json_to_tuple_to_json_using_c++/impl/include/Functions.h
b) Inside that file, search for  this string:  #define CAJUN_API_AVAILABLE
c) If the line with that string is commented out, you have to uncomment that line and save the file.
d) You have to build the project now either inside the Streams Studio or from a command line.
e) While compiling, you will notice the following two C++ compiler errors.

../../.././impl/include/json/reader.inl:323:45: error: incomplete universal character name \u
../../.././impl/include/json/writer.inl:151:15: error: incomplete universal character name \u 

f) To resolve those two errors, open those two CAJUN API source files shown as part of the compiler errors.
g) Go to the indicated lines in the errors, and comment those two erroneous lines and save the two files.
h) At this time, do a full clean of the SPL project and then rebuild the SPL project.
i) Now, it should compile correctly with no errors.
j) If you run the application, it will perform the required Tuple<-->JSON conversions using 
   the CAJUN API you downloaded and configured.
*/
namespace com.acme.test;

composite json_to_tuple_to_json_using_cpp {
	type
		// Define tuple format
		PersonData = tuple<rstring firstName, rstring lastName, rstring ssn,
			rstring streetAddress, uint32 age, float32 height, boolean isEmployed,
			list<rstring> favoriteMovies>;
		// Final result tuple is a combination of the json formatted string and 
		// the tuple attributes obtained via the jsonToTuple conversion.
		ResultData = tuple<rstring jsonString>, tuple<PersonData>;

	graph
		// Read the person data from a file.
		stream<PersonData> Person = FileSource() {
			param
				format: csv;
				file: "PersonData.txt";
		}
		
		// In this Custom operator, we will convert every incoming tuple into a 
		// JSON formatted string.
		stream<rstring jsonString> PersonInJsonFormat = Custom(Person) {
			logic	
				state: {
					mutable rstring _jsonString = "";
					mutable PersonInJsonFormat _personInJsonFormat = {};
				}			
				
				onTuple Person: {
					// Do the tuple to Json conversion using C++ native functions.
					// In this particular implementation, we are going to flatten
					// tuple structure into individual attributes and pass them to
					// the conversion function.
					// Alternatively, we could have passed the tuple directly to the
					// conversion function and used the SPL C++ APIs to parse the
					// individual attributes inside the conversion function.
					// Since that involves extra coding, that is not done in this 
					// example. If you have an interest in doing that, you can
					// write your own conversion function to take a tuple as an
					// input rather than taking flattened attributes as input.
					convertTupleAttributesToJson(_jsonString, firstName, lastName, ssn,
						streetAddress, age, height, isEmployed, favoriteMovies);	
					// Populate the outout tuple attribute and send it.
					_personInJsonFormat.jsonString = _jsonString;
					submit(_personInJsonFormat, PersonInJsonFormat);
				}
		}
		
		// In this Custom operator, we will convert the JSON formatted string into 
		// individual tuple attributes. Then, we will emit both the Json string and
		// the converted tuple attributes to a downstream operator.
		stream<ResultData> FinalResult = Custom(PersonInJsonFormat as PIJF) {
			logic
				state: {
					mutable FinalResult _fr = {};
				}
				
				// As explained in the previous Custom operator, we could have passed a tuple
				// type to the conversion function. To avoid extra C++ coding, this implementation
				// doesn't do that. If you are interested in passing a tuple to the conversion function,
				// you can modify this example to do that.
				onTuple PIJF: {
					// We will directly pass the attributes from our stateful tuple variable so that
					// the conversion function can do the assignment directly.
					convertJsonToTupleAttributes(PIJF.jsonString, _fr.firstName,
						_fr.lastName, _fr.ssn, _fr.streetAddress, 
						_fr.age, _fr.height, _fr.isEmployed, _fr.favoriteMovies);
					// Our final result tuple attributes are already filled in the
					// previous conversion function.
					// In addition, we will populate the final result tuple with the json string.
					_fr.jsonString = PIJF.jsonString;
					// Submit it.
					submit(_fr, FinalResult);
					// Clear the output tuple so that the list attribute will get emptied.
					_fr = (FinalResult){};
				}
		}
		
		// Receive the Final result tuple carrying JSON string and the individual
		// tuple attributes and print it on the screen.
		()as Sink = FileSink(FinalResult){
			param
				// Write the results to stdout error.
				file : "/dev/stderr" ;
				flush : 1u ;
		}			 
}

~~~~~~
