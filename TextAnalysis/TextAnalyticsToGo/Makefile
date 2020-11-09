.PHONY: build all clean distributed

SPL_MAIN_COMPOSITE1 = normalize.example::NormalizeMentions
SPL_MAIN_COMPOSITE2 = tokenize.example::TokenStream
SPL_MAIN_COMPOSITE3 = tokenize.example::TokenBag
SPL_CMD_ARGS ?=
SPLC_FLAGS = -a
OUTPUT_DIR = output
TO_BE_DELETED = 
ifeq ($(STREAMS_STUDIO_BUILDING), 1)
  # Building from Streams Studio, use env vars set by studio
  SPLC = $(STREAMS_STUDIO_SC_PATH)
  SPL_PATH = $(STREAMS_STUDIO_SPL_PATH)
  OUTPUT_DIR = $(STREAMS_STUDIO_OUTPUT_DIRECTORY)
  SPLC_OUT1 = $(OUTPUT_DIR)
  SPLC_OUT2 = $(OUTPUT_DIR)
  SPLC_OUT3 = $(OUTPUT_DIR) 
else ifdef STREAMS_BUILDSERVICE
    $(info Building using buildservice)
    ifeq ($(STREAMS_INSTALL),)
        $(error error: environment variable STREAMS_INSTALL has to be set)
    endif
    SPLC = $(STREAMS_INSTALL)/bin/sc
    SPL_PATH = ../toolkits:$(STREAMS_INSTALL)/toolkits
    SPLC_OUT1 = $(OUTPUT_DIR)
    SPLC_OUT2 = $(OUTPUT_DIR)
    SPLC_OUT3 = $(OUTPUT_DIR) 
else
  # Building outside of Streams Studio...
  ifeq ($(STREAMS_INSTALL),)
    ifeq ($(CP4D_URL),)
      $(error error: environment variable STREAMS_INSTALL has to be set, or set CP4D_URL environment variable for remote build with streamsx-sc)
    else
      SPLC = streamsx-sc --disable-ssl-verify
      SPLC_CLEAN = rm -f toolkit.xml
      SPLC_OUT1 = $(OUTPUT_DIR)
      SPLC_OUT2 = $(OUTPUT_DIR)
      SPLC_OUT3 = $(OUTPUT_DIR) 
    endif
  else
    SPLC = $(STREAMS_INSTALL)/bin/sc
    SPLC_CLEAN = $(SPLC) -C -M $(SPL_MAIN_COMPOSITE1)
    SPLC_OUT1 = $(OUTPUT_DIR)/normalize.example
    SPLC_OUT2 = $(OUTPUT_DIR)/tokenize.stream
    SPLC_OUT3 = $(OUTPUT_DIR)/tokenize.bag
    SPLC_DATA_DIR = --data-directory=data
    # additional toolkits
    SPL_PATH ?= $(STREAMS_INSTALL)/toolkits/com.ibm.streams.text
  endif  
  # delete directories and files generated and required by StreamsStudio
  TO_BE_DELETED += bin .toolkitList
endif
TO_BE_DELETED += $(OUTPUT_DIR)

ifneq ($(SPL_PATH),)
  SPLC_FLAGS += -t $(SPL_PATH)
endif

ifneq ($(SPLC_DATA_DIR),)
  SPLC_FLAGS += $(SPLC_DATA_DIR)
endif

build: distributed 

all: clean build

distributed:
	$(SPLC) $(SPLC_FLAGS) -M $(SPL_MAIN_COMPOSITE1) $(SPL_CMD_ARGS) --output-directory=$(SPLC_OUT1)
	$(SPLC) $(SPLC_FLAGS) -M $(SPL_MAIN_COMPOSITE2) $(SPL_CMD_ARGS) --output-directory=$(SPLC_OUT2)
	$(SPLC) $(SPLC_FLAGS) -M $(SPL_MAIN_COMPOSITE3) $(SPL_CMD_ARGS) --output-directory=$(SPLC_OUT3)
clean:
	$(SPLC_CLEAN)
	rm -rf $(TO_BE_DELETED)
