#####################################################################
# Copyright (C)2014, 2016 International Business Machines Corporation and
# others. All Rights Reserved.
#####################################################################

.PHONY: all clean

SPLC_FLAGS = -t $(STREAMS_INSTALL)/toolkits/com.ibm.streams.db  --data-directory data

SPLC = $(STREAMS_INSTALL)/bin/sc

SPL_CMD_ARGS ?=
SPL_COMP1NAME=Main
SPL_MAIN_COMPOSITE1 = application::$(SPL_COMP1NAME)
BUILD_OUTPUT_DIR = output

all: data clean
	$(SPLC) $(SPLC_FLAGS) -M  $(SPL_MAIN_COMPOSITE1) --output-dir ./$(BUILD_OUTPUT_DIR)  $(SPL_CMD_ARGS)

data:
	mkdir data
clean:
	$(SPLC) $(SPLC_FLAGS) -C -M $(SPL_MAIN_COMPOSITE1) --output-dir output
	-rm -rf toolkit.xml
	-rm -rf data/output.dat


