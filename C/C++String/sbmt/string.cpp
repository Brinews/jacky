#include "String.hpp"

String::String(int s)
{
	m_capacity = s;
	m_data = NULL;
}

String::String(int s, const char *str)
{
	m_capacity = s;
	if (str)
	{
		m_data = new char[strlen(str) + 1];
		strcpy(m_data, str);
	}
}

String::String(char c)
{
	m_data = new char[2];
	assert(m_data != NULL);

	m_data[0] = c;
	m_data[1] = '\0';
}
 
String::String(const char* str)
{
    if (!str) 
	{
		m_capacity = 256;
		m_data = new char[m_capacity];
	}
    else
    {
        m_data = new char[strlen(str)+1];
        strcpy(m_data,str);
    }
}

String::String(const String& other)
{
    if(!other.m_data) m_data=0;
    else
    {
        m_data=new char[strlen(other.m_data)+1];
        strcpy(m_data,other.m_data);
    }
}

const String& String::operator=(const String& other)
{
    if (this != &other)
    {
        delete[] m_data;
        if(!other.m_data) m_data=0;
        else
        {
            m_data = new char[strlen(other.m_data)+1];
            strcpy(m_data,other.m_data);
        }
    }
    return *this;
}

const String& String::operator+=(const String &other)
{
	int len = strlen(m_data) + strlen(other.m_data);
	len ++;

	char *old = m_data;
	m_data = new char[len];
	strcpy(m_data, old);
	delete []old;

	strcat(m_data, other.m_data);
	
	return *this;
}

bool String::operator==(const String &s)    
{
    if ( strlen(s.m_data) != strlen(m_data) )
        return false;
    return strcmp(m_data,s.m_data)?false:true;
}

char& String::operator[](int e)
{
    if (e>=0&&e<=strlen(m_data))
        return m_data[e];
}

char String::operator[](int i) const
{
	return m_data[i];
}

String String::substr(int start, int num = -1)
{
	assert(!(num > strlen(m_data)));
	assert(!(start < 0 || start >= strlen(m_data)));

	if (num < 0) num = strlen(m_data) - start;

	String subStr;
	subStr.m_data = new char(num+1);
	assert(subStr.m_data != NULL);

	int j = 0;
	for (int i = start; --num >= 0; i++)
		subStr.m_data[j++] = m_data[i];

	subStr.m_data[j] = '\0';

	return subStr;
}

/* -1 mean no found */
int String::findstr(const String& str, int start)
{
	int ret = -1;

	assert(!(start > strlen(m_data)));

	for (int i = start; i < strlen(m_data); i++)
	{
		ret = i;
		for (int j = 0; j < str.length(); j++)
		{
			if (i+j >= strlen(m_data)
					|| m_data[i+j] != str.m_data[j])
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
	assert(!(start > strlen(m_data)));

	for (int i = start; i < strlen(m_data); i++)
	{
		if (c == m_data[i]) return i;
	}

	return -1;
}

void String::swap(String &str)
{
	/* swap two pointer */
	char *tmp = str.m_data;
	str.m_data = this->m_data;
	this->m_data = tmp;
}

void String::resetCapacity(int size)
{
	assert(size > 0);

	char *old = m_data;
	m_data = new char[size+1];
	m_capacity = size;

	int i;
	for (i = 0; i < size && i < strlen(old); i++)
		m_data[i] = old[i];

	m_data[i] = '\0';
	delete []old;
}

int String::findNextBlank(int start)
{
	int len = strlen(m_data);

	for (int i = start; i < len; i++)
	{
		if (isspace(m_data[i])) return i;
	}

	return -1;
}

int String::findNextNonBlank(int start)
{
	int len = strlen(m_data);

	for (int i = start; i < len; i++)
	{
		if (!isspace(m_data[i])) return i;
	}

	return -1;
}

const String& String::insertAt(const String &str, int pos)
{
	char *old = m_data;
	m_data = new char[strlen(m_data) + strlen(str.m_data) + 1];

	int index = 0;
	for (int i = 0; i < pos; i++) m_data[index++] = old[i];

	for (int i = 0; i < strlen(str.m_data); i++) 
		m_data[index++] = str.m_data[i];

	for (int i = pos; i < strlen(old); i++)
		m_data[index++] = old[i];

	m_data[index] = '\0';

	delete []old;

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

		//std::cout << "One insert:" << retStr << std::endl;

		i = retStr.findNextNonBlank(i);

		if (i < 0) i = 0;
	}

	return retStr;
}

std::ostream& operator<<(std::ostream& os,String& str)
{
    os << str.m_data;
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
