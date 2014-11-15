#ifndef _STRING_HPP_
#define _STRING_HPP_

#include <iostream>

#include <assert.h>
 
class String {
public:

    String(const char* str=NULL);                //constructor
	String(int , const char *);
	String(int);
	String(char c);

    String(const String &other);                  	//copy (String)
    const String& operator=(const String &other);        	//operator=
    const String& operator+=(const String &other);  	//operator+

    bool operator==(const String&);              	//operator==

    size_t size() const 
	{ 
		return strlen(m_data); 
	};

	size_t length()const 
	{ 
		return strlen(m_data); 
	}

	String substr(int start, int num);
	int findstr(const String& str, int start);
	int findchar(const char c, int start);
	void swap(String &str);
	void resetCapacity(int size);

	int findNextBlank(int start);
	int findNextNonBlank(int start);
	const String& insertAt(const String &str, int pos);
	String justify(int len);

    char& operator[](int);                 //operator[]
	char operator[](int) const;

	const char* c_str() const { return m_data; }

	int capacity() const /* do a trick */
	{ 
		int m_cap;
		m_cap = 256;
		int len = strlen(m_data);

		while (m_cap < len) m_cap <<= 1;

		return m_cap; 
	}

    ~String(void) {delete[] m_data;}

private:
    char *m_data;
	int m_capacity;

	friend std::istream& operator>> (std::istream&, String &str);
    friend std::ostream& operator<< (std::ostream&, String&);
};

String operator+(const String &lhs, const String &rhs);
bool operator==(const String &lhs, const String &rhs);
bool operator>(const String &lhs, const String &rhs);
bool operator<(const String &lhs, const String &rhs);

#endif
