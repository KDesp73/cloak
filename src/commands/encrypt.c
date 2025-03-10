#include "aes.h"
#include "commands.h"
#include "compression.h"
#include "config.h"
#include "context.h"
#include "extern/logging.h"
#include "files.h"
#include "listing.h"
#include "rsa.h"

#include <sodium.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

#define CLOAK_SALT_SIZE crypto_pwhash_SALTBYTES
#define CLOAK_PASSWORD_MAX 256

static int encryptFile(const char* in, const char* out, unsigned char key[CLOAK_KEY_SIZE])
{
    char* output = NULL;
    if (out) {
        output = strdup(out);
        if (!output) {
            ERRO("Failed to allocate memory for output filename.");
            return false;
        }
    } else {
        if (asprintf(&output, "%s.cloak", in) == -1) {
            ERRO("Failed to create output filename.");
            return false;
        }
    }

    char* zip = NULL;
    asprintf(&zip, "%s.bin", in);
    CLOAK_Compress(in, zip);

    if (CLOAK_AESEncryptFile(zip, output, key) != 0) {
        ERRO("Encryption of file %s failed.", in);
        file_delete(zip);
        free(zip);
        free(output);
        return false;
    }
    file_delete(zip);
    free(zip);
    free(output);
    return true;
}


int CLOAK_CommandEncrypt(CLOAK_Context* ctx)
{
    unsigned char aes_key[CLOAK_KEY_SIZE];  // AES key to encrypt files
    unsigned char encrypted_key[CLOAK_RSA_KEY_SIZE];  // Encrypted AES key (using RSA public keys)
    size_t encrypted_key_len;

    // Generate a random AES key
    randombytes_buf(aes_key, CLOAK_KEY_SIZE);  // Generate random AES key for encryption

    const char* public_rsa_path = CLOAK_CONFIG_GET_RSA_PUBLIC(&ctx->config);

    if (!CLOAK_RSAEncrypt(aes_key, CLOAK_KEY_SIZE, public_rsa_path, encrypted_key, &encrypted_key_len)) {
        ERRO("RSA encryption of AES key failed for user: %s", public_rsa_path);
        return false;
    }

    // If key server is defined, store the encrypted AES key there
    if (ctx->key_server) {
        if (ctx->key_server->store_key(public_rsa_path, encrypted_key, encrypted_key_len) != 0) {
            ERRO("Failed to store encrypted AES key on key server for user: %s", public_rsa_path);
            return false;
        }
    } else {
        // Otherwise, store the encrypted AES key in a local file for the user
        FILE* key_file = fopen(CLOAK_KEY_FILE, "wb");
        if (!key_file) {
            ERRO("Failed to open key file for writing: %s", CLOAK_KEY_FILE);
            return false;
        }
        fwrite(encrypted_key, 1, encrypted_key_len, key_file);
        fclose(key_file);
    }

    // Encrypt files or directories using the AES key
    if (ctx->is_dir) {
        CLOAK_List list = {0};
        CLOAK_ListLoad(&list, ctx->input, ctx->include_gitignore, ctx->include_cloakignore);

        for (size_t i = 0; i < list.count; i++) {
            if (!encryptFile(list.files[i], NULL, aes_key)) {
                ERRO("Could not encrypt '%s'", list.files[i]);
            }
        }
        CLOAK_ListFree(&list);
    } else {
        if (!encryptFile(ctx->input, ctx->output, aes_key)) {
            return false;
        }
    }

    sodium_memzero(aes_key, CLOAK_KEY_SIZE);  // Clean up sensitive data
    return true;
}

