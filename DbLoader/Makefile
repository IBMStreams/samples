# -----------------------------------------------------------------------------
# phony targets are targets being not in the filesystem but are always "active"
# -----------------------------------------------------------------------------

.PHONY: all clean standalone distributed clean-standalone clean-distributed clean-output clean-runtime-generated-files

# -----------------------------------------------------------------------------
# The environment has to be prepared.
# -----------------------------------------------------------------------------

ifeq ($(STREAMS_INSTALL),)
$(error error: environment variable STREAMS_INSTALL has to be set)
endif

# -----------------------------------------------------------------------------
# Namespace of this toolkit is the name of the root directory.
# -----------------------------------------------------------------------------

APP_NAME = $(notdir $(CURDIR))

# -----------------------------------------------------------------------------
# InfoSphere Streams Compiler & Arguments
# -----------------------------------------------------------------------------

SPLC_FLAGS = -a

SPLC = $(STREAMS_INSTALL)/bin/sc
# copy com.ibm.streamsx.linuxshell to the $(TEDA_INSTALL)/toolkits directory
SPL_CMD_ARGS = -t $(TEDA_INSTALL)/toolkits

# SPLDOC
SPLDOCDIR?=doc/spldoc
SPLDOC := $(STREAMS_INSTALL)/bin/spl-make-doc


# -----------------------------------------------------------------------------
# Main Composite
# -----------------------------------------------------------------------------

SPL_MAIN_COMPOSITE = com.ibm.streamsx.db.dbloader::Main

# -----------------------------------------------------------------------------
# Output Directories
# -----------------------------------------------------------------------------

OUTPUT=output
STAND_ALONE_OUTPUT_DIR = $(OUTPUT)/Standalone
DISTRIBUTED_OUTPUT_DIR = $(OUTPUT)/Distributed

# -----------------------------------------------------------------------------
# Build and Clean Targets
# -----------------------------------------------------------------------------

all: clean distributed spldoc

dependency:
	@if [ "" == "$(TEDA_INSTALL)" ] ; then echo "error: environment variable TEDA_INSTALL has to be set"; exit 1; fi

standalone: dependency
	$(SPLC) $(SPLC_FLAGS) -T -M $(SPL_MAIN_COMPOSITE) --output-directory $(STAND_ALONE_OUTPUT_DIR) 
#$(SPL_CMD_ARGS) 

distributed: dependency
	$(SPLC) $(SPLC_FLAGS) -M $(SPL_MAIN_COMPOSITE) --output-directory $(DISTRIBUTED_OUTPUT_DIR) 
#$(SPL_CMD_ARGS)

clean: clean-standalone clean-distributed clean-output clean-runtime-generated-files clean-spldoc

clean-standalone:
	$(SPLC) $(SPLC_FLAGS) -C -M $(SPL_MAIN_COMPOSITE) --output-directory $(STAND_ALONE_OUTPUT_DIR)
	-rm -rf toolkit.xml

clean-distributed:
	$(SPLC) $(SPLC_FLAGS) -C -M $(SPL_MAIN_COMPOSITE) --output-directory $(DISTRIBUTED_OUTPUT_DIR)
	-rm -rf toolkit.xml

clean-output:
	-rm -rf $(OUTPUT)

clean-runtime-generated-files:
	-rm -rf data/*.txt

build-spldoc:
	@ echo $@... 
	@echo "Generating SPL documentation:"
	$(SPLDOC) -i . --output-directory $(SPLDOCDIR) --include-all 
	
spldoc: clean-spldoc build-spldoc

clean-spldoc:
	@ echo $@... 
	@echo "Cleaning SPL documentation:"
	rm -fr $(SPLDOCDIR)
	



