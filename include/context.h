#ifndef CONTEXT_H
#define CONTEXT_H

#include "aes.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int argc;
    char** argv;

    unsigned char key[KEY_SIZE];
    bool key_random;

    char* input;
    char* output;
    bool is_dir;
} Context;

static inline void ContextInit(Context* ctx, int argc, char** argv)
{
    ctx->key_random = true;
    memset(ctx->key, 0, KEY_SIZE);
    ctx->input = NULL;
    ctx->output = NULL;
    ctx->is_dir = false;
    ctx->argc = argc;
    ctx->argv = argv;
}

static inline void ContextFree(Context* ctx)
{
    free(ctx->input);
    free(ctx->output);
}

#endif // CONTEXT_H
