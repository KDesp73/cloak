#ifndef AES_H
#define AES_H

#include <sodium/core.h>
#include <sodium/crypto_secretbox.h>
#include <stdbool.h>
#include <stdio.h>
#define KEY_SIZE crypto_secretbox_KEYBYTES
#define NONCE_SIZE crypto_secretbox_NONCEBYTES
#define MAC_SIZE crypto_secretbox_MACBYTES

void GenerateKey(unsigned char key[KEY_SIZE]);
int EncryptFile(const char *input_file, const char *output_file, unsigned char key[KEY_SIZE]);
int DecryptFile(const char *input_file, const char *output_file, unsigned char key[KEY_SIZE]);

static inline bool SodiumInit(void){
    if (sodium_init() < 0) {
        fprintf(stderr, "libsodium initialization failed!\n");
        return false;
    }
    return true;
}

#endif // AES_H

