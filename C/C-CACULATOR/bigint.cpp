/*
 * bigint.cpp
 *
 * Author: Oscar Benjamin
 * Date: 20 Mar 2014
 *
 * Main code file for the BigInt class. The BigInt class represents integers
 * with unlimited precision. There will be no overflow in the BigInt class
 * unless the computer runs out of memory. BigInt objects are slower than
 * ordinary C/C++ int, long etc. but not needing to worry about overflow makes
 * them more useful for certain situations.
 */

#include <iostream>
#include <string>
#include <limits.h>
#include "bigint.h"

/*
 * BigInt constructors:
 *
 * BigInt()       -- give zero
 * BigInt(int)    -- convert int to a BigInt
 * BigInt(string) -- convert decimal string to BigInt
 */

// Default constructor - set to zero
BigInt::BigInt() {
  digits = std::vector<unsigned char>();
  _sign = false;
}

// Constructor from an int
BigInt::BigInt(const int& i) {
  // The standard guarantees overflow behaviour for this particular cast (even
  // if the underlying system does not use 2's complement).
  unsigned int ui = (unsigned int)i;
  if(i < 0) {
    _sign = true;
    ui = UINT_MAX - ui + 1;
  }
  else {
    _sign = false;
  }
  // Generate the digits in reverse order
  digits = std::vector<unsigned char>();
  while(ui) {
    digits.push_back(ui % 10);
    ui /= 10;
  }
}

BigInt::BigInt(const std::string& str) {
  // First check for an empty string
  if(str.empty()) {
    std::cerr << "empty string input!!!!!!!" << std::endl;
    return;
  }

  // Initialise before reading the string
  _sign = false;
  digits = std::vector<unsigned char>();

  // Work backwards from end of string
  for(unsigned long i=str.size()-1; i>0; --i) {
    if(str[i] < '0' || str[i] > '9') {
      std::cerr << "Invalid character in input!!!!!" << std::endl;
      return;
    }
    else {
      digits.push_back((unsigned char)(str[i] - '0'));
    }
  }
  // Test last character
  if('0' <= str[0] && str[0] <= '9')
    digits.push_back((unsigned char)(str[0] - '0'));
  else if(str[0] == '-')
    _sign = true;
  else {
    std::cerr << "Invalid character in input!!!!!" << std::endl;
    return;
  }
  // Pop off any leading zeros e.g. 000123 -> 123
  while(digits.size() && digits[digits.size()-1] == 0)
    digits.pop_back();
  // If we pop all the digits then we have zero and need set the sign to +ve
  if(digits.empty())
    _sign = false; // +ve
}

/*
 * BigInt ostream << function.
 *
 * So that we can print a BigInt to e.g. stdout.
 */

std::ostream& operator<<(std::ostream& out, BigInt bi)
{
  if(!bi.size()) {
    out << "0";
  }
  else {
    if(bi.sign())
      out << "-";
    // Digits are stored in the reverse of display order
    for(unsigned long i=bi.size(); i != 0; i--) {
      out << (char)('0' + bi[i-1]);
    }
  }
  return out; // return out for chaining
}

/*
 * BigInt arithmetic operators:
 *  b1 + b2
 *  b1 - b2
 *  -b1
 */

BigInt operator+ (const BigInt& lhs, const BigInt& rhs)
{
  return lhs.add(rhs);
}

BigInt operator- (const BigInt& lhs, const BigInt& rhs)
{
  return lhs.subtract(rhs);
}

BigInt operator* (const BigInt& lhs, const BigInt& rhs)
{
  return lhs.multiply(rhs);
}

BigInt operator/ (const BigInt& lhs, const BigInt& rhs)
{
  return lhs.divide(rhs);
}

BigInt operator% (const BigInt& lhs, const BigInt& rhs)
{
  return lhs.modulo(rhs);
}

BigInt operator- (const BigInt& bi)
{
  return bi.negate();
}

bool operator< (const BigInt& lhs, const BigInt& rhs)
{
  return lhs.compare(rhs) == 1;
}

bool operator> (const BigInt& lhs, const BigInt& rhs)
{
  return lhs.compare(rhs) == -1;
}

bool operator== (const BigInt& lhs, const BigInt& rhs)
{
  return lhs.compare(rhs) == 0;
}

/*
 * BigInt arithmetic methods.
 *
 * These implement the actions associated with the above operators.
 */

// Return a negative version of this BigInt
BigInt BigInt::negate() const
{
  // Make a copy
  BigInt result = *this;
  result._sign = !result._sign;
  return result;
}

// Add one BigInt to another
BigInt BigInt::add(const BigInt& other) const
{
  // Use subtraction for mixed sign inputs
  if(sign() != other.sign())
    return subtract(other.negate());

  // Compute addition for same sign inputs - copies sign bit
  BigInt result = *this; // copy this object

  // Add each digit from other to the copy
  for(unsigned long i=0; i<other.size(); ++i)
    result.add_at_digit(other[i], i);
  return result;
}

// Subtract one BigInt from another
BigInt BigInt::subtract(const BigInt& other) const
{
  // Use addition for mixed sign inputs - calls operator+
  if(sign() != other.sign())
    return add(other.negate());

  // Only ever subtract smaller magnitude from larger magnitude
  if(compare_magnitude(other) == 1) {
    // return - (y - x) which is the same as x - y
    return other.subtract(*this).negate();
  }

  // Compute subtraction for same sign inputs - copies sign bit
  BigInt result = *this; // copy this object

  // Subtract each digit in other from the copy
  for(unsigned long i=0; i<other.size(); ++i)
    result.sub_at_digit(other[i], i);
  return result;
}

// Multiply two BigInts using long multiplication.
// This is not efficient for large inputs.
BigInt BigInt::multiply(const BigInt& other) const
{
  // We start with 0 and add parts of the result incrementally
  BigInt result;
  unsigned char miniproduct; // product of two digits

  for(unsigned long i=0; i<size(); ++i) {
    for(unsigned long j=0; j<other.size(); ++j) {
      // miniproduct is at most 81 - overflow impossible
      miniproduct = digits[i] * other.digits[j];
      if(miniproduct < 10) {
        result.add_at_digit(miniproduct, i+j);
      }
      else {
        result.add_at_digit(miniproduct % 10, i+j);
        result.add_at_digit(miniproduct / 10, i+j+1);
      }
    }
  }
  result._sign = sign() ^ other.sign();
  return result;
}

// Divide two BigInts using long division (sort of)
// This is not efficient for large inputs
// This computes integer division rounded towards zero
BigInt BigInt::divide(const BigInt& other) const
{
  if(other.compare(BigInt(0)) == 0) {
    std::cerr << "Division by zero!!!!" << std::endl;
  }
  else if(compare_magnitude(other) == 0) {
    if(sign() == other.sign())
      return BigInt(1);
    else
      return BigInt(-1);
  }
  // dividend / divisor -> quotient
  BigInt quotient, divisor(other), dividend(*this);

  // Use positive numbers and then fix the sign at the end
  divisor._sign = dividend._sign = false;

  // Find the smallest power of 10 before the divisor is too big
  unsigned long shift = 0;
  while(divisor.compare_magnitude(dividend) == 1) {
    divisor.mul10();
    shift += 1;
  }
  // Repeatedly subtract then divide by 10 until the divisor is too small.
  while(other.compare_magnitude(dividend) != -1) {
    divisor.div10();
    shift -= 1;
    while(divisor.compare_magnitude(dividend) != -1) {
      dividend = dividend.subtract(divisor);
      quotient.add_at_digit(1, shift);
    }
  }
  quotient._sign = sign() ^ other.sign();
  return quotient;
}

// Compute the modulo division of two BigInts
BigInt BigInt::modulo(const BigInt& other) const
{
  // x % y   <--->   x - (x/y)*y
  return subtract(divide(other).multiply(other));
}

// x.compare(y) is
//   -1 if x>y,
//    0 if x==y,
//    1 if x<y
int BigInt::compare(const BigInt& other) const
{
  // Both positive
  if(!sign() && !other.sign())
    return compare_magnitude(other);
  // x positive, y negative
  else if(!sign() && other.sign())
    return -1;
  // x negative, y positive
  else if(sign() && !other.sign())
    return 1;
  // Both negative
  else
    return - compare_magnitude(other);
}

// as for compare but compares magnitudes
// x.compare_magnitude(y) is:
//   -1 if |x|>|y|,
//    0 if |x|==|y|,
//    1 if |x|<|y|
int BigInt::compare_magnitude(const BigInt& other) const
{
  // x has less digits than y
  if(size() < other.size())
    return 1;
  // y has less digits than x
  else if(size() > other.size())
    return -1;
  // Both BigInts are empty (and hence equal)
  else if(!size())
    return 0;
  // Need to compare digits starting at most significant
  else {
    unsigned long i=size();
    do {
      --i;
      if(digits[i] < other[i])
        return 1;
      else if(digits[i] > other[i])
        return -1;
    } while(i);
    // If we get this far then all digits are equal.
    return 0;
  }
}

/*
 *  BigInt private utility methods.
 *
 *  These are the elementary buliding blocks used by the above arithmetic
 *  methods.
 */

// Add carry*10^dig - this is needed as an elementary operation for addition.
// d should be from 0-9
// Ignores the signs of both operands.
void BigInt::add_at_digit(unsigned char carry, unsigned long dig)
{
  if(size() <= dig)
    digits.resize(dig+1, 0);
  // Loop through digits incrementing by one
  for(unsigned long j = dig; j < digits.size(); ++j) {
    digits[j] += carry;
    // Do we carry?
    if(digits[j] >= 10) {
      digits[j] -= 10;
      carry = 1;
    }
    // If we didn't carry just exit
    else {
      carry = 0;
      break;
    }
  }
  // Did we carry all the way to the end?
  if(carry) {
    // Add a new 1 digit at the end
    digits.push_back(1);
  }
}

// Subtract carry*10^dig - this is needed as an elementary operation for
// subtraction.
// d should be from 0-9
// The result must not be negative.
// Ignores the signs of both operands.
void BigInt::sub_at_digit(unsigned char carry, unsigned long dig)
{
  // Loop through digits incrementing by one
  for(unsigned long j = dig; j < digits.size(); ++j) {
    if(digits[j] >= carry) {
      digits[j] -= carry;
      break;
    }
    else {
      digits[j] += 10 - carry;
      carry = 1;
    }
  }
  // Pop off any leading zeros e.g. 000123 -> 123
  while(digits.size() && digits[digits.size()-1] == 0)
    digits.pop_back();
  // If we pop all the digits then we have zero and need set the sign to +ve
  if(digits.empty())
    _sign = false; // +ve
}

// Multiply by 10 - needed as an elemntary operation for division
void BigInt::mul10()
{
  digits.insert(digits.begin(), 0);
}

// Divide by 10 - needed as an elemntary operation for division
void BigInt::div10()
{
  if(digits.empty() || digits[0] != 0)
    std::cerr << "Discarding useful digits! " << *this << std::endl;
  digits.erase(digits.begin(), digits.begin()+1);
}


std::string BigInt::tostring() const
{
	std::string ret = "";

	if (size() == 0) return "0";

	if (sign()) ret = "-";

	for (unsigned int i = size(); i != 0; i--)
	{
		ret += (char) ('0' + digits[i-1]);
	}

	return ret;

}

