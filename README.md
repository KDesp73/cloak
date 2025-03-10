# cloak 

## Usage

### Encrypt a file 
```bash
cloak encrypt -i <PATH> [-o <PATH>]
```
> Default output: `<FILE>.cloak` 

### Decrypt a file
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

### Validate the signatures or the key
```bash
cloak validate -i <SIG-PATH | KEY-PATH>
```

### Sign the key
```bash
cloak sign -i <KEY-PATH>
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
- `cloak.sig`

## License

[MIT](./LICENSE)
