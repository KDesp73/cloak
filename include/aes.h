#ifndef AES_H
#define AES_H

#include <sodium/core.h>
#include <sodium/crypto_secretbox.h>
#include <stdbool.h>

#define CLOAK_KEY_SIZE crypto_secretbox_KEYBYTES
#define CLOAK_NONCE_SIZE crypto_secretbox_NONCEBYTES
#define CLOAK_MAC_SIZE crypto_secretbox_MACBYTES
#define CLOAK_KEY_FILE "cloak.key"

void CLOAK_AESGenerateKey(unsigned char key[CLOAK_KEY_SIZE]);
int CLOAK_AESEncryptFile(const char *input_file, const char *output_file, unsigned char key[CLOAK_KEY_SIZE]);
int CLOAK_AESDecryptFile(const char *input_file, const char *output_file, unsigned char key[CLOAK_KEY_SIZE]);


#ifdef CLOAK_REMOVE_PREFIXES
#define KEY_SIZE   CLOAK_KEY_SIZE
#define NONCE_SIZE CLOAK_NONCE_SIZE
#define MAC_SIZE   CLOAK_MAC_SIZE
#define KEY_FILE   CLOAK_KEY_FILE

#define AESGenerateKey CLOAK_AESGenerateKey
#define AESEncryptFile CLOAK_AESEncryptFile
#define AESDecryptFile CLOAK_AESDecryptFile
#endif // CLOAK_REMOVE_PREFIXES

#endif // AES_H

