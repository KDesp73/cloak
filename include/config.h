#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>
#include <stddef.h>

// TYPES & METHODS

#define CLOAK_CONFIG_ENTRY_NULL (CLOAK_ConfigEntry) {.section = NULL, .key = NULL, .value = NULL}
typedef struct {
    char* section;
    char* key;
    char* value;
} CLOAK_ConfigEntry;

CLOAK_ConfigEntry CLOAK_ConfigEntryInit(const char* section, const char* key, const char* value);
void CLOAK_ConfigEntryFree(CLOAK_ConfigEntry* entry);


#define CLOAK_CONFIG_MAX_ENTRIES 128
typedef struct {
    CLOAK_ConfigEntry entries[CLOAK_CONFIG_MAX_ENTRIES];
    size_t count;
    bool initialized;
} CLOAK_Config;

int CLOAK_ConfigLoad(CLOAK_Config* config, const char* path);
void CLOAK_ConfigAddEntry(CLOAK_Config* config, CLOAK_ConfigEntry entry);
void CLOAK_ConfigFree(CLOAK_Config* config);
char* CLOAK_ConfigGet(const CLOAK_Config* config, const char* section, const char* key);
void CLOAK_ConfigPrint(const CLOAK_Config* config);

// MACROS

#define CLOAK_CONFIG_BOOL(x) (strcmp(x, "true") == 0 || strcmp(x, "TRUE") == 0 || strcmp(x, "yes") == 0 || strcmp(x, "YES") == 0 || strcmp(x, "1") == 0)

#define CLOAK_CONFIG_FILE                        "cloak.ini"
#define CLOAK_CONFIG_DIRECTORY                   ".cloak"
#define CLOAK_CONFIG_DEFAULT_BACKUP              ".cloak/backup"
#define CLOAK_CONFIG_DEFAULT_DECRYPTED           ".cloak/decrypted"
#define CLOAK_CONFIG_DEFAULT_KEYS                ".cloak/keys"
#define CLOAK_CONFIG_DEFAULT_RSA_PRIVATE         ".cloak/private.pem"
#define CLOAK_CONFIG_DEFAULT_RSA_PUBLIC          ".cloak/public.pem"
#define CLOAK_CONFIG_DEFAULT_INCLUDE_GITIGNORE   "true"
#define CLOAK_CONFIG_DEFAULT_INCLUDE_CLOAKIGNORE "true"
#define CLOAK_CONFIG_DEFAULT_URL                 ""
#define CLOAK_CONFIG_DEFAULT_REQUEST_ENDPOINT    ""
#define CLOAK_CONFIG_DEFAULT_STORE_ENDPOINT      ""
#define CLOAK_CONFIG_DEFAULT_AUTH_TOKEN          ""

// The following macros can be read as  `<section>,   <key>` and are used only as a parameter
#define CLOAK_CONFIG_INCLUDE_GITIGNORE   "behaviour", "include_gitignore"
#define CLOAK_CONFIG_INCLUDE_CLOAKIGNORE "behaviour", "include_cloakignore"
#define CLOAK_CONFIG_BACKUP              "paths",     "backup"
#define CLOAK_CONFIG_DECRYPTED           "paths",     "decrypted"
#define CLOAK_CONFIG_RSA_PRIVATE         "paths",     "rsa_private"
#define CLOAK_CONFIG_RSA_PUBLIC          "paths",     "rsa_public"
#define CLOAK_CONFIG_KEYS                "paths",     "keys"
#define CLOAK_CONFIG_URL                 "keyserver", "url"
#define CLOAK_CONFIG_REQUEST_ENDPOINT    "keyserver", "request_endpoint"
#define CLOAK_CONFIG_STORE_ENDPOINT      "keyserver", "store_endpoint"
#define CLOAK_CONFIG_AUTH_TOKEN          "keyserver", "auth_token"

#define GETTER(x) \
    static inline char* CLOAK_CONFIG_GET_##x(const CLOAK_Config* config) { \
        char* ret = CLOAK_ConfigGet(config, CLOAK_CONFIG_##x); \
        if (!ret) ret = CLOAK_CONFIG_DEFAULT_##x; \
        return ret; \
    }
    
GETTER(RSA_PUBLIC)
GETTER(RSA_PRIVATE)
GETTER(INCLUDE_GITIGNORE)
GETTER(INCLUDE_CLOAKIGNORE)
GETTER(BACKUP)
GETTER(DECRYPTED)
GETTER(KEYS)
GETTER(URL)
GETTER(REQUEST_ENDPOINT)
GETTER(STORE_ENDPOINT)
GETTER(AUTH_TOKEN)

// NO-PREFIX

#ifdef CLOAK_REMOVE_PREFIXES

#define ConfigEntry CLOAK_ConfigEntry
#define ConfigEntryInit CLOAK_ConfigEntryInit
#define ConfigEntryFree CLOAK_ConfigEntryFree

#define CONFIG_MAX_ENTRIES CLOAK_CONFIG_MAX_ENTRIES

#define Config CLOAK_Config
#define ConfigLoad CLOAK_ConfigLoad
#define ConfigFree CLOAK_ConfigFree
#define ConfigAddEntry CLOAK_ConfigAddEntry
#define ConfigGet CLOAK_ConfigGet
#define ConfigPrint CLOAK_ConfigPrint

#define CONFIG_INCLUDE_GITIGNORE CLOAK_CONFIG_INCLUDE_GITIGNORE
#define CONFIG_BACKUP CLOAK_CONFIG_BACKUP
#define CONFIG_DECRYPTED CLOAK_CONFIG_DECRYPTED
#define CONFIG_RSA_PRIVATE CLOAK_CONFIG_RSA_PRIVATE
#define CONFIG_RSA_PUBLIC CLOAK_CONFIG_RSA_PUBLIC
#define CONFIG_KEYS CLOAK_CONFIG_KEYS

#endif // CLOAK_REMOVE_PREFIXES

#endif // CONFIG_H
