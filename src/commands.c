#include "commands.h"
#include "aes.h"
#include "compression.h"
#include "extern/logging.h"
#include "files.h"
#include "hashing.h"
#include "listing.h"
#include <stdlib.h>
#include <string.h>

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

int decryptFile(const char* in, const char* out, unsigned char key[CLOAK_KEY_SIZE])
{
    char* zip = NULL;
    asprintf(&zip, "%s.bin", in);

    if (CLOAK_AESDecryptFile(in, zip, key) != 0) {
        ERRO("Decryption of file %s failed.", in);
        file_delete(zip);
        free(zip);
        return false;
    }

    if(CLOAK_Decompress(zip, out) != 0){
        ERRO("Failed decompressing file %s", in);
        file_delete(zip);
        free(zip);
        return false;
    }

    file_delete(zip);
    free(zip);
    return true;
}

int CLOAK_CommandDecrypt(CLOAK_Context* ctx)
{
    const char* key_path = NULL;
    if(ctx->key){
        key_path = ctx->key;
    } else {
        key_path = CLOAK_KEY_FILE;
    }

    unsigned char key[CLOAK_KEY_SIZE];
    FILE* key_file = fopen(key_path, "rb");
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
        if(!decryptFile(ctx->input, ctx->output, key))
            return false;
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

int CLOAK_CommandLs(CLOAK_Context* ctx)
{
    const char* path = (ctx->input) ? ctx->input : ".";

    CLOAK_List list = {0};
    CLOAK_ListLoad(&list, path);
    CLOAK_ListPrint(&list);
    CLOAK_ListFree(&list);

    return true;
}
