#include "string.hpp"
#include <cstring>

/**
 * Default constructor
 */
String::String() : s(NULL), Capacity(0) {
  resetCapacity(std::max(1, DEFAULT_STRING_CAPACITY));
  s[0] = '\0';
}

/**
 * String of capacity n
 */
String::String(int n):s(NULL), Capacity(0) {
  resetCapacity(n);
}

/**
 * Create a string with a single character
 */
String::String(const char c) :  s(NULL), Capacity(0) {
  resetCapacity(std::max(2, DEFAULT_STRING_CAPACITY));
  s[0] = c;
  s[1] = '\0';
}

/**
 * Create a string with a char array
 */
String::String(const char str[]) : s(NULL), Capacity(0) {
  int strLen = strlen(str);
  resetCapacity(std::max(DEFAULT_STRING_CAPACITY, strLen + 1));
  for (int i = 0; i < strLen; i++) {
    s[i] = str[i];
  }
  s[strLen] = '\0';
}

/**
 * Copy constructor
 */
String::String(const String &str): s(NULL), Capacity(0) {
  int len = str.length();
  resetCapacity(std::max(DEFAULT_STRING_CAPACITY, len + 1));
  for (int i = 0; i < len; i++) {
    s[i] = str[i];
  }
  s[len] = '\0';
}


/**
* Overloads assign operator 
*/
String &String::operator = (String rhs) {
  int len = rhs.length();
  resetCapacity(std::max(DEFAULT_STRING_CAPACITY, len + 1));
  for (int i = 0; i < len; i++) {
    s[i] = rhs[i];
  }
  s[len] = '\0';
  return *this;
}

/**
 * String of capacity n with initial value 
 */
String::String(const int n, const char *str) :  s(NULL), Capacity(0) {
  int strLen = strlen(str);
  resetCapacity(std::max(n, strLen + 1));
  for (int i = 0; i < strLen; i++) {
    s[i] = str[i];
  }
  s[strLen] = '\0';
}

/**
 * Changes the capacity of your string keeping the contents intact. That is, 
 * create a new array and copy contents over to the new array, making sure to
 * clean up memory.
 */
void String::resetCapacity(int new_capacity) {
  /* Only increase the capacity.*/
  if (new_capacity <= Capacity) return;

  /* Create a new array, and copy contents over to the new array*/
  char *new_s = new char [new_capacity];
  if (s != NULL) {
    for (int i = 0; i < Capacity; i++) {
      new_s[i] = s[i];
    }
    /* Clean up memory*/
    delete[] s;
  }
  Capacity = new_capacity;
  s = new_s;
}

/**
 * Returns the capacity
 */
int String::capacity() const {
  return Capacity;
}

/**
 * Returns the length
 */
int String::length() const {
  return strlen(s);
}

/**
 * Overrides modifier operator[]
 */
char &String::operator [] (int idx) {
  if (idx >= 0 && idx < Capacity) {
    return s[idx];
  } else {
    return s[0];
  }
}


/**
 * Overrides accessor operator[]
 */
char String::operator [] (int idx) const {
  return s[idx];
}


/**
 * Overloads operator ==
 */
bool String::operator ==(const String &rhs) const {
  /* compare length */
  if (length() != rhs.length())
    return false;
  /* compare each character*/
  for (int i = 0; i < length(); i++) {
    if (s[i] != rhs[i]) return false;
  }
  return true;
}

/**
 * Overloads operator <
 */
bool String::operator < (const String &rhs) const {
  if (length() == 0) {
    if (rhs.length() == 0)
      return false;
    else
      return true;
  }
  else {
    int shorter = length();
    if (rhs.length() < shorter)
      shorter = rhs.length();
    for (int i = 0; i < shorter; i++) {
      if (s[i] == rhs[i])
        continue;
      else if (s[i] < rhs[i])
        return true;
      else
        return false;
    }
    if (length() == rhs.length())
      return false;
    else if (length() < rhs.length())
      return true;
    else
      return false;
  }
}

/**
 * Overloads operator +
 */
String String::operator + (const String &rhs) const {
  int len1 = length();
  int len2 = rhs.length();
  String res(std::max(DEFAULT_STRING_CAPACITY, length() + rhs.length() + 1));
  for (int i = 0; i < len1; i++) {
    res.s[i] = s[i];
  }
  for (int i = 0; i < len2; i++) {
    res.s[len1 + i] = rhs[i];
  }
  res.s[len1 + len2] = '\0';
  return res;
}

/**
 * Writes a string to the output stream
 */
std::ostream &operator <<(std::ostream &out, const String &str) {
  for (int i = 0; i < str.length(); i++) {
    out << str[i];
  }
  return out;
}

/**
 * Reads a string to the input stream
 */
std::istream &operator >> (std::istream &in, String &str) {
  char buf[1024];
  in >> buf;
  str = buf;
  return in;
}

/**
 * Overloads operator ==
 */
bool operator ==(const char s1[], const String &s2) {
  return String(s1) == s2;
}

/**
 * Overloads operator <
 */
bool operator < (const char s1[], const String &s2) {
  return String(s1) < s2;
}

/**
 * Overloads operator ==
 */
bool operator ==(const char s1, const String &s2) {
  return String(s1) == s2;
}

/**
 * Overloads operator <
 */
bool operator < (const char s1, const String &s2) {
  return String(s1) < s2;
}

/**
 * Overloads operator <=
 */
bool operator <=(const String &s1, const String &s2) {
  return s1 < s2 || s1 == s2;
}

/**
 * Overloads operator !=
 */
bool operator !=(const String &s1, const String &s2) {
  return !(s1 == s2);
}

/**
 * Overloads operator >=
 */
bool operator >=(const String &s1, const String &s2) {
  return !(s1 < s2);
}

/**
 * Overloads operator >
 */
bool operator > (const String &s1, const String &s2) {
  return !(s1 <= s2);
}

/**
 * Overloads operator +
 */
String operator + (const char s1[], const String &s2) {
  return String(s1) + s2;
}

/**
 * Overloads operator +
 */
String operator + (const char s1, const String &s2) {
  return String(s1) + s2;
}


/**
 * Returns a String comprised of the characters from the first idx up
 * to the second idx.
 */
String String::substr(int begin, int end) const {
  begin = std::max(begin, 0);
  end = std::min(end, length() - 1);
  if (begin >= 0 && end >= 0 && begin <= length() - 1 && end <= length() - 1
      && begin <= end) {
    String sub(end - begin + 2);
    for (int i = begin; i <= end; i++) {
      sub.s[i - begin] = s[i];
    }
    sub.s[end - begin + 1] = '\0';
    return sub;

  } else {
    return String();
  }
}

/*
 * Searches for a character in the string. Returns the first idx the same 
 * as or greater than the start idx where  the character occurs, or -1 if 
 * not found.
 */
int String::find_char(char ch, int idx) const {
  for (int i = idx; i < length(); i++) {
    if (s[i] == ch) return i;
  }
  return -1;
}

/**
 * Searches for a substring in the string. Returns the first idx the same 
 * as or greater than the start idx where  the character occurs, or -1 if 
 * not found.
 */
int String::find_substr(String subStr, int idx) const {
  for (int i = idx; i <= length() - subStr.length(); i++) {
    if (substr(i, i + subStr.length() - 1) == subStr) return i;
  }
  return -1;
}

/**
 * Implement a method to find the next blank in a String given a starting
 * position.
 */
int String::next_blank(int idx) const {
  int len = length();
  while (idx >= 0 && idx < len) {
    if (s[idx] == ' ') return idx;
    idx++;
  }
  return -1;
}

/**
 * Implement a method to find the next non blank 
 * in a String given a starting 
 * position.
 */
int String::next_non_blank(int idx) const {
  int len = length();

  while (idx >= 0 && idx < len) {
    if (s[idx] != ' ') return idx;
    idx++;
  }
  return -1;
}

/**
 * Implement a method to justify a String given a width.
 * Makes multiple passes through the string, 
 * adding blanks evenly left to right.
 */
String String::justify(int width) const {
  String line(*this);
  int i = 0;

  while (line.length() < width) {
    i = line.next_blank(i);

    if (i < 0) i = line.next_blank(0);

    line = line.substr(0, i - 1) + " " + line.substr(i, line.length() - 1);
    i = line.next_non_blank(i);
  }
  return line;

}

/**
 * Destructor  
 */
String::~String() {
  if (s) delete[] s;
}


