// eJzNU01v4jAUlP9JhfYAWimJgUVdboSmZUsoLaS09IIc5xEMiZ21HSD99TUfrZbASntc5eTxe5l588ZUpBYLU0tpCSRVW4tx0JYEpdvtXhA8BnmWwITBBmHkmA_0jimXZusjAduF_0g989rx93O08L8N6DUe_0tM_0p7wnNeB3dQ9CfDze_15gI_1CdWOG1TMvVu7Pl54np9OZAzd3YiJ9l4594ffn2fpJPFwzUR9ul4ukNcPTt5d8NAiWy1s65lmWus_0phHE3vp8UedF7bfqtdTDDjY7rsO5maC0qRtwPlDD9zUHjR7_1dZlw36miP4AMizZCMxwesfgFrXMCaJ39D9kKkYIdJDinb2hshVyojFOwByQZE04UdEU0QFxwc1EI0IUo9kBRQSYJzcqxW6F_1XYIlMWV9bqNRQR5ueMNdwKwEODD4LJZHFOUk1Bh0IkayYvmESqBayqP4Dc6VWu_1p_0dVposzRL7ISF9mWxSyJNW0leJqT_0lHWwEB93VBpDcA1bXZ4AH9d3sXgESuSSgkvUmb_14uOOSGiI100zwPpyZhY8JOO3YhZ7thvhM1f8UesSQ3D1JTEXOjXdnLjf3Txaha5TusgmRF8XwK0KmptVECTFm5BGgeSLIHhA8PkUipjThFIY8kISu_1sB3YTJGDufmYg0JCk3KgHAkwqW5_0eIQksV_0mWcPnnE5H3zhH56

#include <SPL/Toolkit/JavaOp.h>


#ifndef SPL_OPER_INSTANCE_WEBSINK_H_
#define SPL_OPER_INSTANCE_WEBSINK_H_

#include <SPL/Runtime/Operator/Operator.h>
#include <SPL/Runtime/Operator/ParameterValue.h>
#include <SPL/Runtime/Operator/OperatorContext.h>
#include <SPL/Runtime/Operator/Port/AutoPortMutex.h>
#include <SPL/Runtime/Operator/State/StateHandler.h>
#include <SPL/Runtime/ProcessingElement/PE.h>
#include <SPL/Runtime/Type/SPLType.h>
#include <SPL/Runtime/Window/Window.h>
#include <SPL/Runtime/Utility/CV.h>
using namespace UTILS_NAMESPACE;

#include "../type/BeJw1zEEKgCAQheEzTRHZARKEoE0XMGeyKVOwqfMnRbv3_1sUnykB9WHErYY_0eDGoVrLBcSLoLKfpvQoN8io2Oxjhl6_1YZWuRMTjjFcSnppmBUmreSCgJVyuyHX4L6vT_13AB0iCwO.h"


#define MY_OPERATOR WebSink$OP
#define MY_BASE_OPERATOR WebSink_Base
#define MY_OPERATOR_SCOPE SPL::_Operator

namespace SPL {
namespace _Operator {

class MY_BASE_OPERATOR : public Operator
{
public:
    
    typedef SPL::BeJw1zEEKgCAQheEzTRHZARKEoE0XMGeyKVOwqfMnRbv3_1sUnykB9WHErYY_0eDGoVrLBcSLoLKfpvQoN8io2Oxjhl6_1YZWuRMTjjFcSnppmBUmreSCgJVyuyHX4L6vT_13AB0iCwO IPort0Type;
    
    MY_BASE_OPERATOR();
    
    ~MY_BASE_OPERATOR();
    
    inline void tupleLogic(Tuple const & tuple, uint32_t port);
    void processRaw(Tuple const & tuple, uint32_t port);
    
    inline void punctLogic(Punctuation const & punct, uint32_t port);
    void processRaw(Punctuation const & punct, uint32_t port);
    void punctPermitProcessRaw(Punctuation const & punct, uint32_t port);
    void punctNoPermitProcessRaw(Punctuation const & punct, uint32_t port);
    
    
    
    SPL::int32 lit$0;
    SPL::rstring lit$1;
    SPL::rstring lit$2;
    SPL::rstring lit$3;
    SPL::int32 lit$4;
    
    
protected:
    Mutex $svMutex;
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

} // namespace _Operator
} // namespace SPL

#undef MY_OPERATOR_SCOPE
#undef MY_BASE_OPERATOR
#undef MY_OPERATOR
#endif // SPL_OPER_INSTANCE_WEBSINK_H_


