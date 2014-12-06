#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/sha.h>

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>


static EVP_CIPHER_CTX Encrypt_Context, Decrypt_Context;

void gen_key_hash(unsigned char *input, int len, unsigned char *hash);

int aes_init(unsigned char *key_data, int key_data_len, unsigned char *salt, EVP_CIPHER_CTX *e_ctx, EVP_CIPHER_CTX *d_ctx);

/*encryption for file content */
unsigned char *aes_encrypt(EVP_CIPHER_CTX *e, unsigned char *plaintext,long int *len);

unsigned char *aes_decrypt(EVP_CIPHER_CTX *e, unsigned char *ciphertext,long int *len);



/*encryption for file hash (AES key) */
unsigned char *rsa_encrypt(unsigned char *str, char *key_file);

unsigned char *rsa_decrypt(unsigned char *str, char *key_file);


