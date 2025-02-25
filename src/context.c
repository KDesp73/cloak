#include "context.h"
#include "cli.h"
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

// TODO: Add error message for each case
bool ContextValidate(Context* ctx)
{
    if(!ctx->input) return false;
    if(!ctx->output) return false;
    if(ctx->command == COMMAND_NONE) return false;

    return true;
}

