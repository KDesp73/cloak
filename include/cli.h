#ifndef CLOAK_CLI_H
#define CLOAK_CLI_H

typedef enum {
    COMMAND_NONE,
    COMMAND_ENCRYPT,
    COMMAND_DECRYPT,
    COMMAND_HASH
} Command;

typedef enum {
    FLAG_HELP    = 'h',
    FLAG_VERSION = 'v',
    FLAG_KEY = 'k',
    FLAG_INPUT = 'i',
    FLAG_OUTPUT = 'o',
} Flag;

char* CommandToString(Command c);
Command GetCommand(const char* command);


#endif // CLOAK_CLI_H
