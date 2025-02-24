#ifndef COMMANDS_H
#define COMMANDS_H

typedef enum {
    COMMAND_NONE,
    COMMAND_ENCODE,
    COMMAND_DECODE
} Command;

typedef enum {
    FLAG_HELP    = 'h',
    FLAG_VERSION = 'v',
} Flag;

char* CommandToString(Command c);
Command GetCommand(const char* command);


#endif // COMMANDS_H
