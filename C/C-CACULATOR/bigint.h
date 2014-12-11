/*
 * bigint.h
 *
 * Author: Oscar Benjamin
 * Date: 20 Mar 2014
 *
 * Header file for the BigInt class. The BigInt class represents integers with
 * unlimited precision. There will be no overflow in the BigInt class unless
 * the computer runs out of memory. BigInt objects are slower than ordinary
 * C/C++ int, long etc. but not needing to worry about overflow makes them
 * more useful for certain situations.
 */

// Prevent double include
#ifndef BIGINT_H
#define BIGINT_H

#include <ostream>
#include <vector>

class BigInt
{
  private:
    // The number will be stored as a std::vector of unsigned chars. Each
    // unsigned char stores the numeric value of 1 digit. The digits are
    // stored in reverse order so that e.g.
    //  BigInt(1234) -> {4, 3, 2, 1}
    std::vector<unsigned char> digits;

    // Need to keep the sign bit separate
    // false indicates positive and true indicates negative
    bool _sign;

    // These are the elementary methods that are used as part of arithmetic
    void add_at_digit(unsigned char carry, unsigned long dig);
    void sub_at_digit(unsigned char carry, unsigned long dig);
    void update_sign();
    void mul10();
    void div10();

  public:
    // Constructors
    BigInt();
    BigInt(const int& i);
    BigInt(const std::string& s);

    // Provide read only access to the digits and sign
    unsigned char operator[](unsigned long index) const { return digits[index]; }
    unsigned long size() const { return digits.size(); }
    bool sign() const { return _sign; }

    // Arithmetic methods in BigInt
    BigInt negate() const;                      // -x
    BigInt add(const BigInt& other) const;      // x + y
    BigInt subtract(const BigInt& other) const; // x - y
    BigInt multiply(const BigInt& other) const; // x * y
    BigInt modulo(const BigInt& other) const; // x * y
    BigInt divide(const BigInt& other) const; // x * y

    // compare is -1 if x>y, 0 if x==y, and 1 if x<y
    int compare(const BigInt& other) const;
    // as for compare but compares magnitudes
    int compare_magnitude(const BigInt& other) const;

	std::string tostring()const;
};

// For printing
std::ostream& operator<<(std::ostream& out, BigInt bi);

// Overloaded arithmetic operations
BigInt operator+ (const BigInt& lhs, const BigInt& rhs);
BigInt operator- (const BigInt& lhs, const BigInt& rhs);
BigInt operator* (const BigInt& lhs, const BigInt& rhs);
BigInt operator/ (const BigInt& lhs, const BigInt& rhs);
BigInt operator% (const BigInt& lhs, const BigInt& rhs);
BigInt operator- (const BigInt& bi); // unary minus

// Overloaded comparison operations
bool operator< (const BigInt& lhs, const BigInt& rhs);
bool operator> (const BigInt& lhs, const BigInt& rhs);
bool operator== (const BigInt& lhs, const BigInt& rhs);

#endif // BIGINT_H
