#ifndef HASHING_H
#define  HASHING_H

#include <sodium/crypto_hash_sha256.h>
#define HASH_SIZE crypto_hash_sha256_BYTES

int HashFile(const char* path, unsigned char output_hash[HASH_SIZE]);
void HashPrint(unsigned char hash[HASH_SIZE]);


#endif // HASHING_H
