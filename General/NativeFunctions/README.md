This example shows how to create C++ native functions in SPL.  The motivating application is encryption and decryption.  See [this StreamsDev article](https://developer.ibm.com/streamsdev/docs/extending-streams-functionality-with-native-functions/) for more information.

There are three directories:
* `CryptoTest`  -- shows how to use the native functions to do encryption and decryption.
* `com.ibm.streamsx.aes` -- contains native functions for doing encryption and decryption.  The implementation is all in the header file.
* `com.imb.streamsx.aesv2` -- contains the native functions, but the implementation and headers are kept separate.
