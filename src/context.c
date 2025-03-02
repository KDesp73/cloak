#include "context.h"
#include "cli.h"
#include "config.h"
#include "extern/logging.h"
#include "files.h"
#include <openssl/err.h>
#include <stdlib.h>
#include <string.h>

void CLOAK_ContextInit(CLOAK_Context* ctx, int argc, char** argv)
{
    // Default values
    ctx->input = NULL;
    ctx->output = NULL;
    ctx->key = NULL;
    ctx->type = NULL;
    ctx->is_dir = false;
    ctx->argc = argc;
    ctx->argv = argv;
    ctx->include_gitignore = true;
    
    // Load ini config
    CLOAK_ConfigLoad(&ctx->config, CLOAK_CONFIG_FILE);
    CLOAK_ContextLoadConfig(ctx, &ctx->config);
}

void CLOAK_ContextLoadConfig(CLOAK_Context* ctx, const CLOAK_Config* config)
{
    if(!config->initialized) return;

    char* include_gitignore = CLOAK_ConfigGet(config, CLOAK_CONFIG_INCLUDE_GITIGNORE);
    if(include_gitignore)
        ctx->include_gitignore = BOOL(include_gitignore);
}


void CLOAK_ContextFree(CLOAK_Context* ctx)
{
    free(ctx->input);
    free(ctx->output);
    free(ctx->key);
    free(ctx->type);
    CLOAK_ConfigFree(&ctx->config);
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

static bool validateCommandEncrypt(CLOAK_Context* ctx)
{
    if (!validateInput(ctx)) {
        return false;
    }

    if (ctx->output && strcmp(file_extension(ctx->output), "cloak") != 0) {
        ERRO("Output file must use `.cloak` as the extension");
        return false;
    }
    
    return true;
}

static bool validateCommandDecrypt(CLOAK_Context* ctx)
{
    if (!validateInput(ctx)) {
        return false;
    }

    if (is_file(ctx->input) && strcmp(file_extension(ctx->input), "cloak") != 0) {
        ERRO("Input file must use `.cloak` as the extension");
        return false;
    }
    
    if (ctx->key && !is_file(ctx->key)) {
        ERRO("%s is not a file", ctx->key);
        return false;
    }
    
    return true;
}

static bool validateCommandHash(CLOAK_Context* ctx)
{
    if (!validateInput(ctx)) {
        return false;
    }

    if (!is_file(ctx->input)) {
        ERRO("Input is not a file");
        return false;
    }
    return true;
}

static bool validateCommandGenerate(CLOAK_Context* ctx)
{
    if(ctx->type == NULL) {
        ERRO("Specify a type (--type=<TYPE>)");
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

    if(ctx->command == CLOAK_COMMAND_GENERATE) {
        if(!validateCommandGenerate(ctx)) {
            return false;
        }
    }

    return true;
}
