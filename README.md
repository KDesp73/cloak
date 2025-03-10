# cloak 

Cloak is a command-line tool for secure source code encryption and decryption, 
designed for CI/CD pipelines and team collaboration. It uses AES-256 for fast
file encryption and RSA for securely sharing encryption keys among collaborators. 
Cloak supports standalone operation and optional key server integration for 
automated key management, ensuring secure and efficient collaboration on 
encrypted projects.

## Usage

### Encrypt a file or a directory
```bash
cloak encrypt -i <PATH> [-o <PATH>]
```
> Default output: `<FILE>.cloak` 

### Decrypt a file or a directory
```bash
cloak decrypt -i <PATH> -o <PATH> -k <KEY-PATH>
```

### Get the hash of a file
```bash
cloak hash -i <PATH>
```

### List files used in the encryption progress
```bash
cloak ls
```

## Ignore file

The `.cloakignore` file is being used to define the patterns that are being ignored in the encryption process.

Anything inside the `.gitignore` file is also included in this list depending on the configuration used

### Patterns ignored by default

- `.git*`
- `.cloakignore`
- `*.cloak`
- `cloak.ini`
- `.cloak/*`
- `cloak.key`

## License

[MIT](./LICENSE)
