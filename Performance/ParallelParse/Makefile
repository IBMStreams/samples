MAIN_COMPOSITES = FileSourceOnly FileSourceString SequentialParse ParallelParse ParallelParseOrdered

SPLC_FLAGS = -a
SPLC = $(STREAMS_INSTALL)/bin/sc
SPL_CMD_ARGS ?= 

.PHONY: all
all: $(MAIN_COMPOSITES)

$(MAIN_COMPOSITES):
	$(SPLC) $(SPLC_FLAGS) -T --output-directory=output/$@ --data-directory=data -M $@ $(SPL_CMD_ARGS)

