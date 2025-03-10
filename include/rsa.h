#ifndef RSA_H
#define RSA_H

#include "aes.h"
#include <sodium.h>
#include <string.h>

#define CLOAK_RSA_KEY_SIZE 256
#define CLOAK_RSA_SIGNATURE_SIZE 256
#define CLOAK_SALT_FILE "./.cloak/cloak.salt"
#define CLOAK_SALT_SIZE crypto_pwhash_SALTBYTES
#define CLOAK_PASSWORD_MAX 256


void CLOAK_RSAGenerateKeys();

bool CLOAK_DeriveKey(const char *password, unsigned char *key, const unsigned char *salt);


int CLOAK_RSAEncrypt(const unsigned char *aes_key, size_t aes_key_len, 
                    const char *public_key_path, unsigned char *encrypted_key, size_t *encrypted_key_len);
int CLOAK_RSADecrypt(const unsigned char *encrypted_key, size_t encrypted_key_len, 
                    const char *private_key_path, unsigned char *aes_key, size_t *aes_key_len);

int CLOAK_RSASign(const unsigned char *aes_key, size_t aes_key_len, const char *private_key_path, unsigned char *signature, size_t *signature_len);
int CLOAK_RSAVerify(const unsigned char *aes_key, size_t aes_key_len, const unsigned char *signature, size_t signature_len, const char *public_key_path);

int CLOAK_PromptPassword(const char* prompt, char* password, size_t max_len);

#ifdef CLOAK_REMOVE_PREFIXES
#define DeriveKey CLOAK_DeriveKey
#define RSAEncrypt CLOAK_RSAEncrypt
#define RSADecrypt CLOAK_RSADecrypt
#endif // CLOAK_REMOVE_PREFIXES


#endif // RSA_H
