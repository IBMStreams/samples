// eJzVlFtzmkAYhmf_1ScfpRTLNcIipUe9QwKqABwhJc_0MssOAq7JJlUbDT_195VO02iZqa97HC4ePc7Px_0ENJNwkEkFZwhmhZQgWuSQY5h2uxPXfrKtAaIZ4qx2aclCBFSgiOsONCRJFjevcyT30Gib1r5hJSttpk36C1V1Rni6frmvoBa6jrsijrKsOV6T2UK97yiPCyWO_0yOrQMt7VvcGxdZXB2bQ_1_14QPyPf1X2f7azsW9lcqP3W01jnafQSV3xbr1skRIPmxBq2qF3XRlRkub7qtJ69cbJcqLlSLNT1gLaUsDPfdXIzfrS_0bkK8WUnLxlnJzI6HimNU091DwV1cjbzYtNveTKlFB2PPXqi2295VnmZoKaHjHrwQpGbO8K4ZkXllz3jdiR_0m4c5XO8RUdW2oTQdts08_1cKPNduw0Q8_0voiez2ZtpgZ5Myr2xApogxfyzAtyp1e0yAQeTBBw09YJ2e6IBeSmYyUFaogxX8paytYAaItmGuQ15uJQjyCEglKA9yjCFReHAbA_13XaD3_0a8a4YfLIl3alcY10LhwDUqOTIbQMZGFAwZZfZ7rKkHcozRdY65jhkJOWX31twU0rq8_1ffn03lrGWZ7KKQ5k92jkiPcGmSUJOaakkFaQCb_0TIjOYxzg9m4V6ub4Dkn3qA6_1TWJtMY8l5pCOzviiBQ8JF67fioTllf5D_1B18UyAADbVDmYpeQJ_1YaIFJmP5yJbtwY_0sD4KXo6HpopTIrjqabrN3PDnvjGTd8ytPneiAYrMcxhBDDhrTshpAITLyME4pTC3xIlyakmhse4QyP0xhWR6ETBxYSgR1iDQKBDkABFEQSOw1Zfh_13P_14JD_07cgQQQx0WP0JiVlOMEEpq_0S8gtmecHX

#include <SPL/Toolkit/JavaOp.h>


#ifndef SPL_OPER_INSTANCE_OSMPOINTMATCHER_18_OSMXMLGEOMETRYSOURCE_1_H_
#define SPL_OPER_INSTANCE_OSMPOINTMATCHER_18_OSMXMLGEOMETRYSOURCE_1_H_

#include <SPL/Runtime/Operator/Operator.h>
#include <SPL/Runtime/Operator/ParameterValue.h>
#include <SPL/Runtime/Operator/OperatorContext.h>
#include <SPL/Runtime/Operator/Port/AutoPortMutex.h>
#include <SPL/Runtime/Operator/State/StateHandler.h>
#include <SPL/Runtime/ProcessingElement/PE.h>
#include <SPL/Runtime/Type/SPLType.h>
#include <SPL/Runtime/Utility/CV.h>
using namespace UTILS_NAMESPACE;

#include "../../type/BeJwlyVELgjAQAOC_11NJiPkq7xaAcSNSjnN0hytiknQ_1790W_0ffCJLseh7ryBGswV1GFbCYUfZeVSDVVrzLmHu3_1C6XKDtldqfxtwyk6nceG3OLI6oMyyEdsmpDj96ZTKgh_1p0s_1kGo60c9Rz9pFfWL5vcivj.h"
#include "../../type/BeJwrMfI0NExPzUstSixJTfFM8TQ0yC_1KTM_1MS8zxTAEAlnoKBa.h"
#include "../../type/BeJyrNI43dnRxMQty9fUPczV19nF1DAIAPG8FCo.h"
#include "../../type/BeJyrNIo38fN3cTVxdXF3BQAbDgOu.h"


#define MY_OPERATOR OSMXMLGeometrySource_1$OP
#define MY_BASE_OPERATOR OSMXMLGeometrySource_1_Base
#define MY_OPERATOR_SCOPE SPL::_Operator::OSMPointMatcher_18

namespace SPL {
namespace _Operator {
namespace OSMPointMatcher_18 {

class MY_BASE_OPERATOR : public Operator
{
public:
    
    typedef SPL::BeJwlyVELgjAQAOC_11NJiPkq7xaAcSNSjnN0hytiknQ_1790W_0ffCJLseh7ryBGswV1GFbCYUfZeVSDVVrzLmHu3_1C6XKDtldqfxtwyk6nceG3OLI6oMyyEdsmpDj96ZTKgh_1p0s_1kGo60c9Rz9pFfWL5vcivj OPort0Type;
    typedef SPL::BeJwrMfI0NExPzUstSixJTfFM8TQ0yC_1KTM_1MS8zxTAEAlnoKBa OPort1Type;
    
    MY_BASE_OPERATOR();
    
    ~MY_BASE_OPERATOR();
    
    
    inline void submit(Tuple & tuple, uint32_t port)
    {
        Operator::submit(tuple, port);
    }
    inline void submit(Punctuation const & punct, uint32_t port)
    {
        Operator::submit(punct, port);
    }
    
    
    
    SPL::rstring lit$0;
    SPL::rstring lit$1;
    SPL::rstring lit$2;
    
    
protected:
    Mutex $svMutex;
    SPL::rstring param$className$0;
    SPL::rstring param$classLibrary$0;
    SPL::rstring param$mapfile$0;
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
#endif // SPL_OPER_INSTANCE_OSMPOINTMATCHER_18_OSMXMLGEOMETRYSOURCE_1_H_


