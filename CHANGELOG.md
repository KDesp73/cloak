# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/), and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).


## [Unreleased]

### Fixed

- Fixed loading encrypted files bug
- Directory decryption working with default output


## [0.1.3] - 2025-03-02 

### Added

- Using cloak.ini as the config file
- `generate` command
- `--gitignore-ignore` flag
- `--cloakignore-ignore` option
- `init` command

### Changed

- Bumped version to 0.1.3


## [0.1.2] - 2025-03-01 

### Added

- Compressing before encrypting
- Implemented `ls` command with ignore file
- Decrypting multiple files

### Changed

- Bumped version to 0.1.2


## [0.1.1] - 2025-02-26 

### Added

- Added `hash` command
- Verifying hashes after decryption
- Added MIT License
- Added specific help message for each command
- Added `CLOAK_` prefix and a macro mechanism to remove it

### Changed

- Included dependencies inside the project
- Bumped version to 0.1.1


## [0.1.0] - 2025-02-25 

### Added

- Working AES encryption/decryption on files
- Command line interface framework


[0.1.0]: https://github.com/KDesp73/cloak/releases/tag/v0.1.0
[0.1.1]: https://github.com/KDesp73/cloak/releases/tag/v0.1.1
[0.1.2]: https://github.com/KDesp73/cloak/releases/tag/v0.1.2
[0.1.3]: https://github.com/KDesp73/cloak/releases/tag/v0.1.3

