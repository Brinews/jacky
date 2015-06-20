#include <iostream>
#include "bigint.h"

using namespace std;

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


void doTest();

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

	//doTest();

	return 0;
}

void doTest()
{
	srand(time(NULL));

	/* big-int test */
	string a = "12342222222222222222222222222243241234234";
	string b = "1";

	BigInt ba(a);
	BigInt bb(b);
	ba = ba + bb;
	cout << ba << endl;

	/* rsa test I */
	BigInt p = 3, q = 11;
	BigInt e = 3, d = 7;
	BigInt n = p * q;

	BigInt M = 15;
	BigInt C = powerMod(M, e, n);

	cout << M << " encrypt to " << C << endl;
	cout << C << " decrypt to " << powerMod(C, d, n) << endl;

	/* ras test II */
	BigInt p1 = randPrime(10000, 1), q1 = randPrime(10000, 1);
	BigInt n1 = p1 * q1;
	BigInt fn = (p1-1)*(q1-1);

	cout << "p1=" << p1 << endl;
	cout << "q1=" << q1 << endl;

	/* find e */
	BigInt e1 = rand()%fn;
	while (!(gcd(fn, e1) == 1)) {
		e1 = e1 + 1;

		if (e1 > fn) e1 = e1 % fn;
	}

	cout << "Find e1 = " << e1 << endl;

	/* calculate d */
	BigInt d1;

	/* need opt */
	/*
	while (!(((d1*e1)%fn) == 1)) {
		d1 = d1 + 1;
	}
	*/
	d1 = Euclid(e1, fn);

	cout << "Get d1 = " << d1 << endl;

	for (int i = 0; i < 256; i++) {
		BigInt M1 = i;

		BigInt C1 = powerMod(M1, e1, n1);

		cout << M1 << " encrypt to " << C1 << endl;
		cout << C1 << " decrypt to " << powerMod(C1, d1, n1) << endl;
	}
}

