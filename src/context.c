#include "context.h"
#include "cli.h"
#include "extern/logging.h"
#include "files.h"
#include <stdlib.h>
#include <string.h>

void CLOAK_ContextInit(CLOAK_Context* ctx, int argc, char** argv)
{
    ctx->input = NULL;
    ctx->output = NULL;
    ctx->key = NULL;
    ctx->is_dir = false;
    ctx->argc = argc;
    ctx->argv = argv;
}

void CLOAK_ContextFree(CLOAK_Context* ctx)
{
    free(ctx->input);
    free(ctx->output);
    free(ctx->key);
}

static bool validateCommandEncrypt(CLOAK_Context* ctx)
{
    if (ctx->output && strcmp(file_extension(ctx->output), "cloak") != 0) {
        ERRO("Output file must use `.cloak` as the extension");
        return false;
    }
    
    return true;
}

static bool validateCommandDecrypt(CLOAK_Context* ctx)
{
    if (!ctx->output) {
        ERRO("Output path is not specified");
        return false;
    }

    if (strcmp(file_extension(ctx->input), "cloak") != 0) {
        ERRO("Input file must use `.cloak` as the extension");
        return false;
    }
    

    if (!ctx->key) {
        ERRO("Provide a path for the key");
        return false;
    }
    
    if (!is_file(ctx->key)) {
        ERRO("%s is not a file", ctx->key);
        return false;
    }
    
    return true;
}

static bool validateCommandHash(CLOAK_Context* ctx)
{
    if (!is_file(ctx->input)) {
        ERRO("Input is not a file");
        return false;
    }
    return true;
}

static bool validateInput(CLOAK_Context* ctx)
{
    if (!ctx->input) {
        ERRO("Input path is not specified");
        return false;
    }

    if (!is_file(ctx->input) && !is_directory(ctx->input)) {
        ERRO("Input is not a file nor a directory");
        return false;
    }

    return true;
}

bool CLOAK_ContextValidate(CLOAK_Context* ctx) 
{
    if (ctx->command == CLOAK_COMMAND_NONE) {
        ERRO("Command is not specified");
        return false;
    }
    
    if (!validateInput(ctx)) {
        return false;
    }

    if (ctx->command == CLOAK_COMMAND_ENCRYPT) {
        if (!validateCommandEncrypt(ctx)) {
            return false;
        }
    }

    if (ctx->command == CLOAK_COMMAND_DECRYPT) {
        if (!validateCommandDecrypt(ctx)) {
            return false;
        }
    }

    if(ctx->command == CLOAK_COMMAND_HASH) {
        if(!validateCommandHash(ctx)) {
            return false;
        }
    }

    return true;
}
