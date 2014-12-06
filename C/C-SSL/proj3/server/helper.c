#include "helper.h"

#include<stdio.h>
#include<stdlib.h>
#include<openssl/rand.h>
#include<openssl/rsa.h>
#include<openssl/err.h>

#define IDSIZE 11
#define LSIZE 512

#define RSA_SIZE 128

unsigned char key_str[20];
unsigned char *key_rsa;

int putting = 0;
int totalLen = 0;

char* get_file_uid(unsigned char* filename) {
	FILE *fpr = fopen(filename, "rb");
	if(fpr == NULL){return;}
	char* buf = (unsigned char*) malloc (IDSIZE);
	fread(buf, IDSIZE, 1, fpr);
	fclose(fpr);

	return buf;
}

//read file, decrypt, return plaintext
char* get(unsigned char* filename) {

	FILE *fpr = fopen(filename, "rb");

	fseek(fpr, 0L, SEEK_END);
	long int size = ftell(fpr);
	printf("Stored file size: %ld\n", size);

	fseek(fpr, 0L, SEEK_SET);

	unsigned char* buf = (char*) malloc (size);

	fread(buf, size, 1, fpr);

	fclose(fpr);

	buf += IDSIZE;

	key_rsa = (unsigned char *) malloc(RSA_SIZE+1);
	memset(key_rsa, 0, RSA_SIZE+1);
	memcpy(key_rsa, buf, RSA_SIZE);

	buf += RSA_SIZE;

	size = size - (RSA_SIZE + IDSIZE);
	totalLen = *(int *)buf;

	buf += sizeof(int);
	size = size - sizeof(int);

	unsigned char *hash_key = rsa_decrypt(key_rsa, "private.key");

	if (aes_init(hash_key, 16, NULL, &Encrypt_Context, &Decrypt_Context) == -1)
		printf("AES init fail\t");

	char* plaintext = (unsigned char*) aes_decrypt(&Decrypt_Context, buf, &size);

	//unsigned char* plaintext = (unsigned char*) aes_decrypt(&Decrypt_Context, buf, &size);
	plaintext[totalLen] = '\0';

	free(key_rsa);
	free(hash_key);

	return plaintext;
}

//read file, encrypt, write to disk
int put(unsigned char* buf, unsigned char* filename, char *uid) {
	int size = strlen(buf);

	FILE *fp;
	if(putting == 1)
		fp = fopen(filename, "wb");
	else
		fp = fopen(filename, "ab+");

	fwrite(buf, size, 1, fp);

	fclose(fp);
}

void encrypt_put(unsigned char* filename, char *uid)
{
	FILE *fp = fopen(filename, "rb");

	fseek(fp, 0L, SEEK_END);
	long int size = ftell(fp);

	fseek(fp, 0L, SEEK_SET);
	char* buf = (char*) malloc (size);

	fread(buf, size, 1, fp);

	fclose(fp);

	int realLen = size;

	printf("Received file-size: %ld\n", size);

	gen_key_hash(buf, strlen(buf), key_str);

	if (aes_init(key_str, 16, NULL, &Encrypt_Context, &Decrypt_Context) == -1)
		printf("AES init fail\t");

	unsigned char* cipher = (unsigned char*) aes_encrypt(&Encrypt_Context, buf, &size);

	fp = fopen(filename, "wb");

	if (fp == NULL)
		printf("open failed: %s\n", filename);

	fwrite(uid, IDSIZE, 1, fp);

	key_rsa = rsa_encrypt(key_str, "public.key");

	if (key_rsa == NULL)
		printf("key failed\n");

	fwrite(key_rsa, RSA_SIZE, 1, fp);

	fwrite(&realLen, sizeof(int), 1, fp);

	fwrite(cipher, size, 1, fp);

	free(cipher);
	free(key_rsa);

	fclose(fp);
}

void gen_key_hash(unsigned char *input, int len, unsigned char *hash)
{
	memset(hash, 0, 20);
	SHA1(input, len, hash);
}

int aes_init(unsigned char *key_data, int key_data_len, unsigned char *salt, EVP_CIPHER_CTX *e_ctx, EVP_CIPHER_CTX *d_ctx)
{
	int i, nrounds = 1;
	unsigned char key[16], iv[16];
	i = EVP_BytesToKey(EVP_aes_128_cbc(), EVP_sha1(), salt, key_data, key_data_len, nrounds, key, iv);
	if (i != 16) {
		printf("Key size is %d bits - should be 128 bits\n", i);
		return -1;
	}

	EVP_CIPHER_CTX_init(e_ctx);
	EVP_EncryptInit_ex(e_ctx, EVP_aes_128_cbc(), NULL, key, iv);
	EVP_CIPHER_CTX_init(d_ctx);
	EVP_DecryptInit_ex(d_ctx, EVP_aes_128_cbc(), NULL, key, iv);
	
	return 0;
}


unsigned char *aes_encrypt(EVP_CIPHER_CTX *e, unsigned char *plaintext,long int *len)
{
	printf("Encrypting file content with file hash\n");
	int c_len = *len + AES_BLOCK_SIZE, f_len = 0;
	unsigned char *ciphertext = (unsigned char *)malloc(c_len);
	EVP_EncryptInit_ex(e, NULL, NULL, NULL, NULL);
	EVP_EncryptUpdate(e, ciphertext, &c_len, plaintext, *len);

	EVP_EncryptFinal_ex(e, ciphertext+c_len, &f_len);
	*len = c_len + f_len;
	return ciphertext;
}

unsigned char *aes_decrypt(EVP_CIPHER_CTX *e, unsigned char *ciphertext,long int *len)
{
	printf("Decrypting file content with file hash\n");
	int p_len = *len, f_len = 0;
	unsigned char *plaintext = (unsigned char*)malloc(p_len);
	EVP_DecryptInit_ex(e, NULL, NULL, NULL, NULL);
	EVP_DecryptUpdate(e, plaintext, &p_len, ciphertext, *len);
	EVP_DecryptFinal_ex(e, plaintext+p_len, &f_len);
	*len = p_len + f_len;
	return plaintext;
}


unsigned char *rsa_encrypt(unsigned char *str, char *key_file)
{
	printf("Encrypting file hash with server's public key\n");
	char *p_en;
	RSA *p_rsa;
	FILE *file;
	int rsa_len;
	if((file=fopen(key_file,"r"))==NULL){
		perror("open key file error");
		return NULL;    
	}   

	if((p_rsa=PEM_read_RSA_PUBKEY(file,NULL,NULL,NULL))==NULL){
		ERR_print_errors_fp(stdout);
		return NULL;
	}   

	rsa_len=RSA_size(p_rsa);
	p_en=(unsigned char *)malloc(rsa_len+1);
	memset(p_en,0,rsa_len+1);
	if(RSA_public_encrypt(rsa_len,(unsigned char *)str,(unsigned char*)p_en,p_rsa,RSA_NO_PADDING)<0){
		ERR_get_error();
		return NULL;
	}
	RSA_free(p_rsa);
	fclose(file);

	return p_en;	
}

unsigned char *rsa_decrypt(unsigned char *str, char *key_file)
{
	printf("Decrypting file hash with server's private key\n");
	char *p_de;
	RSA *p_rsa;
	FILE *file;
	int rsa_len;
	if((file=fopen(key_file,"r"))==NULL){
		perror("open key file error");
		return NULL;
	}
	if((p_rsa=PEM_read_RSAPrivateKey(file,NULL,NULL,NULL))==NULL){
		ERR_print_errors_fp(stdout);
		return NULL;
	}
	rsa_len=RSA_size(p_rsa);
	p_de=(unsigned char *)malloc(rsa_len+1);
	memset(p_de,0,rsa_len+1);
	if(RSA_private_decrypt(rsa_len,(unsigned char *)str,(unsigned char*)p_de,p_rsa,RSA_NO_PADDING)<0){
		return NULL;
	}
	RSA_free(p_rsa);
	fclose(file);
	return p_de;
}
