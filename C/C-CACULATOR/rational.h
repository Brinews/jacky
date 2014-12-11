//rational.h

#ifndef _RATIONAL_H
#define _RATIONAL_H

#include "bigint.h"
#include <string>

class Rational 
{
private:
	BigInt numerator, denominator;
	//other stuff

public:
	//Constructor
	Rational();
	Rational(BigInt, BigInt);
	Rational(std::string);

	BigInt getnumerator()
	{
		return numerator;
	}

	BigInt getdenominator()
	{
		return denominator;
	}

	void setnumerator(BigInt n)
	{
		numerator = n;
	}

	void setdenominator(BigInt d)
	{
		denominator = d;
	}

	//Other method etc.
	Rational add(const Rational& other) const;
	Rational subtract(const Rational& other) const;
	Rational multiply(const Rational& other) const;
	Rational divide(const Rational& other) const;

	Rational dosqrt() const;

	std::string tostring() const;

};

std::ostream& operator<<(std::ostream& out, Rational bi);

//Overloaded arithmetric operations
Rational operator+(const Rational& lhs, const Rational& rhs);
Rational operator-(const Rational& lhs, const Rational& rhs);
Rational operator*(const Rational& lhs, const Rational& rhs);
Rational operator/(const Rational& lhs, const Rational& rhs);

BigInt sqrt(const BigInt& square);
Rational sqrt(const Rational& s);
Rational cos(const Rational& s);

//other stuff

#endif
