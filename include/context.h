#ifndef CONTEXT_H
#define CONTEXT_H

#include "aes.h"
#include "cli.h"
#include <stdbool.h>

typedef struct {
    int argc;
    char** argv;
    Command command;
    char* input;
    char* output;
    bool is_dir;
    char* key;
} Context;

void ContextInit(Context* ctx, int argc, char** argv);
void ContextFree(Context* ctx);
bool ContextValidate(Context* ctx);

#endif // CONTEXT_H
