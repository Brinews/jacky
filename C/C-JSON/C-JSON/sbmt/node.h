#ifndef _NODE_H
#define _NODE_H

# include <string>
# include <vector>
# include <map>
# include <iostream>

typedef int Int;
typedef unsigned int UInt;

enum NodeType
{
	nullNode = 0, // 'null' value
	intNode,      // signed integer value
	uintNode,     // unsigned integer value
	realNode,     // double value
	stringNode,   // UTF-8 string
	booleanNode,  // boolean value
	arrayNode,    // vector array value 
	objectNode    // map object value 
};

class Node 
{
	public:
	typedef std::vector<std::string> Members;
	typedef UInt ArrayIndex;

	static const Node null;
	static const Int minInt;
	static const Int maxInt;
	static const UInt maxUInt;

	public:
	typedef std::map<std::string, Node> ObjectNodes;
	typedef std::vector<Node> ObjectArray;

	public:
	Node( NodeType type = nullNode );
	Node( Int value );
	Node( UInt value );
	Node( double value );
	Node( const char *value );
	Node( const std::string &value );

	Node( bool value );
	Node( const Node &other );
	~Node();

	Node &operator=( const Node &other );
	void swap( Node &other );

	NodeType type() const;

	bool operator ==( const Node &other ) const;
	bool operator !=( const Node &other ) const;

	std::string getString() const;
	Int getInt() const;
	UInt getUInt() const;
	double getDouble() const;
	bool getBool() const;
    bool isArray() const;
	bool isObject() const;
	UInt size() const;

    Node& find_value(const std::string &key);
    Node& get_value(int index);
    const Node& get_value(int index) const;
    const Node& find_value(const std::string &key) const;

	Members getMemberNames() const;
	std::string toString();

	private:
	union NodeHolder
	{
		Int int_;
		UInt uint_;
		double real_;
		bool bool_;
		char *string_;
		ObjectNodes *map_;
        ObjectArray *array_;
	} value_;

	NodeType type_;
};

#endif
