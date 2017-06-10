#ifndef JSON_PRINT
#define JSON_PRINT

# include "node.h"

# include <vector>
# include <string>
# include <iostream>


class Node;

class PrettyPrinter
{
    typedef std::vector<std::string> SubNodes;

	public:
		PrettyPrinter();
		~PrettyPrinter(){}
		std::string print( const Node &root );

	private:
		void printArrayNode( const Node &value );
		void printFormat();
		void printNode( const Node &value );

		void printWithFormat( const std::string &value );
		bool isMoreArray( const Node &value );
		void addNode( const std::string &value );

		SubNodes subnodes;
		std::string doc_;
		std::string tab_;
		int rightGap_;
		int tabSize_;
		bool addSubNodes_;
};

std::ostream& operator<<( std::ostream&, const Node &root );

#endif
