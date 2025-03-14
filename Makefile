# Compiler and flags
CC = gcc
CFLAGS = -Wall -Werror -Iinclude -fPIC
LDFLAGS = -Llib -l:libsodium.a -l:libz.a -lcurl -lssl -lcrypto -ldl

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
DIST_DIR = dist

LIBRARY_NAME = cloak
SO_NAME = lib$(LIBRARY_NAME).so
A_NAME = lib$(LIBRARY_NAME).a

# Target and version info
TARGET = cloak
version_file = include/version.h
VERSION_MAJOR = $(shell sed -n -e 's/\#define CLOAK_VERSION_MAJOR \([0-9]*\)/\1/p' $(version_file))
VERSION_MINOR = $(shell sed -n -e 's/\#define CLOAK_VERSION_MINOR \([0-9]*\)/\1/p' $(version_file))
VERSION_PATCH = $(shell sed -n -e 's/\#define CLOAK_VERSION_PATCH \([0-9]*\)/\1/p' $(version_file))
VERSION = $(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_PATCH)

# Determine the build type
ifeq ($(type), RELEASE)
	CFLAGS += -O3
else
	SANITIZERS = -fsanitize=address,undefined
	CFLAGS  += -DDEBUG -ggdb
	CFLAGS  += $(SANITIZERS)
	LDFLAGS += $(SANITIZERS)
endif

# Source and object files
SRC_FILES := $(shell find $(SRC_DIR) -name '*.c' ! -name 'main.c')
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

# Default target
.DEFAULT_GOAL := help

# Total source file count
TOTAL_FILES := $(words $(SRC_FILES))

# Counter to track progress
counter = 0

# Targets

.PHONY: all
all: check_tools $(BUILD_DIR) static shared $(TARGET) ## Build the project
	@echo "Build complete."

.PHONY: check_tools
check_tools: ## Check if necessary tools are available
	@command -v gcc >/dev/null 2>&1 || { echo >&2 "[ERRO] gcc is not installed."; exit 1; }
	@command -v bear >/dev/null 2>&1 || { echo >&2 "[WARN] bear is not installed. Skipping compile_commands.json target."; }

$(BUILD_DIR): ## Create the build directory if it doesn't exist
	@echo "[INFO] Creating build directory"
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/commands

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c ## Compile source files with progress
	$(eval counter=$(shell echo $$(($(counter)+1))))
	@echo "[$(counter)/$(TOTAL_FILES)] Compiling $< -> $@"
	@$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET): $(BUILD_DIR) static ## Build executable using static library
	@echo "[INFO] Building executable: $(TARGET)"
	@$(CC) src/main.c -o $(TARGET) -L. -l:$(A_NAME) $(LDFLAGS) -I$(INCLUDE_DIR)

.PHONY: shared
shared: $(BUILD_DIR) $(OBJ_FILES) ## Build shared library
	@echo "[INFO] Building shared library: $(SO_NAME)"
	@$(CC) -shared $(CFLAGS) -o $(SO_NAME) $(OBJ_FILES)

.PHONY: static
static: $(BUILD_DIR) $(OBJ_FILES) ## Build static library
	@echo "[INFO] Building static library: $(A_NAME)"
	@$(AR) rcs $(A_NAME) $(OBJ_FILES)

.PHONY: clean
clean: clean_cloak ## Remove all build files and the executable
	@echo "[INFO] Cleaning up build directory and executable."
	rm -rf $(BUILD_DIR) $(TARGET) $(SO_NAME) $(A_NAME)

.PHONY: clean_cloak
clean_cloak: ## Remove all cloak related files
	rm -rf ./src/*.cloak
	rm -rf ./include/*.cloak
	rm -f cloak.key
	rm -f cloak.sig
	rm -rf .cloak/decrypted/*

.PHONY: distclean
distclean: clean ## Perform a full clean, including backup and temporary files
	@echo "[INFO] Performing full clean, removing build directory, dist files, and editor backups."
	rm -f *~ core $(SRC_DIR)/*~ $(DIST_DIR)/*.tar.gz

.PHONY: dist
dist: $(SRC_FILES) ## Create a tarball of the project
	@echo "[INFO] Creating a tarball for version $(VERSION)"
	mkdir -p $(DIST_DIR)
	tar -czvf $(DIST_DIR)/$(TARGET)-$(VERSION).tar.gz $(SRC_DIR) $(INCLUDE_DIR) Makefile README.md

PREFIX = /usr/local

.PHONY: install
install: all ## Install libraries, headers and executable
	@echo "[INFO] Installing cloak using" $(PREFIX) "as the prefix"
	mkdir -p $(PREFIX)/include/cloak
	cp -r ./include/* $(PREFIX)/include/cloak
	cp ./libcloak.* $(PREFIX)/lib
	cp ./cloak $(PREFIX)/bin

.PHONY: uninstall
uninstall: ## Uninstall cloak from the machine
	@echo "[INFO] Uninstalling cloak"
	rm -rf $(PREFIX)/include/cloak
	rm -f $(PREFIX)/lib/libcloak.*
	rm -f $(PREFIX)/bin/cloak

## Generate compile_commands.json
.PHONY: compile_commands.json
compile_commands.json: $(SRC_FILES) ## Generate compile_commands.json
	@echo "[INFO] Generating compile_commands.json"
	bear -- make all

.PHONY: autocomplete
autocomplete: ## Generate autocomplete scripts
	complgen aot ./docs/autocomplete/cloak.usage --zsh-script ./docs/autocomplete/cloak.zsh
	complgen aot ./docs/autocomplete/cloak.usage --bash-script ./docs/autocomplete/cloak.bash
	complgen aot ./docs/autocomplete/cloak.usage --fish-script ./docs/autocomplete/cloak.fish

## Show this help message
.PHONY: help
help: ## Show this help message
	@echo "Available commands:"
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-20s\033[0m %s\n", $$1, $$2}'

## Enable verbose output for debugging
.PHONY: verbose
verbose: CFLAGS += -DVERBOSE
verbose: all ## Build the project in verbose mode

# Phony targets to avoid conflicts with file names
.PHONY: all clean distclean install uninstall dist compile_commands.json help check_tools verbose
