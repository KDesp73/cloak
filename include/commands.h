#ifndef COMMANDS_H
#define COMMANDS_H

#include "cli.h"
#include "context.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int (*CommandFunc)(Context*) ;


int CommandEncrypt(Context* ctx);
int CommandDecrypt(Context* ctx);

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
