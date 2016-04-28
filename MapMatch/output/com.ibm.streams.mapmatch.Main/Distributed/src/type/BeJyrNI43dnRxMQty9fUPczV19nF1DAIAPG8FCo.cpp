// y3_3ADD6REMOVE5CLEAR


#include "BeJyrNI43dnRxMQty9fUPczV19nF1DAIAPG8FCo.h"

#define SELF BeJyrNI43dnRxMQty9fUPczV19nF1DAIAPG8FCo

using namespace SPL;

std::auto_ptr<EnumMappings> SELF::mappings_(SELF::initMappings());


SELF SELF::ADD(0);
SELF SELF::REMOVE(1);
SELF SELF::CLEAR(2);

SELF::SELF(const std::string & v)
: Enum(*mappings_)
{
    // initialize from a string value
    this->Enum::operator=(v);
}

SELF::SELF(const rstring & v)
: Enum(*mappings_)
{
    // initialize from a string value
    this->Enum::operator=(v);
}


EnumMappings* SELF::initMappings()
{
   EnumMappings* em = new EnumMappings();
   // initialize the mappings 
   {
      std::string s("ADD");
      em->nameToIndex_.insert(std::make_pair(s, 0));
      em->indexToName_.push_back(s);
   }
   
   {
      std::string s("REMOVE");
      em->nameToIndex_.insert(std::make_pair(s, 1));
      em->indexToName_.push_back(s);
   }
   
   {
      std::string s("CLEAR");
      em->nameToIndex_.insert(std::make_pair(s, 2));
      em->indexToName_.push_back(s);
   }
   
   return em;
}
