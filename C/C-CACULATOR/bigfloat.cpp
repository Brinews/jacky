#include "bigint.h"
#include "rational.h"
#include "bigfloat.h"

#include <string>

BigInt BigFloat::numdigits = 30;

BigFloat::BigFloat()
{
	significand = 0;
	exponent = 0;
}

BigFloat::BigFloat(BigInt s, BigInt e)
{
	significand = s;
	exponent = e;
}

/* str like 343.34344 */
BigFloat::BigFloat(std::string str)
{
	unsigned int pos = str.find('.');

	if (pos == std::string::npos)
	{
		significand = str;
		exponent = 0;
	}
	else
	{
		std::string b = str.substr(0, pos);
		std::string a = str.substr(pos+1, str.length() - pos - 1);

		b = b + a;
		significand = b;
		exponent = pos + 1 - str.length();
	}

	while (significand % 10 == 0)
	{
		significand = significand / 10;
		exponent = exponent + 1;
	}
}

std::ostream& operator<<(std::ostream& out, BigFloat bf)
{
	out << bf.tostring();

	return out;
}

BigFloat operator+(const BigFloat& lhs, const BigFloat& rhs)
{
	return lhs.add(rhs);
}

BigFloat operator-(const BigFloat& lhs, const BigFloat& rhs)
{
	return lhs.subtract(rhs);
}

BigFloat operator*(const BigFloat& lhs, const BigFloat& rhs)
{
	return lhs.multiply(rhs);
}

BigFloat operator/(const BigFloat& lhs, const BigFloat& rhs)
{
	return lhs.divide(rhs);
}

BigFloat BigFloat::add(const BigFloat& other) const
{
	if (exponent > other.exponent)
	{
		BigInt d = exponent - other.exponent;
		BigInt s = significand;
		while (d > 0)
		{
			d = d - 1;
			s = s * 10;
		}

		s = s + other.significand;

		return BigFloat(s, other.exponent);
	}
	else
	{
		BigInt d = other.exponent - exponent;
		BigInt s = other.significand;

		while (d > 0)
		{
			d = d - 1;
			s = s * 10;
		}

		s = s + significand;

		return BigFloat(s, exponent);
	}
}

BigFloat BigFloat::subtract(const BigFloat& other) const
{
	BigFloat a(-1 * other.significand, other.exponent);

	return add(a);
}

BigFloat BigFloat::multiply(const BigFloat& other) const
{
	BigInt s = significand * other.significand;
	BigInt e = exponent + other.exponent;

	return BigFloat(s, e);
}

BigFloat BigFloat::divide(const BigFloat& other) const
{
	std::string sa = significand.tostring();
	std::string sb = other.significand.tostring();

	BigInt si = significand;
	BigInt ei = exponent;
	int s = sa.length() - sb.length();

	while (s < 3)
	{
		s++;
		ei = ei - 1;
		si = si * 10;
	}

	si = si/other.significand;
	ei = ei - other.exponent;

	return BigFloat(si, ei);
}

BigFloat BigFloat::fsqrt() const
{
	BigInt ex = exponent;
	BigInt si = significand;

	if (!(ex % 2 == 0))
	{
		ex = ex - 1;
		si = si * 10;
	}

	return BigFloat(sqrt(si), ex/2);
}

BigFloat sqrt(const BigFloat& square)
{
	return square.fsqrt();
}

BigFloat cos(const BigFloat& s)
{
	//TODO: no realization
	return BigFloat(s.tostring());
}

std::string BigFloat::tostring() const
{
	std::string s = significand.tostring();

	BigInt e = exponent;

	while (e > 0)
	{
		e = e - 1;
		s += "0";
	}

	if (e < 0)
	{
		e = 0 - e;
		if (e < s.length())
		{
			//s = s.insert(s.length() - e - 1, ".");
			std::string v = "";
			for (unsigned int i = 0; i < s.length(); i++)
			{
				v += s[i];

				if (i == s.length() - e - 1)
					v += ".";
			}
			s = v;
		}
		else
		{
			std::string ns = "0.";
			while (e - s.length() > 0)
			{
				e = e - 1;
				ns += "0";
			}

			ns += s;

			s = ns;
		}
	}

	return s;
}
