#ifndef CONTEXT_H
#define CONTEXT_H

#include "cli.h"
#include "config.h"
#include <stdbool.h>

typedef struct {
    int argc;
    char** argv;
    CLOAK_Command command;
    char* input;
    char* output;
    bool is_dir;
    char* key;
    bool include_gitignore;
    char* type;
    CLOAK_Config config;
} CLOAK_Context;

void CLOAK_ContextInit(CLOAK_Context* ctx, int argc, char** argv);
void CLOAK_ContextLoadConfig(CLOAK_Context* ctx, const CLOAK_Config* config);
void CLOAK_ContextFree(CLOAK_Context* ctx);
bool CLOAK_ContextValidate(CLOAK_Context* ctx);

#ifdef CLOAK_REMOVE_PREFIXES
#define Context CLOAK_Context

#define ContextInit CLOAK_ContextInit
#define ContextLoadConfig CLOAK_ContextLoadConfig
#define ContextFree CLOAK_ContextFree
#define ContextValidate CLOAK_ContextValidate
#endif // CLOAK_REMOVE_PREFIXES

#endif // CONTEXT_H
