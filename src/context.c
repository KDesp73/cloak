#include "context.h"
#include "cli.h"
#include "extern/logging.h"
#include "files.h"
#include <stdlib.h>
#include <string.h>

void ContextInit(Context* ctx, int argc, char** argv)
{
    ctx->input = NULL;
    ctx->output = NULL;
    ctx->key = NULL;
    ctx->is_dir = false;
    ctx->argc = argc;
    ctx->argv = argv;
}

void ContextFree(Context* ctx)
{
    free(ctx->input);
    free(ctx->output);
    free(ctx->key);
}

static bool validateCommandEncrypt(Context* ctx)
{
    if (ctx->output && strcmp(file_extension(ctx->output), "cloak") != 0) {
        ERRO("Output file must use `.cloak` as the extension");
        return false;
    }
    
    return true;
}

static bool validateCommandDecrypt(Context* ctx)
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

static bool validateInput(Context* ctx)
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

bool ContextValidate(Context* ctx) 
{
    if (ctx->command == COMMAND_NONE) {
        ERRO("Command is not specified");
        return false;
    }
    
    if (!validateInput(ctx)) {
        return false;
    }

    if (ctx->command == COMMAND_ENCRYPT) {
        if (!validateCommandEncrypt(ctx)) {
            return false;
        }
    }

    if (ctx->command == COMMAND_DECRYPT) {
        if (!validateCommandDecrypt(ctx)) {
            return false;
        }
    }

    return true;
}
