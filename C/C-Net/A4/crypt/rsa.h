#ifndef _RSA_H
#define _RSA_H

#define debug 1

/* non-symmetric key */
typedef struct key_nsym {
	int rsa_prime;
	int rsa_product;
} rsa_key;

/* symmetric key */
typedef struct key_sym {
	int sym_op1;
	int sym_op2;
	int sym_op3;
}sym_key;

/* yield public key and private key */
void rsa_yield_keys(rsa_key *pub, rsa_key *pri);

/* encryption on key */
sym_key sym_encrypt_key(sym_key *src_key, rsa_key *pub_key);
sym_key sym_decrypt_key(sym_key *src_key, rsa_key *pri_key);

/* encryption */
char* easy_encrypt(char *buf, sym_key *key);

/* descryption */
char *easy_decrypt(char *buf, sym_key *key);

#endif
