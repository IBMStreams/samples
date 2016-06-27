// eJzVlFtzmkAYhmf_1ScfpRTLNcIipUe9QwKqABwhJc_0MssOAq7JJlUbDT_195VO02iZqa97HC4ePc7Px_0ENJNwkEkFZwhmhZQgWuSQY5h2uxPXfrKtAaIZ4qx2aclCBFSgiOsONCRJFjevcyT30Gib1r5hJSttpk36C1V1Rni6frmvoBa6jrsijrKsOV6T2UK97yiPCyWO_0yOrQMt7VvcGxdZXB2bQ_1_14QPyPf1X2f7azsW9lcqP3W01jnafQSV3xbr1skRIPmxBq2qF3XRlRkub7qtJ69cbJcqLlSLNT1gLaUsDPfdXIzfrS_0bkK8WUnLxlnJzI6HimNU091DwV1cjbzYtNveTKlFB2PPXqi2295VnmZoKaHjHrwQpGbO8K4ZkXllz3jdiR_0m4c5XO8RUdW2oTQdts08_1cKPNduw0Q8_0voiez2ZtpgZ5Myr2xApogxfyzAtyp1e0yAQeTBBw09YJ2e6IBeSmYyUFaogxX8paytYAaItmGuQ15uJQjyCEglKA9yjCFReHAbA_13XaD3_0a8a4YfLIl3alcY10LhwDUqOTIbQMZGFAwZZfZ7rKkHcozRdY65jhkJOWX31twU0rq8_1ffn03lrGWZ7KKQ5k92jkiPcGmSUJOaakkFaQCb_0TIjOYxzg9m4V6ub4Dkn3qA6_1TWJtMY8l5pCOzviiBQ8JF67fioTllf5D_1B18UyAADbVDmYpeQJ_1YaIFJmP5yJbtwY_0sD4KXo6HpopTIrjqabrN3PDnvjGTd8ytPneiAYrMcxhBDDhrTshpAITLyME4pTC3xIlyakmhse4QyP0xhWR6ETBxYSgR1iDQKBDkABFEQSOw1Zfh_13P_14JD_07cgQQQx0WP0JiVlOMEEpq_0S8gtmecHX


#include "./OSMXMLGeometrySource_1.h"
using namespace SPL::_Operator::OSMPointMatcher_18;

#include <SPL/Runtime/Function/SPLFunctions.h>
#include <SPL/Runtime/Operator/Port/Punctuation.h>


#define MY_OPERATOR_SCOPE SPL::_Operator::OSMPointMatcher_18
#define MY_BASE_OPERATOR OSMXMLGeometrySource_1_Base
#define MY_OPERATOR OSMXMLGeometrySource_1$OP





#include <dlfcn.h>

#include <SPL/Toolkit/JavaOp.h>
#include <SPL/Toolkit/RuntimeException.h>

MY_OPERATOR_SCOPE::MY_OPERATOR::MY_OPERATOR()
{
  /* Get a handle to the Java virtual machine */
  SPL::JNI::JVMControl *jvmControl = SPL::JNI::JVMControl::getJVM();
   
  /* Attach to the JVM  for the duration of the initialization */
  JNIEnv * env = jvmControl->attach();

  /* How we invoke methods against an OperatorSetup instance */
  SPL::JNI::OpSetupInvoker* osi = jvmControl->getOpSetupInvoker();

  /* OperatorSetup instance specific to this operator */
  jobject setup = osi->newSetup(env, this); 

  /**
     Pass the parameters into my OperatorSetup instance.
  */
     osi->setParameter(env, setup, "className", SPL::rstring("com.ibm.streams.geospatial.OSMXMLGeometrySource"));
   osi->setParameter(env, setup, "classLibrary", (getToolkitDirectory(SPL::rstring("com.ibm.streams.geospatial")) + SPL::rstring("/impl/lib/SpatialNativeFunctions.jar")));
   osi->setParameter(env, setup, "mapfile", (getToolkitDirectory(lit$1) + lit$0));
   osi->setParameter(env, setup, "vmArg", lit$2);


  /**
    Pass input port information into the Java operator.
 
    Are logic clauses present.
   
    Pass the windowing information for each port as
    a list of values for the parameter '[window].N' where
    N is the index of the windowed input port.
  */
   hasTupleLogic = false;


  
  /* At this point all the initialization information has been
     passed to OperatorSetup. Create a JNIBridge instance object
     we use to communicate with the user's Operator implementation
     at runtime.
  */
  
  _bi = jvmControl->getBridgeInvoker();
  _bridge = _bi->newBridge(env, this, setup);
        
  /* Completed Java initialization, detach from the JVM */
  jvmControl->detach();

  setupStateHandler();

  void * handle = dlopen("libstreams-stdtk-javaop.so", RTLD_LAZY);
  if (!handle) {
    const FormattableMessage& m = SPL_APPLICATION_RUNTIME_EXCEPTION("libstreams-stdtk-javaop.so");
    THROW_STRING(SPLRuntimeJavaOperator, m);
  }
  _fp = (FP) dlsym(handle, "callProcessTupleWithNativeByteBuffer");
}

MY_OPERATOR_SCOPE::MY_OPERATOR::~MY_OPERATOR() 
{
}

void MY_OPERATOR_SCOPE::MY_OPERATOR::setupStateHandler()
{
    _stateHandler.reset(new SPL::JNI::JavaOpStateHandler(_bi, _bridge));
    getContext().registerStateHandler(*_stateHandler);
}

void MY_OPERATOR_SCOPE::MY_OPERATOR::allPortsReady() 
{
    _bi->allPortsReady(_bridge);
    createThreads(1);
}
 
void MY_OPERATOR_SCOPE::MY_OPERATOR::prepareToShutdown() 
{
   _bi->shutdown(_bridge);
}

void MY_OPERATOR_SCOPE::MY_OPERATOR::process(uint32_t idx)
{
   _bi->complete(_bridge);
}

void MY_OPERATOR_SCOPE::MY_OPERATOR::process(Tuple & tuple, uint32_t port)
{
}

void MY_OPERATOR_SCOPE::MY_OPERATOR::process(Tuple const & tuple, uint32_t port)
{
   _bi->processTuple(_bridge, tuple, port);
}

void MY_OPERATOR_SCOPE::MY_OPERATOR::process(Punctuation const & punct, uint32_t port)
{
   _bi->processPunctuation(_bridge, punct, port);
}

static SPL::Operator * initer() { return new MY_OPERATOR_SCOPE::MY_OPERATOR(); }
bool MY_BASE_OPERATOR::globalInit_ = MY_BASE_OPERATOR::globalIniter();
bool MY_BASE_OPERATOR::globalIniter() {
    instantiators_.insert(std::make_pair("OSMPointMatcher_18::OSMXMLGeometrySource_1",&initer));
    return true;
}

template<class T> static void initRTC (SPL::Operator& o, T& v, const char * n) {
    SPL::ValueHandle vh = v;
    o.getContext().getRuntimeConstantValue(vh, n);
}

MY_BASE_OPERATOR::MY_BASE_OPERATOR()
 : Operator() {
    PE & pe = PE::instance();
    uint32_t index = getIndex();
    initRTC(*this, lit$0, "lit$0");
    initRTC(*this, lit$1, "lit$1");
    initRTC(*this, lit$2, "lit$2");
    param$className$0 = SPL::rstring("com.ibm.streams.geospatial.OSMXMLGeometrySource");
    addParameterValue ("className", SPL::ConstValueHandle(param$className$0));
    param$classLibrary$0 = (getToolkitDirectory(SPL::rstring("com.ibm.streams.geospatial")) + SPL::rstring("/impl/lib/SpatialNativeFunctions.jar"));
    addParameterValue ("classLibrary", SPL::ConstValueHandle(param$classLibrary$0));
    param$mapfile$0 = (getToolkitDirectory(lit$1) + lit$0);
    addParameterValue ("mapfile", SPL::ConstValueHandle(param$mapfile$0));
    addParameterValue ("vmArg", SPL::ConstValueHandle(lit$2));
    (void) getParameters(); // ensure thread safety by initializing here
}
MY_BASE_OPERATOR::~MY_BASE_OPERATOR()
{
    for (ParameterMapType::const_iterator it = paramValues_.begin(); it != paramValues_.end(); it++) {
        const ParameterValueListType& pvl = it->second;
        for (ParameterValueListType::const_iterator it2 = pvl.begin(); it2 != pvl.end(); it2++) {
            delete *it2;
        }
    }
}



void MY_BASE_OPERATOR::checkpointStateVariables(NetworkByteBuffer & opstate) const {
}

void MY_BASE_OPERATOR::restoreStateVariables(NetworkByteBuffer & opstate) {
}

void MY_BASE_OPERATOR::checkpointStateVariables(Checkpoint & ckpt) {
}

void MY_BASE_OPERATOR::resetStateVariables(Checkpoint & ckpt) {
}

void MY_BASE_OPERATOR::resetStateVariablesToInitialState() {
}

bool MY_BASE_OPERATOR::hasStateVariables() const {
    return false;
}

void MY_BASE_OPERATOR::resetToInitialStateRaw() {
    AutoMutex $apm($svMutex);
    StateHandler *sh = getContext().getStateHandler();
    if (sh != NULL) {
        sh->resetToInitialState();
    }
    resetStateVariablesToInitialState();
}

void MY_BASE_OPERATOR::checkpointRaw(Checkpoint & ckpt) {
    AutoMutex $apm($svMutex);
    StateHandler *sh = getContext().getStateHandler();
    if (sh != NULL) {
        sh->checkpoint(ckpt);
    }
    checkpointStateVariables(ckpt);
}

void MY_BASE_OPERATOR::resetRaw(Checkpoint & ckpt) {
    AutoMutex $apm($svMutex);
    StateHandler *sh = getContext().getStateHandler();
    if (sh != NULL) {
        sh->reset(ckpt);
    }
    resetStateVariables(ckpt);
}




