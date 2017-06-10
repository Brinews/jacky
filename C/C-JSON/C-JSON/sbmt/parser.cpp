#include "parser.h"
#include "node.h"

#include <utility>
#include <cstdio>
#include <cassert>
#include <cstring>
#include <iostream>
#include <stdexcept>


static inline bool 
in( JSONParser::Char c, JSONParser::Char c1, JSONParser::Char c2, 
        JSONParser::Char c3, JSONParser::Char c4 )
{
   return c == c1  ||  c == c2  ||  c == c3  ||  c == c4;
}

static inline bool 
in( JSONParser::Char c, JSONParser::Char c1, JSONParser::Char c2, 
        JSONParser::Char c3, JSONParser::Char c4, JSONParser::Char c5 )
{
   return c == c1  ||  c == c2  ||  c == c3  ||  c == c4  ||  c == c5;
}


static bool 
containsNewLine( JSONParser::Cursor begin, 
                 JSONParser::Cursor end )
{
   for ( ;begin < end; ++begin )
      if ( *begin == '\n'  ||  *begin == '\r' )
         return true;
   return false;
}

JSONParser::JSONParser() {
}


bool
JSONParser::parse( const std::string &document, 
               Node &root,
               bool collectComments )
{
   doc_ = document;
   const char *begin = doc_.c_str();
   const char *end = begin + doc_.length();
   return parse( begin, end, root, collectComments );
}


bool
JSONParser::parse( std::istream& sin,
               Node &root,
               bool collectComments )
{
   std::string doc;
   std::getline(sin, doc, (char)EOF);

   return parse( doc, root, collectComments );
}

bool 
JSONParser::parse( const char *beginDoc, const char *endDoc, 
               Node &root,
               bool collectComments )
{

   start_ = beginDoc;
   tail_ = endDoc;
   current_ = start_;
   lastNodeEnd_ = 0;
   lastNode_ = 0;

   errors_.clear();

   while ( !nodes_.empty() )
      nodes_.pop();

   nodes_.push( &root );
   
   bool successful = parseNode();
   TokenNode tokenNode;

   parseTokenNode(tokenNode);

   if ( !root.isArray()  &&  !root.isObject() )
   {
	   tokenNode.type_ = tokenNodeWrong;
	   tokenNode.start_ = beginDoc;
	   tokenNode.tail_ = endDoc;
	   addWrong( "exptected an array or an object value;", tokenNode );
	   return false;
   }

   return successful;
}

bool
JSONParser::parseNode()
{
	TokenNode tokenNode;
	bool successful = true;

    parseTokenNode(tokenNode);

   switch ( tokenNode.type_ )
   {
   case tokenNodeObjectBegin:
      successful = parseObject( tokenNode );
      break;
   case tokenNodeArrayBegin:
      successful = parseArray( tokenNode );
      break;
   case tokenNodeNumber:
      successful = translateNumber( tokenNode );
      break;
   case tokenNodeString:
      successful = translateString( tokenNode );
      break;
   case tokenNodeTrue:
      currentNode() = true;
      break;
   case tokenNodeFalse:
      currentNode() = false;
      break;
   case tokenNodeNull:
      currentNode() = Node();
      break;
   default:
      return addWrong( "exptected : value, object or array;", tokenNode );
   }

   return successful;
}

bool 
JSONParser::expectTokenNode( TokenNodeType type, TokenNode &tokenNode, const char *message )
{
   parseTokenNode( tokenNode );
   if ( tokenNode.type_ != type )
      return addWrong( message, tokenNode );
   return true;
}


bool 
JSONParser::parseTokenNode( TokenNode &tokenNode )
{
   skipSpaces();
   tokenNode.start_ = current_;
   Char c = getNextChar();
   bool ok = true;
   switch ( c )
   {
   case '{':
      tokenNode.type_ = tokenNodeObjectBegin;
      break;
   case '}':
      tokenNode.type_ = tokenNodeObjectEnd;
      break;
   case '[':
      tokenNode.type_ = tokenNodeArrayBegin;
      break;
   case ']':
      tokenNode.type_ = tokenNodeArrayEnd;
      break;
   case '"':
      tokenNode.type_ = tokenNodeString;
      ok = parseString();
      break;
   case '0':
   case '1':
   case '2':
   case '3':
   case '4':
   case '5':
   case '6':
   case '7':
   case '8':
   case '9':
   case '-':
      tokenNode.type_ = tokenNodeNumber;
      parseNumber();
      break;
   case 't':
      tokenNode.type_ = tokenNodeTrue;
      ok = match( "rue", 3 );
      break;
   case 'f':
      tokenNode.type_ = tokenNodeFalse;
      ok = match( "alse", 4 );
      break;
   case 'n':
      tokenNode.type_ = tokenNodeNull;
      ok = match( "ull", 3 );
      break;
   case ',':
      tokenNode.type_ = tokenNodeArraySeparator;
      break;
   case ':':
      tokenNode.type_ = tokenNodeMemberSeparator;
      break;
   case 0:
      tokenNode.type_ = tokenNodeEndOfStream;
      break;
   default:
      ok = false;
      break;
   }
   if ( !ok )
      tokenNode.type_ = tokenNodeWrong;
   tokenNode.tail_ = current_;
   return true;
}


void 
JSONParser::skipSpaces()
{
   while ( current_ != tail_ )
   {
      Char c = *current_;
      if ( c == ' '  ||  c == '\t'  ||  c == '\r'  ||  c == '\n' )
         ++current_;
      else
         break;
   }
}

bool 
JSONParser::match( Cursor pattern, 
               int patternLength )
{
   if ( tail_ - current_ < patternLength )
      return false;
   int index = patternLength;
   while ( index-- )
      if ( current_[index] != pattern[index] )
         return false;
   current_ += patternLength;
   return true;
}

void 
JSONParser::parseNumber()
{
   while ( current_ != tail_ )
   {
      if ( !(*current_ >= '0'  &&  *current_ <= '9')  &&
           !in( *current_, '.', 'e', 'E', '+', '-' ) )
         break;
      ++current_;
   }
}

bool
JSONParser::parseString()
{
   Char c = 0;
   while ( current_ != tail_ )
   {
      c = getNextChar();
      if ( c == '\\' )
         getNextChar();
      else if ( c == '"' )
         break;
   }
   return c == '"';
}

bool 
JSONParser::parseObject( TokenNode &tokenNodeStart )
{
   TokenNode tokenNodeName;
   std::string name;
   currentNode() = Node( objectNode );
   while ( parseTokenNode( tokenNodeName ) )
   {
      bool initialTokenNodeOk = true;
      while ( tokenNodeName.type_ == tokenNodeComment  &&  initialTokenNodeOk )
         initialTokenNodeOk = parseTokenNode( tokenNodeName );
      if  ( !initialTokenNodeOk )
         break;
      if ( tokenNodeName.type_ == tokenNodeObjectEnd  &&  name.empty() ) 
         return true;
      if ( tokenNodeName.type_ != tokenNodeString )
         break;
      
      name = "";
      if ( !translateString( tokenNodeName, name ) )
         return recoverFromWrong( tokenNodeObjectEnd );

      TokenNode colon;
      if ( !parseTokenNode( colon ) ||  colon.type_ != tokenNodeMemberSeparator )
      {
         return addWrongAndRecover( "exptected <:>", colon, tokenNodeObjectEnd );
      }

      Node &value = currentNode().find_value(name);

      nodes_.push( &value );
      bool ok = parseNode();
      nodes_.pop();
      if ( !ok )
         return recoverFromWrong( tokenNodeObjectEnd );

      TokenNode comma;
      if ( !parseTokenNode( comma )
            ||  ( comma.type_ != tokenNodeObjectEnd  &&  
                  comma.type_ != tokenNodeArraySeparator ) )
      {
         return addWrongAndRecover( "exptected <,> or <}> ", comma, tokenNodeObjectEnd );
      }
      bool finalizeTokenNodeOk = true;
      while ( comma.type_ == tokenNodeComment && finalizeTokenNodeOk )
         finalizeTokenNodeOk = parseTokenNode( comma );
      if ( comma.type_ == tokenNodeObjectEnd )
         return true;
   }

   return addWrongAndRecover( "exptected <}> or object name", tokenNodeName, tokenNodeObjectEnd );
}


bool 
JSONParser::parseArray( TokenNode &tokenNodeStart )
{
   currentNode() = Node( arrayNode );
   skipSpaces();
   if ( *current_ == ']' )
   {
      TokenNode endArray;
      parseTokenNode( endArray );
      return true;
   }
   int index = 0;
   while ( true )
   {
	  Node &value = currentNode().get_value(index++);

      nodes_.push( &value );
      bool ok = parseNode();
      nodes_.pop();
      if ( !ok )
         return recoverFromWrong( tokenNodeArrayEnd );

      TokenNode tokenNode;
      ok = parseTokenNode( tokenNode );
      while ( tokenNode.type_ == tokenNodeComment  &&  ok )
      {
         ok = parseTokenNode( tokenNode );
      }
      bool badTokenNodeType = ( tokenNode.type_ == tokenNodeArraySeparator  &&  
                            tokenNode.type_ == tokenNodeArrayEnd );
      if ( !ok  ||  badTokenNodeType )
      {
         return addWrongAndRecover( "exptected <,> or <]>", tokenNode, 
                 tokenNodeArrayEnd );
      }
      if ( tokenNode.type_ == tokenNodeArrayEnd )
         break;
   }
   return true;
}


bool 
JSONParser::translateNumber( TokenNode &tokenNode )
{
   bool isDouble = false;
   for ( Cursor inspect = tokenNode.start_; inspect != tokenNode.tail_; ++inspect )
   {
      isDouble = isDouble  
                 ||  in( *inspect, '.', 'e', 'E', '+' )  
                 ||  ( *inspect == '-'  &&  inspect != tokenNode.start_ );
   }

   if ( isDouble )
      return translateDouble( tokenNode );

   Cursor current = tokenNode.start_;
   bool isNegative = (*current == '-');

   if ( isNegative )
      ++current;

   UInt threshold = (isNegative ? UInt(-Node::minInt) 
                                       : Node::maxUInt) / 10;

   UInt value = 0;
   while ( current < tokenNode.tail_ )
   {
      Char c = *current++;
      if ( value >= threshold ) return translateDouble( tokenNode );

      value = value * 10 + UInt(c - '0');
   }

   if ( isNegative )
      currentNode() = -Int( value );
   else if ( value <= UInt(Node::maxInt) )
      currentNode() = Int( value );
   else
      currentNode() = value;

   return true;
}


bool 
JSONParser::translateDouble( TokenNode &tokenNode )
{
   double value = 0;
   const int bufferSize = 32;
   Char buffer[bufferSize];

   int count;
   int length = int(tokenNode.tail_ - tokenNode.start_);

   memcpy( buffer, tokenNode.start_, length );
   buffer[length] = 0;
   count = sscanf( buffer, "%lf", &value );

   if ( count != 1 )
      return addWrong( "exptected a double <" + std::string( tokenNode.start_, tokenNode.tail_ ) + ">;", tokenNode );

   currentNode() = value;
   return true;
}


bool 
JSONParser::translateString( TokenNode &tokenNode )
{
   std::string translated;
   if ( !translateString( tokenNode, translated ) )
      return false;

   currentNode() = translated;

   return true;
}

static std::string codePointToUTF8(unsigned int cp)
{
   std::string result;
   
   if (cp <= 0x7f) 
   {
      result.resize(1);
      result[0] = static_cast<char>(cp);
   } 
   else if (cp <= 0x7FF) 
   {
      result.resize(2);
      result[1] = static_cast<char>(0x80 | (0x3f & cp));
      result[0] = static_cast<char>(0xC0 | (0x1f & (cp >> 6)));
   } 
   else if (cp <= 0xFFFF) 
   {
      result.resize(3);
      result[2] = static_cast<char>(0x80 | (0x3f & cp));
      result[1] = 0x80 | static_cast<char>((0x3f & (cp >> 6)));
      result[0] = 0xE0 | static_cast<char>((0xf & (cp >> 12)));
   }
   else if (cp <= 0x10FFFF) 
   {
      result.resize(4);
      result[3] = static_cast<char>(0x80 | (0x3f & cp));
      result[2] = static_cast<char>(0x80 | (0x3f & (cp >> 6)));
      result[1] = static_cast<char>(0x80 | (0x3f & (cp >> 12)));
      result[0] = static_cast<char>(0xF0 | (0x7 & (cp >> 18)));
   }

   return result;
}


bool 
JSONParser::translateString( TokenNode &tokenNode, std::string &translated )
{
   translated.reserve( tokenNode.tail_ - tokenNode.start_ - 2 );
   Cursor current = tokenNode.start_ + 1;
   Cursor end = tokenNode.tail_ - 1; 
   while ( current != end )
   {
      Char c = *current++;
      if ( c == '"' )
         break;
      else if ( c == '\\' )
      {
         Char escape = *current++;
         switch ( escape )
         {
         case '"': translated += '"'; break;
         case '/': translated += '/'; break;
         case '\\': translated += '\\'; break;
         case 'b': translated += '\b'; break;
         case 'f': translated += '\f'; break;
         case 'n': translated += '\n'; break;
         case 'r': translated += '\r'; break;
         case 't': translated += '\t'; break;
         case 'u':
            {
               unsigned int unicode;
               if ( !translateUnicodeCode( tokenNode, current, end, unicode ) )
                  return false;
               translated += codePointToUTF8(unicode);
            }
            break;
         default:
            return addWrong( "exptected normal escape string;", tokenNode );
         }
      }
      else
      {
         translated += c;
      }
   }
   return true;
}

bool 
JSONParser::addWrong( const std::string &message, TokenNode &tokenNode)
{
   WrongInfo info;
   info.tokenNode_ = tokenNode;
   info.message_ = message;
   errors_.push_back( info );
   return false;
}

Node &
JSONParser::currentNode()
{
   return *(nodes_.top());
}


JSONParser::Char 
JSONParser::getNextChar()
{
   if ( current_ == tail_ )
      return 0;
   return *current_++;
}



bool 
JSONParser::recoverFromWrong( TokenNodeType skipUntilTokenNode )
{
   int errorCount = int(errors_.size());
   TokenNode skip;
   while ( true )
   {
      if ( !parseTokenNode(skip) )
         errors_.resize( errorCount ); 
      if ( skip.type_ == skipUntilTokenNode  
              ||  skip.type_ == tokenNodeEndOfStream )
         break;
   }
   errors_.resize( errorCount );
   return false;
}


bool 
JSONParser::addWrongAndRecover( const std::string &message, 
                            TokenNode &tokenNode,
                            TokenNodeType skipUntilTokenNode )
{
   addWrong( message, tokenNode );
   return recoverFromWrong( skipUntilTokenNode );
}


void 
JSONParser::getCursorPosition( Cursor cur,
                                  int &line,
                                  int &column ) const
{
   Cursor current = start_;
   Cursor lastLineStart = current;
   Cursor nextLineStart = current;
   line = 0;

   while ( current < cur  &&  current != tail_ )
   {
      Char c = *current++;
      if ( c == '\r' )
      {
         if ( *current == '\n' )
            ++current;
		 lastLineStart = nextLineStart;
         nextLineStart = current;
         ++line;
      }
      else if ( c == '\n' )
      {
		 lastLineStart = nextLineStart;
         nextLineStart = current;
         ++line;
      }
   }

   do {
	   current--;
   } while (*current == ' ' || *current == '\t');


   if (*current == '\n') {
	   column = int(current - lastLineStart)+1;
   } else {
	   column = int(current - nextLineStart)+1;
	   ++line;
   }
}

std::string
JSONParser::getCursorPosition( Cursor cur ) const
{
   int line, column;
   getCursorPosition( cur, line, column );
   char buffer[51];
   sprintf( buffer, "line %d, position %d", line, column );
   return buffer;
}


std::string 
JSONParser::getWrongMessages() const
{
   std::string fmtMesg;
   for ( Wrongs::const_iterator itWrong = errors_.begin();
         itWrong != errors_.end();
         ++itWrong )
   {
      const WrongInfo &error = *itWrong;
      fmtMesg += " " + getCursorPosition( error.tokenNode_.start_ ) + ": ";
      fmtMesg += "  " + error.message_ + "\n";
   }
   return fmtMesg;
}

/* reference code */

bool
JSONParser::translateUnicodeCode( TokenNode &tokenNode, 
                                     Cursor &current, 
                                     Cursor end, 
                                     unsigned int &unicode )
{

   if ( !translateUnicode( tokenNode, current, end, unicode ) )
      return false;
   if (unicode >= 0xD800 && unicode <= 0xDBFF)
   {
      if (end - current < 6)
         return addWrong( "expected unicode to be <\\uxxxx>;", tokenNode );

      unsigned int surrogatePair;
      if (*(current++) == '\\' && *(current++)== 'u')
      {
         if (translateUnicode( tokenNode, current, end, surrogatePair ))
         {
            unicode = 0x10000 + ((unicode & 0x3FF) << 10) + (surrogatePair & 0x3FF);
         } 
         else
            return false;
      } 
      else
         return addWrong( "expected another \\u; ", tokenNode );
   }
   return true;
}

bool 
JSONParser::translateUnicode( TokenNode &tokenNode, 
                                     Cursor &current, 
                                     Cursor end, 
                                     unsigned int &unicode )
{
   if ( end - current < 4 )
      return addWrong( "exptected <xxxx> four digits;", tokenNode );

   unicode = 0;
   for ( int index =0; index < 4; ++index )
   {
      Char c = *current++;
      unicode *= 16;
      if ( c >= '0'  &&  c <= '9' )
         unicode += c - '0';
      else if ( c >= 'a'  &&  c <= 'f' )
         unicode += c - 'a' + 10;
      else if ( c >= 'A'  &&  c <= 'F' )
         unicode += c - 'A' + 10;
      else
         return addWrong( "exptected hexadecimal digit in unicode;", tokenNode);
   }
   return true;
}




std::istream& operator>>( std::istream &sin, Node &root )
{
    JSONParser parser;
    bool ok = parser.parse(sin, root, true);
    if (!ok) {
		std::cerr << parser.getWrongMessages();
	}
    return sin;
}


