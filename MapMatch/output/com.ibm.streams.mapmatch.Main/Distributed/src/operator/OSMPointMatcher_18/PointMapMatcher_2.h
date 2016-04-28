// eJzNVV2TojgUrfyVqX2YqepSIj229hsC2iqIimJ3v1AIAaJA6BCUuDX_1fWP70a3t1m7tvkzBC_0feczk5uTfxSVrDy7RWMIq8tKhFiBS5x7CXPD6OCc6Y6eWmx_1wYUQCBJJ6f4FutVhcv4zmqd9BgC3e6PoxUZRIjfTebPr0q06FOdOnZ7CE_0dKztW2hm0_0VGdmExz_1i601486fTlxZWQ1iMONTq_0bRBjGOabCRm1MGlY1SpOmi58eV2UU3O2WnV9O8vztDNPKbLVaODwkj893xvNzcyFstKRsLq1avG3L9oS7uhGtFImiqW6EI4GeLx_0e6g8xbdH9iobSTFneJ1NXPjQlhauFIbqwChQ_1EB5p1dsHdjrLtWXefiKHFtzHLoz0qdSLERtPg81lgRvYcW2fN3MfNSTLaPfJCbnelCkubZqN19nwyh2YS4VLlz3SFPy29NdO_0_0GC_0Pnxseb1S3J1Ax7EtddOLTXUtmaV1XRigeGuS6GczgIlmOT_022jszPbxrzgPrQ0uxrs1G2naFQquVGR01H_1Xg6yaWVOGG_0H87G_1c2A760JN6SvjXqv7tzQit8KR7M_0cKnjuyp2JstQiq9wnS_0AeJJj9IQF7bDw_0lqJV5AZ4h_0ABChPiseb9AWvcwOQrDNRjkqL6MilRiqv6ltC16EUf1U89WA885oGMZGj_19xgXjFBu4x0C8EIFPCpTGKN4WTLUpQiBQOR7Yp9mMUVFTJIAXAmFR_1WXtA1KiI8Z_1wda44qW7gXjLJrhFKklI2F4mydf8RrCWZwTenb2_17cEwIC_0jy4QrpHlCvmsHwBhlRCRiFlnZYDASVVCsugSYWIFNvPS_1EiBR33wQ99vPmWfDGiBMhddhGaiyQHKyvTPkaXpd7rW038dQ93Ei4pDTNG0u6luWo5_0pxq6Mv31H_0wTTUfZiAToTEJZcPGNCytDC4_0DJSEJ8rL3g5Zc98DvcsqCVJjZOvQ3CvQg_0lgIjND_0vsjPsPTVoI8c4xSSbrj2Ke0ck84jbGUz6vnri2ztKrbPpmKvMMmsUEBijj8Mfs_0_1jkrXuwuVol_0cxvYAnYdT3JTMPeV_1_1wEIxZHxZbGfC5zAyxIng04lvjrxL2qcSN9_17I8P_0BdsQIng

/* Additional includes go here */
#include "com/ibm/research/st/algorithms/roadnet/mapmatch/PointMapMatcher.h"

#include <string>
#include <SPL/Runtime/Type/Enum.h>


#ifndef SPL_OPER_INSTANCE_OSMPOINTMATCHER_18_POINTMAPMATCHER_2_H_
#define SPL_OPER_INSTANCE_OSMPOINTMATCHER_18_POINTMAPMATCHER_2_H_

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
#include "../../type/BeJwlyVELgjAQAOC_11NJiPkq7xaAcSNSjnN0hytiknQ_1790W_0ffCJLseh7ryBGswV1GFbCYUfZeVSDVVrzLmHu3_1C6XKDtldqfxtwyk6nceG3OLI6oMyyEdsmpDj96ZTKgh_1p0s_1kGo60c9Rz9pFfWL5vcivj.h"
#include "../../type/BeJwrMfG0yE_1KSk0u8Uxxs8hJLMksKU1JdbPMyc9LBzM9LUsyc1ODSxJzCwBs2xCo.h"
#include "../../type/BeJyrNI43dnRxMQty9fUPczV19nF1DAIAPG8FCo.h"
#include "../../type/BeJyrNIo38fN3cTVxdXF3BQAbDgOu.h"

#include <bitset>

#define MY_OPERATOR PointMapMatcher_2$OP
#define MY_BASE_OPERATOR PointMapMatcher_2_Base
#define MY_OPERATOR_SCOPE SPL::_Operator::OSMPointMatcher_18

namespace SPL {
namespace _Operator {
namespace OSMPointMatcher_18 {

class MY_BASE_OPERATOR : public Operator
{
public:
    
    typedef SPL::BeJwrMfG0yE_1KSk0u8Uxxs8hJLMksKU1JdbPMyc9LBzM9LUsyc1ODSxJzCwBs2xCo IPort0Type;
    typedef SPL::BeJwlyVELgjAQAOC_11NJiPkq7xaAcSNSjnN0hytiknQ_1790W_0ffCJLseh7ryBGswV1GFbCYUfZeVSDVVrzLmHu3_1C6XKDtldqfxtwyk6nceG3OLI6oMyyEdsmpDj96ZTKgh_1p0s_1kGo60c9Rz9pFfWL5vcivj IPort1Type;
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
    
    
    
    SPL::uint32 lit$0;
    SPL::float64 lit$1;
    SPL::float64 lit$2;
    SPL::float64 lit$3;
    
    
protected:
    Mutex $svMutex;
    typedef std::bitset<3> OPortBitsetType;
    OPortBitsetType $oportBitset;
    Mutex $fpMutex;
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


#define POINT_MAP_MATCHER_OPERATOR "PointMapMatcher"

class MY_OPERATOR: public MY_BASE_OPERATOR {
public:

	
	enum UpdateFlags {
		ADD,
		REMOVE,
		CLEAR
	};
	
	enum UpdateType {
		NODE,
		EDGE
	};

	// Constructor
	MY_OPERATOR();

	// Destructor
	virtual ~MY_OPERATOR();

	// Notify port readiness
	void allPortsReady();

	// Notify pending shutdown
	void prepareToShutdown();

	// Processing for source and threaded operators
	void process(uint32_t idx);

	// Tuple processing for mutating ports
	void process(Tuple & tuple, uint32_t port);

	// Tuple processing for non-mutating ports
	void process(Tuple const & tuple, uint32_t port);

	// Punctuation processing
	void process(Punctuation const & punct, uint32_t port);
private:
	// Members
	const double _distanceThreshold, _velocityThreshold, _matchingTimeCutoff;
	
	com::ibm ::research::st::algorithms::roadnet::mapmatch::PointMapMatcher *__mapMatcher;
	
	boolean _geometryProcessed;
	
	//for the window size
	int __windowSize;
	
	//com::ibm ::research::st::algorithms::roadnet::mapmatch::ObjectCoordinate &__previousPoint;
	
	UpdateType convertEnumToUpdateType(Enum const &e);
	UpdateFlags convertEnumToUpdateFlags(Enum const &e);
};

} // namespace OSMPointMatcher_18
} // namespace _Operator
} // namespace SPL

#undef MY_OPERATOR_SCOPE
#undef MY_BASE_OPERATOR
#undef MY_OPERATOR
#endif // SPL_OPER_INSTANCE_OSMPOINTMATCHER_18_POINTMAPMATCHER_2_H_


