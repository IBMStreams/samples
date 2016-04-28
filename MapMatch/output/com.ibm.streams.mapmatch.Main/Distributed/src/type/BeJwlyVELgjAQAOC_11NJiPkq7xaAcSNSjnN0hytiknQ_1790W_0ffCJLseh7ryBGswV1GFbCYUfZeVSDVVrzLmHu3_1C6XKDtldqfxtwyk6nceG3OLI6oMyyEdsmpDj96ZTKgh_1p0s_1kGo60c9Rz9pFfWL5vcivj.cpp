// t8y2_4NODE4EDGE10updateTypey3_3ADD6REMOVE5CLEAR11updateFlagsI8objectIdF8latitudeF9longitudeI11startNodeIdI9endNodeIdb8isOneWay


#include "BeJwlyVELgjAQAOC_11NJiPkq7xaAcSNSjnN0hytiknQ_1790W_0ffCJLseh7ryBGswV1GFbCYUfZeVSDVVrzLmHu3_1C6XKDtldqfxtwyk6nceG3OLI6oMyyEdsmpDj96ZTKgh_1p0s_1kGo60c9Rz9pFfWL5vcivj.h"
#include <sstream>

#define SELF BeJwlyVELgjAQAOC_11NJiPkq7xaAcSNSjnN0hytiknQ_1790W_0ffCJLseh7ryBGswV1GFbCYUfZeVSDVVrzLmHu3_1C6XKDtldqfxtwyk6nceG3OLI6oMyyEdsmpDj96ZTKgh_1p0s_1kGo60c9Rz9pFfWL5vcivj

using namespace SPL;

std::auto_ptr<TupleMappings> SELF::mappings_(SELF::initMappings());

static void addMapping(TupleMappings & tm, TypeOffset & offset,
                       std::string const & name, uint32_t index)
{
    tm.nameToIndex_.insert(std::make_pair(name, index)); 
    tm.indexToName_.push_back(name);
    tm.indexToTypeOffset_.push_back(offset);    
}

static Tuple * initer() { return new SELF(); }

TupleMappings* SELF::initMappings()
{
    instantiators_.insert(std::make_pair("tuple<enum{NODE,EDGE} updateType,enum{ADD,REMOVE,CLEAR} updateFlags,int64 objectId,float64 latitude,float64 longitude,int64 startNodeId,int64 endNodeId,boolean isOneWay>",&initer));
    TupleMappings * tm = new TupleMappings();
#define MY_OFFSETOF(member, base) \
    ((uintptr_t)&reinterpret_cast<Self*>(base)->member) - (uintptr_t)base
   
    // initialize the mappings 
    
    {
        std::string s("updateType");
        TypeOffset t(MY_OFFSETOF(updateType_, tm), 
                     Meta::Type::typeOf<SPL::BeJyrNIo38fN3cTVxdXF3BQAbDgOu >(), 
                     &typeid(SPL::BeJyrNIo38fN3cTVxdXF3BQAbDgOu));
        addMapping(*tm, t, s, 0);
    }
    
    {
        std::string s("updateFlags");
        TypeOffset t(MY_OFFSETOF(updateFlags_, tm), 
                     Meta::Type::typeOf<SPL::BeJyrNI43dnRxMQty9fUPczV19nF1DAIAPG8FCo >(), 
                     &typeid(SPL::BeJyrNI43dnRxMQty9fUPczV19nF1DAIAPG8FCo));
        addMapping(*tm, t, s, 1);
    }
    
    {
        std::string s("objectId");
        TypeOffset t(MY_OFFSETOF(objectId_, tm), 
                     Meta::Type::typeOf<SPL::int64 >(), 
                     &typeid(SPL::int64));
        addMapping(*tm, t, s, 2);
    }
    
    {
        std::string s("latitude");
        TypeOffset t(MY_OFFSETOF(latitude_, tm), 
                     Meta::Type::typeOf<SPL::float64 >(), 
                     &typeid(SPL::float64));
        addMapping(*tm, t, s, 3);
    }
    
    {
        std::string s("longitude");
        TypeOffset t(MY_OFFSETOF(longitude_, tm), 
                     Meta::Type::typeOf<SPL::float64 >(), 
                     &typeid(SPL::float64));
        addMapping(*tm, t, s, 4);
    }
    
    {
        std::string s("startNodeId");
        TypeOffset t(MY_OFFSETOF(startNodeId_, tm), 
                     Meta::Type::typeOf<SPL::int64 >(), 
                     &typeid(SPL::int64));
        addMapping(*tm, t, s, 5);
    }
    
    {
        std::string s("endNodeId");
        TypeOffset t(MY_OFFSETOF(endNodeId_, tm), 
                     Meta::Type::typeOf<SPL::int64 >(), 
                     &typeid(SPL::int64));
        addMapping(*tm, t, s, 6);
    }
    
    {
        std::string s("isOneWay");
        TypeOffset t(MY_OFFSETOF(isOneWay_, tm), 
                     Meta::Type::typeOf<SPL::boolean >(), 
                     &typeid(SPL::boolean));
        addMapping(*tm, t, s, 7);
    }
    
    return tm;
}

void SELF::deserialize(std::istream & istr, bool withSuffix)
{
   std::string s;
   char c;

   istr >> c; if (!istr) { return; }
   if (c != '{') { istr.setstate(std::ios_base::failbit); return; }
   
   if (!readAttributeIdentifier(istr, s)) { return; }
   if (s != "updateType") { istr.setstate(std::ios_base::failbit); return; }
   istr >> c; if (!istr) { return; }
   if (c != '=') { istr.setstate(std::ios_base::failbit); return; }
   if (withSuffix)
     SPL::deserializeWithSuffix(istr, updateType_);
   else
     istr >> updateType_;
   if (!istr) { return; }  
   istr >> c; if (!istr) { return; }
   if (c != ',') { istr.setstate(std::ios_base::failbit); return; }

   if (!readAttributeIdentifier(istr, s)) { return; }
   if (s != "updateFlags") { istr.setstate(std::ios_base::failbit); return; }
   istr >> c; if (!istr) { return; }
   if (c != '=') { istr.setstate(std::ios_base::failbit); return; }
   if (withSuffix)
     SPL::deserializeWithSuffix(istr, updateFlags_);
   else
     istr >> updateFlags_;
   if (!istr) { return; }  
   istr >> c; if (!istr) { return; }
   if (c != ',') { istr.setstate(std::ios_base::failbit); return; }

   if (!readAttributeIdentifier(istr, s)) { return; }
   if (s != "objectId") { istr.setstate(std::ios_base::failbit); return; }
   istr >> c; if (!istr) { return; }
   if (c != '=') { istr.setstate(std::ios_base::failbit); return; }
   if (withSuffix)
     SPL::deserializeWithSuffix(istr, objectId_);
   else
     istr >> objectId_;
   if (!istr) { return; }  
   istr >> c; if (!istr) { return; }
   if (c != ',') { istr.setstate(std::ios_base::failbit); return; }

   if (!readAttributeIdentifier(istr, s)) { return; }
   if (s != "latitude") { istr.setstate(std::ios_base::failbit); return; }
   istr >> c; if (!istr) { return; }
   if (c != '=') { istr.setstate(std::ios_base::failbit); return; }
   if (withSuffix)
     SPL::deserializeWithSuffix(istr, latitude_);
   else
     istr >> latitude_;
   if (!istr) { return; }  
   istr >> c; if (!istr) { return; }
   if (c != ',') { istr.setstate(std::ios_base::failbit); return; }

   if (!readAttributeIdentifier(istr, s)) { return; }
   if (s != "longitude") { istr.setstate(std::ios_base::failbit); return; }
   istr >> c; if (!istr) { return; }
   if (c != '=') { istr.setstate(std::ios_base::failbit); return; }
   if (withSuffix)
     SPL::deserializeWithSuffix(istr, longitude_);
   else
     istr >> longitude_;
   if (!istr) { return; }  
   istr >> c; if (!istr) { return; }
   if (c != ',') { istr.setstate(std::ios_base::failbit); return; }

   if (!readAttributeIdentifier(istr, s)) { return; }
   if (s != "startNodeId") { istr.setstate(std::ios_base::failbit); return; }
   istr >> c; if (!istr) { return; }
   if (c != '=') { istr.setstate(std::ios_base::failbit); return; }
   if (withSuffix)
     SPL::deserializeWithSuffix(istr, startNodeId_);
   else
     istr >> startNodeId_;
   if (!istr) { return; }  
   istr >> c; if (!istr) { return; }
   if (c != ',') { istr.setstate(std::ios_base::failbit); return; }

   if (!readAttributeIdentifier(istr, s)) { return; }
   if (s != "endNodeId") { istr.setstate(std::ios_base::failbit); return; }
   istr >> c; if (!istr) { return; }
   if (c != '=') { istr.setstate(std::ios_base::failbit); return; }
   if (withSuffix)
     SPL::deserializeWithSuffix(istr, endNodeId_);
   else
     istr >> endNodeId_;
   if (!istr) { return; }  
   istr >> c; if (!istr) { return; }
   if (c != ',') { istr.setstate(std::ios_base::failbit); return; }

   if (!readAttributeIdentifier(istr, s)) { return; }
   if (s != "isOneWay") { istr.setstate(std::ios_base::failbit); return; }
   istr >> c; if (!istr) { return; }
   if (c != '=') { istr.setstate(std::ios_base::failbit); return; }
   if (withSuffix)
     SPL::deserializeWithSuffix(istr, isOneWay_);
   else
     istr >> isOneWay_;
   if (!istr) { return; }  
   istr >> c; if (!istr) { return; }
   
   if (c != '}') { istr.setstate(std::ios_base::failbit); return; }
}

void SELF::deserializeWithNanAndInfs(std::istream & istr, bool withSuffix)
{
   std::string s;
   char c;

   istr >> c; if (!istr) { return; }
   if (c != '{') { istr.setstate(std::ios_base::failbit); return; }
   
   if (!readAttributeIdentifier(istr, s)) { return; }
   if (s != "updateType") { istr.setstate(std::ios_base::failbit); return; }
   istr >> c; if (!istr) { return; }
   if (c != '=') { istr.setstate(std::ios_base::failbit); return; }
   SPL::deserializeWithNanAndInfs(istr, updateType_, withSuffix);
   if (!istr) { return; }  
   istr >> c; if (!istr) { return; }
   if (c != ',') { istr.setstate(std::ios_base::failbit); return; }

   if (!readAttributeIdentifier(istr, s)) { return; }
   if (s != "updateFlags") { istr.setstate(std::ios_base::failbit); return; }
   istr >> c; if (!istr) { return; }
   if (c != '=') { istr.setstate(std::ios_base::failbit); return; }
   SPL::deserializeWithNanAndInfs(istr, updateFlags_, withSuffix);
   if (!istr) { return; }  
   istr >> c; if (!istr) { return; }
   if (c != ',') { istr.setstate(std::ios_base::failbit); return; }

   if (!readAttributeIdentifier(istr, s)) { return; }
   if (s != "objectId") { istr.setstate(std::ios_base::failbit); return; }
   istr >> c; if (!istr) { return; }
   if (c != '=') { istr.setstate(std::ios_base::failbit); return; }
   SPL::deserializeWithNanAndInfs(istr, objectId_, withSuffix);
   if (!istr) { return; }  
   istr >> c; if (!istr) { return; }
   if (c != ',') { istr.setstate(std::ios_base::failbit); return; }

   if (!readAttributeIdentifier(istr, s)) { return; }
   if (s != "latitude") { istr.setstate(std::ios_base::failbit); return; }
   istr >> c; if (!istr) { return; }
   if (c != '=') { istr.setstate(std::ios_base::failbit); return; }
   SPL::deserializeWithNanAndInfs(istr, latitude_, withSuffix);
   if (!istr) { return; }  
   istr >> c; if (!istr) { return; }
   if (c != ',') { istr.setstate(std::ios_base::failbit); return; }

   if (!readAttributeIdentifier(istr, s)) { return; }
   if (s != "longitude") { istr.setstate(std::ios_base::failbit); return; }
   istr >> c; if (!istr) { return; }
   if (c != '=') { istr.setstate(std::ios_base::failbit); return; }
   SPL::deserializeWithNanAndInfs(istr, longitude_, withSuffix);
   if (!istr) { return; }  
   istr >> c; if (!istr) { return; }
   if (c != ',') { istr.setstate(std::ios_base::failbit); return; }

   if (!readAttributeIdentifier(istr, s)) { return; }
   if (s != "startNodeId") { istr.setstate(std::ios_base::failbit); return; }
   istr >> c; if (!istr) { return; }
   if (c != '=') { istr.setstate(std::ios_base::failbit); return; }
   SPL::deserializeWithNanAndInfs(istr, startNodeId_, withSuffix);
   if (!istr) { return; }  
   istr >> c; if (!istr) { return; }
   if (c != ',') { istr.setstate(std::ios_base::failbit); return; }

   if (!readAttributeIdentifier(istr, s)) { return; }
   if (s != "endNodeId") { istr.setstate(std::ios_base::failbit); return; }
   istr >> c; if (!istr) { return; }
   if (c != '=') { istr.setstate(std::ios_base::failbit); return; }
   SPL::deserializeWithNanAndInfs(istr, endNodeId_, withSuffix);
   if (!istr) { return; }  
   istr >> c; if (!istr) { return; }
   if (c != ',') { istr.setstate(std::ios_base::failbit); return; }

   if (!readAttributeIdentifier(istr, s)) { return; }
   if (s != "isOneWay") { istr.setstate(std::ios_base::failbit); return; }
   istr >> c; if (!istr) { return; }
   if (c != '=') { istr.setstate(std::ios_base::failbit); return; }
   SPL::deserializeWithNanAndInfs(istr, isOneWay_, withSuffix);
   if (!istr) { return; }  
   istr >> c; if (!istr) { return; }
   
   if (c != '}') { istr.setstate(std::ios_base::failbit); return; }
}

void SELF::serialize(std::ostream & ostr) const
{
    ostr << '{'
         << "updateType=" << get_updateType()  << ","
         << "updateFlags=" << get_updateFlags()  << ","
         << "objectId=" << get_objectId()  << ","
         << "latitude=" << get_latitude()  << ","
         << "longitude=" << get_longitude()  << ","
         << "startNodeId=" << get_startNodeId()  << ","
         << "endNodeId=" << get_endNodeId()  << ","
         << "isOneWay=" << get_isOneWay()  
         << '}';
}

void SELF::serializeWithPrecision(std::ostream & ostr) const
{
    ostr << '{';
    SPL::serializeWithPrecision(ostr << "updateType=", get_updateType()) << ",";
    SPL::serializeWithPrecision(ostr << "updateFlags=", get_updateFlags()) << ",";
    SPL::serializeWithPrecision(ostr << "objectId=", get_objectId()) << ",";
    SPL::serializeWithPrecision(ostr << "latitude=", get_latitude()) << ",";
    SPL::serializeWithPrecision(ostr << "longitude=", get_longitude()) << ",";
    SPL::serializeWithPrecision(ostr << "startNodeId=", get_startNodeId()) << ",";
    SPL::serializeWithPrecision(ostr << "endNodeId=", get_endNodeId()) << ",";
    SPL::serializeWithPrecision(ostr << "isOneWay=", get_isOneWay()) ;
    ostr << '}';
}

SELF& SELF::clear()
{
    get_updateType().clear();
    get_updateFlags().clear();
    get_objectId() = 0;
    get_latitude() = 0;
    get_longitude() = 0;
    get_startNodeId() = 0;
    get_endNodeId() = 0;
    get_isOneWay() = false;

    return *this;
}

void SELF::normalizeBoundedSetsAndMaps()
{
    SPL::normalizeBoundedSetsAndMaps(*this);
}


