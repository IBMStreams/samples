.PHONY: all clean

ifeq ($(STREAMS_INSTALL),)
  $(error error: environment variable STREAMS_INSTALL has to be set)
endif

all:
	cd NativeFunctionLib; ./mk; cd -
	cd 032_native_function_at_work; make all; cd -

clean:
	cd 032_native_function_at_work; make clean; cd -

