#include "commands.h"
#include "aes.h"
#include "extern/logging.h"

static int encryptFile(const char* in, const char* out, unsigned char key[KEY_SIZE])
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

    if (AESEncryptFile(in, output, key) != 0) {
        ERRO("Encryption of file %s failed.", in);
        free(output);
        return false;
    }
    free(output);
    return true;
}

int CommandEncrypt(Context* ctx)
{
    unsigned char key[KEY_SIZE];
    AESGenerateKey(key);

    // TODO: backup file or directory

    if (ctx->is_dir) {
        TODO("Handle directory encryption");
    } else {
        if(!encryptFile(ctx->input, ctx->output, key))
            return false;
    }

    
    // TODO: Sign key using RSA
    FILE* key_file = fopen(KEY_FILE, "wb");
    if (!key_file) {
        ERRO("Failed to open key file for writing.");
        return false;
    }

    fwrite(key, 1, KEY_SIZE, key_file);
    fclose(key_file);

    return true;
}

int CommandDecrypt(Context* ctx)
{
    unsigned char key[KEY_SIZE];
    FILE* key_file = fopen(KEY_FILE, "rb");
    if (!key_file) {
        ERRO("Failed to open key file for reading.");
        return false;
    }

    if (fread(key, 1, KEY_SIZE, key_file) != KEY_SIZE) {
        ERRO("Failed to read the complete key from file.");
        fclose(key_file);
        return false;
    }
    fclose(key_file);

    if (ctx->is_dir) {
        TODO("Handle directory decryption");
    } else {
        if (AESDecryptFile(ctx->input, ctx->output, key) != 0) {
            ERRO("Decryption of file %s failed.", ctx->input);
            return false;
        }
    }

    
    return true;
}


