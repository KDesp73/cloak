#ifndef COMMANDS_H
#define COMMANDS_H

#include "cli.h"
#include "context.h"
#include <stdio.h>
#include "extern/logging.h"

typedef int (*CommandFunc)(Context*) ;


static inline int CommandEncrypt(Context* ctx)
{
    if(ctx->is_dir)
        TODO("Handle directory encryption");

    if (EncryptFile(ctx->input, ctx->output, ctx->key) != 0) {
        ERRO("Encryption of file %s failed.", ctx->input);
        return false;
    }
    return true;
}
static inline int CommandDecrypt(Context* ctx)
{
    if(ctx->is_dir)
        TODO("Handle directory decryption");

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
