#ifndef COMMANDS_H
#define COMMANDS_H

#include "cli.h"
#include "context.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int (*CLOAK_CommandFunc)(CLOAK_Context*) ;

int CLOAK_CommandEncrypt(CLOAK_Context* ctx);
int CLOAK_CommandDecrypt(CLOAK_Context* ctx);
int CLOAK_CommandHash(CLOAK_Context* ctx);
int CLOAK_CommandLs(CLOAK_Context* ctx);
int CLOAK_CommandGenerate(CLOAK_Context* ctx);
int CLOAK_CommandInit(CLOAK_Context* ctx);

static inline CLOAK_CommandFunc CLOAK_GetCommandFunc(CLOAK_Command command)
{
    switch (command) {
    case CLOAK_COMMAND_ENCRYPT:
        return CLOAK_CommandEncrypt;
    case CLOAK_COMMAND_DECRYPT:
        return CLOAK_CommandDecrypt;
    case CLOAK_COMMAND_HASH:
        return CLOAK_CommandHash;
    case CLOAK_COMMAND_LS:
        return CLOAK_CommandLs;
    case CLOAK_COMMAND_GENERATE:
        return CLOAK_CommandGenerate;
    case CLOAK_COMMAND_INIT:
        return CLOAK_CommandInit;

    case CLOAK_COMMAND_NONE:
    default:
        return NULL;
    }
}

#ifdef CLOAK_REMOVE_PREFIXES
#define CommandFunc CLOAK_CommandFunc

#define CommandEncrypt CLOAK_CommandEncrypt
#define CommandDecrypt CLOAK_CommandDecrypt
#define CommandHash CLOAK_CommandHash
#define CommandGenerate CLOAK_CommandGenerate
#define CommandInit CLOAK_CommandInit

#define GetCommandFunc CLOAK_GetCommandFunc
#endif // CLOAK_REMOVE_PREFIXES

#endif // COMMANDS_H
