.PHONY: all clean 

SPLC_FLAGS = -a -t $(STREAMS_INSTALL)/toolkits/com.ibm.streams.text --data-directory=data
SPLC = $(STREAMS_INSTALL)/bin/sc

SPL_CMD_ARGS ?= 

all: output/normalize.example/bin/standalone output/tokenize.stream/bin/standalone output/tokenize.bag/bin/standalone

output/normalize.example/bin/standalone:  normalize.example/NormalizeMentions.spl
	$(SPLC) $(SPLC_FLAGS)  -T -M normalize.example::NormalizeMentions $(SPL_CMD_ARGS) --output-dir=output/normalize.example

output/tokenize.stream/bin/standalone: tokenize.example/TokenStream.spl
	$(SPLC) $(SPLC_FLAGS)  -T -M tokenize.example::TokenStream $(SPL_CMD_ARGS) --output-dir=output/tokenize.stream

output/tokenize.bag/bin/standalone: tokenize.example/TokenBag.spl
	$(SPLC) $(SPLC_FLAGS)  -T -M tokenize.example::TokenBag $(SPL_CMD_ARGS) --output-dir=output/tokenize.bag
	

clean: 
	rm -r output
