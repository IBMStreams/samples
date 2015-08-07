ifeq ($(CXX),g++)
  PLATFORM := $(shell uname -i)
  ifeq ($(PLATFORM),ppc64)
    CXX=/opt/at5.0/bin/g++
  endif
endif
SPL_PKGCFG=$(STREAMS_INSTALL)/bin/dst-pe-pkg-config.sh
SPL_PKG=dst-spl-pe-install
SPL_COMPILE_OPTIONS = `$(SPL_PKGCFG) --cflags $(SPL_PKG)`
SPL_LINK_OPTIONS = `$(SPL_PKGCFG) --libs $(SPL_PKG)`
.PHONY: clean distclean

all: lib/libopensslwrapper.so

lib:
	mkdir lib

bin:
	mkdir bin

lib/libopensslwrapper.so: bin/opensslwrapper.o lib
	gcc --shared -o lib/libopensslwrapper.so bin/opensslwrapper.o

bin/opensslwrapper.o: src/opensslwrapper.cpp include/opensslwrapper.h bin
	@$(CXX) -O3 -Wall -fPIC -I include -lOpenCL $(SPL_COMPILE_OPTIONS) -c $< -o $@

clean:
	rm lib/*.so
	rm bin/*.o

distclean: clean
