.PHONY: build all clean distributed

SPL_MAIN_COMPOSITE1 = com.ibm.streams.pricing.test::DynamicScaleOut
SPL_MAIN_COMPOSITE2 = sc -M com.ibm.streams.pricing::Pricer
SPL_CMD_ARGS ?=
SPLC_FLAGS = -a
OUTPUT_DIR = output
TO_BE_DELETED = 
ifeq ($(STREAMS_STUDIO_BUILDING), 1)
  # Building from Streams Studio, use env vars set by studio
  SPLC = $(STREAMS_STUDIO_SC_PATH)
  SPL_PATH = $(STREAMS_STUDIO_SPL_PATH)
  OUTPUT_DIR = $(STREAMS_STUDIO_OUTPUT_DIRECTORY)
else
  # Building outside of Streams Studio...
  ifeq ($(STREAMS_INSTALL),)
    ifeq ($(CP4D_URL),)
      $(error error: environment variable STREAMS_INSTALL has to be set, or set CP4D_URL environment variable for remote build with streamsx-sc)
    else
      SPLC = streamsx-sc --disable-ssl-verify
      SPLC_CLEAN = rm -f toolkit.xml
      SPL_OUTDIR1 = --output-directory=$(OUTPUT_DIR)
      SPL_OUTDIR2 = --output-directory=$(OUTPUT_DIR)
    endif
  else
    SPLC = $(STREAMS_INSTALL)/bin/sc
    SPLC_CLEAN = $(SPLC) -C -M $(SPL_MAIN_COMPOSITE1)
    SPL_OUTDIR1 = --output-directory=$(OUTPUT_DIR)/sample1
    SPL_OUTDIR2 = --output-directory=$(OUTPUT_DIR)/sample2
  endif  
  # additional toolkits
  SPL_PATH ?= 
  # delete directories and files generated and required by StreamsStudio
  TO_BE_DELETED += bin .toolkitList
endif
TO_BE_DELETED += $(OUTPUT_DIR)

ifneq ($(SPL_PATH),)
  SPLC_FLAGS += -t $(SPL_PATH)
endif

build: distributed 

all: clean build

distributed:
	$(SPLC) $(SPLC_FLAGS) -M $(SPL_MAIN_COMPOSITE1) $(SPL_CMD_ARGS) $(SPL_OUTDIR1)
	$(SPLC) $(SPLC_FLAGS) -M $(SPL_MAIN_COMPOSITE2) $(SPL_CMD_ARGS) $(SPL_OUTDIR2)

clean:
	$(SPLC_CLEAN)
	rm -rf $(TO_BE_DELETED)
