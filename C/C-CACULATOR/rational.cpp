#include "bigint.h"
#include "rational.h"

#include <string>

Rational::Rational()
{
	numerator = 0;
	denominator = 1;
}

Rational::Rational(BigInt n, BigInt d)
{
	/* gcd */
	BigInt g, n1 = n, d1 = d;

	/* switch */
	if (n1 > d1)
	{
		g = n1;
		n1 = d1;
		d1 = g;
	}

	while (n1 > 0)
	{
		g = d1 % n1;
		d1 = n1;
		n1 = g;
	}

	/* divide the common divisor */
	numerator = n / d1;
	denominator = d / d1;
}

Rational::Rational(std::string str)
{
	unsigned int pos = str.find('/');

	if (pos == std::string::npos)
	{
		numerator = str;
		denominator = 1;
	}
	else 
	{
		std::string n = str.substr(0, pos);
		std::string d = str.substr(pos+1, str.length() - pos - 1);

		numerator = n;
		denominator = d;

		new (this)Rational(numerator, denominator);
	}
}

std::ostream& operator<<(std::ostream& out, Rational bi)
{
	out << bi.getnumerator();

	if (!(bi.getdenominator() == 1))
	{
		out << '/' << bi.getdenominator();
	}

	return out;
}

Rational operator+(const Rational& lhs, const Rational& rhs)
{
	return lhs.add(rhs);
}

Rational operator-(const Rational& lhs, const Rational& rhs)
{
	return lhs.subtract(rhs);
}

Rational operator*(const Rational& lhs, const Rational& rhs)
{
	return lhs.multiply(rhs);
}

Rational operator/(const Rational& lhs, const Rational& rhs)
{
	return lhs.divide(rhs);
}

Rational Rational::add(const Rational& other) const
{
	BigInt d = denominator * other.denominator;
	BigInt n1 = numerator * other.denominator;
	n1 = n1 + other.numerator * denominator;

	return Rational(n1, d);
}

Rational Rational::subtract(const Rational& other) const
{
	BigInt d = denominator * other.denominator;
	BigInt n1 = numerator * other.denominator;
	n1 = n1 - other.numerator * denominator;

	return Rational(n1, d);
}

Rational Rational::multiply(const Rational& other) const
{
	BigInt n = numerator * other.numerator;
	BigInt d = denominator * other.denominator;

	return Rational(n, d);
}

Rational Rational::divide(const Rational& other) const
{
	BigInt n = numerator * other.denominator;
	BigInt d = denominator * other.numerator;

	return Rational(n, d);
}

BigInt sqrt(const BigInt& square)
{
	/* newton iteration */
	int i = 1, t = 1000;
	BigInt asd = 1;

	while (i < t)
	{
		asd = (asd + (square/asd))/2;
		i = i+1;
	}

	return asd;
}

Rational Rational::dosqrt() const
{
	return Rational(sqrt(numerator), sqrt(denominator));
}

Rational sqrt(const Rational& s)
{
	return s.dosqrt();
}

Rational cos(const Rational& s)
{
	//TODO: no realization
	return Rational(s.tostring());
}

std::string Rational::tostring() const
{
	std::string ret = numerator.tostring();
	
	if (!(denominator == 1))
	{
		ret += "/";
		ret += denominator.tostring();
	}

	return ret;
}

