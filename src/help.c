#include "help.h"
#include "extern/ansi.h"

static void help();

void Help(Command command)
{
    switch (command) {
        case COMMAND_ENCODE:
        case COMMAND_DECODE:

        case COMMAND_NONE:
        default:
            help();
    }
}

#define PB(fmt, ...) \
    printf(ANSI_BOLD fmt ANSI_RESET "\n", ##__VA_ARGS__)
#define PI(fmt, ...) \
    printf("  " fmt "\n", ##__VA_ARGS__)

static void footer()
{
    printf("\nWritten by KDesp73 (Konstantinos Despoinidis)\n");
}

static void help()
{
    PB("USAGE");
    PI("cloak [COMMAND] [OPTIONS...]");
    printf("\n");

    PB("COMMANDS");
    PI("encode <FILE>  Encode a file");
    PI("decode <FILE>  Decode a file");
    printf("\n");

    PB("OPTIONS");
    PI("-h --help       Prints this message");
    PI("-v --version    Prints the current version");

    footer();
}

