#ifndef _STRING_HPP_
#define _STRING_HPP_

#include <iostream>

#include <assert.h>
 
class String {
public:

	String(int , const char *);
	String(int);
	String(char c);
    String(const char* str=NULL);                

	/* part I */
    String(const String &other);                  //copy (String)
    const String& operator=(const String &other); //operator=
    const String& operator+=(const String &other);//operator+
    bool operator==(const String&);               //operator==

	String substr(int start, int num);
	int findstr(const String& str, int start);
	int findchar(const char c, int start);

	/* part II */
	void swap(String &str);
	void resetCapacity(int size);

	/* part III */
	int findNextBlank(int start);
	int findNextNonBlank(int start);
	const String& insertAt(const String &str, int pos);
	String justify(int len);

    char& operator[](int);                 //operator[]
	char operator[](int) const;

	const char* c_str() const { return data; }
	size_t length()const { return strlen(data); }
	int capacity() const { return Capacity; }

    ~String(void) {delete[] data;}

private:
    char *data;
	int Capacity;

	friend std::istream& operator>> (std::istream&, String &str);
    friend std::ostream& operator<< (std::ostream&, String&);
};

String operator+(const String &lhs, const String &rhs);
bool operator==(const String &lhs, const String &rhs);
bool operator>(const String &lhs, const String &rhs);
bool operator<(const String &lhs, const String &rhs);

String  operator+       (const char[],  const String&);
String  operator+       (char,          const String&);
bool    operator==      (const char[],  const String&);
bool    operator==      (char,          const String&);
bool    operator<       (const char[],  const String&);
bool    operator<       (char,          const String&);

#endif
