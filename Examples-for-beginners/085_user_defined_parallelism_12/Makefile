.PHONY: build all clean distributed

SPL_MAIN_COMPOSITE = com.acme.test::UDP12
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
    endif
  else
    SPLC = $(STREAMS_INSTALL)/bin/sc
    SPLC_CLEAN = $(SPLC) -C -M $(SPL_MAIN_COMPOSITE)
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
	$(SPLC) $(SPLC_FLAGS) -M $(SPL_MAIN_COMPOSITE) $(SPL_CMD_ARGS) --output-directory=$(OUTPUT_DIR)

clean:
	$(SPLC_CLEAN)
	rm -rf $(TO_BE_DELETED)
