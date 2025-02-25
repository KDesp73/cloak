#include "commands.h"
#include "aes.h"
#include "extern/logging.h"
#include "hashing.h"

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

    if (CLOAK_AESEncryptFile(in, output, key) != 0) {
        ERRO("Encryption of file %s failed.", in);
        free(output);
        return false;
    }
    free(output);
    return true;
}

int CLOAK_CommandEncrypt(CLOAK_Context* ctx)
{
    unsigned char key[CLOAK_KEY_SIZE];
    CLOAK_AESGenerateKey(key);

    // TODO: backup file or directory

    if (ctx->is_dir) {
        TODO("Handle directory encryption");
    } else {
        if(!encryptFile(ctx->input, ctx->output, key))
            return false;
    }

    
    // TODO: Sign key using RSA
    FILE* key_file = fopen(CLOAK_KEY_FILE, "wb");
    if (!key_file) {
        ERRO("Failed to open key file for writing.");
        return false;
    }

    fwrite(key, 1, CLOAK_KEY_SIZE, key_file);
    fclose(key_file);

    return true;
}

int CLOAK_CommandDecrypt(CLOAK_Context* ctx)
{
    unsigned char key[CLOAK_KEY_SIZE];
    FILE* key_file = fopen(CLOAK_KEY_FILE, "rb");
    if (!key_file) {
        ERRO("Failed to open key file for reading.");
        return false;
    }

    if (fread(key, 1, CLOAK_KEY_SIZE, key_file) != CLOAK_KEY_SIZE) {
        ERRO("Failed to read the complete key from file.");
        fclose(key_file);
        return false;
    }
    fclose(key_file);

    if (ctx->is_dir) {
        TODO("Handle directory decryption");
    } else {
        if (CLOAK_AESDecryptFile(ctx->input, ctx->output, key) != 0) {
            ERRO("Decryption of file %s failed.", ctx->input);
            return false;
        }
    }

    
    return true;
}

int CLOAK_CommandHash(CLOAK_Context* ctx)
{
    unsigned char hash[CLOAK_HASH_SIZE];
    CLOAK_HashFile(ctx->input, hash);
    CLOAK_HashPrint(hash);

    return true;
}

