#include "context.h"
#include "cli.h"
#include "extern/logging.h"
#include "files.h"
#include <stdlib.h>
#include <string.h>

void ContextInit(Context* ctx, int argc, char** argv)
{
    ctx->key_random = true;
    memset(ctx->key, 0, KEY_SIZE);
    ctx->input = NULL;
    ctx->output = NULL;
    ctx->is_dir = false;
    ctx->argc = argc;
    ctx->argv = argv;
}

void ContextFree(Context* ctx)
{
    free(ctx->input);
    free(ctx->output);
}

bool ContextValidate(Context* ctx)
{
    if(!ctx->input) {
        ERRO("Input path is not specified");
        return false;
    } else {
        if(!is_file(ctx->input) && !is_directory(ctx->input)) {
            ERRO("Input is not a file nor a directory");
            return false;
        }
    }
    if(ctx->command == COMMAND_NONE) {
        ERRO("Command is not specified");
        return false;
    } 

    return true;
}

