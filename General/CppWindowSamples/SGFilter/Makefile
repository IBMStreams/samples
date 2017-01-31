# Copyright (C) 2015, International Business Machines Corporation.
# All Rights Reserved.

.PHONY: all clean help init distributed standalone

SPL_MAIN_COMPOSITE = Main
SPLC = $(STREAMS_INSTALL)/bin/sc
SPLC_FLAGS = -a
SPL_CMD_ARGS ?= -t $(STREAMS_INSTALL)/toolkits --data-directory ./data 

all: standalone

distributed:
	$(SPLC) $(SPLC_FLAGS) -M $(SPL_MAIN_COMPOSITE) $(SPL_CMD_ARGS)

standalone:
	$(SPLC) $(SPLC_FLAGS) -T -M $(SPL_MAIN_COMPOSITE) $(SPL_CMD_ARGS)

clean: 
	-$(SPLC) $(SPLC_FLAGS) -C -M $(SPL_MAIN_COMPOSITE)
	-rm -f ./data/*

help:
	@echo "make <all|clean|help>"
	@echo "targets:"
	@echo "help: print this help text"
	@echo "all: builds this project as a standalone application"
	@echo "distributed: builds this project as a distributed application"
	@echo "clean: remove object files"
