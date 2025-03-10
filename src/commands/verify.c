#include "commands.h"
#include "config.h"
#include "extern/logging.h"
#include "files.h"
#include "hashing.h"
#include "signature.h"
#include <sodium/crypto_generichash.h>
#include <sodium/utils.h>
#include <sodium.h>

static int verifySignatures(const char* path)
{


    return true;
}

#define KEY_SIG_SIZE crypto_sign_BYTES
#define KEY_PUBKEY_SIZE crypto_sign_PUBLICKEYBYTES

static int verifyKey(const char* key_path, const char* sig_path, const char* pubkey_path) {
    if (!key_path || !sig_path || !pubkey_path) {
        fprintf(stderr, "Invalid file paths provided.\n");
        return 0;
    }

    // Load public key
    unsigned char public_key[KEY_PUBKEY_SIZE];
    FILE* pubkey_file = fopen(pubkey_path, "rb");
    if (!pubkey_file || fread(public_key, 1, KEY_PUBKEY_SIZE, pubkey_file) != KEY_PUBKEY_SIZE) {
        fprintf(stderr, "Failed to load public key.\n");
        if (pubkey_file) fclose(pubkey_file);
        return 0;
    }
    fclose(pubkey_file);

    // Load key file
    FILE* key_file = fopen(key_path, "rb");
    if (!key_file) {
        fprintf(stderr, "Failed to open key file: %s\n", key_path);
        return 0;
    }

    fseek(key_file, 0, SEEK_END);
    size_t key_size = ftell(key_file);
    rewind(key_file);

    unsigned char* key_data = (unsigned char*)malloc(key_size);
    if (!key_data || fread(key_data, 1, key_size, key_file) != key_size) {
        fprintf(stderr, "Failed to read key file.\n");
        free(key_data);
        fclose(key_file);
        return 0;
    }
    fclose(key_file);

    // Load signature
    unsigned char signature[KEY_SIG_SIZE];
    FILE* sig_file = fopen(sig_path, "rb");
    if (!sig_file || fread(signature, 1, KEY_SIG_SIZE, sig_file) != KEY_SIG_SIZE) {
        fprintf(stderr, "Failed to load signature.\n");
        free(key_data);
        fclose(sig_file);
        return 0;
    }
    fclose(sig_file);

    // Verify signature
    if (crypto_sign_verify_detached(signature, key_data, key_size, public_key) != 0) {
        fprintf(stderr, "Key verification failed! Possible tampering detected.\n");
        free(key_data);
        return 0;
    }

    printf("Key verification successful. The key is valid.\n");
    free(key_data);
    return 1;
}

int CLOAK_CommandVerify(CLOAK_Context* ctx)
{
    const char* file = ctx->input;
    if(!strcmp(file_extension(file), "sig")){
        if(!verifySignatures(file)){
            ERRO("Verification of cloak.key failed");
            return false;
        }
    } else if(!strcmp(file_extension(file), "key")){
        if(!verifyKey(file, CLOAK_SIGNATURE_FILE, CLOAK_CONFIG_GET_RSA_PUBLIC(&ctx->config))){
            ERRO("Verification of cloak.key failed");
            return false;
        }
    } else {
        // TODO: Verify any other file
        ERRO("Please provide either the cloak.sig or cloak.key file");
        return false;
    }
    

    return true;
}

