#include "String.hpp"

String::String(int cap)
{
	Capacity = cap;
	data = new char[Capacity];
}

String::String(int cap, const char *str)
{
	Capacity = cap;
	data = new char[Capacity];

	strcpy(data, str);
}

String::String(char c)
{
	Capacity = 256;
	data = new char[Capacity];
	assert(data != NULL);

	data[0] = c;
	data[1] = '\0';
}
 
String::String(const char* str)
{
	Capacity = 256;
	while (str != NULL && Capacity < strlen(str))
		Capacity <<= 1;

	data = new char[Capacity];

	if (str != NULL)
	{
		strcpy(data,str);
	}
}

String::String(const String& rhs)
{
	data=new char[strlen(rhs.data)+1];
	strcpy(data,rhs.data);
}

const String& String::operator=(const String& rhs)
{
    if (this != &rhs) {
        delete[] data;

        if(!rhs.data) data = NULL;
        else
        {
            data = new char[strlen(rhs.data)+1];
            strcpy(data,rhs.data);
        }
    }
    return *this;
}

const String& String::operator+=(const String &rhs)
{
	int len = strlen(data) + strlen(rhs.data);
	len ++;

	char *old_data = data;
	data = new char[len];
	strcpy(data, old_data);
	delete []old_data;

	strcat(data, rhs.data);
	
	return *this;
}

bool String::operator==(const String &s)    
{
    if ( strlen(s.data) != strlen(data) )
        return false;
    return strcmp(data,s.data)?false:true;
}

char& String::operator[](int e)
{
    if (e>=0&&e<=strlen(data))
        return data[e];
}

char String::operator[](int i) const
{
	return data[i];
}

String String::substr(int start, int num = -1)
{
	if (num < 0) num = strlen(data) - start;

	String subStr;
	subStr.data = new char(num+1);

	int j = 0;
	for (int i = start; --num >= 0; i++)
		subStr.data[j++] = data[i];

	subStr.data[j] = '\0';
	return subStr;
}

/* return -1 = no found */
int String::findstr(const String& str, int start)
{
	int ret = -1;

	for (int i = start; i < strlen(data); i++)
	{
		ret = i;
		for (int j = 0; j < str.length(); j++)
		{
			if (i+j >= strlen(data)
					|| data[i+j] != str.data[j])
			{
				ret = -1;
				break;
			}
		}

		if (ret != -1) return ret;
	}

	return -1;
}

int String::findchar(const char c, int start)
{
	for (int i = start; i < strlen(data); i++)
	{
		if (c == data[i]) return i;
	}

	return -1;
}

void String::swap(String &str)
{
	/* swap two pointer */
	char *tmp = str.data;
	str.data = this->data;
	this->data = tmp;
}

void String::resetCapacity(int size)
{
	assert(size > 0);

	char *old_data = data;
	data = new char[size+1];
	Capacity = size;

	int i;
	for (i = 0; i < size && i < strlen(old_data); i++)
		data[i] = old_data[i];

	data[i] = '\0';
	delete []old_data;
}

int String::findNextBlank(int start)
{
	int len = strlen(data);

	for (int i = start; i < len; i++)
	{
		if (isspace(data[i])) return i;
	}

	return -1;
}

int String::findNextNonBlank(int start)
{
	int len = strlen(data);

	for (int i = start; i < len; i++)
	{
		if (!isspace(data[i])) return i;
	}

	return -1;
}

const String& String::insertAt(const String &str, int pos)
{
	char *old_data = data;
	data = new char[strlen(data) + strlen(str.data) + 1];

	int index = 0;
	for (int i = 0; i < pos; i++) data[index++] = old_data[i];

	for (int i = 0; i < strlen(str.data); i++) 
		data[index++] = str.data[i];

	for (int i = pos; i < strlen(old_data); i++)
		data[index++] = old_data[i];

	data[index] = '\0';

	delete []old_data;

	return *this;
}

String String::justify(int len)
{
	String retStr(*this);
	int i =0;

	i = retStr.findNextBlank(i);
	if (i < 0) return retStr;

	while (retStr.length() < len) {
		i = retStr.findNextBlank(i);

		if (i < 0) {
			i = 0;
			continue;
		}

		retStr.insertAt(' ', i);

		i = retStr.findNextNonBlank(i);
		if (i < 0) i = 0;
	}

	return retStr;
}

String operator+(const String &lhs, const String &rhs)
{
	String ret = lhs;
	ret += rhs;
	return ret;
}

bool operator==(const String &lhs, const String &rhs)
{
	return strcmp(lhs.c_str(), rhs.c_str());
}

bool operator>(const String& lhs, const String& rhs)
{
	return strcmp(lhs.c_str(), rhs.c_str()) > 0;
}

bool operator<(const String& lhs, const String &rhs)
{
	return strcmp(lhs.c_str(), rhs.c_str()) < 0;
}

String  operator+       (const char ar[],  const String & rhs)
{
	String str(ar);
	str += rhs;

	return str;
}

String  operator+       (char c,          const String& rhs)
{
	String str(c);
	str += rhs;

	return str;
}

bool    operator==      (const char p[],  const String& rhs)
{
	return strcmp(p, rhs.c_str());
}

bool    operator==      (char c,          const String& rhs)
{
	const char *p = rhs.c_str();
	if (strlen(p) == 1 && p[0] == c) return true;

	return false;
}

bool    operator<       (const char p[],  const String& rhs)
{
	return strcmp(p, rhs.c_str()) < 0;
}

bool    operator<       (char c,          const String& rhs)
{
	return c - rhs[0] < 0;
}

std::ostream& operator<<(std::ostream& os,String& str)
{
    os << str.data;
    return os;
}

std::istream& operator>>(std::istream& in, String &str)
{
	char ch;
	str = "";

	in >> ch;

	if (!in.fail()) {
		do {
			str = str + ch;
			in.get(ch);
		} while (!in.fail() && !isspace(ch));

		if (isspace(ch)) in.putback(ch);
	}

	return in;
}
