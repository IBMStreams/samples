# NOTE: This makefile is designed to be run from one level up in the directory
# hierarchy compared to where it resides. For instance, if this makefile is
# located at 'output/build/Makefile', then it can be launched as:
# 'make -f build/Makefile' from the 'output' directory.

SHELL=/bin/bash
ifeq ($(CXX),)
  CXX=g++
endif
PKGCONFIG := /home/bluemix/InfoSphere_Streams/4.0.1.0/bin/dst-pe-pkg-config.sh
SPL_BUNDLE_BUILDER := /home/bluemix/InfoSphere_Streams/4.0.1.0/system/impl/bin/spl-bundle-builder
PE_INCLUDE := $(shell $(PKGCONFIG) --cflags dst-spl-pe-install)
PE_LIB := $(shell $(PKGCONFIG) --libs dst-spl-pe-install)
PE_STAND_LIB := $(shell $(PKGCONFIG) --libs dst-spl-pe-install-standalone)
SPL_CXXFLAGS = -Wall -Wno-unused -Wno-sign-compare
SPL_CXXFLAGS += -D_REENTRANT -fPIC -finline-functions -fmessage-length=0
SPL_CXXFLAGS += $(PE_INCLUDE) -O3 -DNDEBUG  -std=gnu++98 
SPL_LDFLAGS = $(PE_LIB)

SPL_TYPES = build/type/BeJwrMfI0NExPzUstSixJTfFM8TQ0yC_1KTM_1MS8zxTAEAlnoKBa.o \
            build/type/BeJw1zEEKgCAQheEzTRHZARKEoE0XMGeyKVOwqfMnRbv3_1sUnykB9WHErYY_0eDGoVrLBcSLoLKfpvQoN8io2Oxjhl6_1YZWuRMTjjFcSnppmBUmreSCgJVyuyHX4L6vT_13AB0iCwO.o \
            build/type/BeJyrNIo38fN3cTVxdXF3BQAbDgOu.o \
            build/type/BeJwrMfG0yE_1KSk0u8Uxxs8hJLMksKU1JdbPMyc9LBzM9LUsyc1ODSxJzCwBs2xCo.o \
            build/type/BeJyrNI43dnRxMQty9fUPczV19nF1DAIAPG8FCo.o \
            build/type/BeJwlyVELgjAQAOC_11NJiPkq7xaAcSNSjnN0hytiknQ_1790W_0ffCJLseh7ryBGswV1GFbCYUfZeVSDVVrzLmHu3_1C6XKDtldqfxtwyk6nceG3OLI6oMyyEdsmpDj96ZTKgh_1p0s_1kGo60c9Rz9pFfWL5vcivj.o 
SPL_FUNCTIONS = 
SPL_OPERATORS = build/operator/WebRequest.o \
                build/operator/WebSink.o \
                build/operator/OSMPointMatcher_18/OSMXMLGeometrySource_1.o \
                build/operator/OSMPointMatcher_18/PointMapMatcher_2.o \
                build/operator/OSMPointMatcher_18/OSMCorrelator_3.o 
SPL_PE_OBJS = build/pe/BeJwzNAhPTQpKLSxNLS6JNweygzPzsuMBWOEIAR.o \
              build/pe/BeJwztPAP9g3Iz8wr8U0sSc5ILYo3tDAyAopF_0Pq4p_0bnppYUVQbnlxYlp8YbxgMAYZkPDf.o \
              build/pe/BeJwztPAP9g3Iz8wr8U0sSc5ILYo3tDA0hwoUwMSM4gEb5w4z.o \
              build/pe/BeJwztPAP9g3Iz8wr8U0sSc5ILYo3tDA0BYo55xcVpeYkluQXxRvHAwD8sg1S.o 
SPL_PE_BINS = bin/com.ibm.streams.mapmatch.Main-a.dpe \
              bin/com.ibm.streams.mapmatch.Main-b.dpe \
              bin/com.ibm.streams.mapmatch.Main-c.dpe \
              bin/com.ibm.streams.mapmatch.Main-d.dpe 

SPL_ADL_FILE = com.ibm.streams.mapmatch.Main.adl

SPL_OUTPUT_DIR_ROOT = $(shell printf '%q' '/home/bluemix/workspace/MapMatch/output/com.ibm.streams.mapmatch.Main')
SPL_OUTPUT_DIR = $(shell printf '%q' 'Distributed')

.PHONY: distclean clean all types functions operators pes svar bundle toolkits standalone sablink

all: bin types functions operators pes svar standalone bundle sablink

distclean clean:
	@rm -fr bin/* build/type build/function build/operator build/pe build/svar build/bundle build/standalone com.ibm.streams.mapmatch.Main.sab

bin:
	@mkdir -m 775 bin

types: $(SPL_TYPES)

build/type/BeJyrNIo38fN3cTVxdXF3BQAbDgOu.o: \
	    src/type/BeJyrNIo38fN3cTVxdXF3BQAbDgOu.h \
	    src/type/BeJyrNIo38fN3cTVxdXF3BQAbDgOu.cpp \
	    build/cppOptions \
	    | build/type
	@echo ' [CXX-type] enum{NODE,EDGE}'
	@$(CXX) -o $@ -c $(SPL_CXXFLAGS) src/type/BeJyrNIo38fN3cTVxdXF3BQAbDgOu.cpp

build/type/BeJyrNI43dnRxMQty9fUPczV19nF1DAIAPG8FCo.o: \
	    src/type/BeJyrNI43dnRxMQty9fUPczV19nF1DAIAPG8FCo.h \
	    src/type/BeJyrNI43dnRxMQty9fUPczV19nF1DAIAPG8FCo.cpp \
	    build/cppOptions \
	    | build/type
	@echo ' [CXX-type] enum{ADD,REMOVE,CLEAR}'
	@$(CXX) -o $@ -c $(SPL_CXXFLAGS) src/type/BeJyrNI43dnRxMQty9fUPczV19nF1DAIAPG8FCo.cpp

build/type/BeJwrMfI0NExPzUstSixJTfFM8TQ0yC_1KTM_1MS8zxTAEAlnoKBa.o: \
	    src/type/BeJwrMfI0NExPzUstSixJTfFM8TQ0yC_1KTM_1MS8zxTAEAlnoKBa.h \
	    src/type/BeJwrMfI0NExPzUstSixJTfFM8TQ0yC_1KTM_1MS8zxTAEAlnoKBa.cpp \
	    build/cppOptions \
	    | build/type
	@echo ' [CXX-type] tuple<int64 generatedId,int64 originalId>'
	@$(CXX) -o $@ -c $(SPL_CXXFLAGS) src/type/BeJwrMfI0NExPzUstSixJTfFM8TQ0yC_1KTM_1MS8zxTAEAlnoKBa.cpp

build/type/BeJw1zEEKgCAQheEzTRHZARKEoE0XMGeyKVOwqfMnRbv3_1sUnykB9WHErYY_0eDGoVrLBcSLoLKfpvQoN8io2Oxjhl6_1YZWuRMTjjFcSnppmBUmreSCgJVyuyHX4L6vT_13AB0iCwO.o: \
	    src/type/BeJw1zEEKgCAQheEzTRHZARKEoE0XMGeyKVOwqfMnRbv3_1sUnykB9WHErYY_0eDGoVrLBcSLoLKfpvQoN8io2Oxjhl6_1YZWuRMTjjFcSnppmBUmreSCgJVyuyHX4L6vT_13AB0iCwO.h \
	    src/type/BeJw1zEEKgCAQheEzTRHZARKEoE0XMGeyKVOwqfMnRbv3_1sUnykB9WHErYY_0eDGoVrLBcSLoLKfpvQoN8io2Oxjhl6_1YZWuRMTjjFcSnppmBUmreSCgJVyuyHX4L6vT_13AB0iCwO.cpp \
	    build/cppOptions \
	    | build/type
	@echo ' [CXX-type] tuple<int64 matchedEdgeId,float64 latitude,float64 longit...oat64 origLongitude>'
	@$(CXX) -o $@ -c $(SPL_CXXFLAGS) src/type/BeJw1zEEKgCAQheEzTRHZARKEoE0XMGeyKVOwqfMnRbv3_1sUnykB9WHErYY_0eDGoVrLBcSLoLKfpvQoN8io2Oxjhl6_1YZWuRMTjjFcSnppmBUmreSCgJVyuyHX4L6vT_13AB0iCwO.cpp

build/type/BeJwrMfG0yE_1KSk0u8Uxxs8hJLMksKU1JdbPMyc9LBzM9LUsyc1ODSxJzCwBs2xCo.o: \
	    src/type/BeJwrMfG0yE_1KSk0u8Uxxs8hJLMksKU1JdbPMyc9LBzM9LUsyc1ODSxJzCwBs2xCo.h \
	    src/type/BeJwrMfG0yE_1KSk0u8Uxxs8hJLMksKU1JdbPMyc9LBzM9LUsyc1ODSxJzCwBs2xCo.cpp \
	    build/cppOptions \
	    | build/type
	@echo ' [CXX-type] tuple<int64 objectId,float64 latitude,float64 longitude,int64 timeStamp>'
	@$(CXX) -o $@ -c $(SPL_CXXFLAGS) src/type/BeJwrMfG0yE_1KSk0u8Uxxs8hJLMksKU1JdbPMyc9LBzM9LUsyc1ODSxJzCwBs2xCo.cpp

build/type/BeJwlyVELgjAQAOC_11NJiPkq7xaAcSNSjnN0hytiknQ_1790W_0ffCJLseh7ryBGswV1GFbCYUfZeVSDVVrzLmHu3_1C6XKDtldqfxtwyk6nceG3OLI6oMyyEdsmpDj96ZTKgh_1p0s_1kGo60c9Rz9pFfWL5vcivj.o: \
	    src/type/BeJwlyVELgjAQAOC_11NJiPkq7xaAcSNSjnN0hytiknQ_1790W_0ffCJLseh7ryBGswV1GFbCYUfZeVSDVVrzLmHu3_1C6XKDtldqfxtwyk6nceG3OLI6oMyyEdsmpDj96ZTKgh_1p0s_1kGo60c9Rz9pFfWL5vcivj.h \
	    src/type/BeJwlyVELgjAQAOC_11NJiPkq7xaAcSNSjnN0hytiknQ_1790W_0ffCJLseh7ryBGswV1GFbCYUfZeVSDVVrzLmHu3_1C6XKDtldqfxtwyk6nceG3OLI6oMyyEdsmpDj96ZTKgh_1p0s_1kGo60c9Rz9pFfWL5vcivj.cpp \
	    build/cppOptions \
	    | build/type
	@echo ' [CXX-type] tuple<enum{NODE,EDGE} updateType,enum{ADD,REMOVE,CLEAR} u...Id,boolean isOneWay>'
	@$(CXX) -o $@ -c $(SPL_CXXFLAGS) src/type/BeJwlyVELgjAQAOC_11NJiPkq7xaAcSNSjnN0hytiknQ_1790W_0ffCJLseh7ryBGswV1GFbCYUfZeVSDVVrzLmHu3_1C6XKDtldqfxtwyk6nceG3OLI6oMyyEdsmpDj96ZTKgh_1p0s_1kGo60c9Rz9pFfWL5vcivj.cpp

build/type:
	-@mkdir -m 775 build/type

operators: $(SPL_OPERATORS)

-include build/operator/WebRequest.d
build/operator/WebRequest.o: \
	    src/operator/WebRequest.h \
	    src/operator/WebRequest.cpp\
	    build/dep/operator/WebRequest.dep  \
	    build/cppOptions \
	    | build/operator
	@echo ' [CXX-operator] WebRequest'
	@$(CXX) -o $@ -MD -I /home/bluemix/com.ibm.streamsx.inet/impl/nl/include -I /home/bluemix/InfoSphere_Streams/4.0.1.0/toolkits/spl/spl/utility/JavaOp/../../../impl/include -I /home/bluemix/InfoSphere_Streams/4.0.1.0/java/include -DSPLJAVAFUNCTION_ADL_FILENAME=\"com.ibm.streams.mapmatch.Main.adl\" -c $(SPL_CXXFLAGS) src/operator/WebRequest.cpp

-include build/operator/WebSink.d
build/operator/WebSink.o: \
	    src/operator/WebSink.h \
	    src/operator/WebSink.cpp\
	    build/dep/operator/WebSink.dep  \
	    build/cppOptions \
	    | build/operator
	@echo ' [CXX-operator] WebSink'
	@$(CXX) -o $@ -MD -I /home/bluemix/com.ibm.streamsx.inet/impl/nl/include -I /home/bluemix/InfoSphere_Streams/4.0.1.0/toolkits/spl/spl/utility/JavaOp/../../../impl/include -I /home/bluemix/InfoSphere_Streams/4.0.1.0/java/include -DSPLJAVAFUNCTION_ADL_FILENAME=\"com.ibm.streams.mapmatch.Main.adl\" -c $(SPL_CXXFLAGS) src/operator/WebSink.cpp

-include build/operator/OSMPointMatcher_18/OSMXMLGeometrySource_1.d
build/operator/OSMPointMatcher_18/OSMXMLGeometrySource_1.o: \
	    src/operator/OSMPointMatcher_18/OSMXMLGeometrySource_1.h \
	    src/operator/OSMPointMatcher_18/OSMXMLGeometrySource_1.cpp\
	    build/dep/operator/OSMPointMatcher_18/OSMXMLGeometrySource_1.dep  \
	    build/cppOptions \
	    | build/operator/OSMPointMatcher_18
	@echo ' [CXX-operator] OSMPointMatcher_18.OSMXMLGeometrySource_1'
	@$(CXX) -o $@ -MD -I /home/bluemix/InfoSphere_Streams/4.0.1.0/toolkits/com.ibm.streams.geospatial/impl/nl/include -I /home/bluemix/InfoSphere_Streams/4.0.1.0/toolkits/spl/spl/utility/JavaOp/../../../impl/include -I /home/bluemix/InfoSphere_Streams/4.0.1.0/java/include -DSPLJAVAFUNCTION_ADL_FILENAME=\"com.ibm.streams.mapmatch.Main.adl\" -c $(SPL_CXXFLAGS) src/operator/OSMPointMatcher_18/OSMXMLGeometrySource_1.cpp

-include build/operator/OSMPointMatcher_18/PointMapMatcher_2.d
build/operator/OSMPointMatcher_18/PointMapMatcher_2.o: \
	    src/operator/OSMPointMatcher_18/PointMapMatcher_2.h \
	    src/operator/OSMPointMatcher_18/PointMapMatcher_2.cpp\
	    build/dep/operator/OSMPointMatcher_18/PointMapMatcher_2.dep  \
	    build/cppOptions \
	    | build/operator/OSMPointMatcher_18
	@echo ' [CXX-operator] OSMPointMatcher_18.PointMapMatcher_2'
	@$(CXX) -o $@ -MD -I /home/bluemix/InfoSphere_Streams/4.0.1.0/toolkits/com.ibm.streams.geospatial/impl/nl/include -I /home/bluemix/InfoSphere_Streams/4.0.1.0/toolkits/com.ibm.streams.geospatial/impl/include/proj4 -I /home/bluemix/InfoSphere_Streams/4.0.1.0/toolkits/com.ibm.streams.geospatial/impl/include/st -DSPLJAVAFUNCTION_ADL_FILENAME=\"com.ibm.streams.mapmatch.Main.adl\" -c $(SPL_CXXFLAGS) src/operator/OSMPointMatcher_18/PointMapMatcher_2.cpp

-include build/operator/OSMPointMatcher_18/OSMCorrelator_3.d
build/operator/OSMPointMatcher_18/OSMCorrelator_3.o: \
	    src/operator/OSMPointMatcher_18/OSMCorrelator_3.h \
	    src/operator/OSMPointMatcher_18/OSMCorrelator_3.cpp\
	    build/dep/operator/OSMPointMatcher_18/OSMCorrelator_3.dep  \
	    build/cppOptions \
	    | build/operator/OSMPointMatcher_18
	@echo ' [CXX-operator] OSMPointMatcher_18.OSMCorrelator_3'
	@$(CXX) -o $@ -MD -I /home/bluemix/InfoSphere_Streams/4.0.1.0/toolkits/com.ibm.streams.geospatial/impl/nl/include -I /home/bluemix/InfoSphere_Streams/4.0.1.0/toolkits/spl/spl/utility/JavaOp/../../../impl/include -I /home/bluemix/InfoSphere_Streams/4.0.1.0/java/include -DSPLJAVAFUNCTION_ADL_FILENAME=\"com.ibm.streams.mapmatch.Main.adl\" -c $(SPL_CXXFLAGS) src/operator/OSMPointMatcher_18/OSMCorrelator_3.cpp

build/operator:
	-@mkdir -m 775 -p build/operator

build/operator/OSMPointMatcher_18:
	-@mkdir -m 775 -p build/operator/OSMPointMatcher_18

functions: $(SPL_FUNCTIONS)

pes: $(SPL_PE_BINS)

build/pe/BeJwzNAhPTQpKLSxNLS6JNweygzPzsuMBWOEIAR.o: \
	    src/pe/BeJwzNAhPTQpKLSxNLS6JNweygzPzsuMBWOEIAR.h \
	    src/pe/BeJwzNAhPTQpKLSxNLS6JNweygzPzsuMBWOEIAR.cpp  \
	    build/cppOptions \
	    | build/pe
	@echo ' [CXX-pe] pe com.ibm.streams.mapmatch.Main-a'
	@$(CXX) -o $@ -c $(SPL_CXXFLAGS) src/pe/BeJwzNAhPTQpKLSxNLS6JNweygzPzsuMBWOEIAR.cpp

PEcom.ibm.streams.mapmatch.Main-a = \
	    build/pe/BeJwzNAhPTQpKLSxNLS6JNweygzPzsuMBWOEIAR.o \
	    build/operator/WebRequest.o \
	    build/type/BeJwrMfG0yE_1KSk0u8Uxxs8hJLMksKU1JdbPMyc9LBzM9LUsyc1ODSxJzCwBs2xCo.o \
	    build/operator/WebSink.o \
	    build/type/BeJw1zEEKgCAQheEzTRHZARKEoE0XMGeyKVOwqfMnRbv3_1sUnykB9WHErYY_0eDGoVrLBcSLoLKfpvQoN8io2Oxjhl6_1YZWuRMTjjFcSnppmBUmreSCgJVyuyHX4L6vT_13AB0iCwO.o 

bin/com.ibm.streams.mapmatch.Main-a.dpe: \
	$(PEcom.ibm.streams.mapmatch.Main-a) \
	build/dep/com.ibm.streams.mapmatch.Main-a.dep \
	build/ldOptions
	@echo ' [LD-pe] pe com.ibm.streams.mapmatch.Main-a'
	@$(CXX) $(SPL_CXXFLAGS) -o bin/com.ibm.streams.mapmatch.Main-a.dpe -fPIC -shared $(PEcom.ibm.streams.mapmatch.Main-a) -lstreams-stdtk-javaop -L /home/bluemix/InfoSphere_Streams/4.0.1.0/java/jre/lib/amd64/j9vm -Wl,-rpath -Wl,/home/bluemix/InfoSphere_Streams/4.0.1.0/java/jre/lib/amd64/j9vm -lpthread -ldl -lstreams-stdtk-runtime -lstreams-stdtk-javaop -lpthread -ldl -lstreams-stdtk-runtime $(SPL_LDFLAGS)

build/pe/BeJwztPAP9g3Iz8wr8U0sSc5ILYo3tDAyAopF_0Pq4p_0bnppYUVQbnlxYlp8YbxgMAYZkPDf.o: \
	    src/pe/BeJwztPAP9g3Iz8wr8U0sSc5ILYo3tDAyAopF_0Pq4p_0bnppYUVQbnlxYlp8YbxgMAYZkPDf.h \
	    src/pe/BeJwztPAP9g3Iz8wr8U0sSc5ILYo3tDAyAopF_0Pq4p_0bnppYUVQbnlxYlp8YbxgMAYZkPDf.cpp  \
	    build/cppOptions \
	    | build/pe
	@echo ' [CXX-pe] pe com.ibm.streams.mapmatch.Main-b'
	@$(CXX) -o $@ -c $(SPL_CXXFLAGS) src/pe/BeJwztPAP9g3Iz8wr8U0sSc5ILYo3tDAyAopF_0Pq4p_0bnppYUVQbnlxYlp8YbxgMAYZkPDf.cpp

PEcom.ibm.streams.mapmatch.Main-b = \
	    build/pe/BeJwztPAP9g3Iz8wr8U0sSc5ILYo3tDAyAopF_0Pq4p_0bnppYUVQbnlxYlp8YbxgMAYZkPDf.o \
	    build/operator/OSMPointMatcher_18/OSMXMLGeometrySource_1.o \
	    build/type/BeJyrNI43dnRxMQty9fUPczV19nF1DAIAPG8FCo.o \
	    build/type/BeJyrNIo38fN3cTVxdXF3BQAbDgOu.o \
	    build/type/BeJwrMfI0NExPzUstSixJTfFM8TQ0yC_1KTM_1MS8zxTAEAlnoKBa.o \
	    build/type/BeJwlyVELgjAQAOC_11NJiPkq7xaAcSNSjnN0hytiknQ_1790W_0ffCJLseh7ryBGswV1GFbCYUfZeVSDVVrzLmHu3_1C6XKDtldqfxtwyk6nceG3OLI6oMyyEdsmpDj96ZTKgh_1p0s_1kGo60c9Rz9pFfWL5vcivj.o 

bin/com.ibm.streams.mapmatch.Main-b.dpe: \
	$(PEcom.ibm.streams.mapmatch.Main-b) \
	build/dep/com.ibm.streams.mapmatch.Main-b.dep \
	build/ldOptions
	@echo ' [LD-pe] pe com.ibm.streams.mapmatch.Main-b'
	@$(CXX) $(SPL_CXXFLAGS) -o bin/com.ibm.streams.mapmatch.Main-b.dpe -fPIC -shared $(PEcom.ibm.streams.mapmatch.Main-b) -lstreams-stdtk-javaop -L /home/bluemix/InfoSphere_Streams/4.0.1.0/java/jre/lib/amd64/j9vm -Wl,-rpath -Wl,/home/bluemix/InfoSphere_Streams/4.0.1.0/java/jre/lib/amd64/j9vm -lpthread -ldl -lstreams-stdtk-runtime $(SPL_LDFLAGS)

build/pe/BeJwztPAP9g3Iz8wr8U0sSc5ILYo3tDA0hwoUwMSM4gEb5w4z.o: \
	    src/pe/BeJwztPAP9g3Iz8wr8U0sSc5ILYo3tDA0hwoUwMSM4gEb5w4z.h \
	    src/pe/BeJwztPAP9g3Iz8wr8U0sSc5ILYo3tDA0hwoUwMSM4gEb5w4z.cpp  \
	    build/cppOptions \
	    | build/pe
	@echo ' [CXX-pe] pe com.ibm.streams.mapmatch.Main-c'
	@$(CXX) -o $@ -c $(SPL_CXXFLAGS) src/pe/BeJwztPAP9g3Iz8wr8U0sSc5ILYo3tDA0hwoUwMSM4gEb5w4z.cpp

PEcom.ibm.streams.mapmatch.Main-c = \
	    build/pe/BeJwztPAP9g3Iz8wr8U0sSc5ILYo3tDA0hwoUwMSM4gEb5w4z.o \
	    build/operator/OSMPointMatcher_18/PointMapMatcher_2.o \
	    build/type/BeJyrNI43dnRxMQty9fUPczV19nF1DAIAPG8FCo.o \
	    build/type/BeJwrMfG0yE_1KSk0u8Uxxs8hJLMksKU1JdbPMyc9LBzM9LUsyc1ODSxJzCwBs2xCo.o \
	    build/type/BeJw1zEEKgCAQheEzTRHZARKEoE0XMGeyKVOwqfMnRbv3_1sUnykB9WHErYY_0eDGoVrLBcSLoLKfpvQoN8io2Oxjhl6_1YZWuRMTjjFcSnppmBUmreSCgJVyuyHX4L6vT_13AB0iCwO.o \
	    build/type/BeJyrNIo38fN3cTVxdXF3BQAbDgOu.o \
	    build/type/BeJwlyVELgjAQAOC_11NJiPkq7xaAcSNSjnN0hytiknQ_1790W_0ffCJLseh7ryBGswV1GFbCYUfZeVSDVVrzLmHu3_1C6XKDtldqfxtwyk6nceG3OLI6oMyyEdsmpDj96ZTKgh_1p0s_1kGo60c9Rz9pFfWL5vcivj.o 

bin/com.ibm.streams.mapmatch.Main-c.dpe: \
	$(PEcom.ibm.streams.mapmatch.Main-c) \
	build/dep/com.ibm.streams.mapmatch.Main-c.dep \
	build/ldOptions
	@echo ' [LD-pe] pe com.ibm.streams.mapmatch.Main-c'
	@$(CXX) $(SPL_CXXFLAGS) -o bin/com.ibm.streams.mapmatch.Main-c.dpe -fPIC -shared $(PEcom.ibm.streams.mapmatch.Main-c) -L /home/bluemix/InfoSphere_Streams/4.0.1.0/toolkits/com.ibm.streams.geospatial/impl/lib/external/proj4 -Wl,-rpath -Wl,\$$ORIGIN/../toolkits/com.ibm.streams.geospatial/impl/lib/external/proj4 -lproj -L /home/bluemix/InfoSphere_Streams/4.0.1.0/toolkits/com.ibm.streams.geospatial/impl/lib -Wl,-rpath -Wl,\$$ORIGIN/../toolkits/com.ibm.streams.geospatial/impl/lib -lst $(SPL_LDFLAGS)

build/pe/BeJwztPAP9g3Iz8wr8U0sSc5ILYo3tDA0BYo55xcVpeYkluQXxRvHAwD8sg1S.o: \
	    src/pe/BeJwztPAP9g3Iz8wr8U0sSc5ILYo3tDA0BYo55xcVpeYkluQXxRvHAwD8sg1S.h \
	    src/pe/BeJwztPAP9g3Iz8wr8U0sSc5ILYo3tDA0BYo55xcVpeYkluQXxRvHAwD8sg1S.cpp  \
	    build/cppOptions \
	    | build/pe
	@echo ' [CXX-pe] pe com.ibm.streams.mapmatch.Main-d'
	@$(CXX) -o $@ -c $(SPL_CXXFLAGS) src/pe/BeJwztPAP9g3Iz8wr8U0sSc5ILYo3tDA0BYo55xcVpeYkluQXxRvHAwD8sg1S.cpp

PEcom.ibm.streams.mapmatch.Main-d = \
	    build/pe/BeJwztPAP9g3Iz8wr8U0sSc5ILYo3tDA0BYo55xcVpeYkluQXxRvHAwD8sg1S.o \
	    build/operator/OSMPointMatcher_18/OSMCorrelator_3.o \
	    build/type/BeJw1zEEKgCAQheEzTRHZARKEoE0XMGeyKVOwqfMnRbv3_1sUnykB9WHErYY_0eDGoVrLBcSLoLKfpvQoN8io2Oxjhl6_1YZWuRMTjjFcSnppmBUmreSCgJVyuyHX4L6vT_13AB0iCwO.o \
	    build/type/BeJwrMfI0NExPzUstSixJTfFM8TQ0yC_1KTM_1MS8zxTAEAlnoKBa.o 

bin/com.ibm.streams.mapmatch.Main-d.dpe: \
	$(PEcom.ibm.streams.mapmatch.Main-d) \
	build/dep/com.ibm.streams.mapmatch.Main-d.dep \
	build/ldOptions
	@echo ' [LD-pe] pe com.ibm.streams.mapmatch.Main-d'
	@$(CXX) $(SPL_CXXFLAGS) -o bin/com.ibm.streams.mapmatch.Main-d.dpe -fPIC -shared $(PEcom.ibm.streams.mapmatch.Main-d) -lstreams-stdtk-javaop -L /home/bluemix/InfoSphere_Streams/4.0.1.0/java/jre/lib/amd64/j9vm -Wl,-rpath -Wl,/home/bluemix/InfoSphere_Streams/4.0.1.0/java/jre/lib/amd64/j9vm -lpthread -ldl -lstreams-stdtk-runtime $(SPL_LDFLAGS)

build/pe:
	-@mkdir -m 775 build/pe

standalone: 
	@rm -f bin/standalone.exe
	@rm -f bin/com.ibm.streams.mapmatch.Main
	@rm -f bin/sab
	@rm -f bin/standalone

sablink:

	@rm -f bin/sab

bundle: com.ibm.streams.mapmatch.Main.sab

-include /home/bluemix/workspace/MapMatch/output/com.ibm.streams.mapmatch.Main/Distributed/build/dep/com.ibm.streams.mapmatch.Main.sab.d
com.ibm.streams.mapmatch.Main.sab: $(SPL_PE_BINS) $(SPL_ADL_FILE)
	@echo ' [Bundle] com.ibm.streams.mapmatch.Main.sab'
	@$(SPL_BUNDLE_BUILDER) $(SPL_OUTPUT_DIR_ROOT) $(SPL_OUTPUT_DIR) $(SPL_ADL_FILE) com.ibm.streams.mapmatch.Main.sab false

