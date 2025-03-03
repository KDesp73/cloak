#ifndef RSA_H
#define RSA_H

#include "aes.h"
#include <sodium.h>
#include <string.h>

#define CLOAK_RSA_KEY_SIZE 256

void CLOAK_RSAGenerateKeys();

bool CLOAK_DeriveKey(const char *password, unsigned char *key, const unsigned char *salt);


int CLOAK_RSAEncrypt(const unsigned char *aes_key, size_t aes_key_len, 
                    const char *public_key_path, unsigned char *encrypted_key, size_t *encrypted_key_len);

int CLOAK_RSADecrypt(const unsigned char *encrypted_key, size_t encrypted_key_len, 
                    const char *private_key_path, unsigned char *aes_key, size_t *aes_key_len);


#ifdef CLOAK_REMOVE_PREFIXES
#define DeriveKey CLOAK_DeriveKey
#define RSAEncrypt CLOAK_RSAEncrypt
#define RSADecrypt CLOAK_RSADecrypt
#endif // CLOAK_REMOVE_PREFIXES


#endif // RSA_H
