.PHONY: all clean

ifeq ($(STREAMS_INSTALL),)
  $(error error: environment variable STREAMS_INSTALL has to be set)
endif

all:
	cd RecursiveDirScanLib; ./mk; cd -
	cd 050_recursive_dir_scan; make all; cd -

clean:
	cd 050_recursive_dir_scan; make clean; cd -

