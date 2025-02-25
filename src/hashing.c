#include "hashing.h"
#include <sodium/crypto_hash_sha256.h>
#include <stdio.h>

int HashFile(const char* path, unsigned char output_hash[HASH_SIZE])
{
    FILE* file = fopen(path, "rb");
    if (!file) {
        perror("Failed to open file");
        return -1;
    }

    unsigned char buffer[4096];
    crypto_hash_sha256_state state;
    crypto_hash_sha256_init(&state);

    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        crypto_hash_sha256_update(&state, buffer, bytes_read);
    }
    fclose(file);

    crypto_hash_sha256_final(&state, output_hash);
    return 0;
}

void HashPrint(unsigned char hash[HASH_SIZE])
{
    for (int i = 0; i < HASH_SIZE; i++)
        printf("%02x", hash[i]);
    printf("\n");
}
