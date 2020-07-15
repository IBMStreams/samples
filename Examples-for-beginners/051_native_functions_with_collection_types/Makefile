.PHONY: all clean

ifeq ($(STREAMS_INSTALL),)
  $(error error: environment variable STREAMS_INSTALL has to be set)
endif

all:
	cd NativeFunctionsWithCollectionTypesLib; ./mk; cd -
	cd 051_native_functions_with_collection_types; make all; cd -

clean:
	cd 051_native_functions_with_collection_types; make clean; cd -

