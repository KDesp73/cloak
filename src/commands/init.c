#include "commands.h"
#include "config.h"
#include "extern/logging.h"
#include "files.h"

int CLOAK_CommandInit(CLOAK_Context* ctx)
{
    if(dir_exists(CLOAK_CONFIG_DIRECTORY)) {
        WARN("cloak is already initialized in this project");
        return false;
    }

    dir_create(CLOAK_CONFIG_DIRECTORY);
    dir_create(CLOAK_CONFIG_DEFAULT_BACKUP);
    dir_create(CLOAK_CONFIG_DEFAULT_DECRYPTED);

    return true;
}

