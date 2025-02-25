#include "aes.h"
#include "extern/logging.h"
#include "hashing.h"
#include <sodium.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void AESGenerateKey(unsigned char key[KEY_SIZE])
{
    randombytes_buf(key, KEY_SIZE);
}

int AESEncryptFile(const char *input_file, const char *output_file, unsigned char key[KEY_SIZE])
{
    FILE *fin = fopen(input_file, "rb");
    FILE *fout = fopen(output_file, "wb");
    if (!fin || !fout) {
        perror("File open error");
        return -1;
    }

    // Compute the hash of the original file
    unsigned char file_hash[HASH_SIZE];
    if (HashFile(input_file, file_hash) != 0) {
        ERRO("Failed to compute file hash\n");
        fclose(fin);
        fclose(fout);
        return -1;
    }

    // Generate and write nonce
    unsigned char nonce[NONCE_SIZE];
    randombytes_buf(nonce, NONCE_SIZE);
    fwrite(nonce, 1, NONCE_SIZE, fout);

    unsigned char buffer[4096];
    unsigned char encrypted[4096 + MAC_SIZE];
    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), fin)) > 0) {
        if (crypto_secretbox_easy(encrypted, buffer, bytes_read, nonce, key) != 0) {
            ERRO("Encryption failed!\n");
            fclose(fin);
            fclose(fout);
            return -1;
        }
        fwrite(encrypted, 1, bytes_read + MAC_SIZE, fout);
    }

    // Append the hash at the end of the encrypted file
    fwrite(file_hash, 1, HASH_SIZE, fout);

    fclose(fin);
    fclose(fout);
    return 0;
}

int AESDecryptFile(const char *input_file, const char *output_file, unsigned char key[KEY_SIZE])
{
    FILE *fin = fopen(input_file, "rb");
    if (!fin) {
        perror("File open error");
        return -1;
    }

    fseek(fin, 0, SEEK_END);
    long file_size = ftell(fin);
    if (file_size < NONCE_SIZE + HASH_SIZE) {
        ERRO("File is too small to be valid.\n");
        fclose(fin);
        return -1;
    }

    unsigned char stored_hash[HASH_SIZE];
    fseek(fin, file_size - HASH_SIZE, SEEK_SET); // Seek to the hash position
    if (fread(stored_hash, 1, HASH_SIZE, fin) != HASH_SIZE) {
        ERRO("Failed to read hash from encrypted file.\n");
        fclose(fin);
        return -1;
    }

    // Rewind to the start and read nonce
    rewind(fin);
    unsigned char nonce[NONCE_SIZE];
    if (fread(nonce, 1, NONCE_SIZE, fin) != NONCE_SIZE) {
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

    unsigned char buffer[4096 + MAC_SIZE];
    unsigned char decrypted[4096];
    size_t bytes_read;
    long remaining = file_size - NONCE_SIZE - HASH_SIZE; // Ignore the last HASH_SIZE bytes

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
        
        size_t decrypted_size = bytes_read - MAC_SIZE;
        fwrite(decrypted, 1, decrypted_size, fout);
    }

    fclose(fin);
    fclose(fout);

    // Compute the hash of the decrypted file
    unsigned char computed_hash[HASH_SIZE];
    if (HashFile(output_file, computed_hash) != 0) {
        ERRO("Failed to compute hash for validation");
        return -1;
    }

    // Compare hashes
    if (sodium_memcmp(stored_hash, computed_hash, HASH_SIZE) != 0) {
        ERRO("Decryption successful, but hash verification failed!");
        return -1;
    }

    return 0;
}

