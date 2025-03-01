#include "help.h"
#include "cli.h"
#include "extern/ansi.h"

static void help();
static void encryptHelp();
static void decryptHelp();
static void hashHelp();
static void lsHelp();
static void footer();

void CLOAK_Help(CLOAK_Command command)
{
    switch (command) {
        case CLOAK_COMMAND_ENCRYPT:
            encryptHelp();
            break;
        case CLOAK_COMMAND_DECRYPT:
            decryptHelp();
            break;
        case CLOAK_COMMAND_HASH:
            hashHelp();
            break;
        case CLOAK_COMMAND_LS:
            lsHelp();
            break;

        case CLOAK_COMMAND_NONE:
        default:
            help();
    }
    footer();
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
    PI("cloak [<COMMAND>] <OPTIONS>...");
    printf("\n");

    PB("COMMANDS");
    PI("encrypt                Encrypt a file");
    PI("decrypt                Decrypt a file");
    PI("hash                   Prints the hash of a file");
    PI("ls                     Lists the files considered in the encryption process");
    printf("\n");

    PB("OPTIONS");
    PI("-h --help              Prints this message");
    PI("-v --version           Prints the current version");
    PI("-k --key <PATH>        Specify the encryption key path");
    PI("-i --input <PATH>      Specify the input path");
    PI("-o --output <PATH>     Specify the output path");
    PI("-G --gitignore-ignore  Do not include the gitignore patterns alongside cloakignore");
}

static void encryptHelp()
{
    PB("USAGE");
    PI("cloak encrypt -i <PATH> [-o <PATH>]"); 
    printf("\n");

    PB("OPTIONS");
    PI("-h --help              Prints this message");
    PI("-i --input <PATH>      Specify the input path");
    PI("-o --output <PATH>     Specify the output path");
    PI("-G --gitignore-ignore  Do not include the gitignore patterns alongside cloakignore");
    printf("\n");

    PB("NOTES");
    PI("- The default output path is <INPUT>.cloak.");
    PI("- Even if an alternative output path is specified the extension must remain `.cloak`.");
    PI("- The AES key is written in `aes.key` so it can be used in the decryption process.");
    printf("\n");

    PB("EXAMPLE");
    PI("Command: cloak encrypt -i src/main.c");
    PI("Output: src/main.c.cloak");
}

static void decryptHelp()
{
    PB("USAGE");
    PI("cloak decrypt -i <PATH> -o <PATH> -k <KEY-PATH>"); 
    printf("\n");

    PB("OPTIONS");
    PI("-h --help              Prints this message");
    PI("-i --input <PATH>      Specify the input path");
    PI("-o --output <PATH>     Specify the output path");
    PI("-k --key <PATH>        Specify the encryption key path");
}

static void hashHelp()
{
    PB("USAGE");
    PI("cloak hash -i <PATH>");
    printf("\n");

    PB("OPTIONS");
    PI("-h --help              Prints this message");
    PI("-i --input <PATH>      Specify the input path");
}

static void lsHelp()
{
    PB("USAGE");
    PI("cloak ls [-i <PATH>]");
    printf("\n");

    PB("OPTIONS");
    PI("-h --help              Prints this message");
    PI("-i --input <PATH>      Specify the input path");
    PI("-G --gitignore-ignore  Do not include the gitignore patterns alongside cloakignore");
}
