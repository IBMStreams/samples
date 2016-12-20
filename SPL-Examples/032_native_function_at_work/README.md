This application shows how native functions written in C++ can be called within an SPL application.

There are two ways in which native functions can be written in C++.

1) Code for the C++ functions can be written in a C++ header file.

2) C++ functions can be written outside of the SPL project and packaged into a shared library (.so) file. All the SPL developer will have to work with are an .so file and a C++ header file.

This application demonstrates incorporating native functions built in both of those ways.

[This example has a companion C++ project called NativeFunctionLib.]

