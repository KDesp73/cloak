#ifndef HASHING_H
#define  HASHING_H

#include <sodium/crypto_hash_sha256.h>
#define CLOAK_HASH_SIZE crypto_hash_sha256_BYTES

int CLOAK_HashFile(const char* path, unsigned char output_hash[CLOAK_HASH_SIZE]);
void CLOAK_HashPrint(unsigned char hash[CLOAK_HASH_SIZE]);


#ifdef CLOAK_REMOVE_PREFIXES
#define HASH_SIZE CLOAK_HASH_SIZE
#define HashFile CLOAK_HashFile
#define HashPrint CLOAK_HashPrint
#endif // CLOAK_REMOVE_PREFIXES

#endif // HASHING_H
