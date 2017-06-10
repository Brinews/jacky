#include "printer.h"
#include "util.h"

#include <utility>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <iomanip>


PrettyPrinter::PrettyPrinter()
   : rightGap_( 72 )
   , tabSize_( 2 )
{
}

std::string 
PrettyPrinter::print( const Node &root )
{
   doc_ = "";
   addSubNodes_ = false;
   tab_ = "";
   printNode( root );
   doc_ += "\n";
   return doc_;
}


void 
PrettyPrinter::printNode( const Node &value )
{
   switch ( value.type() )
   {
   case nullNode:
      addNode( "null" );
      break;
   case intNode:
      addNode( printValueString( value.getInt() ) );
      break;
   case uintNode:
      addNode( printValueString( value.getUInt() ) );
      break;
   case realNode:
      addNode( printValueString( value.getDouble() ) );
      break;
   case stringNode:
      addNode( printValueString( value.getString().c_str() ) );
      break;
   case booleanNode:
      addNode( printValueString( value.getBool() ) );
      break;
   case arrayNode:
      printArrayNode( value);
      break;
   case objectNode:
      {
         Node::Members members( value.getMemberNames() );
         if ( members.empty() )
            addNode( "{}" );
         else
         {
            printWithFormat( "{" );
            tab_ += std::string( tabSize_, ' ' );
            Node::Members::iterator it = members.begin();
            while ( true )
            {
               const std::string &name = *it;
               const Node &childNode = value.find_value(name);
               printWithFormat( printValueString( name.c_str() ) );
               doc_ += " : ";
               printNode( childNode );
               if ( ++it == members.end() )
               {
                  break;
               }
               doc_ += ",";
            }
            tab_.resize( tab_.size() - tabSize_ );
            printWithFormat( "}" );
         }
      }
      break;
   }
}

void 
PrettyPrinter::printArrayNode( const Node &value )
{
   unsigned size = value.size();
   if ( size == 0 )
      addNode( "[]" );
   else
   {
      bool isArrayLines = isMoreArray( value );
      if ( isArrayLines )
      {
         printWithFormat( "[" );
         tab_ += std::string( tabSize_, ' ' );
         bool hasChildNode = !subnodes.empty();
         unsigned idx =0;
         while ( true )
         {
            const Node &childNode = value.get_value(idx);
            if ( hasChildNode )
               printWithFormat( subnodes[idx] );
            else
            {
               printFormat();
               printNode( childNode );
            }
            if ( ++idx == size )
            {
               break;
            }
            doc_ += ",";
         }
         tab_.resize( tab_.size() - tabSize_ );
         printWithFormat( "]" );
      }
      else // output on a single line
      {
         assert( subnodes.size() == size );
         doc_ += "[ ";
         for ( unsigned idx =0; idx < size; ++idx )
         {
            if ( idx > 0 )
               doc_ += ", ";
            doc_ += subnodes[idx];
         }
         doc_ += " ]";
      }
   }
}


bool 
PrettyPrinter::isMoreArray( const Node &value )
{
   int size = value.size();
   bool hasLines = size*3 >= rightGap_ ;

   subnodes.clear();

   for ( int idx =0; idx < size  &&  !hasLines; ++idx )
   {
      const Node &childNode = value.get_value(idx);
      hasLines = hasLines  ||
                     ( (childNode.isArray()  
                        ||  childNode.isObject())  &&  
                        childNode.size() > 0 );
   }

   if ( !hasLines ) 
   {
      subnodes.reserve( size );
      addSubNodes_ = true;
      int lineLength = 4 + (size-1)*2;
      for ( int idx =0; idx < size  &&  !hasLines; ++idx )
      {
         printNode( value.get_value(idx) );
         lineLength += int( subnodes[idx].length() );
         hasLines = hasLines;
      }
      addSubNodes_ = false;
      hasLines = hasLines  ||  lineLength >= rightGap_;
   }
   return hasLines;
}


void 
PrettyPrinter::addNode( const std::string &value )
{
   if ( addSubNodes_ )
      subnodes.push_back( value );
   else
      doc_ += value;
}


void 
PrettyPrinter::printFormat()
{
   if ( !doc_.empty() )
   {
      char last = doc_[doc_.length()-1];
      if ( last == ' ' )  
         return;
      if ( last != '\n' )  
         doc_ += '\n';
   }
   doc_ += tab_;
}


void 
PrettyPrinter::printWithFormat( const std::string &value )
{
   printFormat();
   doc_ += value;
}

std::ostream& operator<<( std::ostream &sout, const Node &root )
{
   PrettyPrinter printr;
   std::string ret = printr.print(root);
   sout << ret;
   return sout;
}

