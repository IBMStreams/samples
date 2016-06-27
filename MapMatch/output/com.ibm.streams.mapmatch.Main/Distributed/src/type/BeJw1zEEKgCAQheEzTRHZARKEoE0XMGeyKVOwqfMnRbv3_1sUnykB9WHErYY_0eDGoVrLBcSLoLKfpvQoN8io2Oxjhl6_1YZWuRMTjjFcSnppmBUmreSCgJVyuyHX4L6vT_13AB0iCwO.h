// t8I13matchedEdgeIdF8latitudeF9longitudeF15distanceOnTrackb17directionOfTravelI8objectIdF12origLatitudeF13origLongitude


#ifndef BEJW1ZEEKGCAQHEEZTRHZARKEOE0XMGEYKVOWQFMNRBV3_1SUNYKB9WHERYY_0EDGOVRLBCSLOLKFPVQON8IO2OXJHL6_1YZWURMTJJFCSNPPMBUMRESCGJVYUYHX4L6VT_13AB0ICWO_H_
#define BEJW1ZEEKGCAQHEEZTRHZARKEOE0XMGEYKVOWQFMNRBV3_1SUNYKB9WHERYY_0EDGOVRLBCSLOLKFPVQON8IO2OXJHL6_1YZWURMTJJFCSNPPMBUMRESCGJVYUYHX4L6VT_13AB0ICWO_H_

#include <SPL/Runtime/Type/Tuple.h>
#include <SPL/Runtime/Type/PrimitiveType.h>
#include <SPL/Runtime/Type/CollectionType.h>
#include <SPL/Runtime/Serialization/NetworkByteBuffer.h>
#include <SPL/Runtime/Serialization/NativeByteBuffer.h>
#include <SPL/Runtime/Serialization/VirtualByteBuffer.h>



#define SELF BeJw1zEEKgCAQheEzTRHZARKEoE0XMGeyKVOwqfMnRbv3_1sUnykB9WHErYY_0eDGoVrLBcSLoLKfpvQoN8io2Oxjhl6_1YZWuRMTjjFcSnppmBUmreSCgJVyuyHX4L6vT_13AB0iCwO

namespace SPL {

class SELF : public Tuple
{
public:
    static const bool facade = false;

    typedef SELF Self;
    
    typedef SPL::int64 matchedEdgeId_type;
    typedef SPL::float64 latitude_type;
    typedef SPL::float64 longitude_type;
    typedef SPL::float64 distanceOnTrack_type;
    typedef SPL::boolean directionOfTravel_type;
    typedef SPL::int64 objectId_type;
    typedef SPL::float64 origLatitude_type;
    typedef SPL::float64 origLongitude_type;

    enum { num_attributes = 8 } ;
    
    SELF() : Tuple(), matchedEdgeId_(), latitude_(), longitude_(), distanceOnTrack_(), directionOfTravel_(), objectId_(), origLatitude_(), origLongitude_() {}
    SELF(const Self & ot) : Tuple(), matchedEdgeId_(ot.matchedEdgeId_), latitude_(ot.latitude_), longitude_(ot.longitude_), distanceOnTrack_(ot.distanceOnTrack_), directionOfTravel_(ot.directionOfTravel_), objectId_(ot.objectId_), origLatitude_(ot.origLatitude_), origLongitude_(ot.origLongitude_) 
      { constructPayload(ot); }
    SELF(const matchedEdgeId_type & _matchedEdgeId, const latitude_type & _latitude, const longitude_type & _longitude, const distanceOnTrack_type & _distanceOnTrack, const directionOfTravel_type & _directionOfTravel, const objectId_type & _objectId, const origLatitude_type & _origLatitude, const origLongitude_type & _origLongitude) : Tuple(), matchedEdgeId_(_matchedEdgeId), latitude_(_latitude), longitude_(_longitude), distanceOnTrack_(_distanceOnTrack), directionOfTravel_(_directionOfTravel), objectId_(_objectId), origLatitude_(_origLatitude), origLongitude_(_origLongitude) { }
    SELF(const Tuple & ot, bool typesafe = true) : Tuple() { assignFrom(ot, typesafe); }
    SELF(const ConstValueHandle & ot) : Tuple() { const Tuple & o = ot; assignFrom(o); }

    virtual ~SELF() {}
    
    matchedEdgeId_type & get_matchedEdgeId() { return matchedEdgeId_; }
    const matchedEdgeId_type & get_matchedEdgeId() const { return matchedEdgeId_; }
    void set_matchedEdgeId(const matchedEdgeId_type & _matchedEdgeId) { matchedEdgeId_ = _matchedEdgeId; }
    
    latitude_type & get_latitude() { return latitude_; }
    const latitude_type & get_latitude() const { return latitude_; }
    void set_latitude(const latitude_type & _latitude) { latitude_ = _latitude; }
    
    longitude_type & get_longitude() { return longitude_; }
    const longitude_type & get_longitude() const { return longitude_; }
    void set_longitude(const longitude_type & _longitude) { longitude_ = _longitude; }
    
    distanceOnTrack_type & get_distanceOnTrack() { return distanceOnTrack_; }
    const distanceOnTrack_type & get_distanceOnTrack() const { return distanceOnTrack_; }
    void set_distanceOnTrack(const distanceOnTrack_type & _distanceOnTrack) { distanceOnTrack_ = _distanceOnTrack; }
    
    directionOfTravel_type & get_directionOfTravel() { return directionOfTravel_; }
    const directionOfTravel_type & get_directionOfTravel() const { return directionOfTravel_; }
    void set_directionOfTravel(const directionOfTravel_type & _directionOfTravel) { directionOfTravel_ = _directionOfTravel; }
    
    objectId_type & get_objectId() { return objectId_; }
    const objectId_type & get_objectId() const { return objectId_; }
    void set_objectId(const objectId_type & _objectId) { objectId_ = _objectId; }
    
    origLatitude_type & get_origLatitude() { return origLatitude_; }
    const origLatitude_type & get_origLatitude() const { return origLatitude_; }
    void set_origLatitude(const origLatitude_type & _origLatitude) { origLatitude_ = _origLatitude; }
    
    origLongitude_type & get_origLongitude() { return origLongitude_; }
    const origLongitude_type & get_origLongitude() const { return origLongitude_; }
    void set_origLongitude(const origLongitude_type & _origLongitude) { origLongitude_ = _origLongitude; }
    
    virtual bool equals(const Tuple & ot) const
    {

        if (typeid(*this) != typeid(ot)) { return false; }
        const SELF & o = static_cast<const SELF &>(ot);
        return (*this == o);

    }

    virtual SELF& clear();

    Tuple* clone() const { return new Self(*this); }
    
    void serialize(VirtualByteBuffer & buf) const
    {
        buf << matchedEdgeId_ << latitude_ << longitude_ << distanceOnTrack_ << directionOfTravel_ << objectId_ << origLatitude_ << origLongitude_;
    }

    template <class BufferType>
    void serialize(ByteBuffer<BufferType> & buf) const
    {        
        buf << matchedEdgeId_ << latitude_ << longitude_ << distanceOnTrack_ << directionOfTravel_ << objectId_ << origLatitude_ << origLongitude_;
    } 
    
    void serialize(NativeByteBuffer & buf) const
    {
        this->serialize<NativeByteBuffer>(buf);
    }

    void serialize(NetworkByteBuffer & buf) const
    {
        this->serialize<NetworkByteBuffer>(buf);
    }
    
    void deserialize(VirtualByteBuffer & buf)
    {
        buf >> matchedEdgeId_ >> latitude_ >> longitude_ >> distanceOnTrack_ >> directionOfTravel_ >> objectId_ >> origLatitude_ >> origLongitude_;
    }

    template <class BufferType>
    void deserialize(ByteBuffer<BufferType> & buf)
    {        
        buf >> matchedEdgeId_ >> latitude_ >> longitude_ >> distanceOnTrack_ >> directionOfTravel_ >> objectId_ >> origLatitude_ >> origLongitude_;
    } 

    void deserialize(NativeByteBuffer & buf)
    {
        this->deserialize<NativeByteBuffer>(buf);
    }    

    void deserialize(NetworkByteBuffer & buf)
    {
        this->deserialize<NetworkByteBuffer>(buf);
    }    

    void serialize(std::ostream & ostr) const;

    void serializeWithPrecision(std::ostream & ostr) const;

    void deserialize(std::istream & istr, bool withSuffix = false);
    
    void deserializeWithNanAndInfs(std::istream & istr, bool withSuffix = false);
    
    size_t hashCode() const
    {
        size_t s = 17;
        s = 37 * s + std::tr1::hash<matchedEdgeId_type >()(matchedEdgeId_);
        s = 37 * s + std::tr1::hash<latitude_type >()(latitude_);
        s = 37 * s + std::tr1::hash<longitude_type >()(longitude_);
        s = 37 * s + std::tr1::hash<distanceOnTrack_type >()(distanceOnTrack_);
        s = 37 * s + std::tr1::hash<directionOfTravel_type >()(directionOfTravel_);
        s = 37 * s + std::tr1::hash<objectId_type >()(objectId_);
        s = 37 * s + std::tr1::hash<origLatitude_type >()(origLatitude_);
        s = 37 * s + std::tr1::hash<origLongitude_type >()(origLongitude_);
        return s;
    }
    
    size_t getSerializedSize() const
    {
        size_t size = sizeof(SPL::int64)+sizeof(SPL::float64)+sizeof(SPL::float64)+sizeof(SPL::float64)+sizeof(SPL::boolean)+sizeof(SPL::int64)+sizeof(SPL::float64)+sizeof(SPL::float64);
        
        return size;

    }
    
    uint32_t getNumberOfAttributes() const 
        { return num_attributes; }

    TupleIterator getBeginIterator() 
        { return TupleIterator(*this, 0); }
    
    ConstTupleIterator getBeginIterator() const 
        { return ConstTupleIterator(*this, 0); }

    TupleIterator getEndIterator() 
        { return TupleIterator(*this, num_attributes); }

    ConstTupleIterator getEndIterator() const 
        { return ConstTupleIterator(*this, num_attributes); }
    
    TupleIterator findAttribute(const std::string & attrb)
    {
        std::tr1::unordered_map<std::string, uint32_t>::const_iterator it = mappings_->nameToIndex_.find(attrb);
        if ( it == mappings_->nameToIndex_.end() ) {
            return this->getEndIterator();
        }
        return TupleIterator(*this, it->second);
    }
    
    ConstTupleIterator findAttribute(const std::string & attrb) const
        { return const_cast<Self*>(this)->findAttribute(attrb); }
    
    TupleAttribute getAttribute(uint32_t index)
    {
        if (index >= num_attributes)
            invalidIndex (index, num_attributes);
        return TupleAttribute(mappings_->indexToName_[index], index, *this);
    }
    
    ConstTupleAttribute getAttribute(uint32_t index) const
        { return const_cast<Self*>(this)->getAttribute(index); }

    ValueHandle getAttributeValue(const std::string & attrb)
        { return getAttributeValueRaw(lookupAttributeName(*mappings_, attrb)->second); }


    ConstValueHandle getAttributeValue(const std::string & attrb) const
        { return const_cast<Self*>(this)->getAttributeValue(attrb); }

    ValueHandle getAttributeValue(uint32_t index) 
        { return getAttributeValueRaw(index); }

    ConstValueHandle getAttributeValue(uint32_t index) const
        { return const_cast<Self*>(this)->getAttributeValue(index); }

    Self & operator=(const Self & ot) 
    { 
        matchedEdgeId_ = ot.matchedEdgeId_;
        latitude_ = ot.latitude_;
        longitude_ = ot.longitude_;
        distanceOnTrack_ = ot.distanceOnTrack_;
        directionOfTravel_ = ot.directionOfTravel_;
        objectId_ = ot.objectId_;
        origLatitude_ = ot.origLatitude_;
        origLongitude_ = ot.origLongitude_; 
        assignPayload(ot);
        return *this; 
    }

    Self & operator=(const Tuple & ot) 
    { 
        assignFrom(ot); 
        return *this; 
    }

    void assign(Tuple const & tuple)
    {
        *this = static_cast<SELF const &>(tuple);
    }


    bool operator==(const Self & ot) const 
    {  
       return ( 
                matchedEdgeId_ == ot.matchedEdgeId_ && 
                latitude_ == ot.latitude_ && 
                longitude_ == ot.longitude_ && 
                distanceOnTrack_ == ot.distanceOnTrack_ && 
                directionOfTravel_ == ot.directionOfTravel_ && 
                objectId_ == ot.objectId_ && 
                origLatitude_ == ot.origLatitude_ && 
                origLongitude_ == ot.origLongitude_  
              ); 
    }
    bool operator==(const Tuple & ot) const { return equals(ot); }

    bool operator!=(const Self & ot) const { return !(*this == ot); }
    bool operator!=(const Tuple & ot) const { return !(*this == ot); }


    void swap(SELF & ot) 
    { 
        std::swap(matchedEdgeId_, ot.matchedEdgeId_);
        std::swap(latitude_, ot.latitude_);
        std::swap(longitude_, ot.longitude_);
        std::swap(distanceOnTrack_, ot.distanceOnTrack_);
        std::swap(directionOfTravel_, ot.directionOfTravel_);
        std::swap(objectId_, ot.objectId_);
        std::swap(origLatitude_, ot.origLatitude_);
        std::swap(origLongitude_, ot.origLongitude_);
      std::swap(payload_, ot.payload_);
    }

    void reset()
    { 
        *this = SELF(); 
    }

    void normalizeBoundedSetsAndMaps(); 

    const std::string & getAttributeName(uint32_t index) const
    {
        if (index >= num_attributes)
            invalidIndex (index, num_attributes);
        return mappings_->indexToName_[index];
    }

    const std::tr1::unordered_map<std::string, uint32_t> & getAttributeNames() const 
        { return mappings_->nameToIndex_; }

protected:

    ValueHandle getAttributeValueRaw(const uint32_t index)
    {
        if (index >= num_attributes)
            invalidIndex(index, num_attributes);
        const TypeOffset & t = mappings_->indexToTypeOffset_[index];
        return ValueHandle((char*)this + t.getOffset(), t.getMetaType(), &t.getTypeId());
    }

private:
    
    matchedEdgeId_type matchedEdgeId_;
    latitude_type latitude_;
    longitude_type longitude_;
    distanceOnTrack_type distanceOnTrack_;
    directionOfTravel_type directionOfTravel_;
    objectId_type objectId_;
    origLatitude_type origLatitude_;
    origLongitude_type origLongitude_;

    static std::auto_ptr<TupleMappings> mappings_;
    static TupleMappings* initMappings();
};

inline VirtualByteBuffer & operator>>(VirtualByteBuffer & sbuf, SELF & tuple) 
    { tuple.deserialize(sbuf); return sbuf; }
inline VirtualByteBuffer & operator<<(VirtualByteBuffer & sbuf, SELF const & tuple) 
    { tuple.serialize(sbuf); return sbuf; }

template <class BufferType>
inline ByteBuffer<BufferType> & operator>>(ByteBuffer<BufferType> & sbuf, SELF & tuple) 
    { tuple.deserialize(sbuf); return sbuf; }
template <class BufferType>
inline ByteBuffer<BufferType> & operator<<(ByteBuffer<BufferType> & sbuf, SELF const & tuple) 
    { tuple.serialize(sbuf); return sbuf; }

inline NetworkByteBuffer & operator>>(NetworkByteBuffer & sbuf, SELF & tuple) 
    { tuple.deserialize(sbuf); return sbuf; }
inline NetworkByteBuffer & operator<<(NetworkByteBuffer & sbuf, SELF const & tuple) 
    { tuple.serialize(sbuf); return sbuf; }

inline NativeByteBuffer & operator>>(NativeByteBuffer & sbuf, SELF & tuple) 
    { tuple.deserialize(sbuf); return sbuf; }
inline NativeByteBuffer & operator<<(NativeByteBuffer & sbuf, SELF const & tuple) 
    { tuple.serialize(sbuf); return sbuf; }

template<>
inline std::ostream & serializeWithPrecision(std::ostream & ostr, SELF const & tuple) 
    { tuple.serializeWithPrecision(ostr); return ostr; }
inline std::ostream & operator<<(std::ostream & ostr, SELF const & tuple) 
    { tuple.serialize(ostr); return ostr; }
inline std::istream & operator>>(std::istream & istr, SELF & tuple) 
    { tuple.deserialize(istr); return istr; }
template<>
inline void deserializeWithSuffix(std::istream & istr, SELF  & tuple) 
{ tuple.deserialize(istr,true);  }
inline void deserializeWithNanAndInfs(std::istream & istr, SELF  & tuple, bool withSuffix = false) 
{ tuple.deserializeWithNanAndInfs(istr,withSuffix);  }



}  // namespace SPL

namespace std
{
    inline void swap(SPL::SELF & a, SPL::SELF & b)
    {
        a.swap(b);
    }
};

namespace std { 
    namespace tr1 {
        template <>
        struct hash<SPL::SELF> {
            inline size_t operator()(const SPL::SELF & self) const 
                { return self.hashCode(); }
        };
    }
}

#undef SELF
#endif // BEJW1ZEEKGCAQHEEZTRHZARKEOE0XMGEYKVOWQFMNRBV3_1SUNYKB9WHERYY_0EDGOVRLBCSLOLKFPVQON8IO2OXJHL6_1YZWURMTJJFCSNPPMBUMRESCGJVYUYHX4L6VT_13AB0ICWO_H_ 



