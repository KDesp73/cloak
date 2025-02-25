#include "aes.h"
#include <sodium.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void GenerateKey(unsigned char key[KEY_SIZE])
{
    randombytes_buf(key, KEY_SIZE);
}

int EncryptFile(const char *input_file, const char *output_file, unsigned char key[KEY_SIZE])
{
    FILE *fin = fopen(input_file, "rb");
    FILE *fout = fopen(output_file, "wb");
    if (!fin || !fout) {
        perror("File open error");
        return -1;
    }

    unsigned char nonce[NONCE_SIZE];
    randombytes_buf(nonce, NONCE_SIZE);
    fwrite(nonce, 1, NONCE_SIZE, fout); // Store nonce at the beginning of the output file

    unsigned char buffer[4096];
    unsigned char encrypted[4096 + MAC_SIZE];
    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), fin)) > 0) {
        if (crypto_secretbox_easy(encrypted, buffer, bytes_read, nonce, key) != 0) {
            fprintf(stderr, "Encryption failed!\n");
            fclose(fin);
            fclose(fout);
            return -1;
        }
        fwrite(encrypted, 1, bytes_read + MAC_SIZE, fout);
    }

    fclose(fin);
    fclose(fout);
    return 0;
}

int DecryptFile(const char *input_file, const char *output_file, unsigned char key[KEY_SIZE])
{
    FILE *fin = fopen(input_file, "rb");
    FILE *fout = fopen(output_file, "wb");
    if (!fin || !fout) {
        perror("File open error");
        return -1;
    }

    // Read the nonce from the input file
    unsigned char nonce[NONCE_SIZE];
    fread(nonce, 1, NONCE_SIZE, fin);

    unsigned char buffer[4096 + MAC_SIZE];
    unsigned char decrypted[4096];
    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), fin)) > 0) {
        if (crypto_secretbox_open_easy(decrypted, buffer, bytes_read, nonce, key) != 0) {
            fprintf(stderr, "Decryption failed!\n");
            fclose(fin);
            fclose(fout);
            return -1;
        }
        fwrite(decrypted, 1, bytes_read - MAC_SIZE, fout);
    }

    fclose(fin);
    fclose(fout);
    return 0;
}

