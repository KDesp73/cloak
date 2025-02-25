#ifndef COMMANDS_H
#define COMMANDS_H

#include "aes.h"
#include "cli.h"
#include "context.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "extern/logging.h"
#include "files.h"

typedef int (*CommandFunc)(Context*) ;


static inline int CommandEncrypt(Context* ctx)
{
    if (ctx->is_dir) {
        TODO("Handle directory encryption");
    }

    char* output = NULL;
    if (ctx->output) {
        output = strdup(ctx->output);
        if (!output) {
            ERRO("Failed to allocate memory for output filename.");
            return false;
        }
    } else {
        if (asprintf(&output, "%s.cloak", ctx->input) == -1) {
            ERRO("Failed to create output filename.");
            return false;
        }
    }

    unsigned char key[KEY_SIZE];
    GenerateKey(key);

    if (EncryptFile(ctx->input, output, key) != 0) {
        ERRO("Encryption of file %s failed.", ctx->input);
        free(output);
        return false;
    }
    
    FILE* key_file = fopen("aes.key", "wb");
    if (!key_file) {
        ERRO("Failed to open key file for writing.");
        free(output);
        return false;
    }

    fwrite(key, 1, KEY_SIZE, key_file);
    fclose(key_file);
    free(output);

    return true;
}

static inline int CommandDecrypt(Context* ctx)
{
    unsigned char key[KEY_SIZE];
    FILE* key_file = fopen("aes.key", "rb");
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
    }

    if (DecryptFile(ctx->input, ctx->output, key) != 0) {
        ERRO("Decryption of file %s failed.", ctx->input);
        return false;
    }
    
    return true;
}

static inline CommandFunc GetCommandFunc(Command command)
{
    switch (command) {
    case COMMAND_ENCRYPT:
        return CommandEncrypt;
    case COMMAND_DECRYPT:
        return CommandDecrypt;
    case COMMAND_NONE:
    default:
        return NULL;
    }
}

#endif // COMMANDS_H
