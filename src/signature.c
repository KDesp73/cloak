#include "signature.h"
#include "config.h"
#include "context.h"
#include "extern/logging.h"

void CLOAK_LoadSignatures(CLOAK_SignatureList* signatures)
{
    FILE* file = fopen(CLOAK_SIGNATURE_FILE, "rb");
    if (!file) {
        signatures->items = NULL;
        signatures->count = 0;
        return;
    }
    
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);
    
    if (file_size % CLOAK_RSA_SIGNATURE_SIZE != 0) {
        fclose(file);
        signatures->items = NULL;
        signatures->count = 0;
        return;
    }
    
    signatures->count = file_size / CLOAK_RSA_SIGNATURE_SIZE;
    signatures->items = (CLOAK_Signature*)malloc(file_size);
    if (!signatures->items) {
        fclose(file);
        signatures->count = 0;
        return;
    }
    
    fread(signatures->items, CLOAK_RSA_SIGNATURE_SIZE, signatures->count, file);
    fclose(file);
}

void CLOAK_ValidateSignatures(const CLOAK_SignatureList* signatures, const CLOAK_Context* ctx)
{
    unsigned char encrypted_key[CLOAK_RSA_KEY_SIZE];
    size_t encrypted_key_len;
    
    FILE* key_file = fopen(CLOAK_KEY_FILE, "rb");
    if (!key_file) {
        ERRO("Failed to open key file for reading.");
        return;
    }
    
    encrypted_key_len = fread(encrypted_key, 1, CLOAK_RSA_KEY_SIZE, key_file);
    fclose(key_file);
    
    if (encrypted_key_len != CLOAK_RSA_KEY_SIZE) {
        ERRO("Invalid encrypted key size.");
        return;
    }
    
    const char* public_rsa_path = CLOAK_CONFIG_GET_RSA_PUBLIC(&ctx->config);
    for (size_t i = 0; i < signatures->count; i++) {
        if (CLOAK_RSAVerify(encrypted_key, CLOAK_RSA_KEY_SIZE, signatures->items[i], CLOAK_RSA_SIGNATURE_SIZE, public_rsa_path) != 0) {
            ERRO("Signature validation failed for signature %zu", i);
        } else {
            INFO("Signature %zu is valid.", i);
        }
    }
}

void CLOAK_FreeSignatures(CLOAK_SignatureList* signatures)
{
    free(signatures->items);
    signatures->items = NULL;
    signatures->count = 0;
}
