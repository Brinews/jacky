#include <iostream>
#include <utility>
#include <stdexcept>
#include <cstring>
#include <cassert>
#include <cstddef>

#include "node.h"
#include "printer.h"
#include "util.h"

#define ASSERT_UNREACHABLE assert( false )
#define ASSERT( condition ) assert( condition );  

const Node Node::null;
const Int Node::minInt = Int( ~(UInt(-1)/2) );
const Int Node::maxInt = Int( UInt(-1)/2 );
const UInt Node::maxUInt = UInt(-1);

Node::Node( NodeType type )
   : type_( type )
{
   switch ( type )
   {
   case nullNode:
      break;
   case intNode:
   case uintNode:
      value_.int_ = 0;
      break;
   case realNode:
      value_.real_ = 0.0;
      break;
   case stringNode:
      value_.string_ = 0;
      break;
   case arrayNode:
      value_.array_ = new ObjectArray();
      break;
   case objectNode:
      value_.map_ = new ObjectNodes();
      break;
   case booleanNode:
      value_.bool_ = false;
      break;
   default:
      ASSERT_UNREACHABLE;
   }
}

Node::Node( Int value )
   : type_( intNode )
{
   value_.int_ = value;
}


Node::Node( UInt value )
   : type_( uintNode )
{
   value_.uint_ = value;
}

Node::Node( double value )
   : type_( realNode )
{
   value_.real_ = value;
}

Node::Node( const char *value )
   : type_( stringNode )
{
    value_.string_ = (char *) new char[strlen(value)+1];
    strcpy(value_.string_, value);
}


Node::Node( const std::string &value )
   : type_( stringNode )
{
    value_.string_ = (char *) new char[value.length()+1];
    strcpy(value_.string_, value.c_str());
}


Node::Node( bool value )
   : type_( booleanNode )
{
   value_.bool_ = value;
}


Node::Node( const Node &other )
   : type_( other.type_ )
{
   switch ( type_ )
   {
   case nullNode:
   case intNode:
   case uintNode:
   case realNode:
   case booleanNode:
      value_ = other.value_;
      break;
   case stringNode:
      if ( other.value_.string_ )
      {
         value_.string_ = (char *) new char[strlen(other.value_.string_)+1];
         strcpy(value_.string_, other.value_.string_);
      }
      else
         value_.string_ = 0;
      break;
   case arrayNode:
      value_.array_ = new ObjectArray( *other.value_.array_);
      break;
   case objectNode:
      value_.map_ = new ObjectNodes( *other.value_.map_ );
      break;
   default:
      ASSERT_UNREACHABLE;
   }
}


Node::~Node()
{
   switch ( type_ )
   {
   case nullNode:
   case intNode:
   case uintNode:
   case realNode:
   case booleanNode:
      break;
   case stringNode:
	  delete value_.string_;
      break;
   case arrayNode:
      delete value_.array_;
      break;
   case objectNode:
      delete value_.map_;
      break;
   default:
      ASSERT_UNREACHABLE;
   }
}

Node &
Node::operator=( const Node &other )
{
   Node temp( other );
   swap( temp );
   return *this;
}

void 
Node::swap( Node &other )
{
   NodeType temp = type_;
   type_ = other.type_;
   other.type_ = temp;
   std::swap( value_, other.value_ );
}

NodeType 
Node::type() const
{
   return type_;
}

bool 
Node::operator ==( const Node &other ) const
{
   int temp = other.type_;
   if ( type_ != temp )
      return false;
   switch ( type_ )
   {
   case nullNode:
      return true;
   case intNode:
      return value_.int_ == other.value_.int_;
   case uintNode:
      return value_.uint_ == other.value_.uint_;
   case realNode:
      return value_.real_ == other.value_.real_;
   case booleanNode:
      return value_.bool_ == other.value_.bool_;
   case stringNode:
      return ( value_.string_ == other.value_.string_ )
             || ( other.value_.string_  
                  &&  value_.string_  
                  && strcmp( value_.string_, other.value_.string_ ) == 0 );
   case arrayNode:
      return value_.array_->size() == other.value_.array_->size()
             && (*value_.array_) == (*other.value_.array_);
   case objectNode:
      return value_.map_->size() == other.value_.map_->size()
             && (*value_.map_) == (*other.value_.map_);
   default:
      ASSERT_UNREACHABLE;
   }
   return 0; 
}

bool 
Node::operator !=( const Node &other ) const
{
   return !( *this == other );
}

std::string 
Node::getString() const
{
    return value_.string_ ? value_.string_ : "";
}

Int 
Node::getInt() const
{
    return value_.int_;
}

UInt 
Node::getUInt() const
{
    return value_.uint_;
}

double 
Node::getDouble() const
{
    return value_.real_;
}

bool 
Node::getBool() const
{
    return value_.bool_;
}

UInt 
Node::size() const
{
   switch ( type_ )
   {
   case nullNode:
   case intNode:
   case uintNode:
   case realNode:
   case booleanNode:
   case stringNode:
      return 0;
   case arrayNode: 
      return Int( value_.array_->size() );
   case objectNode:
      return Int( value_.map_->size() );
   default:
      ASSERT_UNREACHABLE;
   }
   return 0;
}



Node& Node::get_value(int index)
{
    if (value_.array_->size() > index) {
        return value_.array_->at(index);
    } else {
        while (value_.array_->size() <= index) {
            value_.array_->push_back(null);
        }
        return value_.array_->at(index);
    }
}

const Node& Node::get_value(int index) const
{
    if (value_.array_->size() > index) {
        return value_.array_->at(index);
    } else {
		return null;
    }
}


Node& Node::find_value(const std::string &key)
{
    ObjectNodes::iterator it = value_.map_->find(key.c_str());
    if (it != value_.map_->end() && (*it).first == key) {
        return (*it).second;
    }
    ObjectNodes::value_type defVal(key, null);
    it = value_.map_->insert(it, defVal);
    return (*it).second;
}


const Node& Node::find_value(const std::string &key) const
{
    ObjectNodes::iterator it = value_.map_->find(key.c_str());
    if (it != value_.map_->end() && (*it).first == key) {
        return (*it).second;
    }

    return null;
}


Node::Members 
Node::getMemberNames() const
{
   ASSERT( type_ == nullNode  ||  type_ == objectNode );
   if ( type_ == nullNode )
       return Node::Members();
   Members members;
   members.reserve( value_.map_->size() );
   ObjectNodes::const_iterator it = value_.map_->begin();
   ObjectNodes::const_iterator itEnd = value_.map_->end();
   for ( ; it != itEnd; ++it )
      members.push_back( std::string( (*it).first.c_str() ) );

   return members;
}

bool 
Node::isArray() const
{
   return type_ == nullNode  ||  type_ == arrayNode;
}


bool 
Node::isObject() const
{
   return type_ == nullNode  ||  type_ == objectNode;
}


std::string Node::toString()
{
   switch ( type_ )
   {
   case nullNode:
	   return "null";
   case intNode:
	   return printValueString(value_.int_);
   case uintNode:
      return printValueString(value_.uint_);
   case realNode:
      return printValueString(value_.real_);
   case booleanNode:
      return printValueString(value_.bool_);
   case stringNode:
      return printValueString(value_.string_);
   case arrayNode:
	  {
		  std::string document_ = "[";
         int size1 = size();
         for ( int index =0; index < size1; ++index )
         {
            if ( index > 0 )
               document_ += ",";
            document_ += get_value(index).toString();
         }
         document_ += "]";

		 return document_;
	  }
   case objectNode:
	  {
         Node::Members members( getMemberNames() );
		 std::string document_ = "{";
         for ( Node::Members::iterator it = members.begin(); 
               it != members.end(); 
               ++it )
         {
            const std::string &name = *it;
            if ( it != members.begin() )
               document_ += ",";
            document_ += printValueString( name.c_str() );
            document_ +=  ": ";
            document_ += find_value(name).toString();
         }
         document_ += "}";
		 return document_;
	  }
   default:
	  return "";
   }
}
