// eJzNVV2TojgUrfyVqX2YqepSIj229hsC2iqIimJ3v1AIAaJA6BCUuDX_1fWP70a3t1m7tvkzBC_0feczk5uTfxSVrDy7RWMIq8tKhFiBS5x7CXPD6OCc6Y6eWmx_1wYUQCBJJ6f4FutVhcv4zmqd9BgC3e6PoxUZRIjfTebPr0q06FOdOnZ7CE_0dKztW2hm0_0VGdmExz_1i601486fTlxZWQ1iMONTq_0bRBjGOabCRm1MGlY1SpOmi58eV2UU3O2WnV9O8vztDNPKbLVaODwkj893xvNzcyFstKRsLq1avG3L9oS7uhGtFImiqW6EI4GeLx_0e6g8xbdH9iobSTFneJ1NXPjQlhauFIbqwChQ_1EB5p1dsHdjrLtWXefiKHFtzHLoz0qdSLERtPg81lgRvYcW2fN3MfNSTLaPfJCbnelCkubZqN19nwyh2YS4VLlz3SFPy29NdO_0_0GC_0Pnxseb1S3J1Ax7EtddOLTXUtmaV1XRigeGuS6GczgIlmOT_022jszPbxrzgPrQ0uxrs1G2naFQquVGR01H_1Xg6yaWVOGG_0H87G_1c2A760JN6SvjXqv7tzQit8KR7M_0cKnjuyp2JstQiq9wnS_0AeJJj9IQF7bDw_0lqJV5AZ4h_0ABChPiseb9AWvcwOQrDNRjkqL6MilRiqv6ltC16EUf1U89WA885oGMZGj_19xgXjFBu4x0C8EIFPCpTGKN4WTLUpQiBQOR7Yp9mMUVFTJIAXAmFR_1WXtA1KiI8Z_1wda44qW7gXjLJrhFKklI2F4mydf8RrCWZwTenb2_17cEwIC_0jy4QrpHlCvmsHwBhlRCRiFlnZYDASVVCsugSYWIFNvPS_1EiBR33wQ99vPmWfDGiBMhddhGaiyQHKyvTPkaXpd7rW038dQ93Ei4pDTNG0u6luWo5_0pxq6Mv31H_0wTTUfZiAToTEJZcPGNCytDC4_0DJSEJ8rL3g5Zc98DvcsqCVJjZOvQ3CvQg_0lgIjND_0vsjPsPTVoI8c4xSSbrj2Ke0ck84jbGUz6vnri2ztKrbPpmKvMMmsUEBijj8Mfs_0_1jkrXuwuVol_0cxvYAnYdT3JTMPeV_1_1wEIxZHxZbGfC5zAyxIng04lvjrxL2qcSN9_17I8P_0BdsQIng

/* Additional includes go here */
#include "com/ibm/research/st/util/DoubleUtil.h"

#include "com/ibm/research/st/ST.h"
#include "com/ibm/research/st/STException.h"

#include "com/ibm/research/st/algorithms/roadnet/mapmatch/PointMapMatcher.h"

#include <string>
#include <iomanip>
#include <unistd.h>


#include "./PointMapMatcher_2.h"
using namespace SPL::_Operator::OSMPointMatcher_18;

#include <SPL/Runtime/Function/SPLFunctions.h>
#include <SPL/Runtime/Operator/Port/Punctuation.h>

#include <string>

#define MY_OPERATOR_SCOPE SPL::_Operator::OSMPointMatcher_18
#define MY_BASE_OPERATOR PointMapMatcher_2_Base
#define MY_OPERATOR PointMapMatcher_2$OP


using namespace com::ibm::research::st;
using namespace com::ibm::research::st::util;
using namespace com::ibm::research::st::algorithms::roadnet::mapmatch;

using namespace std;

using namespace std::tr1;





// Constructor
MY_OPERATOR_SCOPE::MY_OPERATOR::MY_OPERATOR() : 
	_distanceThreshold(lit$1),
	_velocityThreshold(lit$2),
	_matchingTimeCutoff(lit$3) {
	// Initialization code goes here
	//init the map matcher based on the provided parameters
	ST::init();

	__windowSize = lit$0;
	
	double lcLat = -180.0;
	double lcLon = -180.0;
	double ucLat = 180.0;
	double ucLon = 180.0;
	
	__mapMatcher = new PointMapMatcher (DoubleUtil::toRadians(lcLat),
			DoubleUtil::toRadians(lcLon), DoubleUtil::toRadians(ucLat),
			DoubleUtil::toRadians(ucLon), __windowSize, _distanceThreshold, _velocityThreshold, _matchingTimeCutoff);
	
	
	_geometryProcessed = false;
		
}

// Destructor
MY_OPERATOR_SCOPE::MY_OPERATOR::~MY_OPERATOR() {
	// Finalization code goes here

	delete __mapMatcher;
}

// Notify port readiness
void MY_OPERATOR_SCOPE::MY_OPERATOR::allPortsReady() {
	// Notifies that all ports are ready. No tuples should be submitted before
	// this. Source operators can use this method to spawn threads.

	/*
	 createThreads(1); // Create source thread
	 */
}

// Notify pending shutdown
void MY_OPERATOR_SCOPE::MY_OPERATOR::prepareToShutdown() {
	// This is an asynchronous call
}

// Processing for source and threaded operators   
void MY_OPERATOR_SCOPE::MY_OPERATOR::process(uint32_t idx) {
	// A typical implementation will loop until shutdown
	/*
	 while(!getPE().getShutdownRequested()) {
	 // do work ...
	 }
	 */
}

// Tuple processing for mutating ports 
void MY_OPERATOR_SCOPE::MY_OPERATOR::process(Tuple & tuple, uint32_t port) {
	// Sample submit code
	/*
	 submit(otuple, 0); // submit to output port 0
	 */
}


MY_OPERATOR_SCOPE::MY_OPERATOR::UpdateType MY_OPERATOR_SCOPE::MY_OPERATOR::convertEnumToUpdateType(Enum const &e) {
	string value = e.getValue();
	
	if(value == "NODE") {
		return MY_OPERATOR_SCOPE::MY_OPERATOR::NODE;
	}
	else if(value == "EDGE") {
		return MY_OPERATOR_SCOPE::MY_OPERATOR::EDGE;
	}
	else {
		// throw something
		throw("Invalid update type in convertEnumToUpdateType");
	}
}

MY_OPERATOR_SCOPE::MY_OPERATOR::UpdateFlags MY_OPERATOR_SCOPE::MY_OPERATOR::convertEnumToUpdateFlags(Enum const &e) {
	string value = e.getValue();
	
	if(value == "ADD") {
		return MY_OPERATOR_SCOPE::MY_OPERATOR::ADD;
	}
	else if(value =="REMOVE") {
		return MY_OPERATOR_SCOPE::MY_OPERATOR::REMOVE;
	}
	else if(value == "CLEAR") {
		return MY_OPERATOR_SCOPE::MY_OPERATOR::CLEAR;
	}
	else {
		// throw something
		throw("Invalid update type in convertEnumToUpdateType");
	}
}

// Tuple processing for non-mutating ports
void MY_OPERATOR_SCOPE::MY_OPERATOR::process(Tuple const & tuple, uint32_t port) {
	
	if(port == 0) {
		
		while(!_geometryProcessed) {
			// sleep 100 milliseconds and check again
			usleep(100 /* milliseconds*/ * 1000 /* microseconds per millisecond*/);
		}
	
			
		IPort0Type
		const & iport$0 = static_cast<IPort0Type const&>(tuple);
	
			
	
		int64_t	id = iport$0.get_objectId();
		double latitude = iport$0.get_latitude();
		double longitude = iport$0.get_longitude();
		int64_t timestamp = iport$0.get_timeStamp();
			 
		//find best match and submit the tuple
		ObjectCoordinate objectCoordinate(id, DoubleUtil::toRadians(latitude), DoubleUtil::toRadians(longitude), timestamp);
		MatchedRoadNetPoint matchedCoordinate;
		
		SPLAPPTRC(L_TRACE, "About to match coordinate", POINT_MAP_MATCHER_OPERATOR);
		SPLAPPTRC(L_TRACE, "Original: id=" << id << std::setprecision(15) << " lat=" << latitude << " long=" << longitude << " timestamp=" << timestamp, POINT_MAP_MATCHER_OPERATOR);
		int returnCode = __mapMatcher->getMatchedCoordinate(objectCoordinate, matchedCoordinate);
		SPLAPPTRC(L_TRACE, "Done matching, return code = " << returnCode, POINT_MAP_MATCHER_OPERATOR);
			
		double lat, lon;
		lat = DoubleUtil::toDegrees(matchedCoordinate.matchedLatitude);
		lon = DoubleUtil::toDegrees(matchedCoordinate.matchedLongitude);
		
		SPLAPPTRC(L_TRACE, "Matched: Roadid=" << matchedCoordinate.edgeId << " lat=" << lat	<< " long=" << lon
			<< "distance=" << matchedCoordinate.alongTrackDistance << " direction=" << matchedCoordinate.isStartToEnd, POINT_MAP_MATCHER_OPERATOR
		);
	
		if(returnCode == 0 || returnCode == -1) { // success
			
			
		
			OPort0Type otuple;
			
					otuple.set_matchedEdgeId(matchedCoordinate.edgeId);
					
					otuple.set_latitude(lat);
					
					otuple.set_longitude(lon);
					
					otuple.set_distanceOnTrack(matchedCoordinate.alongTrackDistance);
					
					{					
						otuple.set_directionOfTravel(matchedCoordinate.isStartToEnd);
					
					}
											
						
							otuple.set_objectId(iport$0.get_objectId());
												
						
							otuple.set_origLatitude(iport$0.get_latitude());
												
						
							otuple.set_origLongitude(iport$0.get_longitude());
						

			submit(otuple, 0);
		}
		
		else {
			// error... emit error tuple
			
		}
	}
	
	else if(port == 1) {
		_geometryProcessed = false;
		
		// geometry
		ConstValueHandle updateTypeValueHandle = tuple.getAttributeValue("updateType");
		ConstValueHandle updateFlagsValueHandle = tuple.getAttributeValue("updateFlags");
		
		Enum const &updateTypeEnum = updateTypeValueHandle.operator Enum const&();
		Enum const &updateFlagsEnum = updateFlagsValueHandle.operator Enum const&();
		
		UpdateType updateType = convertEnumToUpdateType(updateTypeEnum);
		UpdateFlags updateFlags = convertEnumToUpdateFlags(updateFlagsEnum);
		
		int64_t objectId = tuple.getAttributeValue("objectId");
		double latitude = tuple.getAttributeValue("latitude");
		double longitude = tuple.getAttributeValue("longitude");
		int64_t startNodeId = tuple.getAttributeValue("startNodeId");
		int64_t endNodeId = tuple.getAttributeValue("endNodeId");
		boolean isOneWay = tuple.getAttributeValue("isOneWay");
		
		switch(updateFlags) {
		
		case ADD:
		
			switch(updateType) {
			
			case NODE:
			{
				SPLAPPTRC(L_TRACE, "Adding node id=" << objectId << std::setprecision(15) << " lat=" << latitude << " long" << longitude, POINT_MAP_MATCHER_OPERATOR);
				double lat, lon;
				lat = DoubleUtil::toRadians(latitude);
				lon = DoubleUtil::toRadians(longitude);
				
				MapNode mapNode(objectId, lat, lon);
				boolean returnCode = __mapMatcher->addMapNode(mapNode);
				
				if(returnCode == false) {
					// error... emit error tuple
					
				
				}
			}
			break;
			
			case EDGE:
			{
				SPLAPPTRC(L_TRACE, "Adding edge id=" << objectId << " startNodeId=" << startNodeId << " endNodeId=" << endNodeId, POINT_MAP_MATCHER_OPERATOR);
				MapEdge mapEdge(objectId, startNodeId, endNodeId, isOneWay);
				boolean returnCode = __mapMatcher->addMapEdge(mapEdge);
				
				if(returnCode == false) {
					// error... emit error tuple
					
				
				}
			}
			break;
			
			}
			
		break;
		
		case REMOVE:
		switch(updateType) {
			
			case NODE:
			{
				SPLAPPTRC(L_TRACE, "Removing node, id=" << objectId, POINT_MAP_MATCHER_OPERATOR);
				boolean returnCode = __mapMatcher->removeMapNode(objectId);
				
				if(returnCode == false) {
					// error... emit error tuple
					
				
				}
			}
			break;
			
			case EDGE:
			{
				SPLAPPTRC(L_TRACE, "Removing edge, id=" << objectId, POINT_MAP_MATCHER_OPERATOR);
				boolean returnCode = __mapMatcher->removeMapEdge(objectId);
				
				if(returnCode == false) {
					// error... emit error tuple
					
				
				}
				
			}
			break;
			
			}
		break;
		
		case CLEAR:
			SPLAPPTRC(L_TRACE, "Clearing map", POINT_MAP_MATCHER_OPERATOR);
			__mapMatcher->clearMap();
		break;
		}
	}
	
}

// Punctuation processing
void MY_OPERATOR_SCOPE::MY_OPERATOR::process(Punctuation const & punct, uint32_t port) {

 if(punct==Punctuation::WindowMarker || punct==Punctuation::FinalMarker) {
 	if(port == 1) {
 		SPLAPPTRC(L_TRACE, "Rebuilding map", POINT_MAP_MATCHER_OPERATOR);
 		bool returnCode = __mapMatcher->rebuildMap();
 		SPLAPPTRC(L_TRACE, "Return code = " << returnCode, POINT_MAP_MATCHER_OPERATOR);
 		_geometryProcessed = true;
 	}
 	
 }
 
}

static SPL::Operator * initer() { return new MY_OPERATOR_SCOPE::MY_OPERATOR(); }
bool MY_BASE_OPERATOR::globalInit_ = MY_BASE_OPERATOR::globalIniter();
bool MY_BASE_OPERATOR::globalIniter() {
    instantiators_.insert(std::make_pair("OSMPointMatcher_18::PointMapMatcher_2",&initer));
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
    initRTC(*this, lit$3, "lit$3");
    addParameterValue ("historySize", SPL::ConstValueHandle(lit$0));
    addParameterValue ("distanceThreshold", SPL::ConstValueHandle(lit$1));
    addParameterValue ("velocityThreshold", SPL::ConstValueHandle(lit$2));
    addParameterValue ("matchingTimeCutoff", SPL::ConstValueHandle(lit$3));
    (void) getParameters(); // ensure thread safety by initializing here
    $oportBitset = OPortBitsetType(std::string("011"));
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

void MY_BASE_OPERATOR::tupleLogic(Tuple const & tuple, uint32_t port) {
    switch(port) {
    }
}


void MY_BASE_OPERATOR::processRaw(Tuple const & tuple, uint32_t port) {
    tupleLogic (tuple, port);
    static_cast<MY_OPERATOR_SCOPE::MY_OPERATOR*>(this)->MY_OPERATOR::process(tuple, port);
}


void MY_BASE_OPERATOR::punctLogic(Punctuation const & punct, uint32_t port) {
}

void MY_BASE_OPERATOR::punctPermitProcessRaw(Punctuation const & punct, uint32_t port) {
    {
        punctNoPermitProcessRaw(punct, port);
    }
}

void MY_BASE_OPERATOR::punctNoPermitProcessRaw(Punctuation const & punct, uint32_t port) {
    switch(punct) {
    case Punctuation::WindowMarker:
        punctLogic(punct, port);
        process(punct, port);
        break;
    case Punctuation::FinalMarker:
        punctLogic(punct, port);
        
        if (punct == Punctuation::FinalMarker) {
            process(punct, port);
            bool forward = false;
            {
                AutoPortMutex $apm($fpMutex, *this);
                $oportBitset.reset(port);
                if ($oportBitset.none()) {
                    $oportBitset.set(2);
                    forward=true;
                }
            }
            if(forward)
                submit(punct, 0);
            return;
        }
        process(punct, port);
        break;
    case Punctuation::DrainMarker:
    case Punctuation::ResetMarker:
    case Punctuation::ResumeMarker:
        break;
    default:
        break;
    }
}

void MY_BASE_OPERATOR::processRaw(Punctuation const & punct, uint32_t port) {
    switch(port) {
    case 0:
        punctNoPermitProcessRaw(punct, port);
        break;
    case 1:
        punctNoPermitProcessRaw(punct, port);
        break;
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



