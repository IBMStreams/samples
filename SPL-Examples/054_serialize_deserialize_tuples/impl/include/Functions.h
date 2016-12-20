#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

// Include this SPL file so that we can use the SPL functions and types in this C++ code.
#include "SPL/Runtime/Function/SPLFunctions.h"
#include <vector>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>

// Define a C++ namespace that will contain our native function code.
namespace tuple_serializer_deserializer {
	// By including this line, we will have access to the SPL namespace and anything defined within that.
	using namespace SPL;

		// Prototype for our native functions are declared here.
		void serializeTuple(Tuple const & tuple, blob & rblob);
		void deserializeTuple(Tuple & tuple, blob const & rblob);

		// Serialize the given tuple into a blob (byte array).
		inline void serializeTuple(Tuple const & tuple, SPL::blob & rblob)
		{
			SPL:NativeByteBuffer buf;
			tuple.serialize(buf);
			rblob.adoptData(buf.getPtr(), buf.getSerializedDataSize());
			buf.setAutoDealloc(false);
		}

		// Deserialize the given blob into its original tuple form.
		inline void deserializeTuple(Tuple & tuple, SPL::blob const & rblob)
		{
			size_t size;
			const unsigned char * data = rblob.getData(size);
			SPL:NativeByteBuffer buf((unsigned char *)data, size);
			tuple.deserialize(buf);
		}
}
#endif
