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
# (C) Copyright IBM Corp. 2010, 2018  All Rights reserved.         
#                                                                  
# end_generated_IBM_copyright_prolog                               
.PHONY: all clean 

SPLC = $(STREAMS_INSTALL)/bin/sc
SPLC_FLAGS = -a -t $(STREAMS_INSTALL)/toolkits/com.ibm.streamsx.inet
SAMPLE_TWITTERSTREAM = sample::TwitterStream
SAMPLE_SMACKDOWN = sample::Smackdown
SPL_DATA_DIR ?= /tmp
SPL_CMD_ARGS ?=

all: distributed

distributed: twitterstream smackdown

twitterstream:
	$(SPLC) $(SPLC_FLAGS) -M $(SAMPLE_TWITTERSTREAM) --data-directory $(SPL_DATA_DIR) $(SPL_CMD_ARGS)

smackdown:
	$(SPLC) $(SPLC_FLAGS) -M $(SAMPLE_SMACKDOWN) --data-directory $(SPL_DATA_DIR) $(SPL_CMD_ARGS)

clean:  twitterstream-clean smackdown-clean

twitterstream-clean:
	$(SPLC) $(SPLC_FLAGS) -C -M $(SAMPLE_TWITTERSTREAM)

smackdown-clean:
	$(SPLC) $(SPLC_FLAGS) -C -M $(SAMPLE_SMACKDOWN)

