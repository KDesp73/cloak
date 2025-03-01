#include "listing.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fnmatch.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define CLOAK_IGNORED_LEN 3
static char CLOAK_Ignored[CLOAK_IGNORED_LEN][64] = {
    ".git*",
    CLOAK_IGNORE_FILE,
    "*.cloak"
};

static size_t loadPatternsFromFile(char*** patterns, const char* path, size_t count)
{
    char** list = *patterns;
    char line[256];
    FILE* file = fopen(path, "r");
    
    if (!file) return count;
    
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0; // Remove newline
        if (*line == '\0' || *line == '#') continue; // Skip empty lines and comments
        
        char** new_list = realloc(list, (count + 1) * sizeof(char*));
        if (!new_list) {
            for (size_t j = 0; j < count; j++) {
                free(list[j]);
            }
            free(list);
            fclose(file);
            *patterns = NULL;
            return 0;
        }
        list = new_list;
        list[count++] = strdup(line);
    }
    
    fclose(file);
    *patterns = list;
    return count;
}

size_t CLOAK_LoadIgnore(char*** patterns)
{
    char** list = NULL;
    size_t count = 0;

    count = loadPatternsFromFile(&list, CLOAK_IGNORE_FILE, count);
    // TODO: Option to specify whether the gitignore file should be loaded or not
    count = loadPatternsFromFile(&list, ".gitignore", count);

    // Append patterns ignored by default
    for (size_t i = 0; i < CLOAK_IGNORED_LEN; i++) {
        char** new_list = realloc(list, (count + 1) * sizeof(char*));
        if (!new_list) {
            for (size_t j = 0; j < count; j++) {
                free(list[j]);
            }
            free(list);
            return 0;
        }
        list = new_list;
        list[count++] = strdup(CLOAK_Ignored[i]);
    }

    *patterns = list;
    return count;
}

int CLOAK_IsIgnored(const char* filename, char** patterns, size_t pattern_count)
{
    for (size_t i = 0; i < pattern_count; i++) {
        if (fnmatch(patterns[i], filename, 0) == 0) {
            return 1;
        }
    }
    return 0;
}

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

size_t CLOAK_ListLoad(CLOAK_List* list, const char* path)
{
    DIR* dir = opendir(path);
    if (!dir) {
        perror("Failed to open directory");
        return 0; // Return 0 for error case
    }

    struct dirent* entry;
    char full_path[PATH_MAX];
    size_t loaded_count = 0; // Count of files loaded in this call

    // Load ignore patterns
    char** ignore_patterns = NULL;
    size_t ignore_count = CLOAK_LoadIgnore(&ignore_patterns);

    while ((entry = readdir(dir))) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue; // Skip . and ..
        }

        if (CLOAK_IsIgnored(entry->d_name, ignore_patterns, ignore_count)) {
            continue; // Skip ignored files
        }

        // Construct the full path
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        // Check if the entry is a directory
        struct stat entry_stat;
        if (stat(full_path, &entry_stat) == 0 && S_ISDIR(entry_stat.st_mode)) {
            // Recursively load files from the child directory
            loaded_count += CLOAK_ListLoad(list, full_path);
        } else {
            // Add the file to the list
            char** new_files = realloc(list->files, (list->count + 1) * sizeof(char*));
            if (new_files) {
                list->files = new_files;
                list->files[list->count++] = strdup(full_path);
                loaded_count++; // Increment the loaded count
            } else {
                perror("Failed to allocate memory");
                // Handle memory allocation failure if needed
            }
        }
    }

    closedir(dir);

    // Free ignore patterns
    for (size_t i = 0; i < ignore_count; i++) {
        free(ignore_patterns[i]);
    }
    free(ignore_patterns);

    return loaded_count; // Return the total count of loaded files
}

size_t CLOAK_ListLoadEncrypted(CLOAK_List* list, const char* path)
{
    DIR* dir = opendir(path);
    if (!dir) {
        perror("Failed to open directory");
        return 0;
    }

    struct dirent* entry;
    char full_path[PATH_MAX];
    size_t loaded_count = 0;

    while ((entry = readdir(dir))) {
        // Skip . and ..
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        if (entry->d_type == DT_REG) {
            const char* ext = strrchr(entry->d_name, '.');
            if (ext && strcmp(ext, ".cloak") == 0) {
                snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
                
                char** new_files = realloc(list->files, (list->count + 1) * sizeof(char*));
                if (!new_files) {
                    closedir(dir);
                    return loaded_count;
                }
                list->files = new_files;
                
                list->files[list->count++] = strdup(full_path);
                loaded_count++;
            }
        } else if (entry->d_type == DT_DIR) {
            loaded_count += CLOAK_ListLoadEncrypted(list, full_path);
        }
    }

    closedir(dir);
    return loaded_count; // Return the total count of loaded .cloak files
}

void CLOAK_ListFree(CLOAK_List* list)
{
    for(size_t i = 0; i < list->count; i++) {
        free(list->files[i]);
    }
    free(list->files);
}

void CLOAK_ListPrint(const CLOAK_List* list)
{
    for(size_t i = 0; i < list->count; i++) {
        printf("%s\n", list->files[i]);
    }
}
