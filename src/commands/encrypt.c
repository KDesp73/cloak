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
    unsigned char key[CLOAK_KEY_SIZE];
    unsigned char salt[CLOAK_SALT_SIZE];
    unsigned char encrypted_key[CLOAK_RSA_KEY_SIZE];
    size_t encrypted_key_len;

    // Ask for the password
    char password[CLOAK_PASSWORD_MAX];
    if (!CLOAK_PromptPassword("Enter encryption password: ", password, sizeof(password))) {
        ERRO("Failed to get password.");
        return false;
    }

    // Check if a salt exists; otherwise, generate a new one
    FILE* salt_file = fopen(CLOAK_SALT_FILE, "rb");
    if (salt_file) {
        if (fread(salt, 1, CLOAK_SALT_SIZE, salt_file) != CLOAK_SALT_SIZE) {
            ERRO("Failed to read existing salt.");
            fclose(salt_file);
            return false;
        }
        fclose(salt_file);
    } else {
        randombytes_buf(salt, CLOAK_SALT_SIZE);
        salt_file = fopen(CLOAK_SALT_FILE, "wb");
        if (!salt_file) {
            ERRO("Failed to store salt.");
            return false;
        }
        fwrite(salt, 1, CLOAK_SALT_SIZE, salt_file);
        fclose(salt_file);
    }

    // Derive key from password and salt
    if (crypto_pwhash(key, CLOAK_KEY_SIZE, password, strlen(password), salt,
                      crypto_pwhash_OPSLIMIT_MODERATE, crypto_pwhash_MEMLIMIT_MODERATE,
                      crypto_pwhash_ALG_DEFAULT) != 0) {
        ERRO("Key derivation failed.");
        return false;
    }

    // Encrypt the derived key with RSA for collaboration
    const char* public_rsa_path = CLOAK_CONFIG_GET_RSA_PUBLIC(&ctx->config);
    if (!CLOAK_RSAEncrypt(key, CLOAK_KEY_SIZE, public_rsa_path, encrypted_key, &encrypted_key_len)) {
        ERRO("RSA encryption of key failed.");
        return false;
    }

    // Store the encrypted AES key
    FILE* key_file = fopen(CLOAK_KEY_FILE, "wb");
    if (!key_file) {
        ERRO("Failed to open key file for writing.");
        return false;
    }
    fwrite(encrypted_key, 1, encrypted_key_len, key_file);
    fclose(key_file);

    // Encrypt files or directories
    if (ctx->is_dir) {
        CLOAK_List list = {0};
        CLOAK_ListLoad(&list, ctx->input, ctx->include_gitignore, ctx->include_cloakignore);

        for (size_t i = 0; i < list.count; i++) {
            if (!encryptFile(list.files[i], NULL, key)) {
                ERRO("Could not encrypt '%s'", list.files[i]);
            }
        }
        CLOAK_ListFree(&list);
    } else {
        if (!encryptFile(ctx->input, ctx->output, key)) {
            return false;
        }
    }

    sodium_memzero(key, CLOAK_KEY_SIZE);
    sodium_memzero(password, CLOAK_PASSWORD_MAX);
    return true;
}

