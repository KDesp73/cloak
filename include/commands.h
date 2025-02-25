#ifndef COMMANDS_H
#define COMMANDS_H

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
    if(ctx->is_dir)
        TODO("Handle directory encryption");

    char* output = NULL;

    if (ctx->output) {
        if(strcmp(file_extension(ctx->output), "cloak") != 0) {
            ERRO("Output file must use `.cloak` as the extension");
            return false;
        }
        output = strdup(ctx->output);
    } else {
        asprintf(&output, "%s.cloak", ctx->input);
    }

    if (EncryptFile(ctx->input, output, ctx->key) != 0) {
        ERRO("Encryption of file %s failed.", ctx->input);
        free(output);
        return false;
    }
    free(output);
    return true;
}
static inline int CommandDecrypt(Context* ctx)
{
    if(ctx->is_dir)
        TODO("Handle directory decryption");

    if(!ctx->output) {
        ERRO("Output path is not specified");
        return false;
    }

    if (DecryptFile(ctx->input, ctx->output, ctx->key) != 0) {
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
