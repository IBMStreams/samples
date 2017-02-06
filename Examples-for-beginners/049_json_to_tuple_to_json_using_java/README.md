~~~~~~ Scala 
/*
=====================================================================
This example shows how an SPL application can consume JSON formatted
data and convert it to SPL tuples. It also shows how to do the
reverse action i.e. converting SPL tuples to JSON formatted data.

JSON<-->Tuple bidirectional conversion is accomplished via two
Java primitive operators that make use of the JSON (Java) libraries 
shipped as part of the Streams product. Those two operators are as
shown below.

1) JSONToTuple
2) TupleToJSON

Note: Performance of this JSON<-->Tuple conversion approach will be limited
by the speed of your Java environment. If you want to achieve better performance,
C++ code is the way to go. There is a separate example that shows
how to do this using C++. Due to licensing restrictions, that example
(055_json_to_tuple_to_json_using_c++) requires the users to obtain
an open source component and install it on their own machine. You can
read all the details inside of that example.

The streamsx.json toolkit was obtained from the Streams GitHub. Then, that toolkit was
built on a RHEL6 server which already had Streams 4.x installed in it.
After building it, the resulting jar file inside that toolkit directory
(streamsx.json-master/com.ibm.streamsx.json/impl/lib/com.ibm.streamsx.json.jar) was 
manually copied to this SPL example project's impl/lib directory. In the same way, 
a directory inside that toolkit containing the new operator model XML files  
(streamsx.json-master/com.ibm.streamsx.json/com.ibm.streamsx.json) was 
manually copied into this SPL example project directory.

SPL code below in this application demonstrates how to use those two
operators to perform JSON<-->Tuple conversion. This application 
explains three different test cases to convert simple, slightly
complex, and a moderately complex JSON data structures. For each of
those test cases, it uses three input files in the ./data directory
containing different JSON data structures.

IMPORTANT TIPS
--------------
It is often required to define the equivalent SPL tuple types to
match all the nested types your JSON data structure will carry.
In order to make that job easier, you will find a shell script
named run.sh in the ./bin directory. You can run that shell script
by passing a command-line argument that provides the fully qualified
path of a text file containing your JSON data structure populated with
dummy data. Then, it will output on the screen with definitions of the
SPL types you will require inside your SPL source file. You can simply
incorporate the displayed SPL types in your SPL source file.

(e-g) ./bin/run.sh   ./data/json_input1.dat

When you compile and run this application, it will read the three JSON formatted
input files in the ./data directory and write its results to six different
output files in the ./data directory.

If you want to use this JSON<-->Tuple conversion facility in your
own SPL project, you have to copy the following from this project to your
SPL project directory and then you can start using those two operators within your own SPL application.

i)   ./impl/lib/com.ibm.streamsx.json.jar   (File)
ii)  ./com.ibm.streamsx.json   (Directory)
iii) ./bin        (Directory)

(Special thanks to our colleague Rohit who wrote the streamsx.json toolkit and
 the associated script used in this particular example.)
=====================================================================
*/
namespace sample;

use com.ibm.streamsx.json::*;

composite Main {

  type 
    MyJsonType = rstring jsonData;
    // Following two SPL types are used in TEST1 as described in the graph section.
    MyAddressType = rstring country;
    MyProfileType = rstring name, uint64 age, MyAddressType address;
    // Following three SPL types are used in TEST2 as described in the graph section.
    addressType = rstring state, rstring street, rstring city, rstring zipCode;
    contactType = addressType address, rstring name, rstring phone;
    contactsType = contactType contact;
    MyContactsType = contactsType contacts;
    // Following four SPL types are used in TEST3 as described in the graph section.
    authorType = rstring first, rstring last;
    bookType = rstring title, authorType author, rstring publisher;
    booksType = list<bookType> book;
    MyBooksType = booksType books;
  	FormulaInfo_t = rstring formula, int32 formulaId;
	RicInfo_t = rstring ric, rstring field, rstring interval, int32 intervalMultiplier;
	ClientInfo_t = rstring uuid, rstring trackingId;
	CalcResult_t = list<int64> result;
	HistoricalCalcResponse_t = ClientInfo_t clientInfo, RicInfo_t ricInfo, FormulaInfo_t formulaInfo, CalcResult_t calcResult;
	// HistoricalCalcResponse_t = ClientInfo_t clientInfo;
	Topic_t = rstring name, int32 id;
	SubscriptionError_t = ClientInfo_t clientInfo, Topic_t topic, rstring errorDescription;    

  graph      
     // *** TEST 1 (Handling a simple JSON data structure)***
     // Do the conversion of JSON<-->Tuple using the json_input1.dat file.
     // Read the json formatted strings from a text file.
     stream<MyJsonType> ProfileInput = FileSource() {
        param
           file: "json_input1.dat";
           format: line;
     } // End of FileSource()

     // Convert the JSON formatted string into a SPL tuple.
     stream<MyProfileType> ProfileTuple = JSONToTuple(ProfileInput) {}

     // Save the "JSON to Tuple" result into a file.
     () as Sink1a = FileSink(ProfileTuple) {
        param
           file: "tuple_output1.dat";
     } // End of FileSink(ProfileTuple)

     // Convert the SPL tuple into a JSON formatted string.
     stream<MyJsonType> ProfileJson = TupleToJSON(ProfileTuple) {}

     // Save the "Tuple to JSON" result into a file.
     () as Sink1b = FileSink(ProfileJson) {
        param
           file: "json_output1.dat"; 
           quoteStrings: false;
     } // End of FileSink(ProfileJson)

     // =============================================================
     // *** TEST 2 (Handling a slightly complex JSON data structure ***
     // Do the conversion of JSON<-->Tuple using the json_input2.dat file.
     // Read the json formatted strings from a text file.
     stream<MyJsonType> ContactsInput = FileSource() {
        param
           file: "json_input2.dat";
           format: line;
     } // End of FileSource()

     // Convert the JSON formatted string into a SPL tuple.
     stream<MyContactsType> ContactsTuple = JSONToTuple(ContactsInput) {}

     // Save the "JSON to Tuple" result into a file.
     () as Sink2a = FileSink(ContactsTuple) {
        param
           file: "tuple_output2.dat";
     } // End of FileSink(ContactsTuple)

     // Convert the SPL tuple into a JSON formatted string.
     stream<MyJsonType> ContactsJson = TupleToJSON(ContactsTuple) {}

     // Save the "Tuple to JSON" result into a file.
     () as Sink2b = FileSink(ContactsJson) {
        param
           file: "json_output2.dat"; 
           quoteStrings: false;
     } // End of FileSink(ContactsJson)

     // =============================================================
     // *** TEST 3 (Handling a moderately complex JSON data structure ***
     // Do the conversion of JSON<-->Tuple using the json_input3.dat file.
     // Read the json formatted strings from a text file.
     stream<MyJsonType> BooksInput = FileSource() {
        param
           file: "json_input3.dat";
           format: line;
     } // End of FileSource()

     // Convert the JSON formatted string into a SPL tuple.
     stream<MyBooksType> BooksTuple = JSONToTuple(BooksInput) {}

     // Save the "JSON to Tuple" result into a file.
     () as Sink3a = FileSink(BooksTuple) {
        param
           file: "tuple_output3.dat";
     } // End of FileSink(BooksTuple)

     // Convert the SPL tuple into a JSON formatted string.
     stream<MyJsonType> BooksJson = TupleToJSON(BooksTuple) {}

     // Save the "Tuple to JSON" result into a file.
     () as Sink3b = FileSink(BooksJson) {
        param
           file: "json_output3.dat"; 
           quoteStrings: false;
     } // End of FileSink(BooksJson)

     // =============================================================
  	 // *** TEST 4 (Read the historical calc response tuple data and convert it to JSON)
  	 // This tuple data contains a list of primitive types i.e. list<int32> 
     // Do the conversion of Tuple-->Json using the historical_data.csv file.
     // Read the tuple data from a CSV file.
     stream<HistoricalCalcResponse_t> HistoricalDataTuple = FileSource() {
        param
           file: "historical_data_tuple_input1.csv";
     } // End of FileSource()

     // Convert the SPL tuple into a JSON formatted string.
     stream<MyJsonType> HistoricalDataJson = TupleToJSON(HistoricalDataTuple) {}

     // Save the "Tuple to JSON" result into a file.
     () as Sink4b = FileSink(HistoricalDataJson) {
        param
           file: "historical_data_json_output1.txt"; 
           quoteStrings: false;
     } // End of FileSink(HistoricalDataJson)

} // End of composite Main

~~~~~~
