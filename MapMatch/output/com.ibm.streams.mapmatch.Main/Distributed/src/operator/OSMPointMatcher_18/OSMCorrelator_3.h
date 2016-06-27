// eJzNlNtu4jAQhuU3qdBeFK2UOKWqKHcJDT2QQCEpPdwgJxmCwbGzjgmEp1_1T7rLltNq9qxLlYmzPP_1N_148QiM2iUGYWSQLLCSEEUOVGUsFarH_1htISUwooREFsL6uUA1wzD1q6ocTAceltbadbtp2x5MwV2Hw7s3e9h1hYtf_1FuouqP_08sfE58OobIyt4olXc_0f6_0c6Vr69jDDe3YiQ9Jw484XUneTkQvSYVF_13VbMquxtbr2_1Ni6IezWScOeJ5nzlMmIWinD6NqUd29XHpXZTi2GraDaXvZN6a1g9qkP7nHPXf1uH4qVEBXD_0Gk4zfDAa7aY6sb_0mPLD5rrVWi7NuOi65BNEqxbZVR9wyh49Fotqa2hPEUImVORgRmxBWR0ZS6FnGurYjB9kvtExVMzIYogLjhg1EAxI0XRIxnodDuJdvOe1_0KTCIwdArU6spU_0Ey0UdCTAh4JHI0lkdShynoIKhWBzqm6ohFhnqM7_1VblWr599P9vdbdIsZyajkRl8bOrpbwmdBY8VFbwwZkTXuF9kmdky3a_1ut8FtfUwRrvRYYURzIbeuf6XJQhTJ9_0FHqImyDWlI3CSF_0wRRrq4ukcZD1SIBNGGCvAcET3cjCd00GkOfh5LE80_1xDRptX3_0iF0pgKNLMgHAkople2WoISVNvX_0c9eKBl_1bLS_0mPlf1_0ETz1foBQ4SKIg2amGcsK2gc3fQXxZgJlu5jg6fAgPH8GHTwLEf0GI9yHi4xjxCZB4cy2snxAvNrd

#include <SPL/Toolkit/JavaOp.h>


#ifndef SPL_OPER_INSTANCE_OSMPOINTMATCHER_18_OSMCORRELATOR_3_H_
#define SPL_OPER_INSTANCE_OSMPOINTMATCHER_18_OSMCORRELATOR_3_H_

#include <SPL/Runtime/Operator/Operator.h>
#include <SPL/Runtime/Operator/ParameterValue.h>
#include <SPL/Runtime/Operator/OperatorContext.h>
#include <SPL/Runtime/Operator/Port/AutoPortMutex.h>
#include <SPL/Runtime/Operator/State/StateHandler.h>
#include <SPL/Runtime/ProcessingElement/PE.h>
#include <SPL/Runtime/Type/SPLType.h>
#include <SPL/Runtime/Utility/CV.h>
using namespace UTILS_NAMESPACE;

#include "../../type/BeJw1zEEKgCAQheEzTRHZARKEoE0XMGeyKVOwqfMnRbv3_1sUnykB9WHErYY_0eDGoVrLBcSLoLKfpvQoN8io2Oxjhl6_1YZWuRMTjjFcSnppmBUmreSCgJVyuyHX4L6vT_13AB0iCwO.h"
#include "../../type/BeJwrMfI0NExPzUstSixJTfFM8TQ0yC_1KTM_1MS8zxTAEAlnoKBa.h"

#include <bitset>

#define MY_OPERATOR OSMCorrelator_3$OP
#define MY_BASE_OPERATOR OSMCorrelator_3_Base
#define MY_OPERATOR_SCOPE SPL::_Operator::OSMPointMatcher_18

namespace SPL {
namespace _Operator {
namespace OSMPointMatcher_18 {

class MY_BASE_OPERATOR : public Operator
{
public:
    
    typedef SPL::BeJw1zEEKgCAQheEzTRHZARKEoE0XMGeyKVOwqfMnRbv3_1sUnykB9WHErYY_0eDGoVrLBcSLoLKfpvQoN8io2Oxjhl6_1YZWuRMTjjFcSnppmBUmreSCgJVyuyHX4L6vT_13AB0iCwO IPort0Type;
    typedef SPL::BeJwrMfI0NExPzUstSixJTfFM8TQ0yC_1KTM_1MS8zxTAEAlnoKBa IPort1Type;
    typedef SPL::BeJw1zEEKgCAQheEzTRHZARKEoE0XMGeyKVOwqfMnRbv3_1sUnykB9WHErYY_0eDGoVrLBcSLoLKfpvQoN8io2Oxjhl6_1YZWuRMTjjFcSnppmBUmreSCgJVyuyHX4L6vT_13AB0iCwO OPort0Type;
    
    MY_BASE_OPERATOR();
    
    ~MY_BASE_OPERATOR();
    
    inline void tupleLogic(Tuple const & tuple, uint32_t port);
    void processRaw(Tuple const & tuple, uint32_t port);
    
    inline void punctLogic(Punctuation const & punct, uint32_t port);
    void processRaw(Punctuation const & punct, uint32_t port);
    void punctPermitProcessRaw(Punctuation const & punct, uint32_t port);
    void punctNoPermitProcessRaw(Punctuation const & punct, uint32_t port);
    
    inline void submit(Tuple & tuple, uint32_t port)
    {
        Operator::submit(tuple, port);
    }
    inline void submit(Punctuation const & punct, uint32_t port)
    {
        Operator::submit(punct, port);
    }
    
    
    
    SPL::rstring lit$0;
    
    
protected:
    Mutex $svMutex;
    typedef std::bitset<3> OPortBitsetType;
    OPortBitsetType $oportBitset;
    Mutex $fpMutex;
    SPL::rstring param$className$0;
    SPL::rstring param$classLibrary$0;
    void checkpointStateVariables(NetworkByteBuffer & opstate) const;
    void restoreStateVariables(NetworkByteBuffer & opstate);
    void checkpointStateVariables(Checkpoint & ckpt);
    void resetStateVariables(Checkpoint & ckpt);
    void resetStateVariablesToInitialState();
    bool hasStateVariables() const;
    void resetToInitialStateRaw();
    void checkpointRaw(Checkpoint & ckpt);
    void resetRaw(Checkpoint & ckpt);
private:
    static bool globalInit_;
    static bool globalIniter();
    ParameterMapType paramValues_;
    ParameterMapType& getParameters() { return paramValues_;}
    void addParameterValue(std::string const & param, ConstValueHandle const& value)
    {
        ParameterMapType::iterator it = paramValues_.find(param);
        if (it == paramValues_.end())
            it = paramValues_.insert (std::make_pair (param, ParameterValueListType())).first;
        it->second.push_back(&ParameterValue::create(value));
    }
    void addParameterValue(std::string const & param)
    {
        ParameterMapType::iterator it = paramValues_.find(param);
        if (it == paramValues_.end())
            it = paramValues_.insert (std::make_pair (param, ParameterValueListType())).first;
        it->second.push_back(&ParameterValue::create());
    }
};


class MY_OPERATOR : public MY_BASE_OPERATOR 
{
public:
  // constructor
  MY_OPERATOR();

  // destructor
  virtual ~MY_OPERATOR(); 

  // notify port readiness
  void allPortsReady(); 

  // notify termination
  void prepareToShutdown(); 

  // processing for source and threaded operators   
  void process(uint32_t idx);
    
  // tuple processing for mutating ports 
  void process(Tuple & tuple, uint32_t port);
    
  // tuple processing for non-mutating ports
  void process(Tuple const & tuple, uint32_t port);

  // punctuation processing
  void process(Punctuation const & punct, uint32_t port);
private:
  void setupStateHandler();

  // members
  
  /** How we invoke actions on the Java Operator implementation */
  SPL::JNI::JNIBridgeInvoker* _bi;
  
  /* The instance of the JNIBridge used to wrap the Java Operator implementation */
  jobject _bridge;

  typedef void (*FP)(SPL::JNI::JNIBridgeInvoker&, jobject, NativeByteBuffer &, uint32_t);
  FP _fp;
  
  bool hasTupleLogic;
  
  std::tr1::shared_ptr<SPL::StateHandler> _stateHandler;

public:

    
  // handle byte buffers being send as is
  virtual void processRaw(NativeByteBuffer & buffer, uint32_t port)
  {
     (*_fp)(*_bi, _bridge, buffer, port);
  }
  
  //Java operators handle byte buffers directly
  virtual bool sendRawBufferData() { return !hasTupleLogic; }
}; 

} // namespace OSMPointMatcher_18
} // namespace _Operator
} // namespace SPL

#undef MY_OPERATOR_SCOPE
#undef MY_BASE_OPERATOR
#undef MY_OPERATOR
#endif // SPL_OPER_INSTANCE_OSMPOINTMATCHER_18_OSMCORRELATOR_3_H_


