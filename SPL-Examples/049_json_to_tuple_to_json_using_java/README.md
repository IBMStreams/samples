This example shows how an SPL application can consume JSON formatted data and convert it to SPL tuples. It also shows how to do the reverse action i.e. converting SPL tuples to JSON formatted data. JSON<-->Tuple bidirectional conversion is accomplished via two Java primitive operators that make use of the JSON (Java) libraries shipped as part of the Streams product. Those two Java operators are JSONToTuple and TupleToJSON.

Note: Performance of the JSON<-->Tuple conversion in this example will be limited by the speed of your Java environment. If you want to get better performance, C++ code would help. There is a separate example (055_json_to_tuple_to_json_using_c++) that shows how to do this conversion using C++.

