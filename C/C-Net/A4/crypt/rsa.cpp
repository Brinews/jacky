#include "rsa.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/* make : -lm */

static bool is_prime(int a)
{
	for (int i = 2; i <= sqrt(a); i++)
		if (a % i == 0) return false;

	return true;
}

/* keep a > b */
static int gcd(int a, int b)
{
	if (a < b) { int tmp = a; a = b; b = tmp; }
	if (b <= 0) return a;

	return gcd(b, a%b);
}

/* quick mod :(a^b)%c */
static int powermod(int a, int b, int c)
{
	long long t;
	if (b == 0) return 1%c;
	if (b == 1) return a%c;

	t = powermod(a, b/2, c);
	t = t * t % c;
	if (b&1) t = t * a % c;

	return t;
}

void rsa_yield_keys(rsa_key *pub_key, rsa_key *pri_key)
{
	/* 1. get two primes (a, b) */
	int a, b;

	srand(time(NULL));

	while ((a = rand()%500+3) && !is_prime(a)) 
		;
	while ((b = rand()%500+3) && !is_prime(b))
		;

	if (debug) {
		printf("primes (a,b):%d,%d\n", a, b);
	}

	int product = a * b;
	int fn = (a-1)*(b-1);

	int pub, priv;

	/* 2. get public key */
	for (pub = 3; gcd(fn, pub) != 1; pub++) 
		;

	/* 3. get private key */
	for (priv = 1; (pub * priv)%fn != 1; priv++)
		;

	/* return the key */
	pub_key->rsa_prime = pub;
	pub_key->rsa_product = product;

	pri_key->rsa_prime = priv;
	pri_key->rsa_product = product;
}

/* key encryption */
sym_key sym_encrypt_key(sym_key *src_key, rsa_key *pub_key)
{
	sym_key ret = *src_key;

	/* optimized (a^b % c) method */

	if (src_key != NULL && pub_key != NULL) {
		/*
		ret.sym_op1 = (int) ((long long)pow(src_key->sym_op1, pub_key->rsa_prime) % pub_key->rsa_product);
		ret.sym_op2 = (int) ((long long)pow(src_key->sym_op2, pub_key->rsa_prime) % pub_key->rsa_product);
		ret.sym_op3 = (int) ((long long)pow(src_key->sym_op3, pub_key->rsa_prime) % pub_key->rsa_product);
		*/

		ret.sym_op1 = powermod(src_key->sym_op1, pub_key->rsa_prime, pub_key->rsa_product);
		ret.sym_op2 = powermod(src_key->sym_op2, pub_key->rsa_prime, pub_key->rsa_product);
		ret.sym_op3 = powermod(src_key->sym_op3, pub_key->rsa_prime, pub_key->rsa_product);
	}

	return ret;
}

sym_key sym_decrypt_key(sym_key *src_key, rsa_key *pri_key)
{
	return sym_encrypt_key(src_key, pri_key);
}

/* message encryption */
char *easy_encrypt(char *buf, sym_key *key)
{
	int i, j = 0;
	int *p = (int *)key;

	for (i = 0; i < strlen(buf); i++) {
		buf[i] = buf[i] + *(p + j);
		j++;
		j = j % 3;
	}

	return buf;
}

char *easy_decrypt(char *buf, sym_key *key)
{
	int i, j = 0;
	int *p = (int *)key;

	for (i = 0; i < strlen(buf); i++) {
		buf[i] = buf[i] - *(p + j);
		j = (++j) % 3;
	}

	return buf;
}
