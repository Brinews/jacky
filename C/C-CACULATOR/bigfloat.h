#ifndef _BIGFLOAT_H
#define _BIGFLOAT_H

#include "bigint.h"
#include <string>

class BigFloat
{
private:
	BigInt significand, exponent;
	static BigInt numdigits;

public:
	BigFloat();
	BigFloat(BigInt s, BigInt e);
	BigFloat(std::string);

	BigInt getsignificand()
	{
		return significand;
	}

	BigInt getexponent()
	{
		return exponent;
	}

	BigFloat add(const BigFloat& o) const;
	BigFloat subtract(const BigFloat& o) const;
	BigFloat multiply(const BigFloat& o) const;
	BigFloat divide(const BigFloat& o) const;

	BigFloat fsqrt() const;

	std::string tostring() const;
};


std::ostream& operator<<(std::ostream& out, BigFloat bf);

//overloaded operations
BigFloat operator+(const BigFloat& lhs, const BigFloat& rhs);
BigFloat operator-(const BigFloat& lhs, const BigFloat& rhs);
BigFloat operator*(const BigFloat& lhs, const BigFloat& rhs);
BigFloat operator/(const BigFloat& lhs, const BigFloat& rhs);

BigFloat sqrt(const BigFloat& b);
BigFloat cos(const BigFloat& b);

#endif
