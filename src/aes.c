#include "aes.h"
#include "extern/logging.h"
#include "hashing.h"
#include <sodium.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void CLOAK_AESGenerateKey(unsigned char key[CLOAK_KEY_SIZE])
{
    randombytes_buf(key, CLOAK_KEY_SIZE);
}

int CLOAK_AESEncryptFile(const char *input_file, const char *output_file, unsigned char key[CLOAK_KEY_SIZE])
{
    FILE *fin = fopen(input_file, "rb");
    FILE *fout = fopen(output_file, "wb");
    if (!fin || !fout) {
        perror("File open error");
        return -1;
    }

    // Compute the hash of the original file
    unsigned char file_hash[CLOAK_HASH_SIZE];
    if (CLOAK_HashFile(input_file, file_hash) != 0) {
        ERRO("Failed to compute file hash\n");
        fclose(fin);
        fclose(fout);
        return -1;
    }

    // Generate and write nonce
    unsigned char nonce[CLOAK_NONCE_SIZE];
    randombytes_buf(nonce, CLOAK_NONCE_SIZE);
    fwrite(nonce, 1, CLOAK_NONCE_SIZE, fout);

    unsigned char buffer[4096];
    unsigned char encrypted[4096 + CLOAK_MAC_SIZE];
    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), fin)) > 0) {
        if (crypto_secretbox_easy(encrypted, buffer, bytes_read, nonce, key) != 0) {
            ERRO("Encryption failed!\n");
            fclose(fin);
            fclose(fout);
            return -1;
        }
        fwrite(encrypted, 1, bytes_read + CLOAK_MAC_SIZE, fout);
    }

    // Append the hash at the end of the encrypted file
    fwrite(file_hash, 1, CLOAK_HASH_SIZE, fout);

    fclose(fin);
    fclose(fout);
    return 0;
}

int CLOAK_AESEncryptChunk(const unsigned char *input, size_t input_size,
                          unsigned char *output, size_t *output_size,
                          const unsigned char key[crypto_secretbox_KEYBYTES],
                          unsigned char nonce[crypto_secretbox_NONCEBYTES])
{
    if (input_size + crypto_secretbox_MACBYTES > *output_size) {
        fprintf(stderr, "Output buffer too small for encryption\n");
        return -1;
    }

    crypto_secretbox_easy(output, input, input_size, nonce, key);
    *output_size = input_size + crypto_secretbox_MACBYTES;
    return 0;
}

int CLOAK_AESDecryptFile(const char *input_file, const char *output_file, unsigned char key[CLOAK_KEY_SIZE])
{
    FILE *fin = fopen(input_file, "rb");
    if (!fin) {
        perror("File open error");
        return -1;
    }

    fseek(fin, 0, SEEK_END);
    long file_size = ftell(fin);
    if (file_size < CLOAK_NONCE_SIZE + CLOAK_HASH_SIZE) {
        ERRO("File is too small to be valid.\n");
        fclose(fin);
        return -1;
    }

    unsigned char stored_hash[CLOAK_HASH_SIZE];
    fseek(fin, file_size - CLOAK_HASH_SIZE, SEEK_SET); // Seek to the hash position
    if (fread(stored_hash, 1, CLOAK_HASH_SIZE, fin) != CLOAK_HASH_SIZE) {
        ERRO("Failed to read hash from encrypted file.\n");
        fclose(fin);
        return -1;
    }

    // Rewind to the start and read nonce
    rewind(fin);
    unsigned char nonce[CLOAK_NONCE_SIZE];
    if (fread(nonce, 1, CLOAK_NONCE_SIZE, fin) != CLOAK_NONCE_SIZE) {
        ERRO("Failed to read nonce.\n");
        fclose(fin);
        return -1;
    }

    FILE *fout = fopen(output_file, "wb");
    if (!fout) {
        perror("Output file open error");
        fclose(fin);
        return -1;
    }

    unsigned char buffer[4096 + CLOAK_MAC_SIZE];
    unsigned char decrypted[4096];
    size_t bytes_read;
    long remaining = file_size - CLOAK_NONCE_SIZE - CLOAK_HASH_SIZE; // Ignore the last HASH_SIZE bytes

    while (remaining > 0) {
        size_t chunk_size = (remaining < sizeof(buffer)) ? remaining : sizeof(buffer);
        bytes_read = fread(buffer, 1, chunk_size, fin);
        if (bytes_read == 0) break;

        remaining -= bytes_read;

        if (crypto_secretbox_open_easy(decrypted, buffer, bytes_read, nonce, key) != 0) {
            ERRO("Decryption failed!");
            fclose(fin);
            fclose(fout);
            return -1;
        }
        
        size_t decrypted_size = bytes_read - CLOAK_MAC_SIZE;
        fwrite(decrypted, 1, decrypted_size, fout);
    }

    fclose(fin);
    fclose(fout);

    // Compute the hash of the decrypted file
    unsigned char computed_hash[CLOAK_HASH_SIZE];
    if (CLOAK_HashFile(output_file, computed_hash) != 0) {
        ERRO("Failed to compute hash for validation");
        return -1;
    }

    // Compare hashes
    if (sodium_memcmp(stored_hash, computed_hash, CLOAK_HASH_SIZE) != 0) {
        ERRO("Decryption successful, but hash verification failed!");
        return -1;
    }

    return 0;
}

int CLOAK_AESDecryptChunk(const unsigned char *input, size_t input_size,
                          unsigned char *output, size_t *output_size,
                          const unsigned char key[crypto_secretbox_KEYBYTES],
                          const unsigned char nonce[crypto_secretbox_NONCEBYTES])
{
    if (input_size < crypto_secretbox_MACBYTES) {
        fprintf(stderr, "Invalid input size for decryption\n");
        return -1;
    }

    if (crypto_secretbox_open_easy(output, input, input_size, nonce, key) != 0) {
        fprintf(stderr, "Decryption failed\n");
        return -1;
    }

    *output_size = input_size - crypto_secretbox_MACBYTES;
    return 0;
}

