#include <iostream>
#include <ostream>
#include <vector>
#include <string>
#include <limits.h>

using namespace std;

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

    // elementary methods that are used as part of arithmetic
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
    unsigned char operator[](unsigned long index) const { 
		return digits[index]; }
    unsigned long size() const { return digits.size(); }
    bool sign() const { return _sign; }

    // Arithmetic methods in BigInt
    BigInt negate() const;                      // -x
    BigInt add(const BigInt& other) const;      // x + y
    BigInt subtract(const BigInt& other) const; // x - y
    BigInt multiply(const BigInt& other) const; // x * y
    BigInt modulo(const BigInt& other) const; // x * y
    BigInt divide(const BigInt& other) const; // x * y


	unsigned char getLastDigit() const;

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
	// The standard guarantees overflow behaviour for this particular
	// cast (even if the underlying system does not use 2's complement).
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

unsigned char BigInt::getLastDigit() const
{
	unsigned char c = 0;
	if (size() == 0) return c;

	return digits[0];
}

struct RSA {
	BigInt p; /* 素数 */
	BigInt q; /* 素数 */
	BigInt e; /* 公钥 */
	BigInt d; /* 私钥 */
	BigInt n; /* p*q  */
};

typedef struct RSA Rsa;

/* keep a > b */
BigInt gcd(BigInt a, BigInt b)
{
	if (a < b) { BigInt tmp = a; a = b; b = tmp; }
	if (b < 0 || b == 0) return a;

	return gcd(b, a%b);
}

int isPrime(BigInt number)
{
	BigInt i = 2;

	BigInt doub = i * i;

	while (doub < number || doub == number) {
		if (number%i == 0) return 0;

		i = i + 1;
		doub = i * i;
	}

	return 1;
}

BigInt randPrime(BigInt max, BigInt min)
{
	BigInt prime = 4;

	while (isPrime(prime) == 0) {
		prime = rand()%(max-min+1) + min;
	}

	return prime;
}

BigInt inverse(BigInt a, BigInt b)
{
	BigInt i = 1;

	while (!((a*i)%b == 1)) {
		i = i + 1;
	}

	return i;
}

/*
 * 指数模除(a^b)%c
 */
BigInt powerMod(BigInt a, BigInt b, BigInt c)
{
	BigInt t;

	if (b == 0) return 1;
	if (b == 1) return a%c;

	t = powerMod(a, b/2, c);
	t = t * t % c;

	if ((b.getLastDigit()&1) == 1) t = t * a % c;

	return t;
}

/*
 * a*x = 1 (mod b)
 * 相当于求解：a*x - b*y = 1
 */
BigInt Euclid(BigInt a, BigInt b)
{
	BigInt m, e, i, j, x, y, xx, yy;

	m = b;
	e = a;
	x = 0;
	y = 1;
	xx = 1;
	yy = 1;

	while (!(e==0)) {
		i = m / e;
		j = m % e;

		m = e;
		e = j;
		j = y;
		y = y * i;

		if (xx == yy) {
			if (x > y) {
				y = x - y;
			} else {
				y = y - x;
				yy = 0;
			}
		} else {
			y = y + x;
			xx = 1 - xx;
			yy = 1 - yy;
		}

		x = j;
	}

	if (xx == 0) x = b - x;

	return x;
}

/*
 * 1. RSA生成
 */
void GenerateRsa(Rsa &rsa, const BigInt &max, const BigInt &min)
{
	rsa.p = randPrime(max, min);
	do {
		rsa.q = randPrime(max, min);
	} while (rsa.p == rsa.q);

	rsa.n = rsa.p * rsa.q;

	BigInt fn = (rsa.p - 1) * (rsa.q - 1);

	/* find e */
	BigInt e = rand() % fn;
	while (!(gcd(fn, e) == 1)) {
		e = e + 1;
		if (e > fn) e = e % fn;
	}
	rsa.e = e;

	/* get d */
	rsa.d = Euclid(e, fn);
}

/*
 * 1.1 RSA加密, 求C
 */
BigInt RsaEncrypt(const Rsa &r, const BigInt &M)
{
	return powerMod(M, r.e, r.n);
}

/*
 * 1.2 RSA解密,求M
 */
BigInt RsaDecrypt(const Rsa &r, const BigInt &C)
{
	return powerMod(C, r.d, r.n);
}

/*
 * 2. 盲化,消息M，求t
 */
BigInt mBlind(const BigInt &M, const BigInt &e, const BigInt &n, 
		BigInt &k)
{
	BigInt ret;

	k = rand() % M; /* get K */

	BigInt ke = powerMod(k, e, n); /* (K^e)%n */

	return (ke*M)%n;
}

/*
 * 3. 签名,求s
 */
BigInt mSign(const BigInt& t, const BigInt &d, const BigInt &n)
{
	return powerMod(t, d, n);
}

/*
 * 4. 解盲,求S
 */
BigInt mUnsign(const BigInt& s, const BigInt &k, const BigInt &n)
{
	BigInt ik = s;

	while (1) {
		ik = ik + n;
		if (ik % k == 0) {
			return (ik/k)%n;
		}
	}

	return 0; /* 解签失败 */
}

/*
 * 5.验证
 */
int mVerify(const BigInt& S, const BigInt &e, const BigInt &n, 
		const BigInt &t)
{
	BigInt v = powerMod(S, e, n);

	if (v == (t%n)) {
		return 1; /* 验证通过 */
	}

	return 0;
}

int main(int argc, char **argv)
{
	srand(time(NULL));

	Rsa rsa;
	GenerateRsa(rsa, 10000, 1);

	cout << "RSA公钥:(e,n)" << rsa.e << "," << rsa.n << endl ;
	cout << "RSA私钥:(d,n)" << rsa.d << "," << rsa.n << endl ;

	BigInt M, k;
	//M = rand(); /* M可以是字符串计算出来的数值 */
	string msg("324234289347982374982374923476893245834758983274982372");
	M = msg;

	cout << "A:消息M＝" << M << endl;

	BigInt t = mBlind(M, rsa.e, rsa.n, k);
	cout << "A:选取k=" << k << ", 盲化后t=" << t << endl;
	
	BigInt s = mSign(t, rsa.d, rsa.n);
	cout << "B:签名后s=" << s << endl;

	BigInt S = mUnsign(s, k, rsa.n);
	cout << "A:解盲后S=" << S << endl;

	int ret = mVerify(S, rsa.e, rsa.n, M);
	if (ret == 1) {
		cout << "A:验证签名通过" << endl;
	} else {
		cout << "A:验证签名失败" << endl;
	}

	/*
	for (int i = 1000; i < 1024; i++) {
		BigInt C = RsaEncrypt(rsa, i);
		cout << i << " encrypt to " << C << endl;
		cout << C << " after Decrypt " << RsaDecrypt(rsa, C) << endl;
	}
	*/

	return 0;
}
