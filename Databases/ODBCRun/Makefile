# begin_generated_IBM_copyright_prolog                             
#                                                                  
# This is an automatically generated copyright prolog.             
# After initializing,  DO NOT MODIFY OR MOVE                       
# **************************************************************** 
# THIS SAMPLE CODE IS PROVIDED ON AN "AS IS" BASIS. IBM MAKES NO   
# REPRESENTATIONS OR WARRANTIES, EXPRESS OR IMPLIED, CONCERNING    
# USE OF THE SAMPLE CODE, OR THE COMPLETENESS OR ACCURACY OF THE   
# SAMPLE CODE. IBM DOES NOT WARRANT UNINTERRUPTED OR ERROR-FREE    
# OPERATION OF THIS SAMPLE CODE. IBM IS NOT RESPONSIBLE FOR THE    
# RESULTS OBTAINED FROM THE USE OF THE SAMPLE CODE OR ANY PORTION  
# OF THIS SAMPLE CODE.                                             
#                                                                  
# LIMITATION OF LIABILITY. IN NO EVENT WILL IBM BE LIABLE TO ANY   
# PARTY FOR ANY DIRECT, INDIRECT, SPECIAL OR OTHER CONSEQUENTIAL   
# DAMAGES FOR ANY USE OF THIS SAMPLE CODE, THE USE OF CODE FROM    
# THIS [ SAMPLE PACKAGE,] INCLUDING, WITHOUT LIMITATION, ANY LOST  
# PROFITS, BUSINESS INTERRUPTION, LOSS OF PROGRAMS OR OTHER DATA   
# ON YOUR INFORMATION HANDLING SYSTEM OR OTHERWISE.                
#                                                                  
# (C) Copyright IBM Corp. 2012, 2017  All Rights reserved.         
#                                                                  
# end_generated_IBM_copyright_prolog                                
.PHONY: all standalone distributed clean 

# Uncomment the variable pertaining to the DB you are using
#export STREAMS_ADAPTERS_ODBC_DB2=1
#export STREAMS_ADAPTERS_ODBC_IDS=1
#export STREAMS_ADAPTERS_ODBC_ORACLE=1
#export STREAMS_ADAPTERS_ODBC_SQLSERVER=1
#export STREAMS_ADAPTERS_ODBC_MYSQL=1
#export STREAMS_ADAPTERS_ODBC_UNIX_OTHER=1

# Uncomment and set these variables to the C++ include and library
# file directories pertaining to the ODBC client you are using
#export STREAMS_ADAPTERS_ODBC_INCPATH=
#export STREAMS_ADAPTERS_ODBC_LIBPATH=

SPLC_FLAGS ?= --data-directory=$(DATA_DIR) -a -t $(STREAMS_INSTALL)/toolkits/com.ibm.streams.db
SPLC = $(STREAMS_INSTALL)/bin/sc
SPL_CMD_ARGS ?=
DATA_DIR ?= data

# SPLDOC
SPLDOCDIR?=doc/spldoc
SPLDOC := $(STREAMS_INSTALL)/bin/spl-make-doc


SPL_MAIN_COMPOSITE = application::Main

all: distributed


distributed:
	$(SPLC) $(SPLC_FLAGS) -M $(SPL_MAIN_COMPOSITE) $(SPL_CMD_ARGS)

clean: 
	$(SPLC) $(SPLC_FLAGS) -C -M $(SPL_MAIN_COMPOSITE)
	rm -rf $(DATA_DIR)/testdata_selected.csv $(DATA_DIR)/sql-errors.log output


build-spldoc:
	@ echo $@... 
	@echo "Generating SPL documentation:"
	$(SPLDOC) -i . --output-directory $(SPLDOCDIR) --include-all 
	
spldoc: clean-spldoc build-spldoc

clean-spldoc:
	@ echo $@... 
	@echo "Cleaning SPL documentation:"
	rm -fr $(SPLDOCDIR)
