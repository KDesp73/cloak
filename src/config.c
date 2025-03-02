#include "config.h"
#include "files.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define INI_IMPLEMENTATION
#include "extern/ini.h"

void CLOAK_ConfigDirCreate()
{
    if(file_exists(CLOAK_CONFIG_DIRECTORY)) return;

    dir_create(CLOAK_CONFIG_DIRECTORY);
}

static int handler(void* user, const char* section, const char* name, const char* value)
{
    CLOAK_Config* config = (CLOAK_Config*) user;
    CLOAK_ConfigAddEntry(config, CLOAK_ConfigEntryInit(section, name, value));
    return 1;
}

int CLOAK_ConfigLoad(CLOAK_Config* config, const char* path)
{
    if (ini_parse(path, handler, config) < 0) {
        config->initialized = false;
        return false;
    }

    config->initialized = true;
    return true;
}

void CLOAK_ConfigAddEntry(CLOAK_Config* config, CLOAK_ConfigEntry entry)
{
    if(config->count >= CLOAK_CONFIG_MAX_ENTRIES) return;
    config->entries[config->count++] = entry;
}

void CLOAK_ConfigFree(CLOAK_Config* config)
{
    for(size_t i = 0; i < config->count; i++) {
        CLOAK_ConfigEntryFree(&config->entries[i]);
    }
}

char* CLOAK_ConfigGet(const CLOAK_Config* config, const char* section, const char* key)
{
#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(key, n) == 0
    for(size_t i = 0; i < config->count; i++) {
        CLOAK_ConfigEntry entry = config->entries[i];
        if(MATCH(entry.section, entry.key)) {
            return entry.value;
        }
    }

#undef MATCH
    return NULL;
}

void CLOAK_ConfigPrint(const CLOAK_Config* config)
{
    for(size_t i = 0; i < config->count; i++) {
        CLOAK_ConfigEntry entry = config->entries[i];
        printf("[%s] %s = %s\n", entry.section, entry.key, entry.value);
    }
}


CLOAK_ConfigEntry CLOAK_ConfigEntryInit(const char* section, const char* key, const char* value)
{
    return (CLOAK_ConfigEntry) {
        .section = strdup(section),
        .key     = strdup(key),
        .value   = strdup(value)
    };
}

void CLOAK_ConfigEntryFree(CLOAK_ConfigEntry* entry)
{
    free(entry->section);
    free(entry->key);
    free(entry->value);
}
