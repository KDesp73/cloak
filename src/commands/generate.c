#include "commands.h"
#include "config.h"
#include "extern/logging.h"
#include "files.h"
#include "rsa.h"
#include <stdio.h>
#include <string.h>

const char* ini =
"[paths]\n"
"backup = .cloak/backup\n"
"decrypted = .cloak/decrypted\n"
"rsa_private = .cloak/private.pem\n"
"rsa_public = .cloak/public.pem\n"
"\n"
"[behaviour]\n"
"include_gitignore = true\n"
"include_cloakignore = true\n";

#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#define TEMP_FILE "/tmp/downloaded_file"

#define BASH_AUTOCOMPLETION_SCRIPT "https://raw.githubusercontent.com/KDesp73/cloak/refs/heads/main/docs/autocomplete/cloak.bash"
#define ZSH_AUTOCOMPLETION_SCRIPT  "https://raw.githubusercontent.com/KDesp73/cloak/refs/heads/main/docs/autocomplete/cloak.zsh"
#define FISH_AUTOCOMPLETION_SCRIPT "https://raw.githubusercontent.com/KDesp73/cloak/refs/heads/main/docs/autocomplete/cloak.fish"

#define BASH_AUTOCOMPLETION_PATH "/etc/bash_completion.d/cloak.bash"
#define ZSH_AUTOCOMPLETION_PATH  "/usr/share/zsh/functions/Completion/_cloak"
#define FISH_AUTOCOMPLETION_PATH "/etc/fish/conf.d/cloak.fish"

static size_t write_callback(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    return fwrite(ptr, size, nmemb, stream);
}

static int download_file(const char* url, const char* output_file)
{
    CURL *curl = curl_easy_init();
    if (!curl) {
        ERRO("Failed to initialize curl");
        return 0;
    }

    FILE *file = fopen(output_file, "wb");
    if (!file) {
        perror("Failed to open temp file");
        curl_easy_cleanup(curl);
        return 0;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);

    CURLcode res = curl_easy_perform(curl);
    fclose(file);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        ERRO("Download failed: %s", curl_easy_strerror(res));
        return 0;
    }

    return 1;
}

static int run_command(const char *command)
{
    FILE *fp = popen(command, "r");
    if (!fp) {
        perror("popen failed");
        return 0;
    }

    char output[256];
    while (fgets(output, sizeof(output), fp)) {
        printf("%s", output);  // Print command output
    }

    int status = pclose(fp);
    if (status == -1) {
        perror("pclose failed");
        return 0;
    } else if (WEXITSTATUS(status) != 0) {
        ERRO("Command failed with exit code %d", WEXITSTATUS(status));
        return 0;
    }

    return 1;
}

static int install_autocompletion(const char* url, const char* dest) {
    if (!download_file(url, TEMP_FILE)) {
        ERRO("File download failed");
        return false;
    }

    char command[512];
    snprintf(command, sizeof(command), "sudo cp %s %s", TEMP_FILE, dest);

    if (!run_command(command)) {
        ERRO("Failed to move file to protected directory");
        return false;
    }

    return true;
}

int CLOAK_CommandGenerate(CLOAK_Context* ctx)
{
    char* type = ctx->type;

    if(!strcmp(type, "ini")) {
        if(!file_write(CLOAK_CONFIG_FILE, ini)){
            ERRO("Could not create %s", CLOAK_CONFIG_FILE);
            return false;
        } else {
            INFO("%s generated successfully", CLOAK_CONFIG_FILE);
        }
    } else if(!strcmp(type, "autocomplete")) {
        if(install_autocompletion(ZSH_AUTOCOMPLETION_SCRIPT, ZSH_AUTOCOMPLETION_PATH))
            INFO("Installed zsh autocompletion");
        else ERRO("Failed to install zsh autocompletion");

        if(install_autocompletion(BASH_AUTOCOMPLETION_SCRIPT, BASH_AUTOCOMPLETION_PATH))
            INFO("Installed bash autocompletion");
        else ERRO("Failed to install bash autocompletion");

        if(install_autocompletion(FISH_AUTOCOMPLETION_SCRIPT, FISH_AUTOCOMPLETION_PATH))
            INFO("Installed fish autocompletion");
        else ERRO("Failed to install fish autocompletion");
    } else if(!strcmp(type, "rsa")) {
        CLOAK_RSAGenerateKeys();
        INFO("RSA keys generated successfully!");
    }
    

    return true;
}
