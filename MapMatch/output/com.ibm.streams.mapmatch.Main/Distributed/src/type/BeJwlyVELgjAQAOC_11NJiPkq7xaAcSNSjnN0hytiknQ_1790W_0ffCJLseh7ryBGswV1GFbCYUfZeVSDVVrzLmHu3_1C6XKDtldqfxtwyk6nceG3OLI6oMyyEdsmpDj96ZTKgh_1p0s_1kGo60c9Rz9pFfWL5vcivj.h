// t8y2_4NODE4EDGE10updateTypey3_3ADD6REMOVE5CLEAR11updateFlagsI8objectIdF8latitudeF9longitudeI11startNodeIdI9endNodeIdb8isOneWay


#ifndef BEJWLYVELGJAQAOC_11NJIPKQ7XAACSNSJNN0HYTIKNQ_1790W_0FFCJLSEH7RYBGSWV1GFBCYUFZEVSDVVRZLMHU3_1C6XKDTLDQFXTWYK6NCEG3OLI6OMYYEDSMPDJ96ZTKGH_1P0S_1KGO60C9RZ9PFFWL5VCIVJ_H_
#define BEJWLYVELGJAQAOC_11NJIPKQ7XAACSNSJNN0HYTIKNQ_1790W_0FFCJLSEH7RYBGSWV1GFBCYUFZEVSDVVRZLMHU3_1C6XKDTLDQFXTWYK6NCEG3OLI6OMYYEDSMPDJ96ZTKGH_1P0S_1KGO60C9RZ9PFFWL5VCIVJ_H_

#include <SPL/Runtime/Type/Tuple.h>
#include <SPL/Runtime/Type/PrimitiveType.h>
#include <SPL/Runtime/Type/CollectionType.h>
#include <SPL/Runtime/Serialization/NetworkByteBuffer.h>
#include <SPL/Runtime/Serialization/NativeByteBuffer.h>
#include <SPL/Runtime/Serialization/VirtualByteBuffer.h>

#include "BeJyrNIo38fN3cTVxdXF3BQAbDgOu.h"
#include "BeJyrNI43dnRxMQty9fUPczV19nF1DAIAPG8FCo.h"


#define SELF BeJwlyVELgjAQAOC_11NJiPkq7xaAcSNSjnN0hytiknQ_1790W_0ffCJLseh7ryBGswV1GFbCYUfZeVSDVVrzLmHu3_1C6XKDtldqfxtwyk6nceG3OLI6oMyyEdsmpDj96ZTKgh_1p0s_1kGo60c9Rz9pFfWL5vcivj

namespace SPL {

class SELF : public Tuple
{
public:
    static const bool facade = false;

    typedef SELF Self;
    
    typedef SPL::BeJyrNIo38fN3cTVxdXF3BQAbDgOu updateType_type;
    typedef SPL::BeJyrNI43dnRxMQty9fUPczV19nF1DAIAPG8FCo updateFlags_type;
    typedef SPL::int64 objectId_type;
    typedef SPL::float64 latitude_type;
    typedef SPL::float64 longitude_type;
    typedef SPL::int64 startNodeId_type;
    typedef SPL::int64 endNodeId_type;
    typedef SPL::boolean isOneWay_type;

    enum { num_attributes = 8 } ;
    
    SELF() : Tuple(), updateType_(), updateFlags_(), objectId_(), latitude_(), longitude_(), startNodeId_(), endNodeId_(), isOneWay_() {}
    SELF(const Self & ot) : Tuple(), updateType_(ot.updateType_), updateFlags_(ot.updateFlags_), objectId_(ot.objectId_), latitude_(ot.latitude_), longitude_(ot.longitude_), startNodeId_(ot.startNodeId_), endNodeId_(ot.endNodeId_), isOneWay_(ot.isOneWay_) 
      { constructPayload(ot); }
    SELF(const updateType_type & _updateType, const updateFlags_type & _updateFlags, const objectId_type & _objectId, const latitude_type & _latitude, const longitude_type & _longitude, const startNodeId_type & _startNodeId, const endNodeId_type & _endNodeId, const isOneWay_type & _isOneWay) : Tuple(), updateType_(_updateType), updateFlags_(_updateFlags), objectId_(_objectId), latitude_(_latitude), longitude_(_longitude), startNodeId_(_startNodeId), endNodeId_(_endNodeId), isOneWay_(_isOneWay) { }
    SELF(const Tuple & ot, bool typesafe = true) : Tuple() { assignFrom(ot, typesafe); }
    SELF(const ConstValueHandle & ot) : Tuple() { const Tuple & o = ot; assignFrom(o); }

    virtual ~SELF() {}
    
    updateType_type & get_updateType() { return updateType_; }
    const updateType_type & get_updateType() const { return updateType_; }
    void set_updateType(const updateType_type & _updateType) { updateType_ = _updateType; }
    
    updateFlags_type & get_updateFlags() { return updateFlags_; }
    const updateFlags_type & get_updateFlags() const { return updateFlags_; }
    void set_updateFlags(const updateFlags_type & _updateFlags) { updateFlags_ = _updateFlags; }
    
    objectId_type & get_objectId() { return objectId_; }
    const objectId_type & get_objectId() const { return objectId_; }
    void set_objectId(const objectId_type & _objectId) { objectId_ = _objectId; }
    
    latitude_type & get_latitude() { return latitude_; }
    const latitude_type & get_latitude() const { return latitude_; }
    void set_latitude(const latitude_type & _latitude) { latitude_ = _latitude; }
    
    longitude_type & get_longitude() { return longitude_; }
    const longitude_type & get_longitude() const { return longitude_; }
    void set_longitude(const longitude_type & _longitude) { longitude_ = _longitude; }
    
    startNodeId_type & get_startNodeId() { return startNodeId_; }
    const startNodeId_type & get_startNodeId() const { return startNodeId_; }
    void set_startNodeId(const startNodeId_type & _startNodeId) { startNodeId_ = _startNodeId; }
    
    endNodeId_type & get_endNodeId() { return endNodeId_; }
    const endNodeId_type & get_endNodeId() const { return endNodeId_; }
    void set_endNodeId(const endNodeId_type & _endNodeId) { endNodeId_ = _endNodeId; }
    
    isOneWay_type & get_isOneWay() { return isOneWay_; }
    const isOneWay_type & get_isOneWay() const { return isOneWay_; }
    void set_isOneWay(const isOneWay_type & _isOneWay) { isOneWay_ = _isOneWay; }
    
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
        buf << updateType_ << updateFlags_ << objectId_ << latitude_ << longitude_ << startNodeId_ << endNodeId_ << isOneWay_;
    }

    template <class BufferType>
    void serialize(ByteBuffer<BufferType> & buf) const
    {        
        buf << updateType_ << updateFlags_ << objectId_ << latitude_ << longitude_ << startNodeId_ << endNodeId_ << isOneWay_;
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
        buf >> updateType_ >> updateFlags_ >> objectId_ >> latitude_ >> longitude_ >> startNodeId_ >> endNodeId_ >> isOneWay_;
    }

    template <class BufferType>
    void deserialize(ByteBuffer<BufferType> & buf)
    {        
        buf >> updateType_ >> updateFlags_ >> objectId_ >> latitude_ >> longitude_ >> startNodeId_ >> endNodeId_ >> isOneWay_;
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
        s = 37 * s + std::tr1::hash<updateType_type >()(updateType_);
        s = 37 * s + std::tr1::hash<updateFlags_type >()(updateFlags_);
        s = 37 * s + std::tr1::hash<objectId_type >()(objectId_);
        s = 37 * s + std::tr1::hash<latitude_type >()(latitude_);
        s = 37 * s + std::tr1::hash<longitude_type >()(longitude_);
        s = 37 * s + std::tr1::hash<startNodeId_type >()(startNodeId_);
        s = 37 * s + std::tr1::hash<endNodeId_type >()(endNodeId_);
        s = 37 * s + std::tr1::hash<isOneWay_type >()(isOneWay_);
        return s;
    }
    
    size_t getSerializedSize() const
    {
        size_t size = sizeof(SPL::int64)+sizeof(SPL::float64)+sizeof(SPL::float64)+sizeof(SPL::int64)+sizeof(SPL::int64)+sizeof(SPL::boolean);
           size += updateType_.getSerializedSize();
   size += updateFlags_.getSerializedSize();

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
        updateType_ = ot.updateType_;
        updateFlags_ = ot.updateFlags_;
        objectId_ = ot.objectId_;
        latitude_ = ot.latitude_;
        longitude_ = ot.longitude_;
        startNodeId_ = ot.startNodeId_;
        endNodeId_ = ot.endNodeId_;
        isOneWay_ = ot.isOneWay_; 
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
                updateType_ == ot.updateType_ && 
                updateFlags_ == ot.updateFlags_ && 
                objectId_ == ot.objectId_ && 
                latitude_ == ot.latitude_ && 
                longitude_ == ot.longitude_ && 
                startNodeId_ == ot.startNodeId_ && 
                endNodeId_ == ot.endNodeId_ && 
                isOneWay_ == ot.isOneWay_  
              ); 
    }
    bool operator==(const Tuple & ot) const { return equals(ot); }

    bool operator!=(const Self & ot) const { return !(*this == ot); }
    bool operator!=(const Tuple & ot) const { return !(*this == ot); }


    void swap(SELF & ot) 
    { 
        std::swap(updateType_, ot.updateType_);
        std::swap(updateFlags_, ot.updateFlags_);
        std::swap(objectId_, ot.objectId_);
        std::swap(latitude_, ot.latitude_);
        std::swap(longitude_, ot.longitude_);
        std::swap(startNodeId_, ot.startNodeId_);
        std::swap(endNodeId_, ot.endNodeId_);
        std::swap(isOneWay_, ot.isOneWay_);
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
    
    updateType_type updateType_;
    updateFlags_type updateFlags_;
    objectId_type objectId_;
    latitude_type latitude_;
    longitude_type longitude_;
    startNodeId_type startNodeId_;
    endNodeId_type endNodeId_;
    isOneWay_type isOneWay_;

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
#endif // BEJWLYVELGJAQAOC_11NJIPKQ7XAACSNSJNN0HYTIKNQ_1790W_0FFCJLSEH7RYBGSWV1GFBCYUFZEVSDVVRZLMHU3_1C6XKDTLDQFXTWYK6NCEG3OLI6OMYYEDSMPDJ96ZTKGH_1P0S_1KGO60C9RZ9PFFWL5VCIVJ_H_ 



