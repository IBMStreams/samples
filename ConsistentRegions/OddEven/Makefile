.PHONY: all standalone distributed clean 

SPLC_FLAGS ?= --data-directory ./data -a
SPLC = $(STREAMS_INSTALL)/bin/sc
SPL_CMD_ARGS ?= $(consistent) 
SPL_MAIN_COMPOSITE = Main

all: distributed 

distributed:
	$(SPLC) $(SPLC_FLAGS) -M $(SPL_MAIN_COMPOSITE) $(SPL_CMD_ARGS)

clean: 
	$(SPLC) $(SPLC_FLAGS) -C -M $(SPL_MAIN_COMPOSITE)

