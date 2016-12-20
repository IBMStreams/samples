This example demonstrates two important techniques that will be commonly used in real-world use cases.

1) Creating a C++ primitive operator.

2) Calling a function available inside a .so shared library from the C++ primitive operator logic.

Application logic here is to receive input tuples as hostnames and then make the C++ primitive operator logic invoke a shared library function that does a name server lookup.

[This example has a companion C++ project called PrimitiveOperatorLib.]
