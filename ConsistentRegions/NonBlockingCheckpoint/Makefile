.PHONY: all test clean 

SPLC_FLAGS ?= -v -z --data-directory ./data -a
SPLC = $(STREAMS_INSTALL)/bin/sc

# compile-time parameters: 
# consistent: whether to use consistent region ("yes") or not ("no")
# period: consistent region checkpointing period in seconds
# tupExpected: total number tuples to process
consistent ?= yes
period ?= 5
tupExpected ?= 50
SPL_CMD_ARGS ?= $(consistent) $(period) $(tupExpected) 
SPL_MAIN_COMPOSITE = test::Main

all: test

test:
	$(SPLC) $(SPLC_FLAGS) -M $(SPL_MAIN_COMPOSITE) $(SPL_CMD_ARGS)

clean: 
	$(SPLC) $(SPLC_FLAGS) -C -M $(SPL_MAIN_COMPOSITE)
	rm -rf output
