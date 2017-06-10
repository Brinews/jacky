#ifndef _PARSER_H
#define _PARSER_H

# include "node.h"

# include <deque>
# include <stack>
# include <string>
# include <iostream>

class JSONParser
{
	public:
		typedef char Char;
		typedef const Char *Cursor;

		JSONParser();
		bool parse( const std::string &document, 
				Node &root,
				bool collectComments = true );
		bool parse( const char *beginDoc, const char *endDoc, 
				Node &root,
				bool collectComments = true );
		bool parse( std::istream &is,
				Node &root,
				bool collectComments = true );

		std::string getWrongMessages() const;

	private:
		enum TokenNodeType
		{
			tokenNodeEndOfStream = 0,
			tokenNodeObjectBegin,
			tokenNodeObjectEnd,
			tokenNodeArrayBegin,
			tokenNodeArrayEnd,
			tokenNodeString,
			tokenNodeNumber,
			tokenNodeTrue,
			tokenNodeFalse,
			tokenNodeNull,
			tokenNodeArraySeparator,
			tokenNodeMemberSeparator,
			tokenNodeComment,
			tokenNodeWrong
		};
		class TokenNode
		{
			public:
				TokenNodeType type_;
				Cursor start_;
				Cursor tail_;
		};
		class WrongInfo
		{
			public:
				TokenNode tokenNode_;
				std::string message_;
				Cursor extra_;
		};

		typedef std::deque<WrongInfo> Wrongs;
		typedef std::stack<Node *> Nodes;

        bool translateNumber( TokenNode &tokenNode );
		bool translateString( TokenNode &tokenNode );
		bool translateDouble( TokenNode &tokenNode );
		bool translateUnicodeCode( TokenNode &tokenNode, 
				Cursor &current, 
				Cursor end, 
				unsigned int &unicode );
		bool translateUnicode( TokenNode &tokenNode, 
				Cursor &current, 
				Cursor end, 
				unsigned int &unicode );
		bool translateString( TokenNode &tokenNode, std::string &translated );

        bool expectTokenNode( TokenNodeType type, TokenNode &tokenNode, const char *message );
		bool parseTokenNode( TokenNode &tokenNode );
		bool match( Cursor pattern, 
				int patternLength );
		void skipSpaces();

		bool parseNode();
		void parseNumber();
		bool parseObject( TokenNode &tokenNode );
		bool parseString();
		bool parseArray( TokenNode &tokenNode );

		bool addWrong( const std::string &message, TokenNode &tokenNode );
		bool addWrongAndRecover( const std::string &message, 
				TokenNode &tokenNode,
				TokenNodeType skipUntilTokenNode );
		void skipUntilSpace();
		bool recoverFromWrong( TokenNodeType skipUntilTokenNode );

		Node &currentNode();
		void getCursorPosition( Cursor cur,
				int &line,
				int &column ) const;
		Char getNextChar();
		std::string getCursorPosition( Cursor cur ) const;

		Nodes nodes_;
		Node *lastNode_;
		Wrongs errors_;
		std::string doc_;
		Cursor tail_;
		Cursor start_;
		Cursor lastNodeEnd_;
		Cursor current_;
};

std::istream& operator>>( std::istream&, Node& );

#endif
