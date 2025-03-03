#include "commands.h"
#include "aes.h"
#include "compression.h"
#include "config.h"
#include "context.h"
#include "extern/logging.h"
#include "files.h"
#include "rsa.h"
#include "listing.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

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
        CLOAK_List list = {0};
        CLOAK_ListLoad(&list, ctx->input, ctx->include_gitignore, ctx->include_cloakignore);

        for (size_t i = 0; i < list.count; i++) {
            if(!encryptFile(list.files[i], NULL, key))
                ERRO("Could not encrypt '%s'", list.files[i]);
        }

        CLOAK_ListFree(&list);
    } else {
        if(!encryptFile(ctx->input, ctx->output, key))
            return false;
    }

    

    unsigned char rsa_encrypted_key[CLOAK_RSA_KEY_SIZE];
    size_t encrypted_key_len;
    // TODO: change the public key path to match the config
    if(CLOAK_RSAEncrypt(key, CLOAK_KEY_SIZE, CLOAK_CONFIG_DEFAULT_RSA_PUBLIC, rsa_encrypted_key, &encrypted_key_len) != 0) {
        ERRO("RSA encryption failed");
        return false;
    }

    FILE* key_file = fopen(CLOAK_KEY_FILE, "wb");
    if (!key_file) {
        ERRO("Failed to open key file for writing.");
        return false;
    }

    // fwrite(key, 1, CLOAK_KEY_SIZE, key_file); // NOTE: No RSA encryption
    fwrite(rsa_encrypted_key, 1, encrypted_key_len, key_file);
    fclose(key_file);

    return true;
}

