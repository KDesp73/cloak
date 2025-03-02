#ifndef FILES_H
#define FILES_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <string.h>

// Check if the path is a file
static inline bool is_file(const char* path) 
{
    struct stat st;
    return (stat(path, &st) == 0) && S_ISREG(st.st_mode);
}

// Check if the path is a directory
static inline bool is_directory(const char* path) 
{
    struct stat st;
    return (stat(path, &st) == 0) && S_ISDIR(st.st_mode);
}

// Get the file name without the extension
static inline char* file_name(const char* path)
{
    char* base = strrchr(path, '/');
    char* filename = base ? base + 1 : (char*)path; // Extract the actual filename

    char* dot = strrchr(filename, '.');
    size_t len = (dot && dot != filename) ? (size_t)(dot - filename) : strlen(filename);

    char* result = (char*)malloc(len + 1);
    if (!result) return NULL;
    strncpy(result, filename, len);
    result[len] = '\0';

    return result;
}

// Get the file extension (returns pointer within `path` or empty string)
static inline char* file_extension(const char* path)
{
    char* dot = strrchr(path, '.');
    return (dot && dot != path) ? dot + 1 : "";
}

static inline int file_delete(const char *filename) {
    if (remove(filename) == 0) {
        return true;
    } else {
        perror("File deletion failed");
        return false;
    }
}

static inline int dir_create(const char* path)
{
    struct stat st = {0};

    if (stat(path, &st) == -1) { // Check if the directory exists
        if (mkdir(path, 0777) != 0 && errno != EEXIST) { // Try to create it
            return 0;
        }
    }
    return 1;
}

static inline bool file_exists(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return true;
    }
    return false;
}

#ifdef CLOAK_REMOVE_PREFIXES
#endif // CLOAK_REMOVE_PREFIXES

#endif // FILES_H
